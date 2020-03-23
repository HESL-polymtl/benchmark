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
* Filename : performance_lib_hm_porting_layer.c
* Original Author : Felipe Magalhaes
* Comments: This file contains the hardware abstraction layer. In this file,
* PowerPPC MPC5777C and ARM TMS570 boards'support functions are defined. 
* If a new architecture is needed, the support functions should be added.
*
* Also, it is possible to rely on the target RTOS API, directly 
* mapping functions. For instance, JetOS provides a GET_CURRENT_TICKS 
* function, which could be directly mapped and used. For example: 
*
*    uint64_t PerfGetTimeTicks(void){
*       return GET_CURRENT_TICKS();
*    }
*
* Nevertheless, this file contains the implementations, as a way to 
* illustrate how to add one, when the target RTOS does not provide it.
*
* __________________________________________________________________________
*/

#include "performance_lib.h"
#include "performance_lib_mapping.h"

#define COMMON_DIVISOR 1000000

#if defined(_TMS570) 
  #define portRTI_GCTRL_REG       ( * ( ( volatile uint32_t * ) 0xFFFFFC00 ) )
  #define portRTI_CNT0_FRC1_REG   ( * ( ( volatile uint32_t * ) 0xFFFFFC30 ) )
  #define portRTI_CNT0_UC1_REG    ( * ( ( volatile uint32_t * ) 0xFFFFFC34 ) )
  #define portRTI_CNT0_CPUC1_REG  ( * ( ( volatile uint32_t * ) 0xFFFFFC38 ) )

#elif defined(_MPC5777C)
/* __________________________________________________________________________
 *
 * FUNCTION NAME : PerfInlineGetTicks
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint64_t PerfInlineGetTicks()
{

  uint32_t high = 0, low = 0;
  uint64_t ticks = 0;

  __asm__ (".equ  TBL_USR, 268 \n\t"
           ".equ  TBU_USR, 269 \n\t"
           "stwu    1, -16(1) \n\t"
           "stw     5,  8(1)  \n\t"
           "tryAgain:           \n\t"
           "mfspr   %0, TBU_USR \n\t"
           "mfspr   %1, TBL_USR \n\t"
           "mfspr   5, TBU_USR \n\t"
           "cmpw    5, %0      \n\t"
           "bne tryAgain        \n\t"
           "lwz     5, 8(1)   \n\t"
           "addi    1, 1, 16"
           : "=r" (high), "=r" (low));

  ticks = low;
  ticks |= (uint64_t) ((uint64_t) high << 32u);
  return ticks;
}

#endif

/* __________________________________________________________________________
 *
 * FUNCTION NAME : PerfGetTimeTicks
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint64_t PerfGetTimeTicks(void)
{
  uint64_t tick = 0;

#if defined(_TMS570) 
  uint32_t RTI_CNT_FRCx = portRTI_CNT0_FRC1_REG;
  uint32_t RTI_UP_CNT =  portRTI_CNT0_UC1_REG;
  tick = RTI_UP_CNT ;
  tick |= (uint64_t) ((uint64_t) RTI_CNT_FRCx<<32u);

#elif defined(_MPC5777C) || defined(_MPC8544)
  tick = PerfInlineGetTicks();
#endif

  return tick;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : init_tick_counter
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void init_tick_counter()
{
#if defined(_TMS570) 
  portRTI_GCTRL_REG = ((uint32_t)((uint32_t)0x5U << 16U) | 0x00000000U);
  portRTI_CNT0_UC1_REG  =  0x00000000U;
  portRTI_CNT0_FRC1_REG =  0x00000000U;
  portRTI_CNT0_CPUC1_REG = 0xFFFFFFFFu;
  portRTI_GCTRL_REG    |= 0x00000002U;
#endif
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_tick_to_ns
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint32_t perf_tick_to_ns(uint64_t ticks)
{
  uint32_t nClkFreq = CLOCK_FREQ;
  nClkFreq          = nClkFreq/COMMON_DIVISOR;

  return ((uint32_t)__div64((uint64_t)(ticks * (1000000000/COMMON_DIVISOR)), (uint64_t)nClkFreq));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_tick_to_us
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint32_t perf_tick_to_us(uint64_t ticks)
{
  return (perf_tick_to_ns(ticks) / 1000);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_get_time_in_us
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint32_t perf_get_time_in_us()
{
  return ((uint32_t)__div64(perf_get_time_in_ns(),1000));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_get_time_in_ms
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint32_t perf_get_time_in_ms()
{
  return ((uint32_t)__div64(perf_get_time_in_ns(),1000000));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_get_time_in_ns
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint32_t perf_get_time_in_ns()
{
  return perf_tick_to_ns(GET_CURRENT_TICKS());
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_ns_to_ticks
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
uint64_t perf_ns_to_ticks(uint64_t ns)
{
  uint32_t nClkFreq = CLOCK_FREQ;
  nClkFreq          = nClkFreq/COMMON_DIVISOR;
  return ((uint64_t)__div64((uint64_t)(ns * nClkFreq), (uint64_t)(1000000000/COMMON_DIVISOR)));
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_init_timer
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_init_timer()
{
#if defined(_TMS570) 
  portRTI_GCTRL_REG = ((uint32_t)((uint32_t)0x5U << 16U) | 0x00000000U);
  portRTI_CNT0_UC1_REG  =  0x00000000U;
  portRTI_CNT0_FRC1_REG =  0x00000000U;
  portRTI_CNT0_CPUC1_REG = 0xFFFFFFFFu;
  portRTI_GCTRL_REG    |= 0x00000002U;
#endif
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_disable_timer
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_disable_timer()
{
#if defined(_TMS570) 
  portRTI_GCTRL_REG    &= 0x00000001U;
#endif
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_reset_timer
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_reset_timer()
{
  perf_init_timer();
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_add_times
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
perf_time_t perf_add_times(const perf_time_t* base, unsigned int milliseconds)
{
  perf_time_t retval = ((*base) + milliseconds * 900000);
  return retval;
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
