#include "fraction.h"

Fraction Fraction::f0 = Fraction(0, 1);
Fraction Fraction::f1 = Fraction(1,1);
Fraction Fraction::f2 = Fraction(2,1);
Fraction Fraction::f0_5 = Fraction(1,2);
Fraction Fraction::f10 = Fraction(10,1);
Fraction Fraction::f90 = Fraction(90,1);
Fraction Fraction::f180 = Fraction(180, 1);

Fraction Fraction::pi = Fraction(3.14159f);
Fraction Fraction::pi_2 = Fraction(1.57080f);
Fraction Fraction::pi_4 = Fraction(0.78540f);
Fraction Fraction::pi2 = Fraction(6.28319f);
Fraction Fraction::rad_unit = Fraction(0.01745f);
Fraction Fraction::deg_unit = Fraction(57.29579f);
Fraction Fraction::e = Fraction(2.71828f);
Fraction Fraction::min = Fraction();
Fraction Fraction::max = Fraction();

int64_t Fraction::mask = LLMAX;

