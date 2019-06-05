#pragma once
#include "pch.h"
#include <vector>

class Clipping
{
public:
	Clipping() {};
	~Clipping() {};

	std::vector<Face> Clip(Face faceClipSpace);
	void ClipEdge(Vertex v0, Vertex v1, std::vector<Vertex> &vertices);



private:
	std::vector<Face> triangle;
};

