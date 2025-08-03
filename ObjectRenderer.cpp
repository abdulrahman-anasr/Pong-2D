#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(Shader& shader)
{
	this->mShader = shader;
	initRenderData();
}

void ObjectRenderer::initRenderData()
{
	unsigned int VBO;
	float vertices[] = {
		// pos
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	glGenVertexArrays(1 , &this->VAO);
	glBindVertexArray(this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ObjectRenderer::draw(glm::vec2 position, glm::vec2 size, glm::vec3 color)
{

	this->mShader.use();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position , 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	mShader.setMat4(model , "model");
	mShader.setVec3(color, "objectColor");

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}