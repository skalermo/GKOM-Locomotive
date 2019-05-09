#pragma once

#include "Composite.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"

class ThreeShapes : public Composite
{
public:
	ThreeShapes()
	{
		auto cylinder = std::make_unique<Cylinder>(glm::vec3(0.0f), 0.7f, 0.2f, "textures/woodBarrel.png");
		auto sphere = std::make_unique<Sphere>(glm::vec3(-0.5f), 0.3f, "textures/wall.jpg");
		auto cube = std::make_unique<Cube>(glm::vec3(0.5f), glm::vec3{ 0.3f, 0.3f, 0.3f }, "textures/container.jpg");
	

		addChild(std::move(sphere));
		addChild(std::move(cylinder));
		addChild(std::move(cube));
	}
};
