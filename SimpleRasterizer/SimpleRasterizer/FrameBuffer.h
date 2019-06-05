#pragma once
#include "pch.h"
#include <vector>

class FrameBuffer
{
public:
	int width;
	int height;
	std::vector<Color> GetColorBuffer();

	FrameBuffer(int w, int h);

	float GetDepth(int x, int y);
	void SetDepth(int x, int y, float depth);
	void SetColor(int x, int y, Color color);
	void Clear();

private:
	std::vector<Color> colorBuffer;
	std::vector<float> depthBuffer;
};

