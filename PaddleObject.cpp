#include "PaddleObject.h"

PaddleObject::PaddleObject(glm::vec2 size, glm::vec2 velocity, glm::vec2 position, glm::vec3 color, bool playerControlled
	, Difficulty aiDifficulty)
	: GameObject(size, velocity, position, color), mPlayerControlled(playerControlled),
	mAiDifficulty(aiDifficulty), mAiVelocity(glm::vec2(0.0f)) {}



bool chance(unsigned int odds)
{
	int random = rand() % odds;
	return random == 0;
}
void PaddleObject::adjustVelocityAndPosition(glm::vec2 position, float timeTaken , bool serve)
{
	mTargetPositionY = position.y;

	int paddleSlashedWidth = static_cast<float>(this->mSize.y) / 1.4f;
	int range = paddleSlashedWidth + paddleSlashedWidth + 1;
	int randomOffset = rand() % range + (-paddleSlashedWidth);


	float distanceNeededToMove = position.y - (this->mPosition.y + (this->mSize.y / 2));


	distanceNeededToMove += static_cast<float>(randomOffset);


	bool mistake = false;
	if (!serve)
	{
		mistake = chance(8);
	}

	if (mistake)
	{
		int paddleWidth = this->mSize.y;
		range = paddleWidth * 2 + 1;
		randomOffset = (rand() % range + (-paddleWidth) );
		distanceNeededToMove += randomOffset;

		mTargetPositionY = this->mPosition.y + distanceNeededToMove;
	}

	float paddleThirdWidth = this->mSize.y / 3.0f;

	if (paddleThirdWidth > std::abs(distanceNeededToMove))
	{
		mAiVelocity.y = 0;
	}
	else
	{
		
		if (mistake)
		{
			mAiVelocity.y = distanceNeededToMove / (timeTaken / 7.0f);
		}
		else
		{
			mAiVelocity.y = distanceNeededToMove / (timeTaken / 9.0f);
		}
	}
}

void PaddleObject::moveTowardsTarget(float deltaTime , int heightLimit)
{

	float paddleCenterPositionY = this->mPosition.y + (this->mSize.y / 2);
	float paddleHalfSize = this->mSize.y / 2.0f;
	if (!(std::abs(paddleCenterPositionY - mTargetPositionY) < 1.0f))
	{
		glm::vec2 distanceMoved = mAiVelocity * deltaTime;
		if (( (this->mAiVelocity.y < 0.0f && mTargetPositionY > this->mPosition.y) ||
			(this->mAiVelocity.y > 0.0f && mTargetPositionY < this->mPosition.y) ) &&
			((std::abs(paddleCenterPositionY - mTargetPositionY) > paddleHalfSize))
			)
		{
			this->mAiVelocity.y = -this->mAiVelocity.y;
			this->mAiVelocity /= 2.0;
		}

		this->mPosition += distanceMoved;

		if (this->mPosition.y < 0.0f)
		{
			this->mPosition.y = 0.0f;
		}
		if ((this->mPosition.y) > (heightLimit - this->mSize.y))
		{
			this->mPosition.y = heightLimit - this->mSize.y;
		}
	}
	else if (!(std::abs(paddleCenterPositionY - mTargetPositionY) < 20.0f))
	{
		float distanceNeededToCover = mTargetPositionY - paddleCenterPositionY;
		float adjustedVelocity = distanceNeededToCover / 0.5f;

		this->mAiVelocity.y = adjustedVelocity;
	}
	
}

void PaddleObject::activateCollisionEffect(float maxHeight)
{
	maxHeight = maxHeight - this->mSize.y;
	
	float y = rand() % static_cast<int>(maxHeight);

	float difference = y - this->mPosition.y;

	float movementSpeed = difference * 0.8f;

	this->mAiVelocity.y = movementSpeed;

	mTargetPositionY = y;
}