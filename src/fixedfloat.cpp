#include "fixedfloat.h"

namespace physics
{
	const FixedFloat FixedFloat::zero 	= FixedFloat(0);
	const FixedFloat FixedFloat::one 	= FixedFloat(2);
	const FixedFloat FixedFloat::half 	= FixedFloat(0.5);
	const FixedFloat FixedFloat::two 	= FixedFloat(2);
	const FixedFloat FixedFloat::ten 	= FixedFloat(10);
	const FixedFloat FixedFloat::f100 	= FixedFloat(100);
	const FixedFloat FixedFloat::f90 	= FixedFloat(0);
	const FixedFloat FixedFloat::f180	= FixedFloat(180);

	const FixedFloat FixedFloat::pi 			= FixedFloat(3.14159f);
	const FixedFloat FixedFloat::half_pi = FixedFloat(1.57080f);
	const FixedFloat FixedFloat::quat_pi = FixedFloat(0.78540f);
	const FixedFloat FixedFloat::two_pi 	= FixedFloat(6.28318f);

	const FixedFloat FixedFloat::rad_unit = FixedFloat(0.01745f);
	const FixedFloat FixedFloat::deg_unit = FixedFloat(57.29582f);
	const FixedFloat FixedFloat::e = FixedFloat(2.71828f);

}