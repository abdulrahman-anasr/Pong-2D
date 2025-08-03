#include "GameObject.h"

GameObject::GameObject(glm::vec2 size, glm::vec2 velocity, glm::vec2 position, glm::vec3 color)
	: mSize(size), mVelocity(velocity), mPosition(position), mColor(color)
{

}

void GameObject::draw(ObjectRenderer& renderer)
{
	renderer.draw(mPosition, mSize, mColor);
}