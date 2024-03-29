#ifndef DISABLE_RENDER
#include "renderer.h"
#endif
#include "aabb.h"
#include "body.h"
#include "contact_generator.h"
#include "gjk_epa.h"
#include "simplex.h"
namespace physics
{
#ifdef DEBUG_GJKEPA
  extern std::vector<SupportPoint> minkowskiPoints;
  extern std::vector<Vector3> allMinkowskiPoints;
  static void renderMinkowskiPoints()
  {
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glBegin(GL_LINES);
    // search lines
    if (minkowskiPoints.size() > 1)
    {
      int count = minkowskiPoints.size();
      for (int i = 1; i < count; i++)
      {
        float c = 1.0f / count * (i - 1);
        glColor3f(c, 0, 0);
        Vector3 pta = minkowskiPoints[i - 1].minkowskiPoint;
        Vector3 ptb = minkowskiPoints[i].minkowskiPoint;
        glVertex3f(pta.x.to_d(), pta.y.to_d(), pta.z.to_d());
        glVertex3f(ptb.x.to_d(), ptb.y.to_d(), ptb.z.to_d());
      }

      if (count > 3)
      {
        Vector3 pta = minkowskiPoints[count - 2].minkowskiPoint;
        Vector3 ptb = minkowskiPoints[count - 4].minkowskiPoint;
        glVertex3f(pta.x.to_d(), pta.y.to_d(), pta.z.to_d());
        glVertex3f(ptb.x.to_d(), ptb.y.to_d(), ptb.z.to_d());
      }
    }
    glEnd();

    // axis
    glBegin(GL_LINES);
    // x
    glColor3f(0.5, 0, 0);
    glVertex3f(-50, 0, 0);
    glVertex3f(0, 0, 0);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(50, 0, 0);

    // y
    glColor3f(0, 0.5, 0);
    glVertex3f(0, -50, 0);
    glVertex3f(0, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 50, 0);

    // z
    glColor3f(0, 0, 0.5);
    glVertex3f(0, 0, -50);
    glVertex3f(0, 0, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 50);
    glEnd();

    glEnable(GL_POINT_SMOOTH);
    glPointSize(3);
    glColor3f(1.0, 0, 0);
    glBegin(GL_POINTS);
    // Draw orignal point
    glVertex3f(0, 0, 0);

    // all points
    if (allMinkowskiPoints.size() > 1)
    {
      int count = allMinkowskiPoints.size();
      for (int i = 0; i < count; i++)
      {
        float c = 1.0f / count * (i - 1);
        glColor3f(0, c, 0);
        Vector3 pt = allMinkowskiPoints[i];
        glVertex3f(pt.x.to_d(), pt.y.to_d(), pt.z.to_d());
      }
    }
    glEnd();

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
  }
#endif

  void Renderer::setColor(float r, float g, float b)
  {
    GLfloat mat_ambient[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_diffuse[] = {r, g, b, 1.0f};
    GLfloat mat_specular[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_emission[] = {0.0f, 0.0f, 0.0f, 1.f};
    GLfloat mat_shininess = 30.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
  }

  void Renderer::renderAABB(const AABB &o)
  {
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // back
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glEnd();

    // front
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

    // side
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.min.x.to_d(), o.max.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.min.y.to_d(), o.max.z.to_d());

    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.min.z.to_d());
    glVertex3f(o.max.x.to_d(), o.max.y.to_d(), o.max.z.to_d());
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  }

  void Renderer::renderPlane(Plane *p)
  {
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // glEnable(GL_BLEND);
    // glDisable(GL_DEPTH_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glColor4f(0.6f, 0.6f, 0.6f, 0.6);
    glColor3f(0.6f, 0.6f, 0.6f);

    Vector3 _lb = Vector3(-p->extents.x, 0, -p->extents.z);
    Vector3 _lt = Vector3(-p->extents.x, 0, p->extents.z);
    Vector3 _rb = Vector3(p->extents.x, 0, -p->extents.z);
    Vector3 _rt = Vector3(p->extents.x, 0, p->extents.z);

    _lb = p->body->getPosInWorldSpace(_lb);
    _lt = p->body->getPosInWorldSpace(_lt);
    _rb = p->body->getPosInWorldSpace(_rb);
    _rt = p->body->getPosInWorldSpace(_rt);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex3f(_lt.x.to_d(), _lt.y.to_d(), _lt.z.to_d());
    glVertex3f(_lb.x.to_d(), _lb.y.to_d(), _lb.z.to_d());
    glVertex3f(_rt.x.to_d(), _rt.y.to_d(), _rt.z.to_d());
    glVertex3f(_rb.x.to_d(), _rb.y.to_d(), _rb.z.to_d());

    glEnd();

    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_BLEND);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
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

    glutSolidSphere(p->radius.to_d(), 50, 50);

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
    glEnable(GL_NORMALIZE);
    glutSolidCube(1.0f);
    glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderCapsule(Capsule *p)
  {
    GLfloat mat[16];
    p->transform.fillArray(mat);

    if (p->body->isAwake)
      setColor(0.6, 0.4, 0.2);
    else
      setColor(0.2, 0.6, 0.4);

    glPushMatrix();
    glMultMatrixf(mat);

    glPushMatrix();
    glTranslatef(p->pointLocalUp.x.to_d(), p->pointLocalUp.y.to_d(), p->pointLocalUp.z.to_d());
    glutSolidSphere(p->radius.to_d(), 50, 50);
    glPopMatrix();

    // Draw the tube
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    GLfloat x = 0.0f;
    GLfloat z = 0.0f;
    GLfloat angle = 0.0f;
    GLfloat step = 0.1f;
    GLfloat pi = 3.1416f;
    GLfloat radius = p->radius.to_d();
    GLfloat halfHeight = p->halfHeight.to_d();

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while (angle < 2 * pi)
    {
      float c = cos(angle);
      float s = sin(angle);
      x = radius * c;
      z = radius * s;
      glNormal3f(c, 0, s);
      glVertex3f(x, halfHeight, z);
      glVertex3f(x, -halfHeight, z);
      angle = angle + step;
    }
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(radius, halfHeight, 0.0f);
    glVertex3f(radius, -halfHeight, 0.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(p->pointLocalDown.x.to_d(), p->pointLocalDown.y.to_d(), p->pointLocalDown.z.to_d());
    glutSolidSphere(p->radius.to_d(), 50, 50);
    glPopMatrix();

    glPopMatrix();
    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderPolyhedron(Polyhedron *p)
  {
    GLfloat mat[16];
    p->transform.fillArray(mat);

    if (p->body->isAwake)
      setColor(0.4, 0.6, 0.2);
    else
      setColor(0.2, 0.6, 0.4);

    glPushMatrix();
    glMultMatrixf(mat);

    glBegin(GL_TRIANGLES);

    for (unsigned i = 0; i < p->indices.size(); i += 3)
    {
      // setColor(0.4 + i * 0.02, 0.6+ i * 0.02, 0.2+ i * 0.02);
      unsigned i0 = p->indices[i];
      unsigned i1 = p->indices[i + 1];
      unsigned i2 = p->indices[i + 2];

      Vector3 va = p->pointsLocal[i0] - p->pointsLocal[i1];
      Vector3 vb = p->pointsLocal[i2] - p->pointsLocal[i1];
      Vector3 normal = vb.cross(va);
      normal.normalise();
      glNormal3f(normal.x.to_d(), normal.y.to_d(), normal.z.to_d());
      glVertex3f(p->pointsLocal[i0].x.to_d(), p->pointsLocal[i0].y.to_d(), p->pointsLocal[i0].z.to_d());
      glVertex3f(p->pointsLocal[i1].x.to_d(), p->pointsLocal[i1].y.to_d(), p->pointsLocal[i1].z.to_d());
      glVertex3f(p->pointsLocal[i2].x.to_d(), p->pointsLocal[i2].y.to_d(), p->pointsLocal[i2].z.to_d());
    }

    glEnd();
    glPopMatrix();

    Renderer::renderAABB(p->aabb);
  }

  void Renderer::renderContactGenerator(ContactGenerator *p)
  {
    return;

    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // Contact normal
    //  Vector3 n = p->_contactNormal;
    //  glColor3f(1, 1, 0);
    //  glDisable(GL_DEPTH_TEST);
    //  glBegin(GL_LINES);
    //    glVertex3f(p->_triangleP0.x.to_d(), p->_triangleP0.y.to_d(), p->_triangleP0.z.to_d());
    //    glVertex3f(p->_triangleP0.x.to_d() + n.x.to_d(),
    //                p->_triangleP0.y.to_d() + n.y.to_d(),
    //                p->_triangleP0.z.to_d() + n.z.to_d());
    //  glEnd();
    //  glEnable(GL_DEPTH_TEST);

    // glDisable(GL_COLOR_MATERIAL);
    // glEnable(GL_LIGHTING);

    // glDisable(GL_LIGHTING);
    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT,GL_DIFFUSE);

    // glEnable(GL_BLEND);
    // glDisable(GL_DEPTH_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glColor4f(1.0f, 0.0f, 0.0f, 0.1);

    //  glBegin(GL_TRIANGLES);
    //   glVertex3f(p->_triangleP0.x.to_d(), p->_triangleP0.y.to_d(), p->_triangleP0.z.to_d());
    //   glVertex3f(p->_triangleP1.x.to_d(), p->_triangleP1.y.to_d(), p->_triangleP1.z.to_d());
    //   glVertex3f(p->_triangleP2.x.to_d(), p->_triangleP2.y.to_d(), p->_triangleP2.z.to_d());
    //  glEnd();

    OCTreeNode::PotentialContacts::iterator it = p->curPotentialContacts.begin();
    for (; it < p->curPotentialContacts.end(); it++)
    {
      AABB &o = it->second->aabb;
      double xmin = o.min.x.to_d();
      double xmax = o.max.x.to_d();
      double ymin = o.min.y.to_d();
      double ymax = o.max.y.to_d();
      double zmin = o.min.z.to_d();
      double zmax = o.max.z.to_d();
      // back
      glBegin(GL_QUADS);
      glVertex3f(xmin, ymin, zmin);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmax, ymax, zmin);
      glVertex3f(xmin, ymax, zmin);
      glEnd();

      // front
      glBegin(GL_QUADS);
      glVertex3f(xmin, ymin, zmax);
      glVertex3f(xmax, ymin, zmax);
      glVertex3f(xmax, ymax, zmax);
      glVertex3f(xmin, ymax, zmax);
      glEnd();

      // up
      glBegin(GL_QUADS);
      glVertex3f(xmin, ymax, zmin);
      glVertex3f(xmax, ymax, zmin);
      glVertex3f(xmax, ymax, zmax);
      glVertex3f(xmin, ymax, zmax);
      glEnd();

      // down
      glBegin(GL_QUADS);
      glVertex3f(xmin, ymin, zmin);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmax, ymin, zmax);
      glVertex3f(xmin, ymin, zmax);
      glEnd();

      // left
      glBegin(GL_QUADS);
      glVertex3f(xmin, ymin, zmin);
      glVertex3f(xmin, ymax, zmin);
      glVertex3f(xmin, ymax, zmax);
      glVertex3f(xmin, ymin, zmax);
      glEnd();

      // right
      glBegin(GL_QUADS);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmax, ymax, zmin);
      glVertex3f(xmax, ymax, zmax);
      glVertex3f(xmax, ymin, zmax);
      glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  }

  void Renderer::renderContact(Contact *p)
  {
#ifdef DEBUG_GJKEPA
    renderMinkowskiPoints();
#endif
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // Contact point
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1);
    glColor3f(1.0, 0, 0);
    glBegin(GL_POINTS);
    glVertex3f(p->contactPoint.x.to_d(),
               p->contactPoint.y.to_d(),
               p->contactPoint.z.to_d());
    glEnd();

    glEnable(GL_DEPTH_TEST);

    // Contact normal
    Vector3 n = p->contactNormal * ffabs(p->penetration);
    glColor3f(1, 1, 0);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    glVertex3f(p->contactPoint.x.to_d(),
               p->contactPoint.y.to_d(),
               p->contactPoint.z.to_d());
    glVertex3f(p->contactPoint.x.to_d() + n.x.to_d(),
               p->contactPoint.y.to_d() + n.y.to_d(),
               p->contactPoint.z.to_d() + n.z.to_d());
    glEnd();
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  }

}