#ifndef __FIXED_FLOAT_H__
#define __FIXED_FLOAT_H__
#include <stdint.h>
#include <cmath>
#include <iostream>
using namespace std;

namespace physics
{
	typedef long long 	        int64;
	typedef unsigned long long 	uint64;

	#define enlarge 100000000LL
	#define ficast(v) 	((int64)v * enlarge)
	#define ffcast(v)		((int64)((double)v * enlarge))
	#define fdcast(v)	((double)v / enlarge)

  class FixedFloat
  {
	public:
		FixedFloat()
		{
		 	value = 0; 
		}

		FixedFloat(const FixedFloat &other) 
		{ 
			value = other.value; 
		}

		FixedFloat(const int other) 		
		{ 
			value = ficast(other);
		}

		FixedFloat(const int64 other) 	
		{ 
			value = other;
		}

		FixedFloat(const float other) 	
		{
			value = ffcast(other); 
		}

		FixedFloat(const double other) 	
		{
			value = ffcast(other); 
		}


	public:
		static const FixedFloat max;
		static const FixedFloat min;
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
		int 	 to_i() const 
		{ 
			return (int)fdcast(value); 
		}
		double to_d() const 
		{ 
			return fdcast(value);
		}
		
		const char * to_s() const
		{
			static char ret[32];
			memset(ret, 0, 32);
			double v = fdcast(value);
			sprintf(ret, "%.8f", v);
			return ret;
		}

	public:
		FixedFloat & operator = (const FixedFloat & other) 
		{ 
			value = other.value; return *this; 
		}

		FixedFloat & operator = (const int other) 	
		{ 
			value = ffcast(other); return *this; 
		}

		FixedFloat & operator = (const int64 other) 
		{ 
			value = other; return *this; 
		}

		FixedFloat & operator = (const long other) 	
		{ 
			value = ffcast(other); return * this; 
		}

		FixedFloat & operator = (const float other) 
		{ 
			value = ffcast(other); return * this; 
		}

		FixedFloat & operator = (const double other)
		{ 
			value = ffcast(other); return * this; 
		}


		bool operator < 	(const FixedFloat & other) const 
		{ 
			return value <  other.value; 
		}

		bool operator <= 	(const FixedFloat & other) const 
		{ 
			return value <= other.value; 
		}

		bool operator > 	(const FixedFloat & other) const 
		{ 
			return value >  other.value; 
		}

		bool operator >= 	(const FixedFloat & other) const 
		{ 
			return value >= other.value; 
		}

		bool operator == 	(const FixedFloat & other) const 
		{ 
			return value == other.value; 
		}

		bool operator != 	(const FixedFloat & other) const 
		{ 
			return value != other.value; 
		}


		FixedFloat operator + (const FixedFloat & other) const 
		{ 
			return value + other.value; 
		}

		FixedFloat operator - (const FixedFloat & other) const 
		{ 
			return value - other.value;
		} 

		FixedFloat operator * (const FixedFloat & other) const
		{
			double a = to_d();
			double b = other.to_d();
			return a * b;
		}

		FixedFloat operator / (const FixedFloat & other) const
		{
			double a = to_d();
			double b = other.to_d();
			double dr = a / b;
			return ((int64)(dr * enlarge));
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
			if(value == 0LL) 
				return *this;

			double a = to_d();
			double b = other.to_d();
			value = ffcast(a / b);
			value = (value == 0LL) ? 1LL : value;
			return *this;
		}

		FixedFloat operator -(void) const { return FixedFloat(-value); }

	public:
		bool positive() const { return value > 0; }
		bool negative() const { return value < 0; }
		int round() const { return to_i(); }
		FixedFloat abs() const { return abs(value); }

		static const int64 sin_table[6283];
		static FixedFloat sin(const FixedFloat &v)
		{
			// double f = v.to_d();
			// return std::sin(f);

			int64 i = std::fmod(v.value/100000, 6283);
			if(i < 0) i += 6283;
			return sin_table[i];
		}

		static const int64 cos_table[6283];
		static FixedFloat cos(const FixedFloat &v)
		{
			// double f = v.to_d();
			// return std::cos(f);

			int64 i = std::fmod(v.value/100000, 6283);
			if(i < 0) i += 6283;
			return cos_table[i];
		}

		static FixedFloat tan(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::tan(f);
			return r;
		}

		static FixedFloat asin(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::asin(f);
			return r;
		}

		static const int64 acos_table[2001];
		static FixedFloat acos(const FixedFloat &v)
		{
			// double d = v.to_d();
			// return std::acos(d);

			if(v > FixedFloat::one || v < -FixedFloat::one)
				return 0;
			
			int64 i = std::fmod(v.value/100000, 2001);
			return acos_table[i+1000];
		}

		static FixedFloat atan(const FixedFloat &v) { return atan2(v, 100000000LL); }

		static FixedFloat atan2(const FixedFloat &y, const FixedFloat &x)
		{
			double fx = x.to_d();
			double fy = y.to_d();
			double r = std::atan2(fy, fx);
			return r;
		}

		static FixedFloat abs(const FixedFloat &v) 
		{ 
			return std::abs(v.value); 
		}

		static FixedFloat pow(const FixedFloat &n, const FixedFloat &e) 
		{ 
			double fn = n.to_d();
			double fe = e.to_d();
			double r = std::pow(fn, fe);
			return r;
		}

		static FixedFloat sqrt(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::sqrt(f);
			return r;
		}

		static FixedFloat exp(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::exp(f);
			return r;
		}

		static FixedFloat mod(const FixedFloat &a, const FixedFloat &b)
		{
			double fa = a.to_d();
			double fb = b.to_d();
			double r = std::fmod(fa, fb);
			return r;
		}

		static FixedFloat log(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::log10(f);
			return r;
		}

		static FixedFloat ln(const FixedFloat &v)
		{
			double f = v.to_d();
			double r = std::log(f);
			return r;
		}

		static FixedFloat to_rad(const FixedFloat &v) 
		{ 	
			return v * rad_unit; 
		}

		static FixedFloat to_deg(const FixedFloat &v) 
		{ 
			return v * deg_unit; 
		}

  public:
    int64 value;

  };
}

#endif
