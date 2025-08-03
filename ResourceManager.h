#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class ResourceManager {
public:
	static ResourceManager* instance;

	Shader loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	Shader& getShader(std::string name);

	void clear();

	static ResourceManager* getInstance();
private:
	std::map<std::string, Shader> mShaders;

	ResourceManager() = default;

	static Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
};