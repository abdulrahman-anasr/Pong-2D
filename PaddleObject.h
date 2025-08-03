#pragma once

#include "GameObject.h"


class PaddleObject : public GameObject {
public:
	bool mPlayerControlled;
	PaddleObject(glm::vec2 size, glm::vec2 velocity , glm::vec2 position , glm::vec3 color, bool playerControlled = true) : GameObject(size , velocity , position , color) , mPlayerControlled(playerControlled) {}
};