// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.

#include "pch.h"

#define REFRESH_DELAY	  10 //ms
uint32_t width = 512, height = 512;



void TimerEvent(int value)
{
	glutPostRedisplay();
	glutTimerFunc(REFRESH_DELAY, TimerEvent, 0);
}

void DisplayGL()
{

}

void KeyboardGL(unsigned char key, int x, int y)
{

}

void Mouse(int button, int state, int x, int y)
{

}

void Motion(int x, int y)
{

}

void Reshape(int w, int h)
{

}

void Idle(void)
{

}

void InitGL(int* argc, char** argv)
{
	//Intialize GLUT
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2 - width / 2,
		glutGet(GLUT_SCREEN_HEIGHT) / 2 - height / 2);
	glutInitWindowSize(width, height);
	glutCreateWindow("Simple Rasterizer");


	//Register callbacks
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutTimerFunc(REFRESH_DELAY, TimerEvent, 0);

	// initialize necessary OpenGL extensions
	glewInit();
	GLboolean bGLEW = glewIsSupported("GL_VERSION_2_0 GL_ARB_pixel_buffer_object");

}

int main(int argc, char** argv)
{
	InitGL(&argc, argv);
	glutMainLoop();

}