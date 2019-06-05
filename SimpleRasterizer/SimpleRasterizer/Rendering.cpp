#include "pch.h"
#include "Rendering.h"

Face Rendering::WorldSpaceToClipSpace(Face faceModelSpace, glm::mat4x4 mvp, glm::mat4x4 lightMatrix)
{
	Vertex v0 = {
		mvp * faceModelSpace.v0.position,
		mvp * faceModelSpace.v0.normal,
		faceModelSpace.v0.uv
	};

	Vertex v1 = {
		mvp * faceModelSpace.v1.position,
		mvp * faceModelSpace.v1.normal,
		faceModelSpace.v1.uv
	};

	Vertex v2 = {
		mvp * faceModelSpace.v2.position,
		mvp * faceModelSpace.v2.normal,
		faceModelSpace.v2.uv
	};

	return Face(v0, v1, v2);
}

Face Rendering::PerspectiveDivide(Face faceClipSpace)
{
	glm::vec4 p0NormalizedSpace = glm::vec4(
		faceClipSpace.v0.position.x / faceClipSpace.v0.position.w,
		faceClipSpace.v0.position.y / faceClipSpace.v0.position.w,
		faceClipSpace.v0.position.z / faceClipSpace.v0.position.w,
		faceClipSpace.v0.position.w
	);

	glm::vec4 p1NormalizedSpace = glm::vec4(
		faceClipSpace.v1.position.x / faceClipSpace.v1.position.w,
		faceClipSpace.v1.position.y / faceClipSpace.v1.position.w,
		faceClipSpace.v1.position.z / faceClipSpace.v1.position.w,
		faceClipSpace.v1.position.w
	);

	glm::vec4 p2NormalizedSpace = glm::vec4(
		faceClipSpace.v2.position.x / faceClipSpace.v2.position.w,
		faceClipSpace.v2.position.y / faceClipSpace.v2.position.w,
		faceClipSpace.v2.position.z / faceClipSpace.v2.position.w,
		faceClipSpace.v2.position.w
	);

	return Face(p0NormalizedSpace, p1NormalizedSpace, p2NormalizedSpace, faceClipSpace);
}

Face Rendering::NormalizedSpaceToScreenSpace(Face faceNormalizedSpace, float width, float height)
{
	glm::vec4 p0ScreenSpace = glm::vec4(
		glm::floor(0.5f * width * (faceNormalizedSpace.v0.position.x + 1.0f)),
		glm::floor(0.5f * height * (faceNormalizedSpace.v0.position.y + 1.0f)),
		faceNormalizedSpace.v0.position.z,
		faceNormalizedSpace.v0.position.w
	);

	glm::vec4 p1ScreenSpace = glm::vec4(
		glm::floor(0.5f * width * (faceNormalizedSpace.v1.position.x + 1.0f)),
		glm::floor(0.5f * height * (faceNormalizedSpace.v1.position.y + 1.0f)),
		faceNormalizedSpace.v1.position.z,
		faceNormalizedSpace.v1.position.w
	);

	glm::vec4 p2ScreenSpace = glm::vec4(
		glm::floor(0.5f * width * (faceNormalizedSpace.v2.position.x + 1.0f)),
		glm::floor(0.5f * height * (faceNormalizedSpace.v2.position.y + 1.0f)),
		faceNormalizedSpace.v2.position.z,
		faceNormalizedSpace.v2.position.w
	);

	return Face(p0ScreenSpace, p1ScreenSpace, p2ScreenSpace, faceNormalizedSpace);
}

void Rendering::Draw(Face fScreen, std::vector<Color>& frameBuffer)
{
}

glm::vec2* Rendering::BoundingBox(Face vs, float width, float height)
{
	glm::vec2 boundingBoxPoints[2];

	//work only in x,y plane
	glm::vec2 v0ScreenSpace = glm::vec2(vs.v0.position.x, vs.v0.position.y);
	glm::vec2 v1ScreenSpace = glm::vec2(vs.v1.position.x, vs.v1.position.y);
	glm::vec2 v2ScreenSpace = glm::vec2(vs.v2.position.x, vs.v2.position.y);

	//find the min and max points
	auto mini = glm::min(glm::min(v0ScreenSpace, v1ScreenSpace), v2ScreenSpace);
	auto maxi = glm::max(glm::max(v0ScreenSpace, v1ScreenSpace), v2ScreenSpace);
	//frame buffer bounds
	auto lim = glm::vec2(width - 1, height - 1);
	//clamp bounding box again the framebuffer
	auto finalMin = glm::clamp(glm::min(mini, maxi), glm::vec2(0.0f, 0.0f), lim);
	auto finalMax = glm::clamp(glm::max(mini, maxi), glm::vec2(0.0f, 0.0f), lim);

	boundingBoxPoints[0] = finalMin;
	boundingBoxPoints[1] = finalMax;

	return boundingBoxPoints;
}

glm::vec3 Rendering::Barycentre(glm::vec2 point, glm::vec4 v0, glm::vec4 v1, glm::vec4 v2)
{
	//v0 will be origin 
	//ab and ac will be basis vectors
	auto ab = v1 - v0;
	auto ac = v2 - v0;

	auto pa = glm::vec2(v0.x, v0.y) - point;
	auto uv1 = glm::cross(glm::vec3(ac.x, ab.x, pa.x), glm::vec3(ac.y, ab.y, pa.y));
	if (glm::abs(uv1.z) < 1e-2)
	{
		return glm::vec3(-1, 1, 1);
	}
	return (1.0f / uv1.z) * glm::vec3(uv1.z - (uv1.x + uv1.y), uv1.y, uv1.x);
}
