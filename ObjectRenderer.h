#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
class ObjectRenderer {
public:
	ObjectRenderer(Shader& shader);


	void draw(glm::vec2 position , glm::vec2 size , glm::vec3 color);
private:
	Shader mShader;
	unsigned int VAO;

	void initRenderData();

};