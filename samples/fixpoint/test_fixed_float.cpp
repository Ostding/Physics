#include "test_fixed_float.h"
#include "timer.h"
#include <stdarg.h>
#include "my_printf.h"

#ifdef __ANDROID__
#include <android/log.h>

void TestFixedFloat::print(const char *pszCap, const FixedFloat &f)
{
	char psz[256] = {0};
	sprintf(psz, "%s => %s \n", pszCap, f.to_s());
	__android_log_print(ANDROID_LOG_INFO, LOG_TGA, "%s", psz);
}

#else

void TestFixedFloat::print(const char *pszCap, const FixedFloat &f)
{
  printf("%s => %s \n", pszCap, f.to_s());
}

#endif



void TestFixedFloat::doTest()
{
  myPrintf("/////////////Test FixedFloat////////////////////\n");
  myPrintf("////test constructors\n");
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

  myPrintf("////test consts\n");
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

  myPrintf("////test functions\n");
  _b = x.positive();
  myPrintf("x.positive() => %s \n", _b ? "true" : "false");
  _b = x.negative();
  myPrintf("x.negative() => %s \n", _b ? "true" : "false");
  n = y.round();
  myPrintf("y.round() => %d \n", n);
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


  myPrintf("//////////////////\n\n");
  // for(double r = 0; r <= 6.283; r += 0.001)
  // {
  //   FixedFloat sv = FixedFloat::cos(FixedFloat(r));
  //   printf("%lld,\n", sv.value);
  // }
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
  print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m - n;
  Timer::stop("FixedFloat -");
  print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m * n;
  Timer::stop("FixedFloat *");
  print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = m / n;
  Timer::stop("FixedFloat /");
  print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sqrt(m);
  Timer::stop("FixedFloat sqrt");
  print("ret1", ret1);
  Timer::start();
  for(int i = 0; i < count; i++)
    ret1 = FixedFloat::sin(FixedFloat::half_pi);
  Timer::stop("FixedFloat sin");
  print("ret1", ret1);
  myPrintf("%s\n\n", "");
}