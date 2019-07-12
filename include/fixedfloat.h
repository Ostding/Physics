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
	#define ficast(v) 	((int64)v * enlarge) 
	#define ffcast(v)		((int64)((double)v * enlarge))
	#define fdcast(v)	((double)v / enlarge)

  class FixedFloat
  {
	public:
		FixedFloat(){ value = 0; }
		FixedFloat(const FixedFloat &other) { value = other.value; }
		FixedFloat(const int other) 		{ value = ficast(other); }
		FixedFloat(const long other)		{ value = ficast(other);}
		FixedFloat(const int64 other) 	{ value = other; }
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
		int 	 to_i() const { return (int)fdcast(value); }
		double to_d() const { return fdcast(value);}
		
		const char * to_s() const
		{
			static char ret[32];
			memset(ret, 0, 32);
			double v = fdcast(value);
			sprintf(ret, "%.5f", v);
			return ret;
		}

	public:
		FixedFloat & operator = (const FixedFloat & other) { value = other.value; return *this; }
		FixedFloat & operator = (const int other) 	{ value = ffcast(other); return *this; }
		FixedFloat & operator = (const int64 other) { value = other; return *this; }
		FixedFloat & operator = (const long other) 	{ value = ffcast(other); return * this; }
		FixedFloat & operator = (const float other) { value = ffcast(other); return * this; }
		FixedFloat & operator = (const double other){ value = ffcast(other); return * this; }

		bool operator < 	(const FixedFloat & other) const { return value <  other.value; }
		bool operator <= 	(const FixedFloat & other) const { return value <= other.value; }
		bool operator > 	(const FixedFloat & other) const { return value >  other.value; }
		bool operator >= 	(const FixedFloat & other) const { return value >= other.value; }
		bool operator == 	(const FixedFloat & other) const { return value == other.value; }

		int64 operator + (const FixedFloat & other) const { return value + other.value; } 
		int64 operator - (const FixedFloat & other) const { return value - other.value; } 
		
		int64 operator * (const FixedFloat & other) const
		{
			double a = to_d();
			double b = other.to_d();
			return ffcast(a * b);
		}

		int64 operator / (const FixedFloat & other) const
		{
			double a = to_d();
			double b = other.to_d();
			return ffcast(a / b);
		}

		FixedFloat & operator += (const FixedFloat & other) { value += other.value; return *this; }
		FixedFloat & operator -= (const FixedFloat & other) { value -= other.value; return *this; }
		
		FixedFloat & operator *= (const FixedFloat & other) 
		{ 
			double a = to_d();
			double b = other.to_d();
			value = ffcast(a * b);
			return *this;
		}

		FixedFloat & operator /= (const FixedFloat & other) 
		{ 
			double a = to_d();
			double b = other.to_d();
			value = ffcast(a / b);
			return *this;
		}

		int64 operator -(void) const { return -value; }

	public:
		bool positive() const { return value > 0; }
		bool negative() const { return value < 0; }
		int round() const { return to_i(); }
		int64 abs() const { return abs(value); }

		static const int64 sin_table[6283];
		static int64 sin_t(const FixedFloat &v)
		{
			int64 i = std::fmod(v.value/100, 6283);
			return sin_table[i];
		}

		static int64 sin(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::sin(f);
			return ffcast(r);
		}

		static int64 cos(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::cos(f);
			return ffcast(r);
		}

		static int64 tan(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::tan(f);
			return ffcast(r);
		}

		static int64 asin(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::asin(f);
			return ffcast(r);
		}

		static int64 acos(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::acos(f);
			return ffcast(r);	
		}

		static int64 atan(const FixedFloat &v) { return atan2(v, one); }
		static int64 atan2(const FixedFloat &y, const FixedFloat &x)
		{
			double fx = x.to_d();
			double fy = y.to_d();
			double r = std::atan2(fy, fx);
			return ffcast(r);
		}

		static int64 abs(const FixedFloat &v) { return std::abs(v.value); }
		static int64 pow(const FixedFloat &n, const FixedFloat &e) 
		{ 
			double fn = n.to_d();
			double fe = e.to_d();
			double r = std::pow(fn, fe);
			return ffcast(r);
		}

		static int64 sqrt(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::sqrt(f);
			return ffcast(r);
		}

		static int64 exp(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::exp(f);
			return ffcast(r);
		}

		static int64 mod(const FixedFloat &a, const FixedFloat &b)
		{
			double fa = a.to_d();
			double fb = b.to_d();
			double r = std::fmod(fa, fb);
			return ffcast(r);
		}

		static int64 log(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::log10(f);
			return ffcast(r);
		}

		static int64 ln(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::log(f);
			return ffcast(r);
		}

		static int64 to_rad(const FixedFloat &v) { return v * rad_unit; }
		static int64 to_deg(const FixedFloat &v) { return v * deg_unit; }


  public:
    int64 value;

  };
}

#endif
