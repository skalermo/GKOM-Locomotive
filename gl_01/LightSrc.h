#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <corecrt_math_defines.h>
#include "Object.h"
#include "ShaderProvider.h"
#include "Texture.h"


class LightSrc : public Object {
	std::shared_ptr<Shader> shader = ShaderProvider::instance().getShader("shLightSrc.vert", "shLightSrc.frag");
	std::vector<float> vertices;
	std::vector<int> indices;
	GLuint EBO, VBO, VAO;

	glm::vec3 coordinates = glm::vec3(0.0f);

	void initProgram() {
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);
	}

	void generateVertices(float radius, const int mLatitudes, const int mMeridians) {
		for (size_t i = 0; i < mMeridians + 1; i++)
		{
			for (size_t j = 0; j < mLatitudes + 2; j++)
			{
				// texCoord in the range [(0,0), (1,1)]
				glm::vec2 texCoords(static_cast<float>(i) / mMeridians, static_cast<float>(j) / (mLatitudes + 1));
				const double theta = 2 * M_PI * texCoords.x;
				const double phi = M_PI * texCoords.y - M_PI_2;
				glm::vec3 pos;
				pos.y = static_cast<float>(std::sinf(phi));
				pos.x = static_cast<float>(std::cos(phi)) * std::cos(theta);
				pos.z = static_cast<float>(std::cos(phi)) * std::sin(theta);

				pos *= radius;

				vertices.push_back(pos.x);
				vertices.push_back(pos.y);
				vertices.push_back(pos.z);
			}
		}

		for (size_t i = 0; i < mMeridians; i++)
		{
			// Construct triangles between successive meridians
			for (size_t j = 0; j < mLatitudes + 1; j++)
			{
				indices.push_back(i * (mLatitudes + 2) + j);
				indices.push_back(i * (mLatitudes + 2) + j + 1);
				indices.push_back((i + 1) * (mLatitudes + 2) + j + 1);

				indices.push_back((i + 1) * (mLatitudes + 2) + j + 1);
				indices.push_back((i + 1) * (mLatitudes + 2) + j);
				indices.push_back(i * (mLatitudes + 2) + j);

			}
		}
	}

public:

	LightSrc(const float radius, int widthSectors = 36, int heightSectors = 30)
		: LightSrc(glm::vec3(0.0f), radius) {}

	LightSrc(const glm::vec3 coordinates, const float radius,
		const int widthSectors = 36, const int heightSectors = 30)
		: coordinates(coordinates)
	{
		generateVertices(radius, widthSectors, heightSectors);
		initProgram();
	}

	~LightSrc() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	static std::shared_ptr<Shader> getShaderPtr() {
		return ShaderProvider::instance().getShader("shLightSrc.vert", "shLightSrc.frag");
	}

	void draw(std::shared_ptr<Shader> shader) override {
		auto model = glm::mat4(1.0f);
		model = translate(model, coordinates);
		shader->use();
		shader->setTransformMatrix("model", model);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glm::vec3 getPosition()
	{
		return coordinates;
	}

	void move(glm::vec3 displacement) override {
		this->coordinates += displacement;
	}

	void rotate(const glm::vec3& rotation) override{
		throw std::exception("Dont use rotation with light");
	}

	void scale(glm::vec3 amount) override {
		throw std::exception("Dont use scale with light");
	}


};