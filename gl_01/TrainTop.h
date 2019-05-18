#pragma once

#include "Composite.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"

class TrainTop : public Composite
{
	const std::string cabinPath = "textures/black-steel.png";
	const std::string bodyPath = "textures/locoBody.png";

	Cylinder* chimneyPtr;
	Cylinder* bodyPtr;
	Cube* cabinPtr;
	Cube* roofPtr;


	//BODY COORDINATES
	float bodyWidth = 12.3f, bodyRadius = 1.66f;
	glm::vec3 bodyCords = glm::vec3(-bodyWidth / 2 + 3.0, 1.9f, 2.0f);

	//CABIN COORDINATES
	float cabinWidth = 1.1f, cabinLength = 2.0f, cabinHeight = 1.0f;
	glm::vec3 cabinCords = glm::vec3(
		bodyCords.x + bodyWidth - 0.45f - cabinLength,
		bodyRadius + bodyCords.y + cabinHeight - 0.4f,
		bodyCords.z
	);

	void createChimney()
	{
		auto coneHeight = 1.15f, radiusCone = 0.66f;
		auto coneCords = glm::vec3(
			bodyCords.x + radiusCone + 0.3,
			cabinCords.y + coneHeight - cabinHeight - 0.3,
			bodyCords.z
		);
		auto cone = std::make_unique<Cone>(
			coneCords,
			coneHeight,
			radiusCone,
			"textures/black-steel.png"
		);

		cone->rotate({ 90.f, 0.f, 0.0 });

		addChild(std::move(cone));

		auto chimneyHeight = 1.15f, radiusChimney = 0.66f;
		auto chimneyCords = glm::vec3(
			bodyCords.x + radiusChimney + 0.3,
			coneCords.y,
			bodyCords.z
		);
		auto chimney = std::make_unique<Cylinder>(chimneyCords,
			chimneyHeight, radiusChimney, bodyPath
		);

		chimney->rotate({ 270.f, 0.f, 0.0 });


		chimneyPtr = chimney.get();
		addChild(std::move(chimney));
	}

	void createBody()
	{
		auto body = std::make_unique<Cylinder>(
			bodyCords,
			bodyWidth, 
			bodyRadius, 
			18, 
			glm::vec3(90.f, 90.f, 0.f),
			bodyPath
		);

		bodyPtr = body.get();
		addChild(std::move(body));

		createChimney();
	}

	void createCabin()
	{
		//CABIN
		auto cabin = std::make_unique<Cube>(
			cabinCords,
			glm::vec3(-cabinLength, cabinHeight, cabinWidth),
			cabinPath
		);

		cabinPtr = cabin.get();
		addChild(std::move(cabin));

		auto windowsCords = glm::vec3(
			cabinCords.x - cabinLength - 0.001f,
			cabinCords.y, 
			cabinCords.z
		);
		auto windows = std::make_unique<Cube>(
			windowsCords,
			glm::vec3(0.0001, cabinHeight, cabinWidth),
			"textures/black-steel-windows.png",
			glm::vec3(-90.f, 0.f, 0.f)
		);

		addChild(std::move(windows));

		// ROOF
		const auto roofWidth = cabinWidth * 1.2f, roofLength = cabinLength * 1.2f, roofHeight = 0.2f;
		const auto roofCords = glm::vec3(cabinCords.x, cabinCords.y + cabinHeight + roofHeight, bodyCords.z);
		auto roof = std::make_unique<Cube>(
			roofCords,
			glm::vec3(-roofLength, roofHeight, roofWidth),
			cabinPath
		);

		roofPtr = roof.get();
		addChild(std::move(roof));
	}
public:

	TrainTop()
	{
		createBody();
		createCabin();
	}


	/**
	 * \brief scales the chimney in upward Y direction
	 * \param amount percentage to scale chimney upwards
	 */
	void scaleChimney(const float amount) const
	{
		chimneyPtr->scale({ 0.f, 0.f, amount });
	}

	/**
	 * \brief scales the body in left(negative) X direction
	 * \param amount percentage to scale body left
	 */
	void scaleBody(const float amount) const
	{
		bodyPtr->scale({ 0.f, 0.f, amount });
		bodyPtr->move({ -amount * bodyWidth , 0.f,0.f });
	}

	/**
	 * \brief scales the cabin in upward Y direction
	 * \param amount percentage to scale cabin upwards
	 */
	void scaleCabin(const float amount) const
	{
		cabinPtr->scale({ 0.f, amount, 0.f });
		cabinPtr->move({ 0.f, cabinHeight* amount,0.f });
		roofPtr->move({ 0.f, cabinHeight * amount * 2,0.f });
	}
};
