#pragma once

#include "Composite.h"
#include "TrainBottom.h"
#include "TrainTop.h"
#include "LightManager.h"

class Train : public Composite
{
	TrainTop* topPtr;
	LightManager& lightManager;
public:
	Train(LightManager& lightManager) : lightManager(lightManager)
	{
		addChild(std::make_unique<TrainBottom>());
		auto top = std::make_unique<TrainTop>();
		topPtr = top.get();
		addChild(std::move(top));
	}

	void move(glm::vec3 displacement) override
	{
		Composite::move(displacement);
		lightManager.getPointLight()->move(displacement);
	}

	void scaleChimney(const float amount) const
	{
		topPtr->scaleChimney(amount);
	}
	void scaleBody(const float amount) const
	{
		topPtr->scaleBody(amount);
	}
	void scaleCabin(const float amount) const
	{
		topPtr->scaleCabin(amount);
	}

};

