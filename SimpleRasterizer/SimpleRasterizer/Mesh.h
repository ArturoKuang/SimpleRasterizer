#pragma once
#include "pch.h"
#include <vector>

class Mesh
{

struct FaceIndex
{
		int pos;
		int uv;
		int normal;
};

struct FaceIndices
{
	FaceIndex f0;
	FaceIndex f1;
	FaceIndex f2;
};

public:
	std::vector<Face> faces;
	std::vector<FaceIndices> indices;

	Mesh(std::string pathName);
	~Mesh();

private:

};

