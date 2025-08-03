#include "Game.h"


ResourceManager* instance;
ObjectRenderer* renderer;
TextRenderer* textRenderer;

extern GLFWwindow* window;

typedef std::tuple<bool, float> Collision;

const glm::vec2 PLAYER_SIZE(20.0f, 130.0f);
const glm::vec2 PLAYER_VELOCITY(0.0f, 800.0f);

const glm::vec2 BALL_SIZE(20.0f, 20.0f);
const glm::vec2 BALL_VELOCITY(-400.0f, -450.0f);

Collision checkCollision(PaddleObject& paddle, BallObject& ball)
{
	bool xCollision = paddle.mPosition.x + paddle.mSize.x >= ball.mPosition.x
		&& ball.mPosition.x + ball.mSize.x >= paddle.mPosition.x;
	bool yCollision = paddle.mPosition.y + paddle.mSize.y >= ball.mPosition.y && ball.mPosition.y + ball.mSize.y >= paddle.mPosition.y;

	if (xCollision && yCollision)
	{
		if (ball.mVelocity.x < 0.0f)
		{
			float difference = (paddle.mPosition.x + paddle.mSize.x) - ball.mPosition.x;
			return std::make_tuple(true, difference);
		}
		else
		{
			float difference = (ball.mPosition.x + ball.mSize.x) - paddle.mPosition.x;
			return std::make_tuple(true, difference);
		}
	}

	return std::make_tuple(false, 0.0f);
}


Game::Game(unsigned int width, unsigned int height) :
	mWidth(width), mHeight(height), mKeys(), mKeysProcessed(), mState(STATE_MENU),
	mFirstPlayerScore(0) , mSecondPlayerScore(0) , mGameWinner(-1) , mMenuSelectedOption(0) , mGameMaxScore(0) ,
	mBallObject(nullptr)
{
}

void Game::init()
{
	instance = ResourceManager::getInstance();

	instance->loadShader("shader", "object.vs", "object.fs", nullptr);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f , -1.0f , 1.0f);
	instance->getShader("shader").setMat4(projection, "projection" , true);

	renderer = new ObjectRenderer(instance->getShader("shader"));

	textRenderer = new TextRenderer(mWidth, mHeight);

	textRenderer->load("fonts/pong.ttf", 92);

}

void Game::update(float deltaTime)
{
	if (mState == STATE_PLAY)
	{
		glm::vec2 ballVelocity = mBallObject->mVelocity * deltaTime;


		mBallObject->mPosition += ballVelocity;

		if (mBallObject->mPosition.x > (mWidth - mBallObject->mSize.x))
		{
			mFirstPlayerScore++;
			resetBall(1);
		}
		else if (mBallObject->mPosition.x < 0)
		{
			mSecondPlayerScore++;
			resetBall(2);
		}


		checkCollisions();
	}
}


void Game::checkCollisions()
{

	float strength = 2.5f;

	glm::vec2 oldVelocity = mBallObject->mVelocity;
	float constantPower = glm::length(oldVelocity);


	if (mBallObject->mPosition.y < 0.0f)
	{
		mBallObject->mPosition.y = 0.0f;
		mBallObject->mVelocity.y = -mBallObject->mVelocity.y;
	}
	else if (mBallObject->mPosition.y > (mHeight - mBallObject->mSize.y))
	{
		mBallObject->mPosition.y = mHeight - mBallObject->mSize.y;
		mBallObject->mVelocity.y = -mBallObject->mVelocity.y;
	}
	if (mBallObject->mVelocity.x < 0.0f)
	{
		Collision firstPaddleBallCollision = checkCollision(*mPaddles[0], *mBallObject);
		if (std::get<0>(firstPaddleBallCollision))
		{

			float paddleCenter = mPaddles[0]->mPosition.y + mPaddles[0]->mSize.y / 2.0f;
			float difference = (mBallObject->mPosition.y + (mBallObject->mSize.y / 2.0f)) - paddleCenter;



			float percentage = difference / static_cast<float>(mPaddles[0]->mSize.y / 2.0f);

			mBallObject->mVelocity.y = std::abs(mBallObject->mVelocity.y) * (percentage * strength);

			mBallObject->mVelocity.x = -mBallObject->mVelocity.x;

			mBallObject->mPosition.x += std::get<1>(firstPaddleBallCollision);
		}
	}
	else
	{
		Collision secondPaddleBallCollision = checkCollision(*mPaddles[1], *mBallObject);

		if (std::get<0>(secondPaddleBallCollision))
		{
			float paddleCenter = mPaddles[1]->mPosition.y + mPaddles[1]->mSize.y / 2.0f;
			float difference = (mBallObject->mPosition.y + (mBallObject->mSize.y / 2.0f)) - paddleCenter;

			float percentage = difference / static_cast<float>(mPaddles[1]->mSize.y / 2.0f);

			mBallObject->mVelocity.y = std::abs(mBallObject->mVelocity.y) * (percentage * strength);

			mBallObject->mVelocity.x = -mBallObject->mVelocity.x;

			mBallObject->mPosition.x += std::get<1>(secondPaddleBallCollision);
		}
	}

	if (mBallObject->mVelocity.y < 0.0f)
	{
		if (mBallObject->mVelocity.y > -std::abs(BALL_VELOCITY.y) + 200.0f)
		{
			mBallObject->mVelocity.y = -std::abs(BALL_VELOCITY.y);
		}
		else if (mBallObject->mVelocity.y < -std::abs(BALL_VELOCITY.y) * 2.7f)
		{
			mBallObject->mVelocity.y = -std::abs(BALL_VELOCITY.y) * 2.7f;
		}
	}
	else
	{
		if (mBallObject->mVelocity.y < std::abs(BALL_VELOCITY.y) - 200.0f)
		{
			mBallObject->mVelocity.y = std::abs(BALL_VELOCITY.y);
		}
		else if (mBallObject->mVelocity.y > std::abs(BALL_VELOCITY.y) * 2.7f)
		{
			mBallObject->mVelocity.y = std::abs(BALL_VELOCITY.y) * 2.7f;
		}
	}
}

void Game::render()
{
	if (mState == STATE_MENU)
	{
		textRenderer->renderText("Pong", (mWidth / 2.0f) - 260.0f, (mHeight / 2.0f) - 200.0f, 3.0f);

		glm::vec3 startColor = mMenuSelectedOption == 0 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 quitColor = mMenuSelectedOption == 1 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		textRenderer->renderText("Start", (mWidth / 2.0f) - 120.0f, (mHeight / 2.0f) + 50.0f, 1.0f , startColor);
		textRenderer->renderText("Quit", (mWidth / 2.0f) - 100.0f, (mHeight / 2.0f) + 150.0f, 1.0f , quitColor);
	}
	if (mState == STATE_PICK)
	{
		textRenderer->renderText("Choose the Score", (mWidth / 2.0f) - 370.0f, (mHeight / 2.0f) - 200.0f, 1.0f);

		glm::vec3 fiveOption = mMenuSelectedOption == 0 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 sevenOption = mMenuSelectedOption == 1 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 nineOption = mMenuSelectedOption == 2 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 elevenOption = mMenuSelectedOption == 3 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);

		textRenderer->renderText("5", (mWidth / 2.0f) - 220.0f, (mHeight / 2.0f) , 1.0f , fiveOption);

		textRenderer->renderText("7", (mWidth / 2.0f) - 110.0f, (mHeight / 2.0f), 1.0f , sevenOption);

		textRenderer->renderText("9", (mWidth / 2.0f), (mHeight / 2.0f), 1.0f , nineOption);

		textRenderer->renderText("11", (mWidth / 2.0f) + 110.0f, (mHeight / 2.0f), 1.0f , elevenOption);
	}
	if (mState == STATE_PLAY || mState == STATE_FINISH)
	{
		unsigned int mUnitHeight = mHeight / 16;

		if (mState == STATE_PLAY)
		{
			for (unsigned int i = 0; i < 8; i++)
			{
				renderer->draw(glm::vec2((mWidth / 2.0f) - 10, 30 + (2 * i * mUnitHeight)), glm::vec2(10, mUnitHeight), glm::vec3(1.0f));
			}
		}

		mPaddles[0]->draw(*renderer);
		mPaddles[1]->draw(*renderer);

		mBallObject->draw(*renderer);

		std::string firstScore = std::to_string(mFirstPlayerScore);
		std::string secondScore = std::to_string(mSecondPlayerScore);

		textRenderer->renderText(firstScore, (mWidth / 2) - 120, 10, 2.0);
		textRenderer->renderText(secondScore, (mWidth / 2) + 40, 10, 2.0);
	}
	if (mState == STATE_FINISH)
	{
		std::string winMessage = "Player " + std::to_string(mGameWinner) + " Wins!!";
		textRenderer->renderText(winMessage, (mWidth / 2) - 320.0f, (mHeight / 2.0f) - 150.0f, 1.0);

		glm::vec3 againOption = mMenuSelectedOption == 0 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 menuOption = mMenuSelectedOption == 1 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);
		glm::vec3 exitOption = mMenuSelectedOption == 2 ? glm::vec3(0.0f, 1.0f, 0.0) : glm::vec3(1.0f);

		textRenderer->renderText("Restart", (mWidth / 2.0f) - 160.0f, (mHeight / 2.0f) + 50.0f, 1.0f, againOption);
		textRenderer->renderText("Menu", (mWidth / 2.0f) - 100.0f, (mHeight / 2.0f) + 150.0f, 1.0f, menuOption);
		textRenderer->renderText("Quit", (mWidth / 2.0f) - 100.0f, (mHeight / 2.0f) + 250.0f, 1.0f, exitOption);
	}
}

void Game::resetBall(int roundWinner)
{
	if (mFirstPlayerScore == mGameMaxScore)
	{
		mBallObject->mPosition = glm::vec2(-100, -100);
		mState = STATE_FINISH;
		mGameWinner = 1; 
		return;
	}
	if (mSecondPlayerScore == mGameMaxScore)
	{
		mBallObject->mPosition = glm::vec2(-100, -100);
		mState = STATE_FINISH;
		mGameWinner = 2;
		return;
	}

	mBallObject->mPosition = glm::vec2(mWidth / 2, mHeight / 2);

	mBallObject->mVelocity = BALL_VELOCITY;

	if (roundWinner == 1)
	{
		if (mPaddles[1]->mPosition.y > (mHeight / 2.0f))
		{
			mBallObject->mVelocity.y = -mBallObject->mVelocity.y;
		}
		mBallObject->mVelocity.x = -mBallObject->mVelocity.x;
	}
	else
	{
		if (mPaddles[0]->mPosition.y > (mHeight / 2.0f))
		{
			mBallObject->mVelocity.y = -mBallObject->mVelocity.y;
		}
	}
}
void Game::processInput(float deltaTime)
{
	if (mState == STATE_MENU)
	{
		if (mKeys[GLFW_KEY_W] && !mKeysProcessed[GLFW_KEY_W])
		{
			mKeysProcessed[GLFW_KEY_W] = true;
			if (mMenuSelectedOption > 0)
			{
				mMenuSelectedOption--;
			}
			else
			{
				mMenuSelectedOption = 1;
			}
		}
		if (mKeys[GLFW_KEY_S] && !mKeysProcessed[GLFW_KEY_S])
		{
			mKeysProcessed[GLFW_KEY_S] = true;
			mMenuSelectedOption = (mMenuSelectedOption + 1) % 2;
		}

		if (mKeys[GLFW_KEY_ENTER] && !mKeysProcessed[GLFW_KEY_ENTER])
		{
			if (mMenuSelectedOption == 0)
			{
				mState = STATE_PICK;
				mMenuSelectedOption = 0;
				mKeysProcessed[GLFW_KEY_ENTER] = true;
			}
			else
			{
				glfwSetWindowShouldClose(window, true);
			}

		}
	}
	if (mState == STATE_PICK)
	{
		if (mKeys[GLFW_KEY_D] && !mKeysProcessed[GLFW_KEY_D])
		{
			mMenuSelectedOption = (mMenuSelectedOption + 1) % 4;
			mKeysProcessed[GLFW_KEY_D] = true;
		}
		if (mKeys[GLFW_KEY_A] && !mKeysProcessed[GLFW_KEY_A])
		{
			if (mMenuSelectedOption > 0)
			{
				mMenuSelectedOption--;
			}
			else
			{
				mMenuSelectedOption = 3;
			}
			mKeysProcessed[GLFW_KEY_A] = true;
		}
		if (mKeys[GLFW_KEY_ENTER] && !mKeysProcessed[GLFW_KEY_ENTER])
		{
			mGameMaxScore = mMenuSelectedOption == 0 ? 5 
				: mMenuSelectedOption == 1 ? 7 : mMenuSelectedOption == 2 ? 9 : 11;
			mMenuSelectedOption = 0;
			startGame();
		}
	}
	if (mState == STATE_PLAY)
	{
		float firstPlayerVelocity = mPaddles[0]->mVelocity.y * deltaTime;
		float secondPlayerVelocity = mPaddles[1]->mVelocity.y * deltaTime;
		if (mKeys[GLFW_KEY_W])
		{
			if (mPaddles[0]->mPosition.y < 0.0f)
			{
				mPaddles[0]->mPosition.y = 0.0f;
			}
			else
			{
				mPaddles[0]->mPosition.y -= firstPlayerVelocity;
			}
		}
		if (mKeys[GLFW_KEY_S])
		{
			if (mPaddles[0]->mPosition.y > mHeight - mPaddles[0]->mSize.y)
			{
				mPaddles[0]->mPosition.y = mHeight - mPaddles[0]->mSize.y;
			}
			else
			{
				mPaddles[0]->mPosition.y += firstPlayerVelocity;
			}
		}

		if (mKeys[GLFW_KEY_UP])
		{
			if (mPaddles[1]->mPosition.y < 0.0f)
			{
				mPaddles[1]->mPosition.y = 0.0f;
			}
			else
			{
				mPaddles[1]->mPosition.y -= secondPlayerVelocity;
			}
		}

		if (mKeys[GLFW_KEY_DOWN])
		{
			if (mPaddles[1]->mPosition.y > mHeight - mPaddles[1]->mSize.y)
			{
				mPaddles[1]->mPosition.y = mHeight - mPaddles[1]->mSize.y;
			}
			else
			{
				mPaddles[1]->mPosition.y += secondPlayerVelocity;
			}
		}
	}
	if (mState == STATE_FINISH)
	{
		if (mKeys[GLFW_KEY_S] && !mKeysProcessed[GLFW_KEY_S])
		{
			mKeysProcessed[GLFW_KEY_S] = true;
			mMenuSelectedOption = (mMenuSelectedOption + 1) % 3;
		}
		if (mKeys[GLFW_KEY_W] && !mKeysProcessed[GLFW_KEY_W])
		{
			mKeysProcessed[GLFW_KEY_W] = true;
			if (mMenuSelectedOption > 0)
			{
				mMenuSelectedOption--;
			}
			else
			{
				mMenuSelectedOption = 2;
			}
		}
		if (mKeys[GLFW_KEY_ENTER] && !mKeysProcessed[GLFW_KEY_ENTER])
		{
			mKeysProcessed[GLFW_KEY_ENTER] = true;
			if (mMenuSelectedOption == 0)
			{
				mState = STATE_PLAY;
				startGame();
			}
			else if (mMenuSelectedOption == 1)
			{
				mState = STATE_MENU;
			}
			else if (mMenuSelectedOption == 2)
			{
				glfwSetWindowShouldClose(window, true);
			}
			mMenuSelectedOption = 0;
		}
	}

}

void Game::startGame()
{
	if (mPaddles[0] == nullptr) delete mPaddles[0];
	if (mPaddles[1] == nullptr) delete mPaddles[1];
	if (mBallObject == nullptr) delete mBallObject;

	mFirstPlayerScore = 0;
	mSecondPlayerScore = 0;
	mPaddles[0] = new PaddleObject(PLAYER_SIZE, PLAYER_VELOCITY, glm::vec2(0.0f, (mHeight - PLAYER_SIZE.y) / 2), glm::vec3(1.0f));
	mPaddles[1] = new PaddleObject(PLAYER_SIZE, PLAYER_VELOCITY, glm::vec2(mWidth - PLAYER_SIZE.x, (mHeight - PLAYER_SIZE.y) / 2), glm::vec3(1.0f));

	mBallObject = new BallObject(BALL_SIZE, BALL_VELOCITY, glm::vec2(mWidth / 2, mHeight / 2), glm::vec3(1.0f));

	mState = STATE_PLAY;
}