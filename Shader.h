#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int mID;
	Shader() = default;

	void compile(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
	Shader& use();

	

	void setInteger(GLint value, std::string name, bool useShader = false);

	void setFloat(GLfloat value, std::string name, bool useShader = false);

	void setVec2(glm::vec2 value, std::string name , bool useShader = false);

	void setVec3(glm::vec3 value, std::string naem, bool useShader = false);

	void setVec4(glm::vec4 value, std::string name, bool useShader = false);

	void setMat4(glm::mat4 value, std::string name, bool useShader = false);

private:
	GLint getUniformLocation(std::string name);

	void checkCompilationErrors(unsigned int object, std::string type);
};