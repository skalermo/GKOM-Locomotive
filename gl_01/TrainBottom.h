#pragma once

#include "Composite.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"

class TrainBottom : public Composite
{
private:

	class Rods : public Composite
	{
	private:
		const std::string rodTexturePath = "textures/rodTexture.png";
		const std::string circleTexturePath = "textures/rodCircleTexture.png";

	public:
		Rods()
		{
			auto rodLeft = std::make_unique<Cube>(glm::vec3(3.0f, 0.0f, -0.35f - 0.01f), glm::vec3(3.2f, 0.2f, 0.1f), rodTexturePath);
			auto rodRight = std::make_unique<Cube>(glm::vec3(3.0f, 0.0f, 4.35f + 0.01f), glm::vec3(3.2f, 0.2f, 0.1f), rodTexturePath);
			auto leftCircleOfLeftRod = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, -0.55f - 0.01f), 0.3f, 0.34f, circleTexturePath);
			auto rightCircleOfLeftRod = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, -0.55f - 0.01f), 0.3f, 0.34f, circleTexturePath);
			auto leftCircleOfRightRod = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, 4.25f + 0.01f), 0.3f, 0.34f, circleTexturePath);
			auto rightCircleOfRightRod = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, 4.25f + 0.01f), 0.3f, 0.34f, circleTexturePath);

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
	private:
		const std::string wheelTexturePath = "textures/steelWheelSmall.png";

	public:
		Wheels()
		{
			auto wheelFrontLeft = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, -0.25f - 0.01f), 0.25f, 1.35f, wheelTexturePath);
			auto wheelBackLeft = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, -0.25f - 0.01f), 0.25f, 1.35f, wheelTexturePath);
			auto wheelFrontRight = std::make_unique<Cylinder>(glm::vec3(0.0f, 0.0f, 4.0f + 0.01f), 0.25f, 1.35f, wheelTexturePath);
			auto wheelBackRight = std::make_unique<Cylinder>(glm::vec3(6.0f, 0.0f, 4.0f + 0.01f), 0.25f, 1.35f, wheelTexturePath);
			addChild(std::move(wheelFrontLeft));
			addChild(std::move(wheelFrontRight));
			addChild(std::move(wheelBackLeft));
			addChild(std::move(wheelBackRight));
		}
	};

	// -150 degrees - start angle
	float dx0 = 0.0f, dy0 = 0.0f, pos = -150.0f;
	Wheels * wheelsPtr;
	Rods * rodsPtr;

public:
	TrainBottom()
	{
		auto wheels = std::make_unique<Wheels>();
		auto rods = std::make_unique<Rods>();
		auto bottom = std::make_unique<Cube>(glm::vec3(3.0f, 0.25f, 2.0f), glm::vec3(7.0f, 0.2, 2.0f), "textures/rivetedSteel.png");

		wheelsPtr = wheels.get();
		rodsPtr = rods.get();

		addChild(std::move(wheels));
		addChild(std::move(rods));
		addChild(std::move(bottom));
	}

	void move(glm::vec3 displacement) override
	{
		Composite::move(displacement);

		// 1.35 - wheel's radius
		auto deltaAngle = -(displacement.x * 180 / M_PI / 1.35f);

		// 0.447 - distance from wheel center to rod; it's 0.3311 * wheel_radius
		float dx = 0.447 * cos(pos / 180 * M_PI);
		float dy = 0.447 * sin(pos / 180 * M_PI);

		wheelsPtr->rotate({ 0.0f, 0.0f, deltaAngle }); //wheels
		rodsPtr->move({ -dx0 + dx, -dy0 + dy, 0.0f }); //rods

		pos += deltaAngle;
		dx0 = dx;
		dy0 = dy;
	}
};


