#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "my_printf.h"
#include "types.h"
#include "timer.h"
#include "vector3.h"
#include "quaternion.h"
#include "matrix3.h"
#include "matrix4.h"
#include "aabb.h"
#include "octree.h"
#include "contact.h"
#include "collision_data.h"
#include "contact_generator.h"
#include "contact_processor.h"
#include "force_generator.h"
#include "force_manager.h"
#include "primitive.h"
#include "plane.h"
#include "sphere.h"
#include "box.h"
#include "polyhedron.h"
#include "capsule.h"
#include "gjk_epa.h"
#include "world.h"
#include "utils.h"

#include "gravity_force.h"
#include "move_force.h"
#include "spring_force.h"

#ifndef DISABLE_RENDER
#include "renderer.h"
#endif

#endif