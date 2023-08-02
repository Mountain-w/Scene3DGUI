#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <pcl\io\pcd_io.h>
#include <vector>
#include <memory>
#include <imgui.h>
#include <backends\imgui_impl_glfw.h>
#include <backends\imgui_impl_opengl3.h>


typedef unsigned int uint;
typedef unsigned char byte;

struct ffRGBA
{
	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;

	ffRGBA() {};
	ffRGBA(byte r, byte g, byte b, byte a) : m_r(r), m_g(g), m_b(b), m_a(a) {};
};