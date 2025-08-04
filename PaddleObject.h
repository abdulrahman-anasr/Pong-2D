#pragma once

#include "GameObject.h"

enum Difficulty {
	EASY,
	MEDIUM,
	EXTREME
};

class PaddleObject : public GameObject {
public:
	bool mPlayerControlled;
	Difficulty mAiDifficulty;
	glm::vec2 mAiVelocity;
	float mTargetPositionY;
	PaddleObject(glm::vec2 size, glm::vec2 velocity, glm::vec2 position, glm::vec3 color, bool playerControlled = true, Difficulty aiDifficulty = EASY);

	void adjustVelocityAndPosition(glm::vec2 position, float timeTaken , bool serve);

	void moveTowardsTarget(float deltaTime , int heightLimit);

	void activateCollisionEffect(float maxHeight);
};