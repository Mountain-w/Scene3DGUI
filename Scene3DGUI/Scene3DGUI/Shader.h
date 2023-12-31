#pragma once
#include "base.h"
#include <string>

class Shader
{
private:
	unsigned int m_shaderProgram;

public:
	Shader()
	{
		m_shaderProgram = 0;
	}
	~Shader()
	{

	}

	void initShader(const char* _vertexPath, const char* _fragPath);
	void start()
	{
		glUseProgram(m_shaderProgram);
	}
	void end()
	{
		glUseProgram(0);
	}
	void setMatrix(const std::string& _name, glm::mat4 _matrix) const;
	void setI(const std::string& _name, int _val) const;
};

