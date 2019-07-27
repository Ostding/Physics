#include "renderer.h"
#include "aabb.h"
#include "body.h"

namespace physics
{
  void Renderer::setColor(float r, float g, float b)
  {
    glColor3f(r,g,b);
  }

  void Renderer::renderAABB(const AABB &o)
  {
    //back
    glBegin(GL_LINE_LOOP);
    setColor(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glEnd();

    //front
    glBegin(GL_LINE_LOOP);
    setColor(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

    //side
    glBegin(GL_LINES);
    setColor(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

  }

  void Renderer::renderPlane(Plane *p)
  {
    Vector3 _lb = Vector3(-p->extents.x, 0, -p->extents.z);
    Vector3 _lt = Vector3(-p->extents.x, 0, p->extents.z);
    Vector3 _rb = Vector3(p->extents.x, 0, -p->extents.z);
    Vector3 _rt = Vector3(p->extents.x, 0, p->extents.z);

    _lb = p->body->getPosInWorldSpace(_lb);
    _lt = p->body->getPosInWorldSpace(_lt);
    _rb = p->body->getPosInWorldSpace(_rb);
    _rt = p->body->getPosInWorldSpace(_rt);

    float r = ((float)((int64)p % 10000))/10000.0f;
		setColor(r, r, r);
	  glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(_lt.x.to_d(), _lt.y.to_d(), _lt.z.to_d());
    glVertex3f(_lb.x.to_d(), _lb.y.to_d(), _lb.z.to_d());
    glVertex3f(_rt.x.to_d(), _rt.y.to_d(), _rt.z.to_d());
    glVertex3f(_rb.x.to_d(), _rb.y.to_d(), _rb.z.to_d());

		glEnd();
  }

  void Renderer::renderSphere(Sphere *p)
  {
    GLfloat mat[16];
		p->transform.fillArray(mat);
  
		if (p->body->isAwake) 
      setColor(0.6, 0.4, 0.2);
		else
      setColor(0.2, 0.6, 0.4);

		glPushMatrix();
		glMultMatrixf(mat);

		glutSolidSphere(p->radius.to_d(), 100, 100);

		glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderBox(Box *p)
  {
    GLfloat mat[16];
		p->transform.fillArray(mat);

    if (p->body->isAwake) 
      setColor(0.6, 0.4, 0.2);
		else
      setColor(0.2, 0.6, 0.4);

    glPushMatrix();
		glMultMatrixf(mat);

		glScalef(p->extents.x.to_d() * 2, p->extents.y.to_d() * 2, p->extents.z.to_d() * 2);

		glutSolidCube(1.0f);
		glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderContact(Contact *p)
  { 
    //Contact point
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1);
    setColor(1.0, 0, 0);
    glBegin(GL_POINTS);
      glVertex3f(p->contactPoint.x.to_d(), 
                 p->contactPoint.y.to_d(),
                 p->contactPoint.z.to_d());
    glEnd();

    glEnable(GL_DEPTH_TEST);

    //Contact normal
    setColor(1, 1, 0);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
      glVertex3f(p->contactPoint.x.to_d(), 
                p->contactPoint.y.to_d(),
                p->contactPoint.z.to_d());
      glVertex3f(p->contactPoint.x.to_d() + p->contactNormal.x.to_d(),
                p->contactPoint.y.to_d() + p->contactNormal.y.to_d(),
                p->contactPoint.z.to_d() + p->contactNormal.z.to_d());
    glEnd();
    glEnable(GL_DEPTH_TEST);

  }


}