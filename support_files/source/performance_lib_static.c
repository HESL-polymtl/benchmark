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
    if (check_in(pos, name) == 1)
    {
      return pos;
    }
  }

  pos = perf_current_pos();
  if (pos < QUANTITY_OF_TESTS)
  {
    pos++;
    strcpy(EXECUTION_TIMES[pos].name, name);
    EXECUTION_TIMES[pos].exec_time = 0;
    EXECUTION_TIMES[pos].currentTime = 0;
    EXECUTION_TIMES[pos].deltaTime = 0;
    EXECUTION_TIMES[pos].sPrevTime = 0;
    EXECUTION_TIMES[pos].first_run = TRUE;
    EXECUTION_TIMES[pos].worstUS = 0.0;
    EXECUTION_TIMES[pos].bestUS = 3.402823466e37;
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
 * __________________________________________________________________________
 */
void perf_validate_measurements(int32_t pos, uint8_t calc_deviation)
{
  float32_t dev = 0.0;
  float32_t work = 0.0;
  float32_t timeUS = 0.0;
  float32_t currentExecutionTime = 0.0;

  if (EXECUTION_TIMES[pos].currentTime != EXECUTION_TIMES[pos].sPrevTime)
  {
    EXECUTION_TIMES[pos].deltaTime = (uint32_t) (EXECUTION_TIMES[pos].currentTime - EXECUTION_TIMES[pos].sPrevTime);
    EXECUTION_TIMES[pos].sPrevTime = EXECUTION_TIMES[pos].currentTime;
    EXECUTION_TIMES[pos].exec_time = EXECUTION_TIMES[pos].deltaTime;
    if (EXECUTION_TIMES[pos].first_run == TRUE)
    {
      EXECUTION_TIMES[pos].average = EXECUTION_TIMES[pos].deltaTime;
      EXECUTION_TIMES[pos].first_run = FALSE;
    }

    else
    {
      //EXECUTION_TIMES[pos].exec_time = (EXECUTION_TIMES[pos].deltaTime + EXECUTION_TIMES[pos].exec_time) / 2;
      EXECUTION_TIMES[pos].average = (EXECUTION_TIMES[pos].average+EXECUTION_TIMES[pos].deltaTime)/2;
    }
  }
  
  currentExecutionTime = (float32_t) perf_tick_to_ns((uint64_t)EXECUTION_TIMES[pos].exec_time);
  if (calc_deviation == 1)
  {
    timeUS = currentExecutionTime / 1000.0;
    work = (timeUS + EXECUTION_TIMES[pos].timeUS)/2;
    work = (((timeUS-work)*(timeUS-work)) + ((EXECUTION_TIMES[pos].timeUS-work)*(EXECUTION_TIMES[pos].timeUS-work))) / 2;
    dev = sqrt(work);
    EXECUTION_TIMES[pos].deviation = dev;
  }

  else
  {
    (void) dev;
    (void) timeUS;
  }

  work = (float32_t) perf_tick_to_ns((uint64_t)EXECUTION_TIMES[pos].average);
  EXECUTION_TIMES[pos].averageUS = work / 1000.0;
  EXECUTION_TIMES[pos].timeNS = currentExecutionTime;
  EXECUTION_TIMES[pos].timeUS = EXECUTION_TIMES[pos].timeNS / 1000.0;
  EXECUTION_TIMES[pos].timeMS = EXECUTION_TIMES[pos].timeNS / 1000000.0;
  if (EXECUTION_TIMES[pos].timeUS < EXECUTION_TIMES[pos].bestUS)
  {
    EXECUTION_TIMES[pos].bestUS = EXECUTION_TIMES[pos].timeUS;
  }

  if (EXECUTION_TIMES[pos].timeUS > EXECUTION_TIMES[pos].worstUS)
  {
    EXECUTION_TIMES[pos].worstUS = EXECUTION_TIMES[pos].timeUS;
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
    PERF_PRINT_STRING("Last Execution Time (us): ");
    PERF_PRINT_FLOAT(current.timeUS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Average (us): ");
    PERF_PRINT_FLOAT(current.averageUS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("WCET (us): ");
    PERF_PRINT_FLOAT(current.worstUS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("BCET (us): ");
    PERF_PRINT_FLOAT(current.bestUS);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Standard Deviation (us): ");
    PERF_PRINT_FLOAT(current.deviation);
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
