#ifndef __TYPES_H__
#define __TYPES_H__
#include "fixed_float.h"

namespace physics
{
	typedef FixedFloat ffloat;

	#define ffsqrt 			FixedFloat::sqrt
	#define ffabs	 			FixedFloat::abs
	#define ffsin  			FixedFloat::sin
	#define ffcos  			FixedFloat::cos
	#define ffacos  		FixedFloat::acos
	#define ffexp  			FixedFloat::exp
	#define ffpow  			FixedFloat::pow
	#define ffmod  			FixedFloat::mod	

	#define ffzero 			FixedFloat::zero
	#define ffone  			FixedFloat::one
	#define ffhalf  		FixedFloat::half
	#define fftwo 			FixedFloat::two;
	#define fften 			FixedFloat::ten;
	#define fff100 			FixedFloat::f100;
	#define fff90 			FixedFloat::f90;
	#define fff180			FixedFloat::f180;
	#define ffpi 				FixedFloat::pi;
	#define ffhalf_pi 	FixedFloat::half_pi;
	#define ffquat_pi 	FixedFloat::quat_pi;
	#define fftwo_pi 		FixedFloat::two_pi;
	#define ffrad_unit 	FixedFloat::rad_unit;
	#define ffdeg_unit 	FixedFloat::deg_unit;
	#define ffe 				FixedFloat::e;

	class Vector3;
}


#endif