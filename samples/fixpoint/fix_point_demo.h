#include "application.h"
#include "fraction.h"

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
};