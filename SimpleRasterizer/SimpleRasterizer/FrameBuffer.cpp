#include "pch.h"
#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(int w, int h)
{
	width = w;
	height = h;
	colorBuffer = std::vector<Color>(width * height);
	for (size_t i = 0; i < width * height; ++i)
	{
		colorBuffer[i] = { 0, 0, 0 };
	}
	depthBuffer = std::vector<float>(width * height);
	for (size_t i = 0; i < width * height; ++i)
	{
		depthBuffer[i] = 1.0f;
	}
}


float FrameBuffer::GetDepth(int x, int y)
{
	return depthBuffer[y * width + x];
}

void FrameBuffer::SetDepth(int x, int y, float depth)
{
	depthBuffer[y * width + x] = depth;
}

void FrameBuffer::SetColor(int x, int y, Color color)
{
	colorBuffer[y * width + x] = color;
}

void FrameBuffer::Clear()
{
	for (size_t i = 0; i < width * height; ++i)
	{
		colorBuffer[i] = { 0, 0, 0 };
		depthBuffer[i] = 1.0;
	}
}

std::vector<Color> FrameBuffer::GetColorBuffer()
{
	return colorBuffer;
}
