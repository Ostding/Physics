#include "application.h"
#include "fixed_float.h"
using namespace physics;

class FixPointDemo : public Application
{
public:
  FixPointDemo(const char *title, int width, int height);

public:
  virtual void onDisplay();
  void onKeyboardPress(unsigned char key);

private:
  void doTestFixedFloat();
  void doTestSpeed();

  void doTestCores();
};