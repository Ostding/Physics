#include "renderer.h"
#include "aabb.h"

namespace physics
{
  void Renderer::renderAABB(const AABB &o)
  {
    glEnable(GL_DEPTH_TEST);
    //back
    glBegin(GL_LINE_LOOP);
    COLOR_AABB
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glEnd();

    //front
    glBegin(GL_LINE_LOOP);
    COLOR_AABB
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

    //side
    glBegin(GL_LINES);
    COLOR_AABB
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

    glDisable(GL_DEPTH_TEST);
  }

  void Renderer::renderPlane(Plane *p)
  {
    GLfloat mat[16];
		p->transform.fillArray(mat);

		glColor3f(0.7, 0.7, 1.0);
		glPushMatrix();
    glMultMatrixf(mat);

		glScalef(p->extents.x.to_d() * 2, p->extents.y.to_d(), p->extents.z.to_d() * 2);
		
    glutSolidCube(1.0f);
		glPopMatrix();
  }

  void Renderer::renderSphere(Sphere *p)
  {
    GLfloat mat[16];
		p->transform.fillArray(mat);

		if (p->body->isAwake) 
      glColor3f(1.0, 0.7, 0.7);
		else 
      glColor3f(0.7, 0.7, 1.0);

		glPushMatrix();
		glMultMatrixf(mat);

		glutSolidSphere(p->radius.to_d(), 100, 100);

		glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

}