#include "application.h"

Application::Application()
:width(0),height(0),title(0)
{
  lastUpdateTime = system_clock::now();
}

Application::Application(const char *title, int width, int height)
:width(width),height(height),title(title)
{
  lastUpdateTime = system_clock::now();
}

Application::~Application()
{}

const char* Application::getTitle()
{
	return title;
}

int Application::getWidth()
{
	return width;
}

int Application::getHeight()
{
	return height;
}

void Application::initGraphics()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	setupView();
}

void Application::setupView()
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width/(double)height, 0.3, 1500.0);
  glMatrixMode(GL_MODELVIEW);
}

void Application::onDisplay()
{
}

void Application::onUpdate()
{
}

double Application::calcDuration()
{
  auto now = system_clock::now();
  auto duration = duration_cast<microseconds>(now - lastUpdateTime);
  double t = double(duration.count()) * microseconds::period::num / microseconds::period::den;
  return t;
}

void Application::updateTime()
{
  lastUpdateTime = system_clock::now();
}

void Application::onResize(int width, int height)
{
	Application::width = width;
	Application::height = height;

	setupView();
}

void Application::onKeyboardPress(unsigned char key)
{
  printf("key pressed %c\n", key);
  switch( key ) {
  case 033: // Escape Key
  case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;
  }
}

//if ( state == GLUT_DOWN ) {
//    switch( button )
//    {
//        case GLUT_LEFT_BUTTON:
//            break;
//        case GLUT_RIGHT_BUTTON:
//            break;
//    }
//}
void Application::onMousePress(int button, int state, int x, int y)
{
}

void Application::onMouseMove(int x, int y)
{
}

void Application::onExit()
{
}

void Application::textOut(float x, float y, const char* text, void * font)
{
  return;
	glDisable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

	if (font == 0) {
		font = GLUT_BITMAP_HELVETICA_10;
	}

	size_t len = strlen(text);

	glRasterPos2f(x, y);
	for (const char *letter = text; letter < text+len; letter++) {
    if (*letter == '\n') {
      y -= 12.0f;
    	glRasterPos2f(x, y);
    }
		glutBitmapCharacter(font, *letter);
	}

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);
}
