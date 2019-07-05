#ifndef __FIXEDPT_H__
#define __FIXEDPT_H__
#include "fixedpt.h"

namespace physics
{

	class FixedPt {
		public:
			fixedpt value;
			FixedPt() { value = 0; }
			FixedPt(const FixedPt &inValue)  { value = inValue.value;             }
			FixedPt(const fixedpt inValue) { value = inValue;                   }
			FixedPt(const float inValue)   { value = fixedpt_rconst(inValue); }
			FixedPt(const double inValue)  { value = fixedpt_rconst(inValue);   }
			FixedPt(const int16_t inValue) { value = fixedpt_fromint(inValue);   }
	#if FIXEDPT_BITS == 64
			FixedPt(const int inValue) { value = fixedpt_fromint(inValue);   }
	#endif
			FixedPt(const long inValue) { value = fixedpt_fromint(inValue);   }

			operator fixedpt() const { return value;                 }
			operator double()  const { return fixedpt_tofloat(value);   }
			operator float()   const { return fixedpt_tofloat(value); }
			operator long()   const { return fixedpt_toint(value); }
			operator int16_t() const { return fixedpt_toint(value);   }

			static const FixedPt zero;
			static const FixedPt one; 

			FixedPt & operator=(const FixedPt &rhs)  { value = rhs.value;             return *this; }
			FixedPt & operator=(const fixedpt rhs) { value = rhs;                   return *this; }
			FixedPt & operator=(const double rhs)  { value = fixedpt_rconst(rhs);   return *this; }
			FixedPt & operator=(const float rhs)   { value = fixedpt_rconst(rhs); return *this; }
			FixedPt & operator=(const int16_t rhs) { value = fixedpt_fromint(rhs);   return *this; }
	#if FIXEDPT_BITS == 64
			FixedPt & operator=(const int rhs)   	 { value = fixedpt_fromint(rhs); return *this; }
	#endif
			FixedPt & operator+=(const FixedPt &rhs)  { value += rhs.value;             return *this; }
			FixedPt & operator+=(const fixedpt rhs) { value += rhs;                   return *this; }
			FixedPt & operator+=(const double rhs)  { value += fixedpt_rconst(rhs);   return *this; }
			FixedPt & operator+=(const float rhs)   { value += fixedpt_rconst(rhs); return *this; }
			FixedPt & operator+=(const int16_t rhs) { value += fixedpt_fromint(rhs);   return *this; }
	#if FIXEDPT_BITS == 64
			FixedPt & operator+=(const int rhs)   	{ value += fixedpt_fromint(rhs); return *this; }
	#endif
			FixedPt & operator-=(const FixedPt &rhs)  { value -= rhs.value; return *this; }
			FixedPt & operator-=(const fixedpt rhs) { value -= rhs; return *this; }
			FixedPt & operator-=(const double rhs)  { value -= fixedpt_rconst(rhs); return *this; }
			FixedPt & operator-=(const float rhs)   { value -= fixedpt_rconst(rhs); return *this; }
			FixedPt & operator-=(const int16_t rhs) { value -= fixedpt_fromint(rhs); return *this; }
	#if FIXEDPT_BITS == 64
			FixedPt & operator-=(const int rhs)   	{ value -= fixedpt_fromint(rhs); return *this; }
	#endif
			FixedPt & operator*=(const FixedPt &rhs)  { value = fixedpt_mul(value, rhs.value); return *this; }
			FixedPt & operator*=(const fixedpt rhs) { value = fixedpt_mul(value, rhs); return *this; }
			FixedPt & operator*=(const double rhs)  { value = fixedpt_mul(value, fixedpt_rconst(rhs)); return *this; }
			FixedPt & operator*=(const float rhs)   { value = fixedpt_mul(value, fixedpt_rconst(rhs)); return *this; }
			FixedPt & operator*=(const int16_t rhs) { value *= rhs; return *this; }
	#if FIXEDPT_BITS == 64
			FixedPt & operator*=(const int rhs)   	{ value = fixedpt_mul(value, fixedpt_fromint(rhs)); return *this; }
	#endif
			FixedPt & operator/=(const FixedPt &rhs)  { value = fixedpt_div(value, rhs.value); return *this; }
			FixedPt & operator/=(const fixedpt rhs) { value = fixedpt_div(value, rhs); return *this; }
			FixedPt & operator/=(const double rhs)  { value = fixedpt_div(value, fixedpt_rconst(rhs)); return *this; }
			FixedPt & operator/=(const float rhs)   { value = fixedpt_div(value, fixedpt_rconst(rhs)); return *this; }
			FixedPt & operator/=(const int16_t rhs) { value /= rhs; return *this; }
	#if FIXEDPT_BITS == 64
			FixedPt & operator/=(const int rhs)   	{ value = fixedpt_div(value, fixedpt_fromint(rhs)); return *this; }
	#endif
			const FixedPt operator+(const FixedPt &other) const { return this->value + other.value;}
			const FixedPt operator+(const fixedpt other) 	const { return this->value + other;}
			const FixedPt operator+(const double other) 	const { return this->value + fixedpt_rconst(other);} 
			const FixedPt operator+(const float other) 		const { return this->value + fixedpt_rconst(other);}
			const FixedPt operator+(const int16_t other) 	const { return this->value + fixedpt_rconst(other);}



			const FixedPt operator-(const FixedPt &other) const { return this->value - other.value;};// { FixedPt ret = *this; ret -= other; return ret; }
			const FixedPt operator-(const fixedpt other) 	const { return this->value - other;}
			const FixedPt operator-(const double other) 	const { return this->value - fixedpt_rconst(other);} 
			const FixedPt operator-(const float other) 		const { return this->value - fixedpt_rconst(other);}
			const FixedPt operator-(const int16_t other) 	const { return this->value - fixedpt_rconst(other);}

			const FixedPt operator-(void) const {return -value;}

			const FixedPt operator*(const FixedPt &other) const { FixedPt ret = *this; ret *= other; return ret; }
			const FixedPt operator*(const fixedpt other) 	const { FixedPt ret = *this; ret *= other; return ret; }
			const FixedPt operator*(const double other) 	const { FixedPt ret = *this; ret *= other; return ret; }
			const FixedPt operator*(const float other) 		const { FixedPt ret = *this; ret *= other; return ret; }
			const FixedPt operator*(const int16_t other) 	const { FixedPt ret = *this; ret *= other; return ret; }



			const FixedPt operator/(const FixedPt &other) const { FixedPt ret = *this; ret /= other; return ret; }
			const FixedPt operator/(const fixedpt other) 	const { FixedPt ret = *this; ret /= other; return ret; }
			const FixedPt operator/(const double other) 	const { FixedPt ret = *this; ret /= other; return ret; }
			const FixedPt operator/(const float other) 		const { FixedPt ret = *this; ret /= other; return ret; }
			const FixedPt operator/(const int16_t other) 	const { FixedPt ret = *this; ret /= other; return ret; }


			const int operator==(const FixedPt &other)  const { return (value == other.value);             }
			const int operator==(const fixedpt other) const { return (value == other);                   }
			const int operator==(const double other)  const { return (value == fixedpt_rconst(other));   }
			const int operator==(const float other)   const { return (value == fixedpt_rconst(other)); }
			const int operator==(const int16_t other) const { return (value == fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator==(const int other) 	 	const { return (value == fixedpt_fromint(other));   }
	#endif
			const int operator!=(const FixedPt &other)  const { return (value != other.value);             }
			const int operator!=(const fixedpt other) const { return (value != other);                   }
			const int operator!=(const double other)  const { return (value != fixedpt_rconst(other));   }
			const int operator!=(const float other)   const { return (value != fixedpt_rconst(other)); }
			const int operator!=(const int16_t other) const { return (value != fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator!=(const int other) 		const { return (value != fixedpt_fromint(other));   }
	#endif
			const int operator<=(const FixedPt &other)  const { return (value <= other.value);             }
			const int operator<=(const fixedpt other) const { return (value <= other);                   }
			const int operator<=(const double other)  const { return (value <= fixedpt_rconst(other));   }
			const int operator<=(const float other)   const { return (value <= fixedpt_rconst(other)); }
			const int operator<=(const int16_t other) const { return (value <= fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator<=(const int other) 		const { return (value <= fixedpt_fromint(other));   }
	#endif
			const int operator>=(const FixedPt &other)  const { return (value >= other.value);             }
			const int operator>=(const fixedpt other) const { return (value >= other);                   }
			const int operator>=(const double other)  const { return (value >= fixedpt_rconst(other));   }
			const int operator>=(const float other)   const { return (value >= fixedpt_rconst(other)); }
			const int operator>=(const int16_t other) const { return (value >= fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator>=(const int other) 		const { return (value >= fixedpt_fromint(other));   }
	#endif
			const int operator< (const FixedPt &other)  const { 
				return (value <  other.value);            
			}
			const int operator< (const fixedpt other) const { return (value <  other);                   }
			const int operator< (const double other)  const { return (value <  fixedpt_rconst(other));   }
			const int operator< (const float other)   const { return (value <  fixedpt_rconst(other)); }
			const int operator< (const int16_t other) const { return (value <  fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator< (const int other) 		const { return (value <  fixedpt_fromint(other));   }
	#endif
			const int operator> (const FixedPt &other)  const { return (value >  other.value);             }
			const int operator> (const fixedpt other) const { return (value >  other);                   }
			const int operator> (const double other)  const { return (value >  fixedpt_rconst(other));   }
			const int operator> (const float other)   const { return (value >  fixedpt_rconst(other)); }
			const int operator> (const int16_t other) const { return (value >  fixedpt_fromint(other));   }
	#if FIXEDPT_BITS == 64
			const int operator> (const int other) 		const { return (value >  fixedpt_fromint(other));   }
	#endif
			FixedPt  sin() { return FixedPt(fixedpt_sin(value));  }
			FixedPt  cos() { return FixedPt(fixedpt_cos(value));  }
			FixedPt  tan() { return FixedPt(fixedpt_tan(value));  }
			FixedPt asin() { return FixedPt(fixedpt_asin(value)); }
			FixedPt acos() { return FixedPt(fixedpt_acos(value)); }
			FixedPt atan() { return FixedPt(fixedpt_atan(value)); }
			FixedPt atan2(const FixedPt &inY) { return FixedPt(fixedpt_atan2(value, inY.value)); }
			FixedPt sqrt() const { return FixedPt(fixedpt_sqrt(value)); }

			FixedPt exp()		const	{ return FixedPt(fixedpt_exp(value));}
			FixedPt abs() 	const	{ return FixedPt(fixedpt_abs(value));}
			FixedPt floor() const	{ return FixedPt(fixedpt_floor(value));}
			FixedPt ceil() 	const	{ return FixedPt(fixedpt_ceil(value));}

			float 	to_f() 	const {return fixedpt_tofloat(value);}
			double 	to_dbl()const {return fixedpt_tofloat(value);}
			int16_t to_i() 	const {return fixedpt_toint(value);}

			FixedPt to_rad() const	{return FixedPt(fixedpt_deg_to_rad(value));}
			FixedPt to_deg() const	{return FixedPt(fixedpt_rad_to_deg(value));}

			char * cstr(int maxDec = 5) {return fixedpt_cstr(value, maxDec);}
	};

}

#endif