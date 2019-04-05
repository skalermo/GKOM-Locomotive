#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <exception>
#include "Object.h"
/*
	This is a class for obejcts that can contain more than one
	Objects. E.g. train can have top part and bottom part 
	so train class should inherit from Composite.

	Tip:
		if you want to add element to a composite use the following line
			addChild(std::make_unique<some_derived_from_object_class>(constructor_args...));
		or if you have created object before:
			auto some_object = make_unique<some_class>(args...);
			...
			addChild(std::move(some_object));
*/
class Composite : public Object {
public:
	virtual void draw() {
		for (auto& child : children)
			child->draw();
	}
	virtual void move(glm::vec3 displacement) {
		for (auto& child : children)
			child->move(displacement);
	}

	void addChild(std::unique_ptr<Object>&& child) {
		children.push_back(std::move(child));
	}
	
	void removeChild(int index) {
		if (children.size() <= index)
			throw std::exception("Children index out of range");
		children.erase(children.begin() + index);
	}

	virtual ~Composite() {}

private:
	std::vector<std::unique_ptr<Object>> children;
};
