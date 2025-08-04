#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "PaddleObject.h"
#include "BallObject.h"
#include "ObjectRenderer.h"
#include "TextRenderer.h"	

enum GameState {
	STATE_MENU,
	STATE_TWO_PLAYER_OPTION,
	STATE_PICK,
	STATE_PLAY,
	STATE_FINISH
};


class Game {
public:

	Game(unsigned int width, unsigned int height);

	GameState mState;

	bool mKeys[1024];
	bool mKeysProcessed[1024];
	unsigned int mWidth, mHeight;

	PaddleObject* mPaddles[2];
	BallObject* mBallObject;

	std::vector<glm::vec2> mBallPositionsPredictions;

	int mFirstPlayerScore;

	int mSecondPlayerScore;

	int mGameWinner;

	int mGameMaxScore;

	int mMenuSelectedOption;

	bool mPause;

	bool mTwoPlayerGame;

	void init();


	void processInput(float deltaTime);

	void update(float deltaTime);

	void render();

	void startGame();

	void checkCollisions();

	void resetBall(int roundWinner);

	void predictTrajectory(BallObject& ball , bool serve = false);


};