#pragma once
#define _USE_MATH_DEFINES

#include <memory>
#include <utility>
#include <vector>
#include <cmath>
#include "SOIL.h"
#include "Object.h"
#include "ShaderProvider.h"
#include "Texture.h"
#include <corecrt_math_defines.h>


class Sphere : Object {
	std::shared_ptr<Shader> shader = ShaderProvider::instance().getShader("shSphere.vert", "shSphere.frag");
	std::vector<float> vertices;
	std::vector<int> indices;
	GLuint EBO, VBO, VAO;
	Texture texture;

	glm::vec3 coordinates = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	std::string texturePath;

	void initPrograme() {
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

	void generateVertices(float radius, int widthSectors, int heightSectors) {
		float prevTexU = 0, prevTexV = 0;
		const float widthStep = 2 * M_PI / widthSectors;
		const float heightStep = M_PI / heightSectors;

		for (int i = 0; i <= heightSectors * 2; i++) {
			float heightAngle = M_PI / 2 - i * heightStep;
			auto z = radius * sinf(heightAngle);
			auto rcos = radius * cosf(heightAngle);

			for (int j = 0; j <= widthSectors; j++) {
				float widthAngle = j * widthStep;
				float x = rcos * cosf(widthAngle);
				float y = rcos * sinf(widthAngle);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				auto n = glm::normalize(glm::vec3(x, y, z));
				float textureU = 0.5 + atan2f(n.x, n.z) / (2 * M_PI);
				float textureV = 0.5 - asinf(n.y) / M_PI;

				vertices.push_back((textureU));
				vertices.push_back((textureV));
			}
		}

		for (int i = 0; i < heightSectors; ++i)
		{
			int k1 = i * (widthSectors + 1);     // beginning of current stack
			int k2 = k1 + widthSectors + 1;      // beginning of next stack

			for (int j = 0; j < widthSectors; ++j, ++k1, ++k2)
			{
				float a, b, c;
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (heightSectors - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}
public:
	/**
		@radius radius of a sphere
		@widthSectors - number of lines that are vertical(goes from north pole to south pole)
		@heightSectors - number of line that are horizontal
	*/
	Sphere(float radius, const std::string& texturePath, int widthSectors = 36, int heightSectors = 36)
		:Sphere(glm::vec3(0.0f), radius, texturePath) {}

	Sphere(glm::vec3 coordinates, float radius, const std::string& texturePath, int widthSectors = 36, int heightSectors = 30, glm::vec3 rotation = glm::vec3(0.0f))
		: coordinates(coordinates), rotation(rotation), texturePath(texturePath)
	{
		generateVertices(radius, widthSectors, heightSectors);
		initPrograme();
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
		model = glm::translate(model, coordinates);
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
	void rotate(const glm::vec3& rotation)
	{
		this->rotation += rotation;
	}


};
