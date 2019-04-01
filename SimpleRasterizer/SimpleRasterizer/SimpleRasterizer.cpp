#include "pch.h"
#include <iostream>


static const float inchToMm = 25.4;
static const float M_PI = 3.14159;

enum FitResolutionGate
{
	kFill = 0,
	kOverscan
};


void computeScreenCoordinates(
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

int main()
{
    std::cout << "Hello World!\n"; 
}
