#pragma once

#include "Composite.h"
#include "Cube.h"

class RailTrack : public Composite
{
public: 
	RailTrack(int number, float offset)
	{
		std::unique_ptr<Cube> rail1;
		std::unique_ptr<Cube> rail2; 
		std::unique_ptr<Cube> wood;
		for (int i = 0; i < number; i++)
		{
			rail1 = std::make_unique<Cube>(glm::vec3(0.2f, 0.0f, 0.0f + offset * i), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
			rail2 = std::make_unique<Cube>(glm::vec3(-0.2f, 0.0f, 0.0f + offset *i), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
			wood = std::make_unique<Cube>(glm::vec3(0.0f, 0.0f, 0.0f+ offset * i), glm::vec3(0.3f, 0.02f, 0.1f), "textures/oldWood.jpg"); 

			addChild(std::move(rail1)); 
			addChild(std::move(rail2));
			addChild(std::move(wood));
		}
			
	}
};
