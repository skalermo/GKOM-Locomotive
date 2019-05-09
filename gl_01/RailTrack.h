#pragma once

#include "Composite.h"
#include "Cube.h"

class RailTrack : public Composite
{
public: 
	RailTrack()
	{
		auto rail1 = std::make_unique<Cube>(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
		auto rail2 = std::make_unique<Cube>(glm::vec3(-0.2f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
		auto wood = std::make_unique<Cube>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.02f, 0.1f), "textures/oldWood.jpg"); 

		addChild(std::move(rail1)); 
		addChild(std::move(rail2));
		addChild(std::move(wood)); 
	}
};
