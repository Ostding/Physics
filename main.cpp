#include <OpenGL/gl.h> 
#include <OpenGL/glu.h> 
#include <GLUT/glut.h> 
#include <iostream>
using namespace std;

#include "application.h"

extern Application *getApp();

Application *app = 0;


void keyboardFunc( unsigned char key, int x, int y )
{
  app -> onKeyboardPress(key);
}

void mousePressFunc( int button, int state, int x, int y )
{
  app -> onMousePress( button, state, x, y);
}

void mouseMoveFunc( int x, int y )
{
  app -> onMouseMove(x, y);
}


#ifdef __APPLE__
bool mojaveWorkAround = true;
#endif

void displayFunc()
{	
#ifdef __APPLE__
	if(mojaveWorkAround)
  {
    glutReshapeWindow(0.8 * app->getWidth(), 0.8 * app->getHeight());
    mojaveWorkAround = false;
  }
#endif

  app -> onDisplay();

	glFlush();
	glutSwapBuffers();
}


void idleFunc( void )
{
  app -> onUpdate();

  glutPostRedisplay();
}


void reshapeFunc(GLsizei w, GLsizei h )
{
  app -> onResize((int)w, (int)h);
}

void createWindow()
{
  app = getApp();
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( app->getWidth(), app->getHeight() );
  glutInitWindowPosition(0,0);
  glutCreateWindow( app->getTitle());
}

void setupCallBacks()
{
  glutDisplayFunc( displayFunc );
  glutReshapeFunc( reshapeFunc );
  glutMouseFunc( mousePressFunc );
  glutMotionFunc(mouseMoveFunc);
  glutKeyboardFunc( keyboardFunc );
  glutIdleFunc( idleFunc );
}

void initGraphics()
{
  app -> initGraphics();
}

void quit()
{
  app->onExit();
  delete app;
  app = 0;
}

void printOpenGL()
{
  const GLubyte* vender = glGetString(GL_VENDOR);
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* OpenGLVersion =glGetString(GL_VERSION);
  const GLubyte* gluVersion=gluGetString(GLU_VERSION); 

  printf("OpenGL vender：%s\n", vender);
  printf("Renderer：%s\n", renderer);
  printf("OpenGL version：%s\n",OpenGLVersion );
  printf("GLU工 version：%s\n", gluVersion);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  createWindow();

  setupCallBacks();

  initGraphics();

  printOpenGL();

  glutMainLoop();

  quit();

  return 0;
}
