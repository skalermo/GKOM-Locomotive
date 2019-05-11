#pragma once
#pragma once

#include "Composite.h"
#include "Cube.h"
#include "TrainBottom.h"
#include "TrainMiddle.h"
#include "TrainTop.h"

class Train : public Composite
{
public:
	Train()
	{
		addChild(std::make_unique<TrainBottom>());
		addChild(std::make_unique<TrainMiddle>());
		addChild(std::make_unique<TrainTop>());
	}
};

