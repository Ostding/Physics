#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cstdlib>
using namespace std;

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application
{
public:
	Application();
	Application(const char *title, int width, int height);
	virtual ~Application();

public:
	virtual const char* getTitle();
	virtual int getWidth();
	virtual int getHeight();
	virtual void initGraphics();
	virtual void setupView();

	virtual void onDisplay();
	virtual void onUpdate();
	virtual void onResize(int width, int height);

	virtual void onKeyboardPress(unsigned char key);
	virtual void onMousePress(int button, int state, int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onExit();

	void textOut(float x, float y, const char* text, void * font = 0);

protected:
	int width;
	int height;
	const char *title;
};


#endif

