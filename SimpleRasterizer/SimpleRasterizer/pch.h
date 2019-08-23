// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include "glm/glm.hpp"

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};


struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 uv;
};

struct Face
{
	Vertex v0;
	Vertex v1;
	Vertex v2;

	Face(Vertex _v0, Vertex _v1, Vertex _v2) :
		v0(_v0), v1(_v1), v2(_v2) {}

	Face(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, Face face)
	{
		v0 = { p0, face.v0.normal, face.v0.uv };
		v1 = { p1, face.v1.normal, face.v1.uv };
		v2 = { p2, face.v2.normal, face.v2.uv };
	}
};

#endif //PCH_H
