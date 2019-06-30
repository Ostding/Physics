#include "fraction_demo.h"

FractionDemo::FractionDemo(const char *title, int width, int height)
:Application(title, width, height)
{

}

void FractionDemo::onDisplay()
{
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glLoadIdentity();
   gluLookAt(5.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
   
   glColor3f(0.0f, 0.0f, 0.0f);
   
   textOut(10.0f, 34.0f, "Physic Demo:\n Test Fraction");
}


void FractionDemo::onKeyboardPress(unsigned char key)
{
  switch( key ) {
  case 'g': case 'G':
    doTestFraction();
    break;
  }

  Application::onKeyboardPress(key);
}

void FractionDemo::print(const char* pszCap, Fraction &f)
{
  float v = f.to_f();
  printf("%s => %.4f nom:%lld den:%lld \n", pszCap, v, f.nom, f.den);
}

void FractionDemo::doTestFraction()
{
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


  char psz[32] = {0};
  a.cstr(psz);
  printf("a.cstr(psz) => %s\n", psz);

}



Application * getApp()
{
    return new FractionDemo("FractionDemo", 1136, 640);
}
