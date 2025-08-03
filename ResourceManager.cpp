#include "ResourceManager.h"


ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}

	return instance;
}

Shader ResourceManager::loadShader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	mShaders[name] = loadShaderFromFile(vertexPath, fragmentPath, geometryPath);
	return mShaders[name];
}

Shader& ResourceManager::getShader(std::string name)
{
	return mShaders[name];
}

Shader ResourceManager::loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode, fragmentCode, geometryCode;
	try
	{
		std::ifstream vertexShaderFile(vertexPath);
		std::ifstream fragmentShaderFile(fragmentPath);

		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexShaderFile.rdbuf();
		fragmentStream << fragmentShaderFile.rdbuf();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		if (geometryPath != nullptr)
		{
			std::ifstream geometryShaderFile(geometryPath);
			std::stringstream geometryStream;
			geometryStream << geometryShaderFile.rdbuf();
			geometryCode = geometryStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::RESOURCEMANAGER: FAILED TO READ SHADER FILES" << std::endl;
	}

	const char* vCode = vertexCode.c_str();
	const char* fCode = fragmentCode.c_str();
	const char* gCode = geometryPath == nullptr ? nullptr : geometryCode.c_str();
	Shader shader;
	shader.compile(vCode, fCode, gCode);

	return shader;
}

