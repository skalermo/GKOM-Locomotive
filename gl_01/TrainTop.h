#pragma once

#include "Composite.h"
#include "Cube.h"
#include "Cylinder.h"

class TrainTop : public Composite
{
public:
	const std::string cabinPath = "textures/oldWood.jpg";
	const std::string bodyPath = "textures/rodCircleTexture.png";
	TrainTop()
	{
		auto bodyWidth = 12.3f, radius = 1.66f;
		auto bodyCords = glm::vec3(-bodyWidth / 2 + 3.0, 1.5f, 2.0f);
		auto body = std::make_unique<Cylinder>(bodyCords,
			bodyWidth, radius, 18, glm::vec3(0.f, 90.f, 0.f), bodyPath
		);

		addChild(std::move(body));

		auto cabinWidth = 1.0f, cabinLength = 2.4f, cabinHeight = 1.2f;
		auto cabinCords = glm::vec3(bodyCords.x + bodyWidth - 0.45f - cabinLength,
			radius + bodyCords.y + cabinHeight - 0.4f, bodyCords.z
		);
		auto cabin = std::make_unique<Cube>(cabinCords, 
			glm::vec3(-cabinLength, cabinHeight, cabinWidth), cabinPath
		);

		addChild(std::move(cabin));

		const auto roofWidth = cabinWidth * 1.2f, roofLength = cabinLength * 1.2f, roofHeight = 0.2f;
		const auto roofCords = glm::vec3(cabinCords.x, cabinCords.y + cabinHeight + roofHeight, bodyCords.z);
		auto roof = std::make_unique<Cube>(roofCords, 
			glm::vec3(-roofLength, roofHeight, roofWidth), cabinPath
		);

		addChild(std::move(roof));
	}
};
