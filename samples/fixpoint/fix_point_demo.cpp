#include "fix_point_demo.h"
#include "timer.h"

FixPointDemo::FixPointDemo(const char *title, int width, int height)
:Application(title, width, height)
{
}

void FixPointDemo::onDisplay()
{
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();
   gluLookAt(5.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
   
   glColor3f(0.0f, 0.0f, 0.0f);
   
   textOut(10.0f, 34.0f, "Physic Demo: Test Fraction \n \
      Press 'g' to run demo and watch console for fraction calculation; \n \
      Press 'q' to quite sample application;");
}


void FixPointDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    // doTestFraction();
    // doTestFixedPt();

    doTestFixedFloat();
    // doTestSpeed();

    break;
  }

  Application::onKeyboardPress(key);
}

void FixPointDemo::print(const char* pszCap, Fraction &f)
{
  float v = f.to_f();
  printf("%s => %.5f nom:%lld den:%lld \n", pszCap, v, f.nom, f.den);
}

void FixPointDemo::print(const char* pszCap, fixedpt &f)
{
  printf("%s => %s \n", pszCap, fixedpt_cstr(f, 5));
}

void FixPointDemo::print(const char* pszCap, FixedPt &f)
{
  printf("%s => %s \n", pszCap, f.cstr());
}

void FixPointDemo::print(const char *pszCap, const FixedFloat &f)
{
  printf("%s => %s \n", pszCap, f.to_s());
}

void FixPointDemo::doTestSpeed()
{
  printf("//////////////////test speed///////////////////////////\n");
  int count = 10000000;
  //base value type's calculation
  double i = 123456;
  double j = 3.141592;
  double tmp2 = 0.0;
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    tmp2 = i + j;
  Timer::stop("Base Value type +");
  printf("tmp2:%.8f \n", tmp2);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    tmp2 = i - j;
  Timer::stop("Base Value type -");
  printf("tmp2:%.8f \n", tmp2);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    tmp2 = i * j;
  Timer::stop("Base Value type *");
  printf("tmp2:%.8f \n", tmp2);
  Timer::start();
  for(int _i = 0; _i < count; _i++)
    tmp2 = i / j;
  Timer::stop("Base Value type /");
  printf("tmp2:%.8f \n", tmp2);
   Timer::start();
  for(int _i = 0; _i < count; _i++)
    tmp2 = sqrt(i);
  Timer::stop("Base Value type /");
  printf("tmp2:%.8f \n", tmp2);
  printf("\n\n");

  //test normal calculation of fraction
  // Fraction a = Fraction(123456);
  // Fraction b = Fraction(3.14159f);
  // Fraction tmp;
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp = a + b;
  // Timer::stop("Fraction +");
  // print("tmp", tmp);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp = a - b;
  // Timer::stop("Fraction -");
  // print("tmp", tmp);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp = a * b;
  // Timer::stop("Fraction *");
  // print("tmp", tmp);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp = a / b;
  // Timer::stop("Fraction /");
  // print("tmp", tmp);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp = a.sqrt();
  // Timer::stop("Fraction sqrt");
  // print("tmp", tmp);
  // printf("\n\n");

  //test normal calculation of FixedFloat
  FixedFloat m = FixedFloat(123456);
  FixedFloat n = FixedFloat(3.14159f);
  FixedFloat tmp3;
  Timer::start();
  for(int i = 0; i < count; i++)
    tmp3 = m + n;
  Timer::stop("FixedFloat +");
  print("tmp3", tmp3);
  Timer::start();
  for(int i = 0; i < count; i++)
    tmp3 = m - n;
  Timer::stop("FixedFloat -");
  print("tmp3", tmp3);
  Timer::start();
  for(int i = 0; i < count; i++)
    tmp3 = m * n;
  Timer::stop("FixedFloat *");
  print("tmp3", tmp3);
  Timer::start();
  for(int i = 0; i < count; i++)
    tmp3 = m / n;
  Timer::stop("FixedFloat /");
  print("tmp3", tmp3);
  Timer::start();
  for(int i = 0; i < count; i++)
    tmp3 = FixedFloat::sqrt(m);
  Timer::stop("FixedFloat sqrt");
  print("tmp3", tmp3);
  printf("\n\n");

  // test normal calculation of FixedPt
  // FixedPt x = FixedPt(123456);
  // FixedPt y = FixedPt(3.14159f);
  // FixedPt tmp1;

  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp1 = x + y;
  // Timer::stop("FixedPt +");
  // print("tmp1", tmp1);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp1 = x - y;
  // Timer::stop("FixedPt -");
  // print("tmp1", tmp1);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp1 = x * y;
  // Timer::stop("FixedPt *");
  // print("tmp1", tmp1);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp1 = x / y;
  // Timer::stop("FixedPt /");
  // print("tmp1", tmp1);
  // Timer::start();
  // for(int i = 0; i < count; i++)
  //   tmp1 = x.sqrt();
  // Timer::stop("FixedPt sqrt");
  // print("tmp1", tmp1);

}

// void FixPointDemo::doTestFixedPt()
// {
//   printf("/////////////Test FixedPt////////////////////\n");
//   fixedpt a = fixedpt_rconst(2.123);
//   fixedpt b = fixedpt_rconst(20);
//   fixedpt c = fixedpt_mul(a,b);
//   printf("fixedpt a*b => %s \n", fixedpt_cstr(c, -1));
 
//   c = fixedpt_div(b, a);
//   printf("fixedpt b/a => %s \n", fixedpt_cstr(c, -1));
//   // c = fixedpt_div_test(b, a);
//   // printf("fixedpt b/a test => %s \n", fixedpt_cstr(c, 5));


//   a = fixedpt_rconst(3.1415926/2);
//   fixedpt r = fixedpt_sin(a);
//   print("fixedpt sin(pi/2)", r);
//   r = fixedpt_cos(a);
//   print("fixedpt cos(pi/2)", r);

//   a = fixedpt_rconst(3.1415926/6);
//   r = fixedpt_sin(a);
//   print("fixedpt sin(pi/6)", r);
//   r = fixedpt_cos(a);
//   print("fixedpt cos(pi/6)", r);

  
//   FixedPt x = FixedPt(3.1415926);
//   print("x = FixedPt(3.1415926)", x);
//   x = FixedPt(2);
//   print("x = FixedPt(2)", x);
//   FixedPt y = x.sqrt();
//   print("x.sqrt()", y);
//   x = FixedPt(0.040f);
//   print("x = FixedPt(0.04)", x);
//   y = x.sqrt();
//   print("x.sqrt()", y);

// }

void FixPointDemo::doTestFixedFloat()
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
  //   FixedFloat sv = FixedFloat::sin(FixedFloat(r));
  //   printf("%lld,\n", sv.value);
  // }

  x = FixedFloat::sin_t(FixedFloat::zero);
  print("FixedFloat::sin_t(FixedFloat::zero)", x);
  x = FixedFloat::sin_t(FixedFloat::half_pi);
  print("FixedFloat::sin_t(FixedFloat::half_pi)", x);
  x = FixedFloat::sin_t(FixedFloat::quat_pi);
  print("FixedFloat::sin_t(FixedFloat::quat_pi)", x);
  x = FixedFloat::sin_t(FixedFloat::pi);
  print("FixedFloat::sin_t(FixedFloat::pi)", x);
  x = FixedFloat::sin_t(FixedFloat::two_pi);
  print("FixedFloat::sin_t(FixedFloat::two_pi)", x);

  for(i = 0; i< 1000000; i++)
  {
    
  }
}

void FixPointDemo::doTestFraction()
{
  printf("/////////////Test Fraction////////////////////\n");
  printf(">>>>>>>> constructors\n");
  Fraction a = Fraction();
  print("Fraction()", a);

  a = Fraction(1, 2);
  print("Fraction(1,2)", a);  
  Fraction b = Fraction(a);
  print("Fraction b = Fraction(a)", b);

  a = Fraction(1LL, 2LL);
  print("Fraction(1LL, 2LL)", a);  

  a = Fraction(0.123456f);
  print("Fraction(0.123456f)", a);  

  a = Fraction(0.123456);
  print("Fraction(0.123456)", a);  

  printf("\n>>>>>>>> const variables\n");
  print("Fraction::f0", Fraction::f0);
  print("Fraction::f1", Fraction::f1);
  print("Fraction::f2", Fraction::f2);
  print("Fraction::f0_5", Fraction::f0_5);
  print("Fraction::f10", Fraction::f10);
  print("Fraction::f90", Fraction::f90);
  print("Fraction::f180", Fraction::f180);
  print("Fraction::pi", Fraction::pi);
  print("Fraction::pi_2", Fraction::pi_2);
  print("Fraction::pi_4", Fraction::pi_4);
  print("Fraction::pi2", Fraction::pi2);
  print("Fraction::rad_unit", Fraction::rad_unit);
  print("Fraction::deg_unit", Fraction::deg_unit);
  print("Fraction::e", Fraction::e);

  printf("\n>>>>>>>> from methods\n");

  a = Fraction();
  a.from(0.12345);
  print("a.from(0.12345)", a);

  a = Fraction();
  a.from(0.12345f);
  print("a.from(0.12345f)", a);

  a = Fraction();
  a.from(12345);
  print("a.from(12345)", a);

  a = Fraction();
  a.from(12345LL);
  print("a.from(12345LL)", a);


  printf("\n>>>>>>>> force cast\n");
  a = Fraction(1.12345);
  int nx = (int)a;
  printf("(int)Fraction(1.12345):%d\n", nx);  
  float fx = (float)a;
  printf("(float)Fraction(1.12345):%.4f\n", fx);    
  double dx = (double)a;
  printf("(double)Fraction(1.12345):%.4f\n", dx);    
  int64_t ix = (int64_t)a;
  printf("(int64_t)Fraction(1.12345):%lld\n", ix);    


  printf("\n>>>>>>>> operators\n");
  a = Fraction(1,2);
  print("a = Fraction(1,2)", a);
  b = a;
  print("b = a", b);

  b = 2;
  print("b = 2", b);

  b = 0.2f;
  print("b = 0.2f", b);

  b = 0.3456;
  print("b = 0.3456", b);  

  b = 345LL;
  print("b = 345LL", b);  

  bool _ret = b < a;
  char _c = _ret ? 'Y':'N';
  printf("b < a => %c \n", _c);    

  _ret = b > a;
  _c = _ret ? 'Y':'N';
  printf("b > a => %c \n", _c);    

  _ret = b <= a;
  _c = _ret ? 'Y':'N';
  printf("b <= a => %c \n", _c);  

  _ret = b >= a;
  _c = _ret ? 'Y':'N';
  printf("b >= a => %c \n", _c);  


  a = Fraction(1, 2);
  print("a = Fraction(1, 2)", a);
  b = Fraction(0.5);
  print("b = Fraction(0.5)", b);
  _ret = (b == a);
  _c = _ret ? 'Y':'N';
  printf("b == a => %c \n", _c);  

  _ret = (b != a);
  _c = _ret ? 'Y':'N';
  printf("b != a => %c \n", _c);  

  _ret = b < 10LL;
  _c = _ret ? 'Y':'N';
  printf("b < 10LL => %c \n", _c);   

  _ret = b > 10LL;
  _c = _ret ? 'Y':'N';
  printf("b > 10LL => %c \n", _c);     


  _ret = b <= 10LL;
  _c = _ret ? 'Y':'N';
  printf("b <= 10LL => %c \n", _c);   

  _ret = b >= 10LL;
  _c = _ret ? 'Y':'N';
  printf("b >= 10LL => %c \n", _c);   


  _ret = b == 10LL;
  _c = _ret ? 'Y':'N';
  printf("b == 10LL => %c \n", _c);   

  _ret = b != 10LL;
  _c = _ret ? 'Y':'N';
  printf("b != 10LL => %c \n", _c);   


  Fraction c = a + b;
  print("a+b", c);

  c = a - b;
  print("a - b", c);

  c = a * b;
  print("a * b", c);

  c = a / b;
  print("a / b", c);


  c = a + 10LL;
  print("a + 10LL", c);

  c = a - 10LL;
  print("a - 10LL", c);

  c = a * 10LL;
  print("a * 10LL", c);

  c = a / 10LL;
  print("a / 10LL", c);


  a += b;
  print("a += b", a);

  a -= b;
  print("a -= b", a);

  a *= b;
  print("a *= b", a);

  a /= b;
  print("a /= b", a);

  a += 10LL;
  print("a += 10LL", a);

  a -= 10LL;
  print("a -= 10LL", a);

  a *= 10LL;
  print("a *= 10LL", a);

  a /= 10LL;
  print("a /= 10LL", a);

  c = -a;
  print("c = -a", c);


  printf("\n>>>>>>>> public methods\n");
  c = c.invserse();
  print("c = c.invserse()", c);

  _ret = c.isPositive();
  _c = _ret ? 'Y':'N';
  printf("c.isPositive() => %c \n", _c);  

  _ret = c.isNegative();
  _c = _ret ? 'Y':'N';
  printf("c.isNegative() => %c \n", _c); 

  printf("a.round() => %d \n",  a.round());
  printf("a.to_i() => %d \n",  a.to_i());
  printf("a.to_l() => %ld \n",  a.to_l());
  printf("a.to_f() => %f \n",  a.to_f());
  printf("a.to_d() => %f \n",  a.to_d());

  char psz[32] = {0};
  a.cstr(psz);
  printf("a.cstr(psz) => %s\n", psz);

  printf("\n>>>>>>>> math methods\n");

  a = Fraction(0, 1);
  print("a=Fraction(0,1)", a);

  b = a.sin();
  print("a.sin()", b);

  b = a.cos();
  print("a.cos()", b); 

  b = a.tan();
  print("a.tan()", b);   
  
  a = Fraction::pi / 4LL;
  print("a=Fraction::pi / 4", a);
  b = a.sin();
  print("a.sin()", b);

  b = a.cos();
  print("a.cos()", b);

  b = a.tan();
  print("a.tan()", b); 

  a = Fraction(0, 1);
  b = a.asin();
  print("a.asin()", b); 

  b = a.acos();
  print("a.acos()", b); 

  b = a.atan();
  print("a.atan()", b); 

  a = Fraction(1);
  print("a=Fraction(1)", a);

  b = a.exp();
  print("a.exp()", b); 

  b = b.ln();
  print("b.ln()", b); 


  a = Fraction(100, 1);
  print("a=Fraction(100, 1)", a);

  b = a.log();
  print("a.log()", b); 

  b = a.sqrt();
  print("a.sqrt()", b); 

  a = Fraction(180);
  print("a=Fraction(180)", a);

  b = a.to_rad();
  print("a.to_rad()", b);

  a = b.to_deg();
  print("b.to_deg()", a);

  a = Fraction(2);
  b = Fraction(4);
  print("a = Fraction(2)", a);
  print("b = Fraction(4)", b);
  c = Fraction::pow(a, b);
  print("Fraction::pow(a, b)", c);

  a = Fraction(5);
  b = Fraction(3);
  print("a = Fraction(5)", a);
  print("b = Fraction(3)", b);
  c = Fraction::mod(a, b);
  print("Fraction::mod(a, b)", c);


}



Application * getApp()
{
    return new FixPointDemo("FixPointDemo", 1136, 640);
}
