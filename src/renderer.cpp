#include "renderer.h"
#include "aabb.h"
#include "body.h"

namespace physics
{
  void Renderer::setColor(float r, float g, float b)
  {
    GLfloat sun_mat_ambient[]  = {0.0f, 0.0f, 1.0f, 1.0f};
		GLfloat sun_mat_diffuse[]  = {r, g, b, 1.0f};
		GLfloat sun_mat_specular[] = {0.8f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_shininess  = 30.0f;        
		glMaterialfv(GL_FRONT, GL_AMBIENT,    sun_mat_ambient);    
		glMaterialfv(GL_FRONT, GL_DIFFUSE,    sun_mat_diffuse);   
		glMaterialfv(GL_FRONT, GL_SPECULAR,   sun_mat_specular);    
		glMaterialfv(GL_FRONT, GL_EMISSION,   sun_mat_emission);   
		glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);
    glMaterialfv(GL_BACK, GL_AMBIENT,    sun_mat_ambient);    
		glMaterialfv(GL_BACK, GL_DIFFUSE,    sun_mat_diffuse);   
		glMaterialfv(GL_BACK, GL_SPECULAR,   sun_mat_specular);    
		glMaterialfv(GL_BACK, GL_EMISSION,   sun_mat_emission);   
		glMaterialf (GL_BACK, GL_SHININESS, sun_mat_shininess);
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
		setColor(0.5, 0.3, 0.2);
	  glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(p->ptLT.x.to_d(), p->ptLT.y.to_d(), p->ptLT.z.to_d());
    glVertex3f(p->ptLB.x.to_d(), p->ptLB.y.to_d(), p->ptLB.z.to_d());
    glVertex3f(p->ptRT.x.to_d(), p->ptRT.y.to_d(), p->ptRT.z.to_d());
    glVertex3f(p->ptRB.x.to_d(), p->ptRB.y.to_d(), p->ptRB.z.to_d());

		glEnd();
  }

  void Renderer::renderSphere(Sphere *p)
  {
    GLfloat mat[16];
		p->transform.fillArray(mat);

		if (p->body->isAwake) 
      setColor(1.0, 0.7, 0.7);
		else 
      setColor(0.7, 0.7, 1.0);

		glPushMatrix();
		glMultMatrixf(mat);

		glutSolidSphere(p->radius.to_d(), 100, 100);

		glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderContact(Contact *p)
  { 
    //Contact point
    glDisable(GL_DEPTH_TEST);
    // glEnable(GL_POINT_SMOOTH);
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