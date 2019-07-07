#ifndef __FIXEDPTC_H__
#define __FIXEDPTC_H__
/*
 * fixedptc.h is a 32-bit or 64-bit fixed point numeric library.
 *
 * The symbol FIXEDPT_BITS, if defined before this library header file
 * is included, determines the number of bits in the data type (its "width").
 * The default width is 32-bit (FIXEDPT_BITS=32) and it can be used
 * on any recent C99 compiler. The 64-bit precision (FIXEDPT_BITS=64) is
 * available on compilers which implement 128-bit "long long" types. This
 * precision has been tested on GCC 4.2+.
 *
 * The FIXEDPT_WBITS symbols governs how many bits are dedicated to the
 * "whole" part of the number (to the left of the decimal point). The larger
 * this width is, the larger the numbers which can be stored in the fixedpt
 * number. The rest of the bits (available in the FIXEDPT_FBITS symbol) are
 * dedicated to the fraction part of the number (to the right of the decimal
 * point).
 *
 * Since the number of bits in both cases is relatively low, many complex
 * functions (more complex than div & mul) take a large hit on the precision
 * of the end result because errors in precision accumulate.
 * This loss of precision can be lessened by increasing the number of
 * bits dedicated to the fraction part, but at the loss of range.
 *
 * Adventurous users might utilize this library to build two data types:
 * one which has the range, and one which has the precision, and carefully
 * convert between them (including adding two number of each type to produce
 * a simulated type with a larger range and precision).
 *
 * The ideas and algorithms have been cherry-picked from a large number
 * of previous implementations available on the Internet.
 * Tim Hartrick has contributed cleanup and 64-bit support patches.
 *
 * == Special notes for the 32-bit precision ==
 * Signed 32-bit fixed point numeric library for the 24.8 format.
 * The specific limits are -8388608.999... to 8388607.999... and the
 * most precise number is 0.00390625. In practice, you should not count
 * on working with numbers larger than a million or to the precision
 * of more than 2 decimal places. Make peace with the fact that PI
 * is 3.14 here. :)
 */

namespace physics
{

#ifndef FIXEDPT_BITS
#define FIXEDPT_BITS  64
#endif

//if your platform do not support int128 please define this macro
#define FIXEDPT_NO_SSE 1

#if FIXEDPT_BITS == 32
typedef int32_t fixedpt;
typedef int64_t fixedptd;
typedef uint32_t fixedptu;
typedef uint64_t fixedptud;
#elif FIXEDPT_BITS == 64

#if defined(FIXEDPT_NO_SSE)
typedef struct {
    uint64_t hi;
    uint64_t lo;
    char     sign;
} int128;
#endif

typedef int64_t  fixedpt;
typedef uint64_t fixedptu;

#if !defined(FIXEDPT_NO_SSE)
typedef __int128_t fixedptd;
typedef __uint128_t fixedptud;
#endif

#else
#error "FIXEDPT_BITS must be equal to 32 or 64"
#endif

#ifndef FIXEDPT_WBITS
#define FIXEDPT_WBITS 48
#endif

#if FIXEDPT_WBITS >= FIXEDPT_BITS
#error "FIXEDPT_WBITS must be less than or equal to FIXEDPT_BITS"
#endif

#define FIXEDPT_VCSID "$Id$"

#define FIXEDPT_FBITS (FIXEDPT_BITS - FIXEDPT_WBITS)
#define FIXEDPT_FMASK (((fixedpt)1 << FIXEDPT_FBITS) - 1)

#define fixedpt_rconst(R) ((fixedpt)((R) * FIXEDPT_ONE + ((R) >= 0 ? 0.5 : -0.5)))
#define fixedpt_fromint(I) (((fixedpt)I) << FIXEDPT_FBITS)
#define fixedpt_toint(F) ((int)(F >> FIXEDPT_FBITS))
#define fixedpt_add(A,B) ((A) + (B))
#define fixedpt_sub(A,B) ((A) - (B))

#if !defined(FIXEDPT_NO_SSE)
#define fixedpt_xmul(A,B)           \
  ((fixedpt)(((fixedptd)(A) * (fixedptd)(B)) >> FIXEDPT_FBITS))
#define fixedpt_xdiv(A,B)           \
  ((fixedpt)(((fixedptd)(A) << FIXEDPT_FBITS) / (fixedptd)(B)))
#endif

#define fixedpt_fracpart(A) ((fixedpt)(A) & FIXEDPT_FMASK)
#define fixedpt_intpart(A)  (A - fixedpt_fracpart(A)) 

#define FIXEDPT_ONE ((fixedpt)((fixedpt)1 << FIXEDPT_FBITS))
#define FIXEDPT_ONE_HALF (FIXEDPT_ONE >> 1)
#define FIXEDPT_TWO (FIXEDPT_ONE + FIXEDPT_ONE)
#define FIXEDPT_PI  fixedpt_rconst(3.14159265358979323846)
#define FIXEDPT_TWO_PI  fixedpt_rconst(2 * 3.14159265358979323846)
#define FIXEDPT_HALF_PI fixedpt_rconst(3.14159265358979323846 / 2)

#define FIXED_QUATER_PI fixedpt_rconst(3.14159265358979323846 / 4)
#define FIXED_THREE_QUATER_PI fixedpt_rconst(3.14159265358979323846 * 3 / 4)

#define FIXEDPT_E fixedpt_rconst(2.7182818284590452354)

#define fixedpt_abs(A) ((A) < 0 ? -(A) : (A))

#define fixedpt_tofloat(T) ((float) ((T)*((double)(1)/(double)(1 << FIXEDPT_FBITS))))


#define fixedpt_floor(A) (A - fixedpt_fracpart(A))
#define fixedpt_ceil(A) (fixedpt_intpart(A) + fixedpt_fracpart(A) ? FIXEDPT_ONE : 0 )


/* Multiplies two fixedpt numbers, returns the result. */
static const uint64_t roll = (1LL << 32);
static const uint64_t pmax = (1LL << 32) - 1;
static inline fixedpt fixedpt_mul(fixedpt A, fixedpt B)
{
#if FIXEDPT_BITS == 64 && defined(FIXEDPT_NO_SSE)
  /* A bit complicated but non-SSE version. */
  int128 tmp;
  uint64_t a0, a1;
  uint64_t b0, b1;
  uint64_t d, d0, d1;
  uint64_t e, e0, e1;
  uint64_t f, f0, f1;
  uint64_t g, g0, g1;
  uint64_t sum, carry;
  fixedpt result;

  tmp.sign = 0;

  if (0 > A) {
    tmp.sign = !tmp.sign;
    A = -A;
  }

  if (0 > B) {
    tmp.sign = !tmp.sign;
    B = -B;
  }

  a1 = A >> 32;
  a0 = A - (a1 << 32);

  b1 = B >> 32;
  b0 = B - (b1 << 32);

  d = a0 * b0;
  d1 = d >> 32;
  d0 = d - (d1 << 32);

  e = a0 * b1;
  e1 = e >> 32;
  e0 = e - (e1 << 32);

  f = a1 * b0;
  f1 = f >> 32;
  f0 = f - (f1 << 32);

  g = a1 * b1;
  g1 = g >> 32;
  g0 = g - (g1 << 32);

  sum = d1 + e0 + f0;
  carry = 0;

  for (; pmax < sum; sum -= roll, carry++);

  tmp.lo = d0 + (sum << 32);
  tmp.hi = carry + e1 + f1 + g0 + (g1 << 32);

  result = ((tmp.hi >> FIXEDPT_FBITS) << FIXEDPT_FBITS) +
      ((tmp.hi & FIXEDPT_FMASK) << FIXEDPT_FBITS) + (tmp.lo >> FIXEDPT_FBITS);

  result = tmp.sign ? -result : result;

  return result;
#else
  return (((fixedptd)A * (fixedptd)B) >> FIXEDPT_FBITS);
#endif
}


static double test_inverse(double a, int count)
{
  if(a == 0)
    return 0;

  double x = 0.4;
  int i = 0;
  while(i < count)
  {
    x = (2 - a * x)*x;
    i ++;
  }
  return (double)x;
}

const uint64_t hib = 0x8000000000000000ULL;
static inline fixedpt fixedpt_div(fixedpt A, fixedpt B)
{
#if FIXEDPT_BITS == 64 && defined(FIXEDPT_NO_SSE)
  /* A bit complicated but non-SSE version. */
  int128 tmp;
  fixedpt result, rem;
  uint64_t s;
  int i;
  
  tmp.sign = 0;

  if (0 > A) {
    tmp.sign = !tmp.sign;
    A = -A;
  }

  tmp.hi = A >> FIXEDPT_WBITS;//(FIXEDPT_BITS - FIXEDPT_FBITS);
  tmp.lo = A << FIXEDPT_FBITS;

  if (0 > B) {
    tmp.sign = !tmp.sign;
    B = -B;
  }

  // uint64_t y = tmp.hi;
  // int x = 0;
  // while(true)
  // {
    // printf(">>>hi:%llu lo:%llu\n", tmp.hi, tmp.lo);
  //   printf(">>>yhib:%llu y:%llu\n", y&hib, y);
  //   if (y & hib)
  //     break;
  //   else
  //     y <<= 1;

  //   x ++;
  // }
  // printf(">>>x:%d\n", x);

  rem = 0;

  tmp.hi <<= FIXEDPT_WBITS;
  tmp.hi += (tmp.lo >> FIXEDPT_FBITS);
  tmp.lo <<= FIXEDPT_WBITS;

  for (i = FIXEDPT_WBITS; i < 128; i++) {

  // for (i = 0; i < 128; i++) {
    rem <<= 1;
    // if(i == FIXEDPT_WBITS)
    //   printf(">>ttt i:%d hi:%llu lo:%llu rem:%llu\n", i, tmp.hi, tmp.lo, rem);

    if (tmp.hi & hib)
    {
      // printf(">>hhh i:%d hi:%llu lo:%llu rem:%llu\n", i, tmp.hi, tmp.lo, rem);
      rem |= 1;
    }

    s = tmp.lo & hib;
    tmp.hi <<= 1;
    tmp.lo <<= 1;

    if (s)
    {
      // printf(">>lll i:%d hi:%llu lo:%llu\n", i, tmp.hi, tmp.lo);
      tmp.hi |= 1;
    }

    if (rem >= B) {
      rem -= B;
      tmp.lo |= 1;
    }
  }

  // result = tmp.lo << FIXEDPT_FBITS;
  result = tmp.lo;
  result = tmp.sign ? -result : result;

  return result;
#else
  return (((fixedptd)A << FIXEDPT_FBITS) / (fixedptd)B);
#endif
}

/* Divides two fixedpt numbers, returns the result. */
// const uint64_t hib = 0x8000000000000000ULL;
// static inline fixedpt fixedpt_div(fixedpt A, fixedpt B)
// {
// #if FIXEDPT_BITS == 64 && defined(FIXEDPT_NO_SSE)
//   /* A bit complicated but non-SSE version. */
//   int128 tmp;
//   fixedpt result, rem;
//   uint64_t s;
//   int i;
  
//   tmp.sign = 0;

//   if (0 > A) {
//     tmp.sign = !tmp.sign;
//     A = -A;
//   }

//   tmp.hi = A >> (FIXEDPT_BITS - FIXEDPT_FBITS);
//   tmp.lo = A << FIXEDPT_FBITS;

//   if (0 > B) {
//     tmp.sign = !tmp.sign;
//     B = -B;
//   }

//   rem = 0;
//   for (i = 0; i < 128; i++) {
//     rem <<= 1;

//     if (tmp.hi & hib)
//       rem |= 1;

//     s = tmp.lo & hib;
//     tmp.hi <<= 1;
//     tmp.lo <<= 1;

//     if (s)
//       tmp.hi |= 1;

//     if (rem >= B) {
//       rem -= B;
//       tmp.lo |= 1;
//     }
//   }

//   result = tmp.lo;
//   result = tmp.sign ? -result : result;

//   return result;
// #else
//   return (((fixedptd)A << FIXEDPT_FBITS) / (fixedptd)B);
// #endif
// }

/*
 * Note: adding and substracting fixedpt numbers can be done by using
 * the regular integer operators + and -.
 */

/**
 * Convert the given fixedpt number to a decimal string.
 * The max_dec argument specifies how many decimal digits to the right
 * of the decimal point to generate. If set to -1, the "default" number
 * of decimal digits will be used (2 for 32-bit fixedpt width, 10 for
 * 64-bit fixedpt width); If set to -2, "all" of the digits will
 * be returned, meaning there will be invalid, bogus digits outside the
 * specified precisions.
 */
static inline void
fixedpt_str(fixedpt A, char *str, int max_dec)
{
  int ndec = 0, slen = 0;
  char tmp[12] = {0};
  fixedptu fr, ip;

  if (max_dec == -1)
#if FIXEDPT_BITS == 32
    max_dec = 2;
#elif FIXEDPT_BITS == 64
    max_dec = 10;
#else
#error Invalid width
#endif
  else if (max_dec == -2)
    max_dec = 15;

  if (A < 0) {
    str[slen++] = '-';
    A *= -1;
  }

  ip = fixedpt_toint(A);
  do {
    tmp[ndec++] = '0' + ip % 10;
    ip /= 10;
  } while (ip != 0);

  while (ndec > 0)
    str[slen++] = tmp[--ndec];
  str[slen++] = '.';

  fr = fixedpt_fracpart(A);
  do {
    fr *= 10;
    str[slen++] = '0' + (fr >> FIXEDPT_FBITS);
    fr &= (((fixedpt)1 << FIXEDPT_FBITS) - 1);
    ndec++;
  } while (fr != 0 && ndec < max_dec);

  if (ndec > 1 && str[slen-1] == '0')
    str[slen-1] = '\0'; /* cut off trailing 0 */
  else
    str[slen] = '\0';
}


/* Converts the given fixedpt number into a string, using a static
 * (non-threadsafe) string buffer */
static inline char*
fixedpt_cstr(const fixedpt A, const int max_dec)
{
  static char str[25];

  fixedpt_str(A, str, max_dec);
  return (str);
}


/* Returns the square root of the given number, or -1 in case of error */
static inline fixedpt
fixedpt_sqrt(fixedpt A)
{
  int invert = 0;
  int iter = FIXEDPT_FBITS;
  int i;
  fixedpt l;

  if (A < 0)
    return (-1);
  if (A == 0 || A == FIXEDPT_ONE)
    return (A);
  if (A < FIXEDPT_ONE && A > 6) {
    invert = 1;
    A = fixedpt_div(FIXEDPT_ONE, A);
  }
  if (A > FIXEDPT_ONE) {
    fixedpt s = A;

    iter = 0;
    while (s > 0) {
      s >>= 2;
      iter++;
    }
  }

  /* Newton's iterations */
  l = (A >> 1) + 1;
  for (i = 0; i < iter; i++)
    l = (l + fixedpt_div(A, l)) >> 1;

  if (invert)
    return (fixedpt_div(FIXEDPT_ONE, l));

  return (l);
}


/* Returns the sine of the given fixedpt number. 
 * Note: the loss of precision is extraordinary! */
static inline fixedpt
fixedpt_sin(fixedpt fp)
{
  int sign = 1;
  fixedpt sqr, result;
  const fixedpt SK[2] = {
    fixedpt_rconst(7.61e-03),
    fixedpt_rconst(1.6605e-01)
  };

  fp %= 2 * FIXEDPT_PI;
  if (fp < 0)
    fp = FIXEDPT_PI * 2 + fp;
  if ((fp > FIXEDPT_HALF_PI) && (fp <= FIXEDPT_PI)) 
    fp = FIXEDPT_PI - fp;
  else if ((fp > FIXEDPT_PI) && (fp <= (FIXEDPT_PI + FIXEDPT_HALF_PI))) {
    fp = fp - FIXEDPT_PI;
    sign = -1;
  } else if (fp > (FIXEDPT_PI + FIXEDPT_HALF_PI)) {
    fp = (FIXEDPT_PI << 1) - fp;
    sign = -1;
  }
  sqr = fixedpt_mul(fp, fp);
  result = SK[0];
  result = fixedpt_mul(result, sqr);
  result -= SK[1];
  result = fixedpt_mul(result, sqr);
  result += FIXEDPT_ONE;
  result = fixedpt_mul(result, fp);
  return sign * result;
}


/* Returns the cosine of the given fixedpt number */
static inline fixedpt
fixedpt_cos(fixedpt A)
{
  return (fixedpt_sin(FIXEDPT_HALF_PI - A));
}


/* Returns the tangens of the given fixedpt number */
static inline fixedpt
fixedpt_tan(fixedpt A)
{
  return fixedpt_div(fixedpt_sin(A), fixedpt_cos(A));
}


/* Returns the value exp(x), i.e. e^x of the given fixedpt number. */
static inline fixedpt
fixedpt_exp(fixedpt fp)
{
  fixedpt xabs, k, z, R, xp;
  const fixedpt LN2 = fixedpt_rconst(0.69314718055994530942);
  const fixedpt LN2_INV = fixedpt_rconst(1.4426950408889634074);
  const fixedpt EXP_P[5] = {
    fixedpt_rconst(1.66666666666666019037e-01),
    fixedpt_rconst(-2.77777777770155933842e-03),
    fixedpt_rconst(6.61375632143793436117e-05),
    fixedpt_rconst(-1.65339022054652515390e-06),
    fixedpt_rconst(4.13813679705723846039e-08),
  };

  if (fp == 0)
    return (FIXEDPT_ONE);
  xabs = fixedpt_abs(fp);
  k = fixedpt_mul(xabs, LN2_INV);
  k += FIXEDPT_ONE_HALF;
  k &= ~FIXEDPT_FMASK;
  if (fp < 0)
    k = -k;
  fp -= fixedpt_mul(k, LN2);
  z = fixedpt_mul(fp, fp);
  /* Taylor */
  R = FIXEDPT_TWO +
      fixedpt_mul(z, EXP_P[0] + fixedpt_mul(z, EXP_P[1] +
      fixedpt_mul(z, EXP_P[2] + fixedpt_mul(z, EXP_P[3] +
      fixedpt_mul(z, EXP_P[4])))));
  xp = FIXEDPT_ONE + fixedpt_div(fixedpt_mul(fp, FIXEDPT_TWO), R - fp);
  if (k < 0)
    k = FIXEDPT_ONE >> (-k >> FIXEDPT_FBITS);
  else
    k = FIXEDPT_ONE << (k >> FIXEDPT_FBITS);
  return (fixedpt_mul(k, xp));
}


/* Returns the natural logarithm of the given fixedpt number. */
static inline fixedpt
fixedpt_ln(fixedpt x)
{
  fixedpt log2, xi;
  fixedpt f, s, z, w, R;
  const fixedpt LN2 = fixedpt_rconst(0.69314718055994530942);
  const fixedpt LG[7] = {
    fixedpt_rconst(6.666666666666735130e-01),
    fixedpt_rconst(3.999999999940941908e-01),
    fixedpt_rconst(2.857142874366239149e-01),
    fixedpt_rconst(2.222219843214978396e-01),
    fixedpt_rconst(1.818357216161805012e-01),
    fixedpt_rconst(1.531383769920937332e-01),
    fixedpt_rconst(1.479819860511658591e-01)
  };

  if (x < 0)
    return (0);
  if (x == 0)
    return 0xffffffff;

  log2 = 0;
  xi = x;
  while (xi > FIXEDPT_TWO) {
    xi >>= 1;
    log2++;
  }
  f = xi - FIXEDPT_ONE;
  s = fixedpt_div(f, FIXEDPT_TWO + f);
  z = fixedpt_mul(s, s);
  w = fixedpt_mul(z, z);
  R = fixedpt_mul(w, LG[1] + fixedpt_mul(w, LG[3]
      + fixedpt_mul(w, LG[5]))) + fixedpt_mul(z, LG[0]
      + fixedpt_mul(w, LG[2] + fixedpt_mul(w, LG[4]
      + fixedpt_mul(w, LG[6]))));
  return (fixedpt_mul(LN2, (log2 << FIXEDPT_FBITS)) + f
      - fixedpt_mul(s, f - R));
}
  

/* Returns the logarithm of the given base of the given fixedpt number */
static inline fixedpt
fixedpt_log(fixedpt x, fixedpt base)
{
  return (fixedpt_div(fixedpt_ln(x), fixedpt_ln(base)));
}


/* Return the power value (n^exp) of the given fixedpt numbers */
static inline fixedpt
fixedpt_pow(fixedpt n, fixedpt exp)
{
  if (exp == 0)
    return (FIXEDPT_ONE);
  if (n < 0)
    return 0;
  return (fixedpt_exp(fixedpt_mul(fixedpt_ln(n), exp)));
}







static inline fixedpt
fixedpt_atan2(fixedpt inY, fixedpt inX)
{
  fixedpt abs_inY, mask, angle, r, r_3;

  mask = (inY >> (sizeof(fixedpt)*CHAR_BIT-1));
  abs_inY = (inY + mask) ^ mask;
  
  //fixedpt ct1 = fixedpt_rconst(0.1963);
  //fixedpt ct2 = fixedpt_rconst(0.9817);
  if (inX >= 0)
  {
    r = fixedpt_div( (inX - abs_inY), (inX + abs_inY));
    r_3 = fixedpt_mul(fixedpt_mul(r, r),r);
    angle = fixedpt_mul(0x3FFEF391 , r_3) - fixedpt_mul(0x4FF865D8,r) + FIXED_QUATER_PI;
  } else {
    r = fixedpt_div( (inX + abs_inY), (abs_inY - inX));
    r_3 = fixedpt_mul(fixedpt_mul(r, r),r);
    angle = fixedpt_mul(0x3FFEF391 , r_3)
      - fixedpt_mul(0x4FF865D8,r)
      + FIXED_THREE_QUATER_PI;
  }
  if (inY < 0)
  {
    angle = -angle;
  }
  return angle;
}


static inline fixedpt
fixedpt_atan(fixedpt x)
{
  return fixedpt_atan2(x, FIXEDPT_ONE);
}

static inline fixedpt
fixedpt_asin(fixedpt x)
{
  if((x > FIXEDPT_ONE)
    || (x < -FIXEDPT_ONE))
    return 0;

  fixedpt out;
  out = (FIXEDPT_ONE - fixedpt_mul(x, x));
  out = fixedpt_div(x, fixedpt_sqrt(out));
  out = fixedpt_atan(out);
  return out;
}

static inline fixedpt 
fixedpt_acos(fixedpt x)
{
  return ((FIXEDPT_PI >> 1) - fixedpt_asin(x));
}


static const fixedpt fixedpt_rad_to_deg_mult = fixedpt_div(FIXEDPT_PI, fixedpt_rconst(180)) ;
static inline fixedpt fixedpt_deg_to_rad(fixedpt degrees)
{
  return fixedpt_mul(degrees, fixedpt_rad_to_deg_mult);
}

static const fixedpt fixedpt_deg_to_rad_mult = fixedpt_div(fixedpt_rconst(180), FIXEDPT_PI) ;
static inline fixedpt fixedpt_rad_to_deg(fixedpt radians)
{
  return fixedpt_mul(radians, fixedpt_deg_to_rad_mult);
}

}

#endif