#pragma once
#include "GL\glew.h"

//BASE ANIMATION CLASS
class Animation
{
public:
	GLuint* sendAnimation();
private:
	GLuint targetVBOs;
};