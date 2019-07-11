#ifndef __FIXED_FLOAT_H__
#define __FIXED_FLOAT_H__
#include <stdint.h>
#include <cmath>
#include <iostream>
using namespace std;

namespace physics
{
	typedef int64_t 	int64;
	typedef uint64_t 	uint64;

	#define enlarge 100000
	#define ffcast(v)		((int64)(v * enlarge)) 
	#define tofloat(v)	(((float)v / (float)enlarge))

  class FixedFloat
  {
	public:
		FixedFloat(){ value = 0; }
		FixedFloat(const FixedFloat &other) { value = other.value; }
		FixedFloat(const int other) 		{ value = (int64)other; }
		FixedFloat(const long other)		{ value = ffcast(other);}
		FixedFloat(const int64 other) 	{ value = ffcast(other); }
		FixedFloat(const float other) 	{ value = ffcast(other); }
		FixedFloat(const double other) 	{ value = ffcast(other); }

	public:
		static const FixedFloat zero;
		static const FixedFloat one;
		static const FixedFloat half;
		static const FixedFloat two;
		static const FixedFloat ten;
		static const FixedFloat f100;
		static const FixedFloat f90;
		static const FixedFloat f180;

		static const FixedFloat pi;
		static const FixedFloat half_pi;
		static const FixedFloat quat_pi;
		static const FixedFloat two_pi;
		
		static const FixedFloat rad_unit;
		static const FixedFloat deg_unit;
		static const FixedFloat e;

	public:
		int 	to_i() const { return (int)tofloat(value); }
		float to_f() const { return tofloat(value); }
		const char * to_s() const
		{
			char ret[32] = {0};
			float v = tofloat(value);
			sprintf(ret, "%.5f", v);
			return ret;
		}

	public:
		FixedFloat & operator = (const FixedFloat & other) { value = other.value; return *this; }
		FixedFloat & operator = (const int other) 	{ value = ffcast(other); return *this; }
		FixedFloat & operator = (const int64 other) { value = other; return *this; }
		FixedFloat & operator = (const long other) 	{ value = ffcast(other); return * this; }
		FixedFloat & operator = (const float other) { value = ffcast(other); return * this; }

		bool operator < 	(const FixedFloat & other) const { return value <  other.value; }
		bool operator <= 	(const FixedFloat & other) const { return value <= other.value; }
		bool operator > 	(const FixedFloat & other) const { return value >  other.value; }
		bool operator >= 	(const FixedFloat & other) const { return value >= other.value; }
		bool operator == 	(const FixedFloat & other) const { return value == other.value; }

		int64 operator + (const FixedFloat & other) const { return value + other.value; } 
		int64 operator - (const FixedFloat & other) const { return value - other.value; } 
		
		int64 operator * (const FixedFloat & other) const
		{
			float a = to_f();
			float b = other.to_f();
			return ffcast(a * b);
		}

		int64 operator / (const FixedFloat & other) const
		{
			float a = to_f();
			float b = other.to_f();
			return ffcast(a / b);
		}

		FixedFloat & operator += (const FixedFloat & other) { value += other.value; return *this; }
		FixedFloat & operator -= (const FixedFloat & other) { value -= other.value; return *this; }
		
		FixedFloat & operator *= (const FixedFloat & other) 
		{ 
			float a = to_f();
			float b = other.to_f();
			value = ffcast(a * b);
			return *this;
		}

		FixedFloat & operator /= (const FixedFloat & other) 
		{ 
			float a = to_f();
			float b = other.to_f();
			value = ffcast(a / b);
			return *this;
		}

		int64 operator -(void) const { return -value; }

	public:
		bool positive() const { return value > 0; }
		bool negative() const { return value < 0; }
		int round() const { return to_i(); }
		int64 abs() const { return abs(value); }

		static int64 sin(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::sin(f);
			return ffcast(r);
		}

		static int64 cos(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::cos(f);
			return ffcast(r);
		}

		static int64 tan(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::tan(f);
			return ffcast(r);
		}

		static int64 asin(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::asin(f);
			return ffcast(r);
		}
		static int64 atan(const FixedFloat &v) { return atan2(v, one); }
		static int64 atan2(const FixedFloat &y, const FixedFloat &x)
		{
			float fx = x.to_f();
			float fy = y.to_f();
			float r = std::atan2f(fy, fx);
			return ffcast(r);
		}

		static int64 abs(const FixedFloat &v) { return std::abs(v.value); }
		static int64 pow(const FixedFloat &n, const FixedFloat &e) 
		{ 
			float fn = n.to_f();
			float fe = e.to_f();
			float r = std::pow(fn, fe);
			return ffcast(r);
		}

		static float fast_inv_sqrt(float x)
		{
	    float xhalf = 0.5f * x;
	    int i = *(int*)&x;
	    i = 0x5f3759df - (i >> 1);
	    x = *(float*)&i;  x = x*(1.5f - (xhalf * x * x));
	    return x;
		}

		static int64 rsqrt(const FixedFloat &v)
		{
			float f =  v.to_f();
			float r = fast_inv_sqrt(f);
			return ffcast(r);
		}

		static int64 sqrt(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = f * fast_inv_sqrt(f);
			return ffcast(r);
		}

		static int64 exp(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::exp(f);
			return ffcast(r);
		}

		static int64 mod(const FixedFloat &a, const FixedFloat &b)
		{
			float fa = a.to_f();
			float fb = b.to_f();
			float r = std::fmod(fa, fb);
			return ffcast(r);
		}

		static int64 log(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::log10(f);
			return ffcast(r);
		}

		static int64 ln(const FixedFloat &v)
		{
			float f = v.to_f();
			float r = std::log(f);
			return ffcast(r);
		}

		static int64 to_rad(const FixedFloat &v) { return v * rad_unit; }
		static int64 to_deg(const FixedFloat &v) { return v * deg_unit; }


  public:
    int64 value;

  };
}

#endif
