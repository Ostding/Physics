#include "test_fixed_float.h"
#include <stdarg.h>


void TestFixedFloat::doTest()
{
  myPrintf("/////////////Test FixedFloat////////////////////\n");
  myPrintf("////test constructors\n");
  FixedFloat a = FixedFloat();
  printff("FixedFloat a = FixedFloat()", a);
  FixedFloat b = FixedFloat(1);
  printff("FixedFloat b = FixedFloat()", b);
  FixedFloat c = FixedFloat(12345678);
  printff("FixedFloat c = FixedFloat(12345678)", c);
  FixedFloat d = FixedFloat(12345678LL);
  printff("FixedFloat d = FixedFloat(12345678LL)", d);
  FixedFloat e = FixedFloat(3.141592);
  printff("FixedFloat e = FixedFloat(3.141592)", e);
  FixedFloat f = FixedFloat(3.141592f);
  printff("FixedFloat f = FixedFloat(3.141592f)", f);

  myPrintf("////test consts\n");
  printff("FixedFloat::zero", FixedFloat::zero);
  printff("FixedFloat::one", FixedFloat::one);
  printff("FixedFloat::half", FixedFloat::half);
  printff("FixedFloat::two", FixedFloat::two);
  printff("FixedFloat::ten", FixedFloat::ten);
  printff("FixedFloat::f100", FixedFloat::f100);
  printff("FixedFloat::f90", FixedFloat::f90);
  printff("FixedFloat::pi", FixedFloat::pi);
  printff("FixedFloat::half_pi", FixedFloat::half_pi);
  printff("FixedFloat::quat_pi", FixedFloat::quat_pi);
  printff("FixedFloat::two_pi", FixedFloat::two_pi);
  printff("FixedFloat::rad_unit", FixedFloat::rad_unit);
  printff("FixedFloat::deg_unit", FixedFloat::deg_unit);
  printff("FixedFloat::e", FixedFloat::e);

  myPrintf("////test convertion\n");
  FixedFloat x = FixedFloat(12);
  int n = x.to_i();
  myPrintf("FixedFloat(12).to_i() => %d \n", n);

  x = FixedFloat(12.12345f);
  n = x.to_i();
  myPrintf("FixedFloat(12.12345).to_i() => %d \n", n);
  double dd = x.to_d();
  myPrintf("FixedFloat(12.12345).to_d() => %.5f \n", dd);

  myPrintf("////test operation override\n");
  FixedFloat y = FixedFloat(12345678.12345);
  x = FixedFloat();
  printff("x", x);
  printff("y", y);
  x = y;
  printff("x = y", x);
  x = 123;
  printff("x = 123", x);
  x = 123456789LL;
  printff("x = 123456789LL", x);
  x = 123.456f;
  printff("x = 123.456f", x);
  x = 456.789;
  printff("x = 456.789", x);
  bool _b = x < y;
  myPrintf("x < y => %s \n", _b ? "true" : "false");
  _b = x <= y;
  myPrintf("x <= y => %s \n", _b ? "true" : "false");
  _b = x > y;
  myPrintf("x > y => %s \n", _b ? "true" : "false");
  _b = x >= y;
  myPrintf("x >= y => %s \n", _b ? "true" : "false");
  _b = x == y;
  myPrintf("x == y => %s \n", _b ? "true" : "false");
  _b = FixedFloat(123.456f) == FixedFloat(12345600LL);
  myPrintf("FixedFloat(123.456f) == FixedFloat(1234500000LL) => %s \n", _b ? "true" : "false");

  x = FixedFloat(10);
  y = FixedFloat(2.5f);
  printff("x", x);
  printff("y", y);
  FixedFloat z = x + y;
  printff("z = x + y", z);
  z = x - y;
  printff("z = x - y", z);
  z = x * y;
  printff("z = x * y", z);
  z = x / y;
  printff("z = x / y", z);

  x += y;
  printff("x += y", x);
  x -= y;
  printff("x -= y", x);
  x *= y;
  printff("x *= y", x);
  x /= y;
  printff("x /= y", x);
  x = -x;
  printff("x = -x", x);

  myPrintf("////test functions\n");
  _b = x.positive();
  myPrintf("x.positive() => %s \n", _b ? "true" : "false");
  _b = x.negative();
  myPrintf("x.negative() => %s \n", _b ? "true" : "false");
  n = y.round();
  myPrintf("y.round() => %d \n", n);
  x = x.abs();
  printff("x.abs()", x);

  x = FixedFloat::sin(FixedFloat::zero);
  printff("FixedFloat::sin(FixedFloat::zero)", x);
  x = FixedFloat::sin(FixedFloat::half_pi);
  printff("FixedFloat::sin(FixedFloat::half_pi)", x);
  x = FixedFloat::sin(FixedFloat::quat_pi);
  printff("FixedFloat::sin(FixedFloat::quat_pi)", x);
  x = FixedFloat::sin(FixedFloat::pi);
  printff("FixedFloat::sin(FixedFloat::pi)", x);

  x = FixedFloat::cos(FixedFloat::zero);
  printff("FixedFloat::cos(FixedFloat::zero)", x);
  x = FixedFloat::cos(FixedFloat::half_pi);
  printff("FixedFloat::cos(FixedFloat::half_pi)", x);
  x = FixedFloat::cos(FixedFloat::quat_pi);
  printff("FixedFloat::cos(FixedFloat::quat_pi)", x);
  x = FixedFloat::cos(FixedFloat::pi);
  printff("FixedFloat::cos(FixedFloat::pi)", x);

  x = FixedFloat::tan(FixedFloat::zero);
  printff("FixedFloat::tan(FixedFloat::zero)", x);
  x = FixedFloat::tan(FixedFloat::half_pi);
  printff("FixedFloat::tan(FixedFloat::half_pi)", x);
  x = FixedFloat::tan(FixedFloat::quat_pi);
  printff("FixedFloat::tan(FixedFloat::quat_pi)", x);
  x = FixedFloat::tan(FixedFloat::pi);
  printff("FixedFloat::tan(FixedFloat::pi)", x);

  x = FixedFloat::asin(FixedFloat::zero);
  printff("FixedFloat::asin(FixedFloat::zero)", x);
  x = FixedFloat::asin(FixedFloat::one);
  printff("FixedFloat::asin(FixedFloat::one)", x);
  x = FixedFloat::asin(FixedFloat::half);
  printff("FixedFloat::asin(FixedFloat::half)", x);

  x = FixedFloat::acos(FixedFloat::zero);
  printff("FixedFloat::acos(FixedFloat::zero)", x);
  x = FixedFloat::acos(FixedFloat::one);
  printff("FixedFloat::acos(FixedFloat::one)", x);

  x = FixedFloat::atan(FixedFloat::zero);
  printff("FixedFloat::atan(FixedFloat::zero)", x);
  x = FixedFloat::atan(FixedFloat::one);
  printff("FixedFloat::atan(FixedFloat::one)", x);
  x = FixedFloat::atan(FixedFloat::half);
  printff("FixedFloat::atan(FixedFloat::half)", x);
  
  x = FixedFloat::atan2(FixedFloat::one, FixedFloat::zero);
  printff("FixedFloat::atan2(FixedFloat::one, FixedFloat::zero)", x);
  x = FixedFloat::atan2(FixedFloat::two, FixedFloat::one);
  printff("FixedFloat::atan2(FixedFloat::two, FixedFloat::one)", x);

  x = FixedFloat::sqrt(FixedFloat(4));
  printff("FixedFloat::sqrt(FixedFloat(4))", x);
  x = FixedFloat::sqrt(FixedFloat(2));
  printff("FixedFloat::sqrt(FixedFloat(2))", x);

  x = FixedFloat::sqrt(FixedFloat(1));
  printff("FixedFloat::sqrt(FixedFloat(1))", x);
  x = FixedFloat::sqrt(FixedFloat(2));
  printff("FixedFloat::sqrt(FixedFloat(2))", x);

  x = FixedFloat::mod(FixedFloat(5), FixedFloat(2));
  printff("FixedFloat::mod(FixedFloat(5), FixedFloat(2))", x);
  x = FixedFloat::mod(FixedFloat(3.5f), FixedFloat(1.5f));
  printff("FixedFloat::mod(FixedFloat(3.5f), FixedFloat(1.5f))", x);
  
  x = FixedFloat::log(FixedFloat(1000));
  printff("FixedFloat::log(FixedFloat(1000))", x);
  x = FixedFloat::log(FixedFloat(100));
  printff("FixedFloat::log(FixedFloat(100))", x);
  x = FixedFloat::log(FixedFloat(0.1f));
  printff("FixedFloat::log(FixedFloat(0.1f))", x);

  x = FixedFloat::ln(FixedFloat(16));
  printff("FixedFloat::ln(FixedFloat(16))", x);  
  x = FixedFloat::ln(FixedFloat::e);
  printff("FixedFloat::ln(FixedFloat::e)", x);  

  x = FixedFloat::to_rad(FixedFloat(90));
  printff("FixedFloat::to_rad(FixedFloat(90))", x);
  x = FixedFloat::to_rad(FixedFloat(180));
  printff("FixedFloat::to_rad(FixedFloat(180))", x);

  x = FixedFloat::to_deg(FixedFloat::pi);
  printff("FixedFloat::to_deg(FixedFloat::pi)", x);
  x = FixedFloat::to_deg(FixedFloat::half_pi);
  printff("FixedFloat::to_deg(FixedFloat::half_pi)", x);


  myPrintf("//////////////////\n\n");
  //Precalculated

  // for(double r = 0; r <= 6.283; r += 0.001)
  // {
  //   FixedFloat sv = FixedFloat::sin(FixedFloat(r));
  //   printf("%lld,\n", sv.value);
  // }

  // for(double r = 0; r <= 6.283; r += 0.001)
  // {
  //   FixedFloat sv = FixedFloat::cos(FixedFloat(r));
  //   printf("%lld,\n", sv.value);
  // }

  for(double r = -1.0; r <= 1.0 ; r += 0.001)
  {
     FixedFloat sv = FixedFloat::acos(FixedFloat(r));
     myPrintf("%lld,\n", sv.value);
  }

}

void TestFixedFloat::doTestSpeed()
{
  myPrintf("//////////////////test speed///////////////////////////\n");
  int count = 10000000;
  //base value type's calculation
  double i = 123456;
  double j = 3.141592;
  double ret = 0.0;
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i + j;
  Timer::stop("Base Value type +");
  myPrintf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i - j;
  Timer::stop("Base Value type -");
  myPrintf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i * j;
  Timer::stop("Base Value type *");
  myPrintf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = i / j;
  Timer::stop("Base Value type /");
  myPrintf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = sqrt(i);
  Timer::stop("Base Value type sqrt");
  myPrintf("ret:%.8f \n", ret);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    ret = sin(1.571f);
  Timer::stop("Base Value type sin");
  myPrintf("ret:%.8f \n", ret);
  myPrintf("%s\n\n", "");


  //test normal calculation of FixedFloat
  FixedFloat m = FixedFloat(123456);
  FixedFloat n = FixedFloat(3.14159f);
  FixedFloat ret1;
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m + n;
  Timer::stop("FixedFloat +");
  printff("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m - n;
  Timer::stop("FixedFloat -");
  printff("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m * n;
  Timer::stop("FixedFloat *");
  printff("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m / n;
  Timer::stop("FixedFloat /");
  printff("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sqrt(m);
  Timer::stop("FixedFloat sqrt");
  printff("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sin(FixedFloat::half_pi);
  Timer::stop("FixedFloat sin");
  printff("ret1", ret1);
  myPrintf("%s\n\n", "");
}