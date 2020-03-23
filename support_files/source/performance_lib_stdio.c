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
* Filename : performance_lib.c
* Original Author : Felipe Magalhaes
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* PREPROCESSOR DIRECTIVES:
* ------------------------
*/
#include "performance_lib.h"
#include "performance_lib_mapping.h"

#ifdef PERFORMANCE_PRINT

static int8_t digit[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumberUnsigned
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PERF_PRINT_EOL() { 
  PRINT_OUTPUT_CHAR('\n'); 
  PRINT_OUTPUT_CHAR('\r'); 
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumberUnsigned
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintNumberUnsigned(const uint32_t number)
{
    uint32_t divisor = 1;

    while (number / divisor > 9)
    {
        divisor *= 10;
    }

    do
    {
     PRINT_OUTPUT_CHAR((int32_t)('0' + (number / divisor % 10)));
        divisor /= 10;
    } while (divisor > 0);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perfUltoa
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perfUltoa(char_t * buf, uint64_t val)
{
  uint8_t  temp[80];
  uint8_t  i;
  uint8_t  j;
  uint8_t  mod;
  uint64_t valPos;

  valPos = (uint64_t)val;


  j = 0;
  do
  {
   mod       = (uint8_t)__mod64(valPos, 10);
   valPos    = __div64(valPos,10);
   temp[j++] = digit[mod];
  }
  while(valPos > 0);

  i = 0;

  while(j > 0)
  {
   buf[i++] = temp[j-1];
   j--;
  }

  buf[i++] = 0;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumberUnsigned64
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintNumberUnsigned64(const uint64_t number)
{
    
    uint32_t high = (uint32_t) (number >> 32);

    if (high == 0){
      PrintNumberUnsigned((uint32_t) number);
    }
    else{
      char_t chanValStr[30];
      perfUltoa(chanValStr, number);
      PrintString((const int8_t*)chanValStr);
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintString
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintString(const int8_t* string)
{
  const int8_t* pch = string;

  if (pch != NULL)
  {
    while (*pch)
    {
      if ((*pch <= 126) && (*pch >= 32))
      {
        PRINT_OUTPUT_CHAR((int32_t)*pch);
      }

      /* write escaped carriage returns */
      else if (*pch == 13)
      {
        PRINT_OUTPUT_CHAR((int32_t)'\\');
        PRINT_OUTPUT_CHAR((int32_t)'r');
      }

      /* write escaped line feeds */
      else if (*pch == 10)
      {
        PRINT_OUTPUT_CHAR((int32_t)'\\');
        PRINT_OUTPUT_CHAR((int32_t)'n');
      }

      /* unprintable characters are shown as codes */
      else
      {
        PRINT_OUTPUT_CHAR((int32_t)'\\');
        PRINT_OUTPUT_CHAR((int32_t)'x');
        PERF_PRINT_HEX((uint32_t)*pch, 2);
      }

      pch++;
    }
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumberHex
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintNumberHex(const uint32_t number, const int8_t nibbles_to_print)
{
  int32_t nibble;
  int8_t nibbles = nibbles_to_print;
  if ((unsigned)nibbles > (2 * sizeof(number)))
  {
    nibbles = 2 * sizeof(number);
  }

  while (nibbles > 0)
  {
    nibbles--;
    nibble = (int32_t)(number >> (nibbles * 4)) & 0x0F;
    if (nibble <= 9)
    {
      int8_t a = (int8_t)('0' + nibble);
      int8_t c1[2];
      c1[0]=a;
      c1[1]='\0';
      PrintString(c1);
    }

    else
    {
      int8_t b = (int8_t)('A' - 10 + nibble);
      int8_t c2[2];
      c2[0]=b;
      c2[1]='\0';
      PrintString(c2);
    }
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumberHex64
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintNumberHex64(const uint64_t number, const int8_t nibbles_to_print)
{
  uint32_t index = (uint32_t) (number >> 32);
  PrintString((const int8_t*)"0x");
  PrintNumberHex(index, nibbles_to_print);
  index = (uint32_t) number;
  PrintNumberHex(index, nibbles_to_print);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintNumber
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintNumber(const int32_t number_to_print)
{
  int32_t number = number_to_print;
  if (number_to_print < 0)
  {
    PRINT_OUTPUT_CHAR((int32_t)'-');
    number = (int32_t)(-number_to_print);
  }

  PrintNumberUnsigned(number);
}

#define testnan(n) ((n != n) ? 1 : 0)
#define testinf(n) (testnan((n) - (n)) && !testnan(n))
#define ROUND_TO_EVEN(orig, num_int, num)                                          \
  if (num_int > (num) && (num) - (num_int-1) <= 0.5 && (num_int & 1) == 1 && orig < (float32_t)1e22) \
    num_int -= 1

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PrintFloat
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void PrintFloat(const float32_t input_number)
{
  float32_t number;
  const float32_t kSmallNumberMax = 0.0000005;
  const float32_t kNormalNumberMax = 4294967295.9999995;
  const float32_t kTento6th = 1000000.0;
  const float32_t kTento9th = 1000000000.0;
  const float32_t kHalf = 0.5;

  if (input_number < 0)
  {
    PRINT_OUTPUT_CHAR((int32_t)'-');
    number = -input_number;
  }

  else
  {
    number = input_number;
  }

  if (testnan(number))
  {
    PrintString((int8_t*)"NaN");
  }

  else if (testinf(number))
  {
    PrintString((int8_t*)"Inf");
  }

  else if (number <= kSmallNumberMax && number > 0)
  {
    PrintString((int8_t*)"0.000000...");
  }

  else if (number < kNormalNumberMax)
  {
    int32_t divisor = 1000000/10;
    uint32_t integer_part = (uint32_t)number;
    int32_t fraction_part = (int32_t)((number - (float32_t)integer_part) * kTento6th + kHalf);

    ROUND_TO_EVEN(number, fraction_part, (number - (float32_t)integer_part)*kTento6th);
    if (fraction_part == 1000000)
    {
      fraction_part = 0;
      integer_part += 1;
    }

    PrintNumberUnsigned(integer_part);
    PRINT_OUTPUT_CHAR((int32_t)'.');
    while (divisor > 0)
    {
      int8_t c = (int8_t)('0' + fraction_part / divisor);
      int8_t c2[2];
      c2[0]=c;
      c2[1]='\0';
      PrintString(c2);
      fraction_part %= divisor;
      divisor /= 10;
      if (fraction_part == 0)
      {
        break;
      }
    }
  }

  else
  {
    int32_t divisor = 1000000000/10;
    int32_t integer_part;
    float32_t divide = 10.0;
    int32_t fraction_part;
    int32_t exponent = 9;

    while (number / divide >= kTento9th - kHalf)
    {
      divide *= 10;
      exponent++;
    }

    integer_part = (int32_t)(number / divide + kHalf);
    ROUND_TO_EVEN(number, integer_part, number / divide);
    int8_t ca = (int8_t)('0' + integer_part / divisor);
    int8_t c2[2];
    c2[0]=ca;
    c2[1]='\0';
    PrintString(c2);
    fraction_part = integer_part % divisor;
    divisor = divisor / 10;
    PRINT_OUTPUT_CHAR((int32_t)'.');
    while (divisor > 0)
    {
      int8_t c = (int8_t)('0' + fraction_part / divisor);
      int8_t c2[2];
      c2[0]=c;
      c2[1]='\0';
      PrintString(c2);
      fraction_part %= divisor;
      divisor /= 10;
      if (fraction_part == 0)
      {
        break;
      }
    }

    PRINT_OUTPUT_CHAR((int32_t)'e');
    PRINT_OUTPUT_CHAR((int32_t)'+');
    if (exponent < 10)
    {
      PRINT_OUTPUT_CHAR((int32_t)'0');
    }

    PrintNumber(exponent);
  }
}
#endif
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
