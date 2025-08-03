#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "ObjectRenderer.h"

class GameObject {
public:
	glm::vec2 mSize;
	glm::vec2 mVelocity;
	glm::vec2 mPosition;
	glm::vec3 mColor;


	virtual void draw(ObjectRenderer& renderer);
	
	GameObject() = default;
	GameObject(glm::vec2 size, glm::vec2 velocity, glm::vec2 position, glm::vec3 color = glm::vec3(1.0f));
};