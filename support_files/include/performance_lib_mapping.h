/* __________________________________________________________________________
 * DEVELOPMENT HISTORY:
 * --------------------
 *
 * $Author:: $: Felipe Magalhaes
 * $Rev:: $: Revision of last commit
 * $Date:: $: 11/12/2018
 * $Comments:: $: Initial version of the file.
 *  __________________________________________________________________________
 */
/* __________________________________________________________________________
 * MODULE DESCRIPTION:
 * -------------------
 * Filename : performance_lib_mapping.h
 * Original Author : Felipe Magalhaes
 *
 * TODO
 *
 * ASSUMPTIONS, CONSTRAINTS OR LIMITATIONS:
 * ----------------------------------------
 * None.
 *
 * REFERENCES:
 * -----------
 * None.
 * __________________________________________________________________________
 */
/*INCLUDE LIBS*/
/*This should be adapted to reflect target OS*/
#include <ARINC653.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


/*PLATFORM DEPENDENT DEFINITIONS*/
/*These are the definitions and declarations specific for the target OS*/
#define CLOCK_FREQ 300000000    					//the system clock can be hardcoded or rely on a function (GET_CLOCK_FREQUENCY(), for instance)

//functions definitions
void init_tick_counter();                          //function to initialize system timer
uint64_t PerfGetTimeTicks(void);                   //function to read system timer

#define START_TICK_COUNTER() init_tick_counter();   //mapping for the function
#define GET_CURRENT_TICKS() PerfGetTimeTicks()  	//mapping for the function

//base print function (i.e. putchar(c))
#ifdef PERFORMANCE_PRINT
  #define PRINT_OUTPUT_CHAR(x) printf("%c", x)
#else
  #define PRINT_OUTPUT_CHAR(x) (void)x
#endif 

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
