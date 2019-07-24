#ifndef __PLANE_H__
#define __PLANE_H__

#include "types.h"
#include "primitive.h"

namespace physics
{
  //Plz note that: plane usually used as a static ground or platform 
  //with massive mass, so it can not move 
  class Plane : public Primitive
  {
  public:
    Vector3 direction;
    Vector3 ptLocalLB;
    Vector3 ptLocalLT;
    Vector3 ptLocalRB;
    Vector3 ptLocalRT;

    Vector3 ptLB;
    Vector3 ptLT;
    Vector3 ptRB;
    Vector3 ptRT;

  public:
    Plane();
    Plane(Vector3 direction, Vector3 extents);

    virtual void refreshAABB();
    virtual void render();
    virtual void updateTransform();
  private:
    void clacDiagonalPoints(const Vector3 &extents);
  };
}


#endif