// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.

#include "pch.h"
#include "Mesh.h"
#include "Texture.h"
#include <iostream>
#include "FrameBuffer.h"


#define WIDTH 800
#define HEIGHT 600


int main()
{
	std::cout << "Loading data ... " << std::endl;
	Mesh mesh = Mesh("dragon.tga");
	Texture texture = Texture("dragon");
	
	std::cout << "Loading Scene ... " << std::endl;
	

}