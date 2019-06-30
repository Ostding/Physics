#ifndef __FRACTION_H__
#define __FRACTION_H__

#include <stdint.h>
#include <cmath>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
using namespace std;

#include "atan2_table.h"
#include "acos_table.h"
#include "sincos_table.h"

#define LLMAX 9223372036854775807LL
#define LLMIN (-9223372036854775807LL - 1)

#define DEN_PERCISION  10000LL
#define llabs(x) (x>0?x:-x)

class Fraction
{
public:
  int64_t nom;
  int64_t den;

public:
  Fraction(): nom(0), den(1) {};
  Fraction(const Fraction &other): nom(other.nom), den(other.den){};
  Fraction(int nom, int den): nom(nom), den(den){};
  Fraction(int64_t nom, int64_t den): nom(nom), den(den){};
  Fraction(int n): nom(n), den(1){};
  Fraction(float f) { from(f); };
  Fraction(double d) { from(d); };
  
  static Fraction f0;
  static Fraction f1;
  static Fraction f2;
  static Fraction f0_5;
  static Fraction f10;
  static Fraction f90;
  static Fraction f180;
  
  static Fraction pi;
  static Fraction pi_2;
  static Fraction pi_4;
  static Fraction pi2;
  static Fraction rad_unit;
  static Fraction deg_unit;
  static Fraction e;
  
  static Fraction min;
  static Fraction max;
  static int64_t mask;
  
  void from(double d)
  {
    if (d >= -0.000001f && d <= 0.000001f)
    {
        nom = 0;
        den = 1;
        return;
    }
    
    den = (int64_t)DEN_PERCISION;
    nom = (int64_t)(d * den);
    
    // printf("from double d:%f nom:%lld \n", d, nom);
    strip();
  };
  
  void from(int n)
  {
    nom = n;
    den = 1;
  };
  
  void from(int64_t n)
  {
    nom = n;
    den = 1;
  };
  
  void from(float f)
  {
    from((double)f);
  };
  
  operator int() const
  {
    return to_i();
  };
  
  operator float() const
  {
    return to_f();
  };
  
  operator double() const
  {
    return (double)to_f();
  };
  
  operator int64_t() const
  {
    return (int64_t)to_l();
  };
  
  Fraction& operator = (const Fraction &b)
  {
    nom = b.nom;
    den = b.den;
    return *this;
  };
  
  Fraction& operator = (const int n)
  {
    nom = n;
    den = 1LL;
    return *this;
  };
  
  Fraction& operator = (const float f)
  {
    den = (int64_t)DEN_PERCISION;
    nom = (int64_t)(f * den);
    strip();
    return *this;
  }
  
  Fraction& operator = (const double d)
  {
    den = (int64_t)DEN_PERCISION;
    nom = (int64_t)(d * den);
    strip();
    return *this;
  };
  
  Fraction& operator = (const int64_t n)
  {
    nom = n;
    den = 1LL;
    return *this;
  };
  
  bool operator < (const Fraction &b)  const
  {
    int64_t num = nom * b.den;
    int64_t num2 = b.nom * den;
    bool flag = (b.den > 0LL) ^ (den > 0LL);
    return (!flag) ? (num < num2) : (num > num2);
  };
  
  bool operator > (const Fraction &b)  const
  {
    int64_t num = nom * b.den;
    int64_t num2 = b.nom * den;
    bool flag = (b.den > 0LL) ^ (den > 0LL);
    return (!flag) ? (num > num2) : (num < num2);
  };
  
  bool operator <= (const Fraction &b)  const
  {
    int64_t num = nom * b.den;
    int64_t num2 = b.nom * den;
    bool flag = (b.den > 0LL) ^ (den > 0LL);
    return (!flag) ? (num <= num2) : (num >= num2);
  };
  
  bool operator >= (const Fraction &b)  const
  {
    int64_t num = nom * b.den;
    int64_t num2 = b.nom * den;
    bool flag = (b.den > 0LL) ^ (den > 0LL);
    return (!flag) ? (num >= num2) : (num <= num2);
  };
  
  bool operator == (const Fraction &b)  const
  {
    return nom * b.den == b.nom * den;
  };
  
  bool operator != (const Fraction &b)  const
  {
    return nom * b.den != b.nom * den;
  };
  
  bool operator < (const int64_t b) const
  {
    int64_t num = nom;
    int64_t num2 = b * den;
    return (den <= 0LL) ? (num > num2) : (num < num2);
  };
  
  bool operator > (const int64_t b) const
  {
    int64_t num = nom;
    int64_t num2 = b * den;
    return (den <= 0L) ? (num < num2) : (num > num2);
  };
  
  bool operator <= (const int64_t b) const
  {
    int64_t num = nom;
    int64_t num2 = b * den;
    return (den <= 0L) ? (num >= num2) : (num <= num2);
  };
  
  bool operator >= (const int64_t b) const
  {
    int64_t num = nom;
    int64_t num2 = b * den;
    return (den <= 0L) ? (num <= num2) : (num >= num2);
  };
  
  bool operator == (const int64_t b) const
  {
    return nom == b * den;
  };
  
  bool operator != (const int64_t b) const
  {
    return nom != b * den;
  };
  
  Fraction operator + (const Fraction &b)  const
  {
    Fraction ret = Fraction(nom * b.den + b.nom * den, den * b.den);
    ret.strip();
    return ret;
  };
  
  Fraction operator - (const Fraction &b)  const
  {
    Fraction ret = Fraction(nom * b.den - b.nom * den, den * b.den);
    ret.strip();
    return ret;
  };
  
  Fraction operator * (const Fraction &b)  const
  {
    Fraction ret = Fraction(nom * b.nom, den * b.den);
    ret.strip();
    return ret;
  };
  
  Fraction operator / (const Fraction &b)  const
  {
    Fraction ret = Fraction(nom * b.den, den * b.nom);
    ret.strip();
    return ret;
  };
  
  Fraction operator + (const int64_t b) const
  {
    Fraction ret = Fraction(nom + b * den, den);
    ret.strip();
    return ret;
  };
  
  Fraction operator - (const int64_t b) const
  {
    Fraction ret = Fraction(nom - b * den, den);
    ret.strip();
    return ret;
  };
  
  Fraction operator * (const int64_t b) const
  {
    Fraction ret = Fraction(nom * b, den);
    ret.strip();
    return ret;
  };
  
  Fraction operator / (const int64_t b) const
  {
    Fraction ret = Fraction(nom, den * b);
    ret.strip();
    return ret;
  };
  
  Fraction& operator += (const Fraction &b)
  {
    nom = nom * b.den + b.nom * den;
    if (nom == 0LL)
    {
        den = 1LL;
        return *this;
    }
    
    den = den * b.den;
    strip();
    return *this;
  };
  
  Fraction& operator -= (const Fraction &b)
  {
    nom = nom * b.den - b.nom * den;
    den = den * b.den;
    if (nom == 0LL)
    {
        den = 1LL;
        return *this;
    }
    
    strip();
    return *this;
  };
  
  Fraction& operator *= (const Fraction &b)
  {
    nom *= b.nom;
    den *= b.den;
    
    strip();
    return *this;
  };
  
  Fraction& operator /= (const Fraction &b)
  {
    nom *= b.den;
    den *= b.nom;
    
    strip();
    return *this;
  };
  
  Fraction& operator += (const int64_t b)
  {
    nom = nom + b * den;
    if (nom == 0LL)
    {
        den = 1LL;
        return *this;
    }
    
    strip();
    return *this;
  };
  
  Fraction& operator -= (const int64_t b)
  {
    nom = nom - b * den;
    if (nom == 0LL)
    {
        den = 1LL;
        return *this;
    }
    
    strip();
    return *this;
  };
  
  Fraction& operator *= (const int64_t b)
  {
    nom = nom * b;
    if (nom == 0LL)
    {
        den = 1LL;
        return *this;
    }
    
    strip();
    return *this;
  };
  
  Fraction& operator /= (const int64_t b)
  {
    bool isNeg = isNegative();
    den = den * b;
    if (den == 0LL)
    {
        nom = isNeg ? LLMIN : LLMAX;
        den = 1LL;
        return *this;
    }
    
    strip();
    return *this;
  };
  
  Fraction operator-(void) const
  {
    return Fraction(-nom, den);
  };
  
public:
  Fraction invserse()
  {
    assert(den != 0);
    return Fraction(den, nom);
  };
  
  static bool positive(int64_t nom, int64_t den)
  {
    if (nom == 0LL)
    return false;
    
    bool flag = nom > 0LL;
    bool flag2 = den > 0LL;
    return !(flag ^ flag2);
  }
  
  bool isPositive() const
  {
    return positive(nom, den);
  };
  
  static bool negative(int64_t nom, int64_t den)
  {
    if (nom == 0LL)
    return false;
    
    bool flag = nom > 0LL;
    bool flag2 = den > 0LL;
    return flag ^ flag2;
  }
  
  bool isNegative() const
  {
    return negative(nom, den);
  };
  
  int round() const
  {
    return (int)div32(nom, den);
  };
  
  int to_i() const
  {
    return (int)(nom / den);
  };
  
  long to_l() const
  {
    return (long)(nom / den);
  };
  
  float to_f() const
  {
    double num = (double)nom / (double)den;
    return (float)num;
  };
  
  double to_d() const
  {
    return (double)nom / (double)den;
  };
  
  void cstr(char* psz)
  {
    assert(psz != 0);
    
    double d = this->to_d();
    sprintf(psz, "%.5f", d);
  };
  
  void strip()
  {
    if (nom == 0ULL)
    {
      nom = 0LL;
      den = 1LL;
      return;
    }
    
    uint64_t un = (uint64_t)llabs(nom);
    uint64_t ud = (uint64_t)llabs(den);
    
    int64_t g = gcb(un, ud);
    nom /= g;
    den /= g;
    
    while (den > (int64_t)DEN_PERCISION)
    {
      if(nom == 1LL || nom == -1LL)
      {
          nom = 0;
          den = 1;
          break;
      }
      
      nom >>= 1;
      den >>= 1;
    }
  };
  
  
  Fraction sin()
  {
    return sin(*this);
  };
  
  Fraction cos()
  {
    return cos(*this);
  }
  
  Fraction tan()
  {
    return tan(*this);
  };
  
  Fraction asin()
  {
    return asin(*this);
  };
  
  Fraction acos()
  {
    return acos(*this);
  };
  
  Fraction atan()
  {
    return atan(*this);
  };
  
  Fraction exp()
  {
    return exp(*this);
  };
  
  Fraction ln()
  {
    return ln(*this);
  };
  
  Fraction log()
  {
    return log(*this);
  };
  
  Fraction to_rad() const
  {
    return to_rad(*this);
  };
  
  Fraction to_deg() const
  {
    return to_deg(*this);
  };

  Fraction abs() const
  {
    return abs(*this);
  };
  
  static Fraction atan2(int y, int x)
  {
    int num;
    int num2;
    if (x < 0)
    {
        if (y < 0)
        {
            x = -x;
            y = -y;
            num = 1;
        }
        else
        {
            x = -x;
            num = -1;
        }
        num2 = -31416;
    }
    else
    {
        if (y < 0)
        {
            y = -y;
            num = -1;
        }
        else
        {
            num = 1;
        }
        num2 = 0;
    }
    int dIM = Atan2Table::DIM;
    int64_t num3 = (int64_t)(dIM - 1);
    int64_t b = (int64_t)((x >= y) ? x : y);
    
    int num4 = (int)div64((int64_t)x * num3, b);
    int num5 = (int)div64((int64_t)y * num3, b);
    int num6 = Atan2Table::table[num5 * dIM + num4];
    
    int64_t nom = (int64_t)((num6 + num2) * num);
    int64_t den = (int64_t)DEN_PERCISION;
    
    return Fraction(nom, den);
  };

  static Fraction acos(int64_t nom, int64_t den)
  {
    int num = (int)div64(nom * (int64_t)AcosTable::HALF_COUNT, den) + AcosTable::HALF_COUNT;
    num = clamp(num, 0, AcosTable::COUNT);
    int64_t _nom = (int64_t)AcosTable::table[num];
    int64_t _den = (int64_t)DEN_PERCISION;
    return Fraction(_nom, _den);
  };
  
  static Fraction sin(int64_t nom, int64_t den)
  {
    int index = SincosTable::getIndex(nom, den);
    int64_t n = (int64_t)SincosTable::sin_table[index];
    int64_t d = (int64_t)SincosTable::FACTOR;
    return Fraction(n, d);
  };
  
  static Fraction cos(int64_t nom, int64_t den)
  {
    int index = SincosTable::getIndex(nom, den);
    int64_t n = (int64_t)SincosTable::cos_table[index];
    int64_t d = (int64_t)SincosTable::FACTOR;
    return Fraction(n, d);
  };
  
  static Fraction tan(int64_t nom, int64_t den)
  {
    Fraction c = cos(nom, den);
    if(c == Fraction::f0)
    {
        if (positive(nom, den))
        return Fraction(LLMAX, (int64_t)1LL);
        else
        return Fraction(LLMIN, (int64_t)1LL);
    }
    
    Fraction s = sin(nom, den);
    return s/c;
  };
  
  static Fraction atan(const Fraction &x)
  {
    return atan2(x, f1);
  };
  
  static Fraction atan2(const Fraction &y, const Fraction &x)
  {
    int64_t nom = y.nom * x.den;
    int64_t den = x.nom * y.den;
    
    if(den == 0)
    {
        int _y = 1;
        int _x = 0;
        return atan2(_y, _x);
    }
    
    if(nom == 0)
    {
        int _y = 0;
        int _x = 1;
        return atan2(_y, _x);
    }
    
    int64_t g = gcb(llabs(nom), llabs(den));
    int _y = (int)(nom / g);
    int _x = (int)(den / g);
    
    return atan2(_y, _x);
  };
  
  static Fraction asin(const Fraction &x)
  {
    if((x > f1) ||
       (x < -f1) ||
       (x == f0))
    return f0;
    
    if (x == f1)
    return pi_2;
    
    if (x == -f1)
    return -pi_4;
    
    Fraction y = (f1 - x * x).sqrt();
    if (y == f0)
    {
        if (x > f0)
        return pi_2;
        
        if (x < f0)
        return -pi_2;
    }
    
    Fraction tan = x / y;
    return atan(tan);
  };
  
  static Fraction acos(const Fraction &f)
  {
    return acos(f.nom, f.den);
  };
  
  static Fraction sin(const Fraction &f)
  {
    return sin(f.nom, f.den);
  };
  
  static Fraction cos(const Fraction &f)
  {
    return cos(f.nom, f.den);
  };
  
  static Fraction tan(const Fraction &f)
  {
    return tan(f.nom, f.den);
  };
  
  static Fraction to_deg(const Fraction &rad)
  {
    return deg_unit * rad;
  };
  
  static Fraction to_rad(const Fraction &deg)
  {
    return rad_unit * deg;
  };
  
  static Fraction abs(const Fraction &x)
  {
    return (x < Fraction::f0) ? -x : x;
  };
  
  static Fraction pow(const Fraction &num, const Fraction &m)
  {
    double dn = num.to_f();
    double dm = m.to_f();
    double r = std::pow(dn, dm);
    return Fraction(r);
  };
  
  Fraction sqrt()
  {
    return sqrt(*this);
  };
  
  static Fraction sqrt(const Fraction &x)
  {
    if (x < f0)
    return f0;
    
    double dx = x.to_d();
    double r = std::sqrt(dx);
    return Fraction(r);
  };
  
  static Fraction exp(const Fraction &x)
  {
    double dx = x.to_d();
    dx = std::exp(dx);
    return Fraction(dx);
  };
  
  static Fraction mod(const Fraction &x, const Fraction &y)
  {
    double dx = x.to_d();
    double dy = y.to_d();
    double r = std::fmod(dx, dy);
    return Fraction(r);
  };
  
  static Fraction log(const Fraction &x)
  {
    double d = x.to_d();
    double r = std::log10(d);
    return Fraction(r);
  };
  
  static Fraction ln(const Fraction &x)
  {
    double d = x.to_d();
    double r = std::log(d);
    return Fraction(r);
  };
  
  static int64_t div64(int64_t a, int64_t b)
  {
    return a / b;
  };
  
  static int	div32(int a, int b)
  {
    int num = (int)(((unsigned int)((a ^ b) & -2147483648)) >> 31);
    int num2 = num * -2 + 1;
    return (a + b / 2 * num2) / b;
  };
  
  static unsigned int	sqrt32(unsigned int a)
  {
    uint32_t num  = 0u;
    uint32_t num2 = 0u;
    for (int i = 0; i < 16; i++)
    {
        num2 <<= 1;
        num <<= 2;
        num += a >> 30;
        a <<= 2;
        if (num2 < num)
        {
            num2 += 1u;
            num -= num2;
            num2 += 1u;
        }
    }
    return num2 >> 1 & 0xffffu;
  };
  
  static uint64_t	sqrt64(uint64_t a)
  {
    uint64_t num = 0ULL;
    uint64_t num2 = 0ULL;
    for (int i = 0; i < 32; i++)
    {
        num2 <<= 1;
        num <<= 2;
        num += a >> 62;
        a <<= 2;
        if (num2 < num)
        {
            num2 += 1uL;
            num -= num2;
            num2 += 1uL;
        }
    }
    return (uint64_t)(num2 >> 1 & 0xffffffffu);
  };
  
  static int sqrtInt(uint64_t a)
  {
    if (a <= 0L)
    {
        return 0;
    }
    if (a <= (int64_t)(0xffffu))
    {
        return (int)sqrt32((unsigned int)a);
    }
    return (int)sqrt64((uint64_t)a);
  };
  
  static int64_t	sqrtLong(uint64_t a)
  {
    if (a <= 0L)
    {
        return 0;
    }
    if (a <= (int64_t)(0xffffu))
    {
        return (int64_t)(uint64_t)sqrt32((unsigned int)a);
    }
    return (int64_t)sqrt64((uint64_t)a);
  };
  
  static int64_t clamp(int64_t a, int64_t min, int64_t max)
  {
    if (a < min)
    {
        return min;
    }
    if (a > max)
    {
        return max;
    }
    return a;
  };
  
  static int	lerpInt(int src, int dst, int nom, int den)
  {
    return div32(src * den + (dst - src) * nom, den);
  };
  
  static int64_t lerpLong(int64_t src, int64_t dst, int64_t nom, int64_t den)
  {
    return div64(src * den + (dst - src) * nom, den);
  };
  
  static uint64_t gcb(uint64_t a, uint64_t b)
  {
    // make sure a < b
    if (a > b)
    {
        uint64_t temp = a;
        a = b;
        b = temp;
    }
    
    uint64_t r = b % a;
    while (r != 0)
    {
        b = a;
        a = r;
        r = b % a;
    }
    
    return a;
  };
};



#endif