#pragma once
#include "pch.h"
#include <vector>

class Clipping
{
public:
	Clipping(Face faceClipSpace);
	~Clipping();

private:
	std::vector<Face> triangle;
};

