#pragma once

#include "Composite.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"


class Rods : public Composite
{
private:
	const std::string rodTexturePath = "textures/rodTexture.jpg";
	const std::string circleTexturePath = "textures/steelWheelSmall.png";
	const std::string circle2TexturePath = "textures/rodCircleTexture.png";


public:
	Rods()
	{
		auto rodLeft = std::make_unique<Cube>(glm::vec3(3.0f, 0.0f, -0.35f - 0.01f), glm::vec3(3.2f, 0.2f, 0.1f), rodTexturePath);
		auto rodRight = std::make_unique<Cube>(glm::vec3(3.0f, 0.0f, 4.35f + 0.01f), glm::vec3(3.2f, 0.2f, 0.1f), rodTexturePath);
		auto leftCircleOfLeftRod = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, -0.55f - 0.01f), 0.3f, 0.34f, circle2TexturePath);
		auto rightCircleOfLeftRod = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, -0.55f - 0.01f), 0.3f, 0.34f, circle2TexturePath);
		auto leftCircleOfRightRod = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, 4.25f + 0.01f), 0.3f, 0.34f, circle2TexturePath);
		auto rightCircleOfRightRod = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, 4.25f + 0.01f), 0.3f, 0.34f, circle2TexturePath);

		addChild(std::move(rodLeft));
		addChild(std::move(rodRight));
		addChild(std::move(leftCircleOfLeftRod));
		addChild(std::move(rightCircleOfLeftRod));
		addChild(std::move(leftCircleOfRightRod));
		addChild(std::move(rightCircleOfRightRod));

	}
};

class Wheels : public Composite
{
public:
	Wheels()
	{
		auto wheelFrontLeft = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, -0.25f - 0.01f), 0.25f, 1.35f, "textures/steelWheelSmall.png");
		auto wheelBackLeft = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, -0.25f - 0.01f), 0.25f, 1.35f, "textures/steelWheelSmall.png");
		auto wheelFrontRight = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, 4.0f + 0.01f), 0.25f, 1.35f, "textures/steelWheelSmall.png");
		auto wheelBackRight = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, 4.0f + 0.01f), 0.25f, 1.35f, "textures/steelWheelSmall.png");
		addChild(std::move(wheelFrontLeft));
		addChild(std::move(wheelFrontRight));
		addChild(std::move(wheelBackLeft));
		addChild(std::move(wheelBackRight));
	}
};

class TrainBottom : public Composite
{
public:
	Wheels * wheelsPtr;
	Rods * rodsPtr;
	TrainBottom()
	{
		auto wheels = std::make_unique<Wheels>();
		wheelsPtr = wheels.get();
		addChild(std::move(wheels));
		auto rods = std::make_unique<Rods>();
		rodsPtr = rods.get();
		addChild(std::move(rods));
		auto bottom = std::make_unique<Cube>(glm::vec3(3.0f, 0.25f, 2.0f), glm::vec3(7.0f, 0.5, 2.0f), "textures/steel.jpg");
		addChild(std::move(bottom));
	}

	// override
	/*void move(glm::vec3 displacement) {
		for (auto& child : children)
			child->move(displacement);
	}*/
};


