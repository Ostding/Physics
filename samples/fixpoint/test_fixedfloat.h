#ifndef __TEST_FIXED_FLOAT_H__
#define __TEST_FIXED_FLOAT_H__

#include <iostream>
#include <cstdlib>
#include "fixedfloat.h"
using namespace std;
using namespace physics;


class TestFixedFloat
{
public:
	static void print(const char *pszCap, const FixedFloat &f);
	static void print(const char *psz);
	static void doTest();
};

#endif