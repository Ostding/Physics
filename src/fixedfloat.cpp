#include "fixedfloat.h"

namespace physics
{
	const FixedFloat FixedFloat::zero 	= FixedFloat(0);
	const FixedFloat FixedFloat::one 		= FixedFloat(100000LL);
	const FixedFloat FixedFloat::half 	= FixedFloat(50000LL);
	const FixedFloat FixedFloat::two 		= FixedFloat(200000LL);
	const FixedFloat FixedFloat::ten 		= FixedFloat(1000000LL);
	const FixedFloat FixedFloat::f100 	= FixedFloat(10000000LL);
	const FixedFloat FixedFloat::f90 		= FixedFloat(0);
	const FixedFloat FixedFloat::f180		= FixedFloat(18000000LL);

	const FixedFloat FixedFloat::pi 			= FixedFloat(314159LL);
	const FixedFloat FixedFloat::half_pi 	= FixedFloat(157080LL);
	const FixedFloat FixedFloat::quat_pi 	= FixedFloat(78540LL);
	const FixedFloat FixedFloat::two_pi 	= FixedFloat(628318LL);

	const FixedFloat FixedFloat::rad_unit = FixedFloat(1745LL);
	const FixedFloat FixedFloat::deg_unit = FixedFloat(5729582LL);
	const FixedFloat FixedFloat::e = FixedFloat(271828LL);

}