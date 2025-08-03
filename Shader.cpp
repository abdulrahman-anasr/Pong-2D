#include "Shader.h"

void Shader::compile(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
{
	unsigned int vShader, fShader, gShader;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexCode, NULL);
	glCompileShader(vShader);
	checkCompilationErrors(vShader, "VERTEX");

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentCode, NULL);
	glCompileShader(fShader);
	checkCompilationErrors(fShader, "FRAGMENT");

	if (geometryCode != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometryCode, NULL);
		glCompileShader(gShader);
		checkCompilationErrors(gShader, "GEOMETRY");
	}

	this->mID = glCreateProgram();
	glAttachShader(this->mID, vShader);
	glAttachShader(this->mID, fShader);
	if (geometryCode != nullptr) glAttachShader(this->mID, gShader);
	glLinkProgram(this->mID);
	checkCompilationErrors(this->mID, "PROGRAM");

}

Shader& Shader::use()
{
	glUseProgram(this->mID);
	return *this;
}

void Shader::setInteger(GLint value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(GLfloat value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(glm::vec2 value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(glm::vec3 value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(glm::vec4 value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat4(glm::mat4 value, std::string name, bool useShader)
{
	if (useShader) glUseProgram(this->mID);

	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

/*									PRIVATE FUNCTIONS								*/

GLint Shader::getUniformLocation(std::string name)
{
	return glGetUniformLocation(this->mID, name.c_str());
}

void Shader::checkCompilationErrors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type == "PROGRAM")
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "ERROR::PROGRAM: Compile-time error: " << type << " " << infoLog << "\n" <<
				"---------------------------------------------------------------------" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER: Link-time error: " << type << " " << infoLog << "\n" <<
				"---------------------------------------------------------------------" << std::endl;
		}
	}
}
