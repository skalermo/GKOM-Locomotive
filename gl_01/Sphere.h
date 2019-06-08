#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <corecrt_math_defines.h>
#include "ShaderProvider.h"
#include "Primitive.h"


class Sphere : public Primitive {
	float radius;
	unsigned int mLatitudes, mMeridians;

	void generateVertices() override {
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
				vertices.push_back(pos.x);
				vertices.push_back(pos.y);
				vertices.push_back(pos.z);
				vertices.push_back(texCoords.x);
				vertices.push_back(texCoords.y);
			}
		}

	}

	void generateIndices() override
	{
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
	Sphere(const float radius, const std::string& texturePath)
		:Sphere(glm::vec3(0.0f), radius, texturePath) {}

	Sphere(const glm::vec3 coordinates, const float radius, const std::string& texturePath,
		const glm::vec3& rotation = glm::vec3(0.0f),
		const unsigned int widthSectors = 36, const unsigned int heightSectors = 30)
		: Primitive(coordinates, rotation, texturePath),
	      radius(radius),  mLatitudes(widthSectors), mMeridians(heightSectors)
	{
//		shader = getShaderPtr();
		init();
	}

	static std::shared_ptr<Shader> getShaderPtr() {
		return ShaderProvider::instance().getShader("shSphere.vert", "shSphere.frag");
	}
};
