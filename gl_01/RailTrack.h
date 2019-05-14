#pragma once

#include "Composite.h"
#include "Cube.h"

class RailTrack : public Composite
{
public: 
	RailTrack(int number)
	{
		std::unique_ptr<Cube> rail1;
		std::unique_ptr<Cube> rail2; 
		std::unique_ptr<Cube> wood;
		for (int i = 0; i < number; i++)
		{
			rail1 = std::make_unique<Cube>(glm::vec3(4.0f * i, -1.27f, -0.125f), glm::vec3(2.0f, 0.2f, 0.132f), "textures/metal.jpg");
			rail2 = std::make_unique<Cube>(glm::vec3(4.0f * i, -1.27f, 4.125f), glm::vec3(2.0f, 0.2f, 0.132f), "textures/metal.jpg");
			wood = std::make_unique<Cube>(glm::vec3(4.0f * i, -1.55f, 2.0f), glm::vec3(0.7f, 0.12f, 3.0f), "textures/oldWood.jpg");
			addChild(std::move(rail1)); 
			addChild(std::move(rail2));
			addChild(std::move(wood));
		}
			
	}
};
