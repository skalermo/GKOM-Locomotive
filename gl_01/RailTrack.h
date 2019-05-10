#pragma once

#include "Composite.h"
#include "Cube.h"

class RailTrack : public Composite
{
public: 
	RailTrack(int number, float offset)
	{
		std::unique_ptr<Cube> *rail1 = new std::unique_ptr<Cube> [number];
		std::unique_ptr<Cube> *rail2 = new std::unique_ptr<Cube> [number]; 
		std::unique_ptr<Cube> *wood = new std::unique_ptr<Cube>[number];
		for (int i = 0; i < number; i++)
		{
			rail1[i] = std::make_unique<Cube>(glm::vec3(0.2f, 0.0f, 0.0f + offset * i), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
			rail2[i] = std::make_unique<Cube>(glm::vec3(-0.2f, 0.0f, 0.0f + offset *i), glm::vec3(0.05f, 0.05f, 0.3f), "textures/metal.jpg");
			wood[i] = std::make_unique<Cube>(glm::vec3(0.0f, 0.0f, 0.0f+ offset * i), glm::vec3(0.3f, 0.02f, 0.1f), "textures/oldWood.jpg"); 

			addChild(std::move(rail1[i])); 
			addChild(std::move(rail2[i]));
			addChild(std::move(wood[i]));
		}
			
		
		
		//= std::make_unique<Cube>
		//auto rail2 = 		//auto wood = 
		//addChild(std::move(rail1)); 
		//addChild(std::move(rail2));
		//addChild(std::move(wood)); 
	}
};
