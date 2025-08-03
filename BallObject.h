#pragma once

#include "GameObject.h"

class BallObject : public GameObject {
public:
	BallObject(glm::vec2 size, glm::vec2 velocity, glm::vec2 position, glm::vec3 color) :
		GameObject(size, velocity, position, color) {
	}
};