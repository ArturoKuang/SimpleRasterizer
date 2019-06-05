#pragma once
#include "pch.h"
#include "FrameBuffer.h"
#include <vector>
#include "Textures.h"

class Rendering
{
public:
	Rendering() {};
	~Rendering() {};

	Face WorldSpaceToClipSpace(Face faceModelSpace, glm::mat4x4 mvp, glm::mat4x4 lightMatrix);
	Face PerspectiveDivide(Face faceClipSpace);
	Face NormalizedSpaceToScreenSpace(Face faceNormalizedSpace, float width, float height);
	void Draw(Face fScreen, FrameBuffer* frameBuffer, Textures* texture);
	void BoundingBox(Face vs, float width, float height, glm::vec2& v0, glm::vec2& v1);
	glm::vec3 Barycentre(glm::vec2 point, glm::vec4 v0, glm::vec4 v1, glm::vec4 v2);

};

