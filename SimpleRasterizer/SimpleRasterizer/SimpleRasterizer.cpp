// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.

#include "pch.h"
#include "Mesh.h"
#include "Texture.h"
#include "Rendering.h"
#include "FrameBuffer.h"
#include "Clipping.h"
#include <iostream>

#define WIDTH 800
#define HEIGHT 600


int main()
{
	std::cout << "Loading data ... " << std::endl;
	Mesh* mesh = new Mesh("../Resources/dragon.obj");
	Texture* texture = new Texture("../Resources/dragon.tga");
	
	std::cout << "Loading Scene ... " << std::endl;
	
	FrameBuffer* frameBuffer = new FrameBuffer(WIDTH, HEIGHT);

	const glm::mat4 identity(1.f);

	auto modelMatrix = glm::scale(identity, glm::vec3(.5f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(1.f), glm::vec3(0, 1, 0));

	auto viewMatrix = glm::lookAt(glm::vec3(-2.0, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.0f, 10.f);

	auto mvp = projectionMatrix * viewMatrix * modelMatrix;

	auto lightMatrix = glm::transpose(glm::inverse(modelMatrix));

	std::cout << "Rendering...." << std::endl;

	Rendering render = Rendering();
	Clipping clipping = Clipping();

	for (auto faceModelSpace : mesh->faces)
	{
		auto faceClipSpace = render.WorldSpaceToClipSpace(faceModelSpace, mvp, lightMatrix);
		auto clippedFaces = clipping.Clip(faceClipSpace);

		for (auto clippedFace : clippedFaces)
		{
			auto faceNDSpace = render.PerspectiveDivide(clippedFace);
			if (clipping.CullFace(faceNDSpace))
				continue;

			auto faceScreenSpace = render.NormalizedSpaceToScreenSpace(faceNDSpace, WIDTH, HEIGHT);
			render.Draw(faceScreenSpace, frameBuffer, texture);
		}

		std::cout << "Writting to tga..." << std::endl;
		auto sucess = frameBuffer->Write("results.tga");

		std::cout << "Results: " << sucess << std::endl;
	}
}