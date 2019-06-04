///
///https://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl
///
#pragma once
#include "pch.h"
#include <vector>
#include <fstream>

typedef union PixelInfo
{
	uint32_t Color;
	struct 
	{
		uint8_t R, G, B, A;
	};
} *PPixelInfo;

class Textures
{
public:
	Textures(std::string pathName);
	std::vector<std::uint8_t> GetPixels() { return this->pixels; }
	std::uint32_t GetWidth() const { return this->width; }
	std::uint32_t GetHeight() const { return this->height; };
	bool HasAlphaChannel() { return BitsPerPixel == 32; }

private:
	uint32_t width, height, size, BitsPerPixel;
	bool ImageCompressed;
	std::vector<uint8_t> pixels;
};

 