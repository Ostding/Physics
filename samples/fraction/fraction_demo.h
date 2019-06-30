#include "application.h"
#include "fraction.h"

class FractionDemo : public Application
{
public:
  FractionDemo(const char *title, int width, int height);

public:
  virtual void onDisplay();
  void onKeyboardPress(unsigned char key);

private:
  void doTestFraction();
  void print(const char* pszCap, Fraction &f);
};