#include "pch.h"
#include "FrameBuffer.h"
#include <iostream>
#include <fstream>
#include <string>

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

bool FrameBuffer::Write(std::string path)
{
	std::ofstream tgafile(path.c_str(), std::ios::binary);
	
	if (!tgafile)
		return false;

	byte header[18] = { 0 };
	header[2] = 1;  // truecolor
	header[12] = width & 0xFF;
	header[13] = (width >> 8) & 0xFF;
	header[14] = height & 0xFF;
	header[15] = (height >> 8) & 0xFF;
	header[16] = 24;  // bits per pixel

	tgafile.write((const char*)header, 18);

	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			tgafile.put((char)(colorBuffer[(y * width) + x].b * 255));
			tgafile.put((char)(colorBuffer[(y * width) + x].g * 255));
			tgafile.put((char)(colorBuffer[(y * width) + x].r * 255));
		}
	}

	// The file footer. This part is totally optional.
	static const char footer[26] =
		"\0\0\0\0"  // no extension area
		"\0\0\0\0"  // no developer directory
		"TRUEVISION-XFILE"  // yep, this is a TGA file
		".";
	tgafile.write(footer, 26);
	tgafile.close();
	return true;
}

std::vector<Color> FrameBuffer::GetColorBuffer()
{
	return colorBuffer;
}
