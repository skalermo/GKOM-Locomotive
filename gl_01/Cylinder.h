#pragma once

#include <GL/glew.h>
#include "Object.h"
#include "Composite.h"

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 25

/*
Unit vector
*/
#define DEFAULT_VECTOR glm::vec3(1.0f, 0.0f, 0.0f)

class Cylinder : Object
{
private:
	/*
	Center of the base/bottom plane
	*/
	glm::vec3 coordinates;
	GLfloat height;
	GLfloat radius;

	/*
	Defines "smoothness" of a cylinder
	*/
	GLuint segments;

	/*
	Vector pointing at the opposite top plane. Allows to rotate cylinder
	*/
	glm::vec3 direction;

	std::unique_ptr<GLfloat> vertices;
	std::unique_ptr<GLuint> indices;

public:

	Cylinder() :
		coordinates(DEFAULT_VECTOR),
		height(10),
		radius(5),
		segments(DEFAULT_SEGMENTS_NUMBER),
		direction(DEFAULT_VECTOR) {}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r) :
		coordinates(coords),
		height(h),
		radius(r),
		segments(DEFAULT_SEGMENTS_NUMBER),
		direction(DEFAULT_VECTOR) {}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r, GLuint s, glm::vec3 dir):
		coordinates(coords),
		height(h),
		radius(r),
		segments(s),
		direction(dir) {}

	~Cylinder() {}

	virtual void draw()
	{
		//todo
	}
	
	virtual void move(glm::vec3 displacement)
	{
		this->coordinates += displacement;
		//todo
	}
};
