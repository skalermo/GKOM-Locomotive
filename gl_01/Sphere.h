#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <corecrt_math_defines.h>
#include "Object.h"
#include "ShaderProvider.h"
#include "Texture.h"


class Sphere : Object {
	std::shared_ptr<Shader> shader = ShaderProvider::instance().getShader("shSphere.vert", "shSphere.frag");
	std::vector<float> vertices;
	std::vector<int> indices;
	GLuint EBO, VBO, VAO;

	std::string texturePath;
	Texture texture;

	glm::vec3 coordinates = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	void initProgram() {
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		texture = Texture(texturePath);
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

				vertices.push_back(pos.x);
				vertices.push_back(pos.y);
				vertices.push_back(pos.z);
				vertices.push_back(texCoords.x);
				vertices.push_back(texCoords.y);
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
	/**
		@radius radius of a sphere
		@widthSectors - number of lines that are vertical(goes from north pole to south pole)
		@heightSectors - number of line that are horizontal
	*/
	Sphere(const float radius, const std::string& texturePath, int widthSectors = 36, int heightSectors = 30)
		:Sphere(glm::vec3(0.0f), radius, texturePath) {}

	Sphere(const glm::vec3 coordinates, const float radius, const std::string& texturePath,
		const glm::vec3& rotation = glm::vec3(0.0f),
		const int widthSectors = 36, const int heightSectors = 30)
		: texturePath(texturePath), coordinates(coordinates), rotation(rotation)
	{
		generateVertices(radius, widthSectors, heightSectors);
		initProgram();
	}

	~Sphere() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	static std::shared_ptr<Shader> getShaderPtr() {
		return ShaderProvider::instance().getShader("shSphere.vert", "shSphere.frag");
	}

	void draw() override {
		auto model = glm::mat4(1.0f);

		model = translate(model, coordinates);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->use();
		texture.useTexture(shader);
		shader->setTransformMatrix("model", model);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void move(glm::vec3 displacement) override {
		this->coordinates += displacement;
	}
	void rotate(const glm::vec3& rotation) {
		this->rotation += rotation;
	}


};
