#include "application.h"
#include "fraction.h"
#include "fixedpt.h"
#include "fixedptc.h"
using namespace physics;

class FixPointDemo : public Application
{
public:
  FixPointDemo(const char *title, int width, int height);

public:
  virtual void onDisplay();
  void onKeyboardPress(unsigned char key);

private:
  void doTestFraction();
  void doTestFixedPt();
  void print(const char* pszCap, Fraction &f);
  void print(const char* pszCap, fixedpt &f);
  void print(const char* pszCap, FixedPt &f);
};