/* __________________________________________________________________________
* DEVELOPMENT HISTORY:
* --------------------
*
* $Author:: $: Felipe Magalhaes
* $Rev:: $: Revision of last commit
* $Date:: $: 13/12/2018
* $Comments (Refer to applicable SCR/PR) $: Initial version of the file.
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* MODULE DESCRIPTION:
* -------------------
* Filename : performance_lib_libc.c
* Original Author : Felipe Magalhaes
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* PREPROCESSOR DIRECTIVES:
* ------------------------
*/
#include "performance_lib.h"
#include "performance_lib_mapping.h"
#if MEASURE_STATIC == 1

#ifndef RAND_MAX
  #define RAND_MAX  32767
#endif
#ifndef ULONG_MAX
  #define ULONG_MAX   4294967295          //Maximum value for an object of type uint64_t.
#endif

#ifndef MIN
  #define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
  #define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

static int32_t rand1=0xfa57f00d;

const uint8_t __kZeroChar = '0';

#define BASE_MAX 36  // largest valid base

static const uint8_t kDigits[BASE_MAX] = {"0123456789abcdefghijklmnopqrstuvwxyz"};
static const int32_t kNonZero = 1;
static const int32_t __kZero = 0;
static const int32_t kUpperCaseA = 65;       /* 'A' */
static const int32_t kLowerCaseA = 97;       /* 'a' */
static const int32_t kUpperCaseZ = 90;       /* 'Z' */
static const int32_t kCharSpace = 32;        /* ' ' */
static const int32_t kCharHT = 9;            /* '\t' */
static const int32_t kCharCR = 13;           /* Carriage return */


#ifndef __div64
#pragma weak __div64
/* __________________________________________________________________________
 *
 * FUNCTION NAME : __div64
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint64_t __div64(uint64_t dividend, uint64_t divisor)
{
  uint32_t i;
  uint64_t r;
  uint64_t q;
  uint64_t b;
  int64_t  diff;

   q = dividend;
   b = divisor;
   r = 0;

   /* do 64 times */
   for (i = 0; i < 64; i++)
   {
      /*
       * Double left shift r-q pair.
       * Check if carry from q to r.
       */
      if ((q & 0x8000000000000000) != 0)
      {
         /*
          * left shift r w/ carry in
          */
         r = (r << 1) + 1;
      }

      else
      {
         /* left shift r w/o carry */
         r = r << 1;
      }

      /* left shift q */
      q = q << 1;
      diff = (int64_t)(r - b);

      /* Test if b can be subtracted from q */
      if (diff >= 0)
      {
         /* Set lsb of q */
         q = q | 0x01;
         r = r - b;
      }
   }

   /* return the division result */
   return(q);
}
#endif

#ifndef __mod64
#pragma weak __mod64
/* __________________________________________________________________________
*
* FUNCTION NAME : __mod64
* DESCRIPTION   : This function performs modulo operation in 64 bits
*                 Note: This function is required because the processor does not
*                 implement the 64-bit instruction.
* PARAMETERS :
*   INPUT  : Dividend (64 bits).
*            Divisor  (64 bits).
*   OUTPUT : None.
* RETURN   : Modulo result (64 bits).
*
* __________________________________________________________________________
*/
uint64_t __mod64(uint64_t dividend, uint64_t divisor)
{
  uint64_t reminder = dividend - (divisor * __div64(dividend, divisor));
  return reminder;
}
#endif



/* __________________________________________________________________________
 *
 * FUNCTION NAME : __isspace
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t __isspace(int32_t c)
{
  int32_t returnValue;
  if ((c >= kCharHT && c <= kCharCR) || (c == kCharSpace))
  {
    returnValue = kNonZero;
  }

  else
  {
    returnValue = __kZero;
  }

  return returnValue;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : __tolower
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t __tolower(int32_t c)
{
  int32_t convertedCharacter = c;
  if (c >= kUpperCaseA && c <= kUpperCaseZ)
  {
    convertedCharacter += kLowerCaseA - kUpperCaseA;
  }

  return convertedCharacter;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : __memchr
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void *__memchr(const void *str, int c, size_t n)
{
  void *pReturn = NULL;
  uint32_t i;
  const uint8_t *characterString = (uint8_t *)str;
  if (characterString != NULL)
  {
    for (i=0; (i < n) && (pReturn == NULL); i++)
    {
      if (characterString[i] == (uint8_t)c)
      {
        pReturn = (void *)&characterString[i];
      }
    }
  }

  return pReturn;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perfStrtoull
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint64_t perfStrtoull(const uint8_t *str)
{
  uint64_t y = 0;
  const uint8_t *pString = str;
  uint8_t *pDigit;

  //skip white spaces
  for ( ; __isspace(*pString) != 0; pString++)
  {}

  /* skip leading zeros */
  for (; *pString == __kZeroChar; pString++)
  {}

  pDigit = (uint8_t *)__memchr(kDigits, __tolower(*pString), 10);
  while(pDigit != NULL)
  {
    if (*pDigit < '0' || *pDigit > '9')
    {
       break;
    }

    y *= 10;
    y += (uint64_t)(*pDigit - '0');
    pString++;
    pDigit = (uint8_t *)__memchr(kDigits, __tolower(*pString), 10);
  }

  return y;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : rand
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t rand(void)
{
  rand1 = rand1 * 1103515245 + 0xB0B0;
  return (int32_t)(rand1 >> 16) & 32767;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : srand
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void srand(uint32_t seed)
{
  rand1 = seed;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : reverse
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void reverse(T_char *str, int32_t len)
{
  int32_t i=0, j=len-1, temp;
  while (i<j)
  {
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++; j--;
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : intToStr
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t intToStr(int32_t x, T_char str[], int32_t d)
{
  int32_t i = 0;
  while (x)
  {
    str[i++] = (x%10) + '0';
    x = x/10;
  }

  while (i < d)
  {
    str[i++] = '0';
  }

  reverse(str, i);
  str[i] = '\0';
  return i;
}

#if LIBC == 1
#define PI 3.14159265358979323846264338327950288
  #define PI_2    (PI / 2.0)
  #define PI_4    (PI / 4.0)
  #define PI2   (PI * 2.0)
  #define ONE_PI    (1.0 / PI)
  #define TWO_PI    (2.0 / PI)
  #define E_    2.7182818284590452353602874
  #define LOG2E   1.4426950408889634073599247
  #define LOG10E    0.43429448190325182765
  #define SQRT2   1.4142135623730950488016887
  #define TWO_SQRTPI  1.12837916709551257390
  #define LN2   0.69314718055994530942
  #define LN10    2.30258509299404568402
  #define SQRT1_2   0.707106781186547524

  const T_float32 kHuge = 1.701411733192644270e38;

#define isdigit(c) (c >= '0' && c <= '9')

/* __________________________________________________________________________
 *
 * FUNCTION NAME : ftoa
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void ftoa(T_float32 n, T_char *res, int32_t afterpoint)
{
  int32_t ipart = (int32_t)n;
  T_float32 fpart = n - (T_float32)ipart;
  // convert integer part to string
  int32_t i = intToStr(ipart, res, 0);

  // check for display option after point
  if (afterpoint != 0)
  {
    res[i] = '.';  // add dot

    // Get the value of fraction part upto given no.
    // of points after dot. The third parameter is needed
    // to handle cases like 233.007
    fpart = fpart * pow((T_float32)10, (T_float32)afterpoint);

    intToStr((int32_t)fpart, res + i + 1, afterpoint);
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : atof
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 atof(const T_char *s)
{
  T_float32 a = 0.0;
  int32_t e = 0;
  int32_t c;
  while ((c = *s++) != '\0' && isdigit(c))
  {
    a = a*10.0 + (c - '0');
  }

  if (c == '.')
  {
    while ((c = *s++) != '\0' && isdigit(c))
    {
      a = a*10.0 + (c - '0');
      e = e-1;
    }
  }

  if (c == 'e' || c == 'E')
  {
    int32_t sign = 1;
    int32_t i = 0;
    c = *s++;
    if (c == '+')
    {
      c = *s++;
    }

    else if (c == '-')
    {
      c = *s++;
      sign = -1;
    }

    while (isdigit(c))
    {
      i = i*10 + (c - '0');
      c = *s++;
    }

    e += i*sign;
  }

  while (e > 0)
  {
    a *= 10.0;
    e--;
  }

  while (e < 0)
  {
    a *= 0.1;
    e++;
  }

  return a;
}
/* __________________________________________________________________________
 *
 * FUNCTION NAME : modf
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 modf(T_float32 val, T_float32 *iptr)
{
  union{
      T_float32 v;
      struct{
#ifdef LITTLE_ENDIAN
      uint32_t u_mant : 23;
      uint32_t u_exp  : 8;
      uint32_t u_sign : 1;
#else
      uint32_t u_sign : 1;
      uint32_t u_exp  : 8;
      uint32_t u_mant : 23;
#endif
                } s;
        } u, v;
  uint32_t frac;

  u.v = val;
  if (u.s.u_exp == 255)
    return (u.v);

  if ((int32_t)u.s.u_exp - 127 > 23 - 1) {
    *iptr = u.v;
    v.v = 0.0;
    v.s.u_sign = u.s.u_sign;
    return (v.v);
  }

  if (u.s.u_exp < 127) {
    v.v = 0.0;
    v.s.u_sign = u.s.u_sign;
    *iptr = v.v;
    return (u.v);
  }

  v.v = u.v;
  /* Zero the low bits of the fraction, the sleazy way. */
  frac = (uint32_t)v.s.u_mant;
  frac >>= 23 - (u.s.u_exp - 127);
  frac <<= 23 - (u.s.u_exp - 127);
  v.s.u_mant = frac;
  *iptr = v.v;

  u.v -= v.v;
  u.s.u_sign = v.s.u_sign;
  return (u.v);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : fabs
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
/*
T_float32 fabs(T_float32 n)
{
  T_float32 f;

  if (n >= 0.0) f = n;
  else f = -n;
  return f;
}
*/

/* __________________________________________________________________________
 *
 * FUNCTION NAME : ldexp
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 ldexp(T_float32 val, int32_t expon){
  int32_t oldexp, newexp;
  union {
    T_float32 v;
    struct {
#ifdef LITTLE_ENDIAN
    uint32_t u_mant : 23;
    uint32_t u_exp  : 8;
    uint32_t u_sign : 1;
#else
    uint32_t u_sign : 1;
    uint32_t u_exp  : 8;
    uint32_t u_mant : 23;
#endif
    } s;
  } u, mul;

  u.v = val;
  oldexp = u.s.u_exp;
  /* If input is zero, Inf or NaN, just return it. */
  if (u.v == 0.0 || oldexp == (int32_t)kHuge)
  {
    return (val);
  }

  if (oldexp == 0)
  {
    /*
     * u.v is denormal.  We must adjust it so that the exponent
     * arithmetic below will work.
     */
    if (expon <= 127)
    {
      /*
       * Optimization: if the scaling can be done in a single
       * multiply, or underflows, just do it now.
       */
      if (expon <= -23)
      {
        return (val < 0.0 ? -0.0 : 0.0);
      }

      mul.v = 0.0;
      mul.s.u_exp = expon + 127;
      u.v *= mul.v;
      if (u.v == 0.0)
      {
        return (val < 0.0 ? -0.0 : 0.0);
      }

      return (u.v);
    }

    else
    {
      /*
       * We know that expon is very large, and therefore the
       * result cannot be denormal (though it may be Inf).
       * Shift u.v by just enough to make it normal.
       */
      mul.v = 0.0;
      mul.s.u_exp = 23 + 127;
      u.v *= mul.v;
      expon -= 23;
      oldexp = u.s.u_exp;
    }
  }

  /*
   * u.v is now normalized and oldexp has been adjusted if necessary.
   * Calculate the new exponent and check for underflow and overflow.
   */
  newexp = oldexp + expon;

  if (newexp <= 0)
  {
    /*
     * The output number is either denormal or underflows (see
     * comments in machine/ieee.h).
     */
    if (newexp <= -23)
    {
      return (val < 0.0 ? -0.0 : 0.0);
    }
    /*
     * Denormalize the result.  We do this with a multiply.  If
     * expon is very large, it won't fit in a double, so we have
     * to adjust the exponent first.  This is safe because we know
     * that u.v is normal at this point.
     */
    if (expon <= -127)
    {
      u.s.u_exp = 1;
      expon += oldexp - 1;
    }

    mul.v = 0.0;
    mul.s.u_exp = expon + 127;
    u.v *= mul.v;
    return (u.v);
  }

  else if (newexp >= 255)
  {
    /*
     * The result overflowed; return +/-Inf.
     */
    u.s.u_exp = 255;
    u.s.u_mant = 0;
    return (u.v);
  }

  else
  {
    /*
     * The result is normal; just replace the old exponent with the
     * new one.
     */
    u.s.u_exp = newexp;
    return (u.v);
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : floor
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 floor(T_float32 d)
{
  T_float32 fract;

  if (d<0.0f)
  {
    d = -d;
    fract = modf(d, &d);
    if (fract != 0.0f)
      d += 1.0f;
    d = -d;
  }

  else
  {
    modf(d, &d);
  }

  return (d);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : frexp
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 frexp(T_float32 value, int32_t *eptr){
  union{
    T_float32 v;
    struct{
#ifdef LITTLE_ENDIAN
      uint32_t u_mant : 23;
      uint32_t u_exp  : 8;
      uint32_t u_sign : 1;
#else
      uint32_t u_sign : 1;
      uint32_t u_exp  : 8;
      uint32_t u_mant : 23;
#endif
    }s;
  }u;

  if (value)
  {
    u.v = value;
    if (u.s.u_exp != 255)
    {
      *eptr = u.s.u_exp - (127 - 1);
      u.s.u_exp = 127 - 1;
    }

    return (u.v);
  }

  else
  {
    *eptr = 0;
    return (0.0);
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : exp
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 exp(T_float32 arg)
{
  static T_float32 p0 = 0.2080384346694663001443843411e7;
  static T_float32 p1 = 0.3028697169744036299076048876e5;
  static T_float32 p2 = 0.6061485330061080841615584556e2;
  static T_float32 q0 = 0.6002720360238832528230907598e7;
  static T_float32 q1 = 0.3277251518082914423057964422e6;
  static T_float32 q2 = 0.1749287689093076403844945335e4;

  T_float32 fract;
  T_float32 temp1, temp2, xsq;
  int32_t ent;
  if(arg == 0.0)
  {
    return(1.0);
  }

  if(arg < -10000.0)
  {
    return(0.0);
  }

  if(arg > 10000.0)
  {
    return(kHuge * kHuge);
  }

  arg = arg * 1.4426950408889634073599247;
  ent = floor(arg);
  fract = (arg - ent) - 0.5;
  xsq = fract*fract;
  temp1 = ((p2*xsq+p1)*xsq+p0)*fract;
  temp2 = ((1.0*xsq+q2)*xsq+q1)*xsq + q0;

  return(ldexp(SQRT2*(temp2+temp1)/(temp2-temp1), ent));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : sinus
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 sinus(T_float32 arg, int32_t quad)
{
  static T_float32 p0 =  0.1357884097877375669092680e8;
  static T_float32 p1 = -0.4942908100902844161158627e7;
  static T_float32 p2 =  0.4401030535375266501944918e6;
  static T_float32 p3 = -0.1384727249982452873054457e5;
  static T_float32 p4 =  0.1459688406665768722226959e3;
  static T_float32 q0 =  0.8644558652922534429915149e7;
  static T_float32 q1 =  0.4081792252343299749395779e6;
  static T_float32 q2 =  0.9463096101538208180571257e4;
  static T_float32 q3 =  0.1326534908786136358911494e3;
  T_float32 e, f;
  T_float32 ysq;
  T_float32 x,y;
  int32_t k;
  T_float32 temp1, temp2;

  x = arg;
  if(x<0)
  {
    x = -x;
    quad = quad + 2;
  }

  x = x * TWO_PI; /*underflow?*/
  if(x>32764)
  {
    y = modf(x,&e);
    e = e + quad;
    modf(0.25f*e,&f);
    quad = e - 4*f;
  }

  else
  {
    k = x;
    y = x - k;
    quad = (quad + k) & 03;
  }

  if (quad & 01)
  {
    y = 1-y;
  }

  if(quad > 1)
  {
    y = -y;
  }

  ysq = y*y;
  temp1 = ((((p4*ysq+p3)*ysq+p2)*ysq+p1)*ysq+p0)*y;
  temp2 = ((((ysq+q3)*ysq+q2)*ysq+q1)*ysq+q0);
  return(temp1/temp2);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : cos
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 cos(T_float32 arg)
{
  if(arg<0)
  {
    arg = -arg;
  }

  return(sinus(arg, 1));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : sin
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 sin(T_float32 arg)
{
  return(sinus(arg, 0));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : sqrt
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 sqrt(T_float32 arg){
  T_float32 x, temp;
  int32_t exp;
  int32_t i;

  if(arg <= 0.0)
  {
    return(0.0);
  }

  x = frexp(arg,&exp);
  while(x < 0.5)
  {
    x *= 2.0;
    exp--;
  }

  if(exp & 1)
  {
    x *= 2.0;
    exp--;
  }

  temp = 0.5*(1.0+x);
  while(exp > 60)
  {
    temp *= (1L<<30);
    exp -= 60;
  }

  while(exp < -60)
  {
    temp /= (1L<<30);
    exp += 60;
  }

  if(exp >= 0)
  {
    temp *= 1L << (exp/2);
  }

  else
  {
    temp /= 1L << (-exp/2);
  }

  for(i=0; i<=4; i++)
  {
    temp = 0.5f*(temp + arg/temp);
  }

  return(temp);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : log
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 log(T_float32 arg)
{
  static T_float32 p0 = -0.240139179559210510e2;
  static T_float32 p1 =  0.309572928215376501e2;
  static T_float32 p2 = -0.963769093368686593e1;
  static T_float32 p3 =  0.421087371217979714e0;
  static T_float32 q0 = -0.120069589779605255e2;
  static T_float32 q1 =  0.194809660700889731e2;
  static T_float32 q2 = -0.891110902798312337e1;

  T_float32 x,z, zsq, temp;
  int32_t exp;

  if(arg <= 0.0)
  {
    return(-kHuge);
  }

  x = frexp(arg,&exp);
  while(x < 0.5)
  {
    x *= 2.0;
    exp--;
  }

  if(x < SQRT1_2)
  {
    x *= 2.0;
    exp--;
  }

  z = (x-1.0)/(x+1.0);
  zsq = z*z;
  temp = ((p3*zsq + p2)*zsq + p1)*zsq + p0;
  temp = temp/(((1.0*zsq + q2)*zsq + q1)*zsq + q0);
  temp = temp*z + exp*LN2;
  return(temp);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : log10
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 log10(T_float32 arg)
{
  return(log(arg)/LN10);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : log10
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
T_float32 pow(T_float32 arg1, T_float32 arg2)
{
  T_float32 temp;
  int32_t l;

  if(arg1 <= 0.0)
  {
    if((arg1 == 0.0) || (arg2 <= 0.0))
    {
      return(0.0);
    }

    l = (int32_t)arg2;
    if (l != arg2)
    {
      return(0.0);
    }

    temp = exp(arg2 * log(-arg1));
    if(l & 1)
    {
      temp = -temp;
    }

    return(temp);
  }

  return(exp(arg2 * log(arg1)));
}

#endif
#endif

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
