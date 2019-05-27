#pragma once
#include "Composite.h"
#include "Cube.h"

class Floor : public Composite
{
public:
	Floor(float floorSize, int pieces)
	{
		const auto y = -2;
		const auto blockSize = floorSize / pieces;
		const auto startX = pieces % 2 == 0 ? -blockSize * pieces / 2 + blockSize / 2 
			: -blockSize * pieces / 2 - blockSize / 2;
		for (int i = 0; i < pieces; ++i)
		{
			auto x = startX + i * blockSize - blockSize;
			for (int j = 0; j < pieces; ++j)
			{
				auto z = startX + j * blockSize;
				addChild(
					std::make_unique<Cube>(
						glm::vec3{ x, y, z },
						glm::vec3{ blockSize / 2, 0.1, blockSize/ 2 },
						"textures/ground.png")
				);
			}
		}
	}


};

