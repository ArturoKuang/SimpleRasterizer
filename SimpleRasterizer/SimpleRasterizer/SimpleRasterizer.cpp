#include "pch.h"
#include <iostream>
#include <vector>
#include <chrono> 

static const float inchToMm = 25.4;
static const float M_PI = 3.14159;

enum FitResolutionGate
{
	kFill = 0,
	kOverscan
};

uint32_t nvertices[9468];
glm::vec3 vertices[1732];

struct RGBChannel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
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

float EdgeFunction(glm::vec3& a, glm::vec3& b, glm::vec3& c)
{
	return glm::length(glm::cross(b - a, c));
}

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;
const glm::mat4x4 worldToCamera = { 0.707107, -0.331295, 0.624695, 0, 0, 0.883452, 0.468521, 0, -0.707107, -0.331295, 0.624695, 0, -1.63871, -5.747777, -40.400412, 1 };

const uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPlane = 1000;
//35mm full aperture 
const float focalLength = 20;
const float filmApertureWidth = 0.980;
const float filmApertureHeight = 0.735;


int main()
{
	glm::mat4x4 cameraToWorld = glm::inverse(worldToCamera);

	float t, b, l, r;

	ComputeScreenCoordinates(
		filmApertureWidth, filmApertureHeight,
		imageWidth, imageHeight,
		kOverscan,
		nearClippingPlane,
		focalLength,
		t, b, l, r);

	//define the frame buffer and depth buffer. Intialize depth buffer to far 
	//clipping plane 
	std::vector<RGBChannel> frameBuffer(imageWidth * imageHeight);
	for (size_t i = 0; i < imageWidth * imageHeight; ++i)
		frameBuffer[i] = RGBChannel{ 255, 255, 255 };

	float* depthBuffer = new float[imageWidth * imageHeight];
	for (size_t i = 0; i < imageWidth * imageHeight; ++i)
		depthBuffer[i] = farClippingPlane;

	auto t_start = std::chrono::high_resolution_clock::now();

	for (uint32_t i = 0; i < ntris; ++i)
	{
		const glm::vec3& v0 = vertices[nvertices[i * 3]];
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];

		//convert vertices of triangle to raster space
		glm::vec3 v0Raster, v1Raster, v2Raster;
		ConvertToRaster(v0, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v0Raster);
		ConvertToRaster(v1, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v1Raster);
		ConvertToRaster(v2, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v2Raster);

		v0Raster.z = 1 / v0Raster.z;
		v1Raster.z = 1 / v1Raster.z;
		v2Raster.z = 1 / v2Raster.z;

	
	}							 



    std::cout << "Hello World!\n"; 
}
