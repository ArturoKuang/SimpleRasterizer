#include "pch.h"
#include "Clipping.h"

std::vector<Face> Clipping::Clip(Face faceClipSpace)
{
	std::vector<Face> triangles;

	//All vertices are in front of the camera and inside the frustrum
	if (faceClipSpace.v0.position.w > 0.0f
		&& faceClipSpace.v1.position.w > 0.0f
		&& faceClipSpace.v2.position.w > 0.0f
		&& glm::abs(faceClipSpace.v0.position.z) < faceClipSpace.v0.position.w
		&& glm::abs(faceClipSpace.v1.position.z) < faceClipSpace.v1.position.w
		&& glm::abs(faceClipSpace.v2.position.z) < faceClipSpace.v2.position.w)
	{
		triangles.push_back(faceClipSpace);
	}
	else
	{
		//clip each edge accumulating vertices 
		std::vector<Vertex> vertices;
		ClipEdge(faceClipSpace.v0, faceClipSpace.v1, vertices);
		ClipEdge(faceClipSpace.v1, faceClipSpace.v2, vertices);
		ClipEdge(faceClipSpace.v2, faceClipSpace.v0, vertices);

		//not enough to form triangular face
		if (vertices.size() < 3)
		{
			return {};
		}
		//potentially have duplicate at the end
		if (vertices.end == vertices.front)
		{
			vertices.pop_back();
		}
		//generate fan of triangles, all sharing the first vertex
		for (size_t i = 1; i < vertices.size() - 1; i++)
		{
			triangles.push_back(Face(vertices[0], vertices[i], vertices[i + 1]));
		}

		return triangles;
	}
}

void Clipping::ClipEdge(Vertex v0, Vertex v1, std::vector<Vertex>& vertices)
{
	auto v0New = v0;
	auto v1New = v1;

	auto v0Inside = v0.position.w > 0.0 && v0.position.z > -v0.position.w;
	auto v1Inside = v1.position.w > 0.0 && v1.position.z > -v1.position.w;

	if (v0Inside && v1Inside) {}
	else if (v0Inside || v1Inside)
	{
		//compute interpolation coefficients
		float d0 = v0.position.z + v0.position.w;
		float d1 = v1.position.z + v1.position.w;
		float factor = 1.0 / (d1 - d0);

		Vertex newVertex = {
			factor * (d1 * v0.position - d0 * v1.position),
			factor * (d1 * v0.normal - d0 * v1.normal),
			factor * (d1 * v0.uv - d0 * v1.uv)
		};

		if (v0Inside)
		{
			v1New = newVertex;
		}
		else
		{
			v0New = newVertex;
		}
	}
	else {
		//both outside, remove edge
		return;
	}

	//Add first vertex if not already added
	if (vertices.size == 0 || (vertices.end != v0New))
	{
		vertices.push_back(v0New);
	}
	vertices.push_back(v1New);
}

bool Clipping::CullFace(Face faceNormalizedSpace)
{
	//cross product v1-v0 x v2-v0 to get orientation
	//in comparison to camera
	float d =
		(faceNormalizedSpace.v1.position.x - faceNormalizedSpace.v0.position.x) *
		(faceNormalizedSpace.v2.position.y - faceNormalizedSpace.v0.position.y) -
		(faceNormalizedSpace.v1.position.y - faceNormalizedSpace.v0.position.y) *
		(faceNormalizedSpace.v2.position.x - faceNormalizedSpace.v0.position.x);

	return d < 0.0f;
}



