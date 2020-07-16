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
* Filename : performance_lib_static.c
* Original Author : Felipe Magalhaes
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* PREPROCESSOR DIRECTIVES:
* ------------------------
*/
/* INCLUDE */
#include "performance_lib.h"
#include "performance_lib_mapping.h"

#if MEASURE_STATIC == 1

perf_struct EXECUTION_TIMES[QUANTITY_OF_TESTS];
int32_t     perf_pos = -1;

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_current_pos
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t perf_current_pos()
{
  return (int32_t) perf_pos;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : check_in
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t check_in(int8_t pos, char_t name[])
{
  char_t current[18];
  int32_t i, status, size;
  status = 1;
  strcpy(current, EXECUTION_TIMES[pos].name);
  size = strlen((const char_t *)current);
  for (i = 0; i < size; i++)
  {
    if (current[i] != name[i])
    {
      status = 0;
      break;
    }
  }

  return status;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_init
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
int32_t perf_init(char_t name[])
{
  int32_t pos;
  for (pos = 0; pos <= perf_current_pos(); pos++)
  {
    if (check_in((int8_t)pos, name) == 1)
    {
      return pos;
    }
  }

  pos = perf_current_pos();
  if (pos < QUANTITY_OF_TESTS)
  {
    pos++;
    strcpy(EXECUTION_TIMES[pos].name, name);
    EXECUTION_TIMES[pos].currentTime = 0;
    EXECUTION_TIMES[pos].deltaTime = 0;
    EXECUTION_TIMES[pos].sPrevTime = 0;
    EXECUTION_TIMES[pos].worstNS = 0;
    EXECUTION_TIMES[pos].bestNS = 0xFFFFFFFFFFFFFFFF;/;

    EXECUTION_TIMES[pos].worst = 0;
    EXECUTION_TIMES[pos].best = 0xFFFFFFFFFFFFFFFF;
    EXECUTION_TIMES[pos].average = 0;
    EXECUTION_TIMES[pos].averageNS = 0;
    EXECUTION_TIMES[pos].sumTicks = 0;
    EXECUTION_TIMES[pos].sumNSSQ = 0.0f;    
    EXECUTION_TIMES[pos].noSamples = 0;
    perf_pos = pos;
    return (pos);
  }

  else
  {
    return -1;
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_start_measurements
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_start_measurements(int32_t pos)
{
  EXECUTION_TIMES[pos].currentTime = GET_CURRENT_TICKS();
  EXECUTION_TIMES[pos].sPrevTime = EXECUTION_TIMES[pos].currentTime;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_end_measurements
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_end_measurements(int32_t pos)
{
  EXECUTION_TIMES[pos].currentTime = GET_CURRENT_TICKS();
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_validate_measurements
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 *
 * STATUS: Due to a refactoring in the execution calculation, std deviation
 *  is disabled for the moment.
*
 * TODO: Fix std.dev calculation
 * __________________________________________________________________________
 */
void perf_validate_measurements(int32_t pos, uint8_t calc_deviation)
{
  EXECUTION_TIMES[pos].noSamples++;
  EXECUTION_TIMES[pos].deltaTime = (EXECUTION_TIMES[pos].currentTime - EXECUTION_TIMES[pos].sPrevTime);

  EXECUTION_TIMES[pos].timeNS = perf_tick_to_ns(EXECUTION_TIMES[pos].deltaTime);
  EXECUTION_TIMES[pos].timeUS = perf_tick_to_us(EXECUTION_TIMES[pos].deltaTime);
  EXECUTION_TIMES[pos].timeMS = perf_tick_to_ms(EXECUTION_TIMES[pos].deltaTime);

  EXECUTION_TIMES[pos].sumTicks += EXECUTION_TIMES[pos].deltaTime;
  EXECUTION_TIMES[pos].average = EXECUTION_TIMES[pos].sumTicks / EXECUTION_TIMES[pos].noSamples;
  EXECUTION_TIMES[pos].averageNS = perf_tick_to_ns(EXECUTION_TIMES[pos].average);
  EXECUTION_TIMES[pos].averageUS = perf_tick_to_us(EXECUTION_TIMES[pos].average);

  if (EXECUTION_TIMES[pos].deltaTime < EXECUTION_TIMES[pos].best)
    {
      EXECUTION_TIMES[pos].best = EXECUTION_TIMES[pos].deltaTime;
      EXECUTION_TIMES[pos].bestNS = EXECUTION_TIMES[pos].timeNS;
    }

  if (EXECUTION_TIMES[pos].deltaTime > EXECUTION_TIMES[pos].worst)
    {
      EXECUTION_TIMES[pos].worst = EXECUTION_TIMES[pos].deltaTime;
      EXECUTION_TIMES[pos].worstNS = EXECUTION_TIMES[pos].timeNS;
    }

 if (calc_deviation == 1)
    {       
      /*
      EXECUTION_TIMES[pos].sumNSSQ += EXECUTION_TIMES[pos].timeNS * EXECUTION_TIMES[pos].timeNS;
      variance = (float32_t)((float32_t)((float32_t)EXECUTION_TIMES[pos].sumNSSQ / (float32_t)EXECUTION_TIMES[pos].noSamples) - (float32_t)(EXECUTION_TIMES[pos].averageNS * EXECUTION_TIMES[pos].averageNS));
      if (variance < 0)
        {
          // Rough estimation of standard deviation
          EXECUTION_TIMES[pos].deviation = (float32_t)((EXECUTION_TIMES[pos].worstUS - EXECUTION_TIMES[pos].bestUS) / 4.0);
        }
      else
        {
          EXECUTION_TIMES[pos].deviation = (float32_t)(sqrt(variance) / 1000.0);
        }*/
    }
}


/* __________________________________________________________________________
 *
 * FUNCTION NAME : get_perf
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
perf_struct get_perf(int32_t pos)
{
  return EXECUTION_TIMES[pos];
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : print_perf
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void print_perf()
{
  int32_t pos;
  perf_struct current;
  PERF_PRINT_EOL();
  PERF_PRINT_STRING("******************");
  PERF_PRINT_EOL();
  for(pos = 0; pos <= perf_current_pos(); pos++)
  {
    current = get_perf(pos);
    PERF_PRINT_STRING("*!*");
    PERF_PRINT_STRING(current.name);
    PERF_PRINT_STRING("*!*");
    PERF_PRINT_EOL();

    PERF_PRINT_STRING("Last Execution Time (tick): ");
    PERF_PRINT_UNSIGNED64(current.deltaTime);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Average (tick): ");
    PERF_PRINT_UNSIGNED64(current.average);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("WCET (tick): ");
    PERF_PRINT_UNSIGNED64(current.worst);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("BCET (tick): ");
    PERF_PRINT_UNSIGNED64(current.best);
    PERF_PRINT_EOL();

    PERF_PRINT_STRING("Last Execution Time (ns): ");
    PERF_PRINT_UNSIGNED64(current.timeNS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Average (ns): ");
    PERF_PRINT_UNSIGNED64(current.averageNS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("WCET (ns): ");
    PERF_PRINT_UNSIGNED64(current.worstNS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("BCET (ns): ");
    PERF_PRINT_UNSIGNED64(current.bestNS);
    PERF_PRINT_EOL();

    PERF_PRINT_STRING("Standard Deviation (us): ");
    PERF_PRINT_FLOAT(current.deviation);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Number of samples: ");
    PERF_PRINT_UNSIGNED(current.noSamples);
    PERF_PRINT_EOL();
    PERF_PRINT_EOL();
  }

  PERF_PRINT_STRING("******************");
  PERF_PRINT_EOL();
}

#endif
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
