#pragma once
#include <glm/glm.hpp>
/*
	Base primitive class for the composite.
*/
class Object {
public:
	/*
	This method should all the things to display the object to the screen
	e.g. biding Array Objects, using glDraw... and unbinding the arrays
	*/
	virtual void draw() = 0;

	/*
	Move should only update the state of the vertecies accorging to the 3d
	displacement specified by argument
	*/
	virtual void move(glm::vec3 displacement) = 0;

	virtual void scale(glm::vec3 amount) = 0;

	virtual void rotate(const glm::vec3& displacement) = 0;
	virtual ~Object() {}
};
