#pragma once
#include "pch.h"
#include <vector>

class Textures
{
public:
	Textures(std::string pathName);
	~Textures();

private:
	int width;
	int height;
	std::vector<Color> pixels;
};

 