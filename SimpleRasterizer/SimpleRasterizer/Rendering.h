#pragma once
#include "pch.h"
#include <vector>

class Rendering
{
public:
	Rendering() {};
	~Rendering() {};

	Face WorldSpaceToClipSpace(Face faceModelSpace, glm::mat4x4 mvp, glm::mat4x4 lightMatrix);
	Face PerspectiveDivide(Face faceClipSpace);
	Face NormalizedSpaceToScreenSpace(Face faceNormalizedSpace, float width, float height);
	void Draw(Face fScreen, std::vector<Color>& frameBuffer);
	glm::vec2* BoundingBox(Face vs, float width, float height);
	glm::vec3 Barycentre(glm::vec2 point, glm::vec4 v0, glm::vec4 v1, glm::vec4 v2);

};

