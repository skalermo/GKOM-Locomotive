#pragma once

#include "Composite.h"
#include "TrainBottom.h"
#include "TrainTop.h"

class Train : public Composite
{
	TrainTop* topPtr;
public:
	Train()
	{
		addChild(std::make_unique<TrainBottom>());
		auto top = std::make_unique<TrainTop>();
		topPtr = top.get();
		addChild(std::move(top));
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

