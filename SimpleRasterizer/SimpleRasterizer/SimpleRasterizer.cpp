#include "pch.h"
#include <iostream>

static const float inchToMm = 25.4;
static const float M_PI = 3.14159;

enum FitResolutionGate
{
	kFill = 0,
	kOverscan
};


void ComputeScreenCoordinates(
	const float& filmApertureWidth,
	const float& filmApertureHeight,
	const uint32_t& imageWidth,
	const uint32_t& imageHeight,
	const FitResolutionGate& fitFilm,
	const float& nearClippingPlane,
	const float& focalLength,
	float& top, float& bottom, float& left, float& right
)
{
	float filmAspectRatio = filmApertureWidth / filmApertureHeight;
	float deviceAspectRatio = imageWidth / (float)imageHeight;

	top = ((filmApertureHeight * inchToMm / 2) / focalLength) * nearClippingPlane;
	right = ((filmApertureWidth * inchToMm / 2) / focalLength) * nearClippingPlane;

	//field of view (horizontal)
	float fov = 2 * 180 / M_PI * atan((filmApertureWidth * inchToMm) / 2) / focalLength;
	std::cerr << "Field of view " << fov << std::endl;

	float xScale = 1;
	float yScale = 1;

	switch (fitFilm)
	{
	default:
	case kFill:
		if (filmAspectRatio > deviceAspectRatio)
		{
			xScale = deviceAspectRatio / filmAspectRatio;
		}
		else
		{
			yScale = filmAspectRatio / deviceAspectRatio;
		}
	case kOverscan:
		if (filmAspectRatio > deviceAspectRatio)
		{
			yScale = filmAspectRatio / deviceAspectRatio;
		}
		else
		{
			xScale = deviceAspectRatio / filmAspectRatio;
		}
		break;
	}

	right *= xScale;
	top *= yScale;
}


void ConvertToRaster(
	const glm::vec4& vertexWorld,
	const glm::mat4x4& worldToCamera,
	const float& l,
	const float& r,
	const float& t,
	const float& b,
	const float& near,
	const uint32_t& imageWidth,
	const uint32_t& imageHeight,
	glm::vec4& vertexRaster
)
{
	glm::vec3 vertexCamera;
	glm::vec4 temp = worldToCamera * vertexWorld;
	vertexCamera = glm::vec3(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w);

	//convert to screen space
	glm::vec2 vertexScreen;
	vertexScreen.x = near * vertexCamera.x / -vertexCamera.z;
	vertexScreen.y = near * vertexCamera.y / -vertexCamera.z;

	//convert point from screen space to NDC ([-1, 1])
	glm::vec2 vertexNDC;
	vertexNDC.x = 2 * vertexScreen.x / (r - l) - (r + l) / (r - l);
	vertexNDC.y = 2 * vertexScreen.y / (t - b) - (t + b) / (t - b);

	//convert to raster space
	vertexRaster.x = (vertexNDC.x + 1) / 2 * imageWidth;
	vertexRaster.y = (1 - vertexNDC.y) / 2 * imageHeight;
	vertexRaster.z = -vertexCamera.z;
}


int main()
{
    std::cout << "Hello World!\n"; 
}
