#pragma once

#include "Composite.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include <cstdlib>
#include <ctime>

class Cactus : public Composite
{
private:
	float groundLevel;
public:
	void createBranch(float rotation, glm::vec3 coords, float height, float radius)
	{
		std::unique_ptr<Cylinder> core; 
		std::unique_ptr<Sphere> top;

		glm::vec3 sphereCoords;
		if (rotation == 0)
			sphereCoords = glm::vec3(coords.x, coords.y,coords.z + height);
		else if (rotation == 1)
			sphereCoords = glm::vec3(coords.x,coords.y -height, coords.z);
		else if (rotation == 2)
			sphereCoords = glm::vec3(coords.x, coords.y,coords.z -height);
		else if (rotation == 3)
			sphereCoords = glm::vec3(coords.x, coords.y + height, coords.z); 
		core = std::make_unique<Cylinder>(coords, height, radius, 20, glm::vec3(rotation*90,0,0),  "textures/cactus.png");
		top = std::make_unique<Sphere>(sphereCoords, radius, "textures/cactus.png");
		addChild(std::move(core)); 
		addChild(std::move(top)); 
	}

	void generateCactus()
	{
		int minus = random(1.11f, 1.12f) == 1.11f ? -1 : 1; 
		glm::vec3 coords = glm::vec3(-random(0, 400), 0, minus * random(8, 50)); 
		float coreHeight = random(1, 5);
		float branch1Height = random(0.8, coreHeight); 
		float branch2Height = random(0.8, coreHeight); 
		float branch1Length = random(0.4, coreHeight - 0.6); 
		float branch2Length = random(0.4, coreHeight - 0.6);
		float branch3Height = random(0.3, coreHeight - 0.6); 
		float branch4Height = random(0.3, coreHeight - 0.6); 
		float radius = random(0.1, 0.6); 
		
		groundLevel = -1.55f;
		createBranch(3, glm::vec3(0, 0 + groundLevel, 0) + coords, coreHeight, radius); 
		createBranch(0, glm::vec3(0, branch1Height + groundLevel, 0) + coords, branch1Length, radius); 
		createBranch(3, glm::vec3(0, branch1Height + groundLevel, branch1Length) + coords, branch3Height, radius); 
		createBranch(2, glm::vec3(0, branch2Height + groundLevel, 0) + coords, branch2Length, radius); 
		createBranch(3, glm::vec3(0, branch2Height + groundLevel, -branch2Length) + coords, branch4Height, radius);
	}

	float random(float min, float max)
	{
		min *= 100;
		max *= 100;
		float result = static_cast<float> (min + (rand() % (int)(max - min + 1)))/100.0f;
		return result; 
	}
	
	Cactus(int number)
	{
		srand(time(NULL));
		for(int i = 0; i < number; i ++)
			generateCactus();

	}
};
