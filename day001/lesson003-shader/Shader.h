#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//����ID
	unsigned int ID;

	//��������ȡ��������ɫ��
	Shader(const GLchar*vertexPath, const GLchar *fragmentPath);

	//!ʹ�ü������
	void use();

	//uniform ���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	
};

