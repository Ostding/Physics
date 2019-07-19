#include <assert.h>
#include "primitive.h"


namespace physics
{
    Primitive::Primitive(PRIMITIVE_TYPE t)
    : id( 0 )
    , tContact( -1 )
    , tProfile( -1 )
    , tPrimitive (t)
    , body( 0 )
    , parent( 0 )
    , node( 0 )
    , isStatic( false )
    , interCount( 0 )
    {} 

    Primitive::~Primitive()
    {
      if(body != 0)
      {
        delete body;
        body = 0;
      }
    }

    void Primitive::refreshAABB()
    {      
      assert("You shoule implement refreshAABB method!!!");
    }

    void Primitive::updateTransform()
    {
      transform = body->transformMatrix * offset;
    }

    Vector3 Primitive::getColumnVector(unsigned index) const
    {
      return transform.getColumnVector(index);
    }

    Vector3 Primitive::getTransformPos() const
    {
      return transform.getTransformPos();
    }
}