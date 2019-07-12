#include "test_fixedfloat.h"
#include "timer.h"


void TestFixedFloat::print(const char *pszCap, const FixedFloat &f)
{
  printf("%s => %s \n", pszCap, f.to_s());
}

void TestFixedFloat::doTest()
{
	printf("/////////////Test FixedFloat////////////////////\n");
  printf("////test constructors\n");
  FixedFloat a = FixedFloat();
  print("FixedFloat a = FixedFloat()", a);
  FixedFloat b = FixedFloat(1);
  print("FixedFloat b = FixedFloat()", b);
  FixedFloat c = FixedFloat(12345678);
  print("FixedFloat c = FixedFloat(12345678)", c);
  FixedFloat d = FixedFloat(12345678LL);
  print("FixedFloat d = FixedFloat(12345678LL)", d);
  FixedFloat e = FixedFloat(3.141592);
  print("FixedFloat e = FixedFloat(3.141592)", e);
  FixedFloat f = FixedFloat(3.141592f);
  print("FixedFloat f = FixedFloat(3.141592f)", f);

  printf("////test consts\n");
  print("FixedFloat::zero", FixedFloat::zero);
  print("FixedFloat::one", FixedFloat::one);
  print("FixedFloat::half", FixedFloat::half);
  print("FixedFloat::two", FixedFloat::two);
  print("FixedFloat::ten", FixedFloat::ten);
  print("FixedFloat::f100", FixedFloat::f100);
  print("FixedFloat::f90", FixedFloat::f90);
  print("FixedFloat::pi", FixedFloat::pi);
  print("FixedFloat::half_pi", FixedFloat::half_pi);
  print("FixedFloat::quat_pi", FixedFloat::quat_pi);
  print("FixedFloat::two_pi", FixedFloat::two_pi);
  print("FixedFloat::rad_unit", FixedFloat::rad_unit);
  print("FixedFloat::deg_unit", FixedFloat::deg_unit);
  print("FixedFloat::e", FixedFloat::e);

  printf("////test convertion\n");
  FixedFloat x = FixedFloat(12);
  int n = x.to_i();
  printf("FixedFloat(12).to_i() => %d \n", n);

  x = FixedFloat(12.12345f);
  n = x.to_i();
  printf("FixedFloat(12.12345).to_i() => %d \n", n);
  double dd = x.to_d();
  printf("FixedFloat(12.12345).to_d() => %.5f \n", dd);

  printf("////test operation override\n");
  FixedFloat y = FixedFloat(12345678.12345);
  x = FixedFloat();
  print("x", x);
  print("y", y);
  x = y;
  print("x = y", x);
  x = 123;
  print("x = 123", x);
  x = 123456789LL;
  print("x = 123456789LL", x);
  x = 123.456f;
  print("x = 123.456f", x);
  x = 456.789;
  print("x = 456.789", x);
  bool _b = x < y;
  printf("x < y => %d \n", _b);
  _b = x <= y;
  printf("x <= y => %d \n", _b);
  _b = x > y;
  printf("x > y => %d \n", _b);
  _b = x >= y;
  printf("x >= y => %d \n", _b);
  _b = x == y;
  printf("x == y => %d \n", _b);
  _b = FixedFloat(123.456f) == FixedFloat(12345600LL);
  printf("FixedFloat(123.456f) == FixedFloat(1234500000LL) => %d \n", _b);

  x = FixedFloat(10);
  y = FixedFloat(2.5f);
  print("x", x);
  print("y", y);
  FixedFloat z = x + y;
  print("z = x + y", z);
  z = x - y;
  print("z = x - y", z);
  z = x * y;
  print("z = x * y", z);
  z = x / y;
  print("z = x / y", z);

  x += y;
  print("x += y", x);
  x -= y;
  print("x -= y", x);
  x *= y;
  print("x *= y", x);
  x /= y;
  print("x /= y", x);
  x = -x;
  print("x = -x", x);

  printf("////test functions\n");
  _b = x.positive();
  printf("x.positive() => %d \n", _b);
  _b = x.negative();
  printf("x.negative() => %d \n", _b);
  n = y.round();
  printf("y.round() => %d \n", n);
  x = x.abs();
  print("x.abs()", x);

  x = FixedFloat::sin(FixedFloat::zero);
  print("FixedFloat::sin(FixedFloat::zero)", x);
  x = FixedFloat::sin(FixedFloat::half_pi);
  print("FixedFloat::sin(FixedFloat::half_pi)", x);
  x = FixedFloat::sin(FixedFloat::quat_pi);
  print("FixedFloat::sin(FixedFloat::quat_pi)", x);
  x = FixedFloat::sin(FixedFloat::pi);
  print("FixedFloat::sin(FixedFloat::pi)", x);

  x = FixedFloat::cos(FixedFloat::zero);
  print("FixedFloat::cos(FixedFloat::zero)", x);
  x = FixedFloat::cos(FixedFloat::half_pi);
  print("FixedFloat::cos(FixedFloat::half_pi)", x);
  x = FixedFloat::cos(FixedFloat::quat_pi);
  print("FixedFloat::cos(FixedFloat::quat_pi)", x);
  x = FixedFloat::cos(FixedFloat::pi);
  print("FixedFloat::cos(FixedFloat::pi)", x);

  x = FixedFloat::tan(FixedFloat::zero);
  print("FixedFloat::tan(FixedFloat::zero)", x);
  x = FixedFloat::tan(FixedFloat::half_pi);
  print("FixedFloat::tan(FixedFloat::half_pi)", x);
  x = FixedFloat::tan(FixedFloat::quat_pi);
  print("FixedFloat::tan(FixedFloat::quat_pi)", x);
  x = FixedFloat::tan(FixedFloat::pi);
  print("FixedFloat::tan(FixedFloat::pi)", x);

  x = FixedFloat::asin(FixedFloat::zero);
  print("FixedFloat::asin(FixedFloat::zero)", x);
  x = FixedFloat::asin(FixedFloat::one);
  print("FixedFloat::asin(FixedFloat::one)", x);
  x = FixedFloat::asin(FixedFloat::half);
  print("FixedFloat::asin(FixedFloat::half)", x);

  x = FixedFloat::acos(FixedFloat::zero);
  print("FixedFloat::acos(FixedFloat::zero)", x);
  x = FixedFloat::acos(FixedFloat::one);
  print("FixedFloat::acos(FixedFloat::one)", x);

  x = FixedFloat::atan(FixedFloat::zero);
  print("FixedFloat::atan(FixedFloat::zero)", x);
  x = FixedFloat::atan(FixedFloat::one);
  print("FixedFloat::atan(FixedFloat::one)", x);
  x = FixedFloat::atan(FixedFloat::half);
  print("FixedFloat::atan(FixedFloat::half)", x);
  
  x = FixedFloat::atan2(FixedFloat::one, FixedFloat::zero);
  print("FixedFloat::atan2(FixedFloat::one, FixedFloat::zero)", x);
  x = FixedFloat::atan2(FixedFloat::two, FixedFloat::one);
  print("FixedFloat::atan2(FixedFloat::two, FixedFloat::one)", x);

  x = FixedFloat::sqrt(FixedFloat(4));
  print("FixedFloat::sqrt(FixedFloat(4))", x);
  x = FixedFloat::sqrt(FixedFloat(2));
  print("FixedFloat::sqrt(FixedFloat(2))", x);

  x = FixedFloat::sqrt(FixedFloat(1));
  print("FixedFloat::sqrt(FixedFloat(1))", x);
  x = FixedFloat::sqrt(FixedFloat(2));
  print("FixedFloat::sqrt(FixedFloat(2))", x);

  x = FixedFloat::mod(FixedFloat(5), FixedFloat(2));
  print("FixedFloat::mod(FixedFloat(5), FixedFloat(2))", x);
  x = FixedFloat::mod(FixedFloat(3.5f), FixedFloat(1.5f));
  print("FixedFloat::mod(FixedFloat(3.5f), FixedFloat(1.5f))", x);
  
  x = FixedFloat::log(FixedFloat(1000));
  print("FixedFloat::log(FixedFloat(1000))", x);
  x = FixedFloat::log(FixedFloat(100));
  print("FixedFloat::log(FixedFloat(100))", x);
  x = FixedFloat::log(FixedFloat(0.1f));
  print("FixedFloat::log(FixedFloat(0.1f))", x);

  x = FixedFloat::ln(FixedFloat(16));
  print("FixedFloat::ln(FixedFloat(16))", x);  
  x = FixedFloat::ln(FixedFloat::e);
  print("FixedFloat::ln(FixedFloat::e)", x);  

  x = FixedFloat::to_rad(FixedFloat(90));
  print("FixedFloat::to_rad(FixedFloat(90))", x);
  x = FixedFloat::to_rad(FixedFloat(180));
  print("FixedFloat::to_rad(FixedFloat(180))", x);

  x = FixedFloat::to_deg(FixedFloat::pi);
  print("FixedFloat::to_deg(FixedFloat::pi)", x);
  x = FixedFloat::to_deg(FixedFloat::half_pi);
  print("FixedFloat::to_deg(FixedFloat::half_pi)", x);


  printf("//////////////////\n\n");
  // for(double r = 0; r <= 6.283; r += 0.001)
  // {
  //   FixedFloat sv = FixedFloat::cos(FixedFloat(r));
  //   printf("%lld,\n", sv.value);
  // }
}