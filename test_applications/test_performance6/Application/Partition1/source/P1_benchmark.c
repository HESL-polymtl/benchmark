
/* __________________________________________________________________________
* DEVELOPMENT HISTORY:
* --------------------
*
* $Author:: $: Felipe Gohring de Magalhaes
* $Rev:: $: Revision of last commit
* $Date:: $: 10/25/2018
* $Comments (Refer to applicable SCR/PR) $: Initial version of the file.
* __________________________________________________________________________
*/
/* __________________________________________________________________________
* MODULE DESCRIPTION:
* -------------------
* Filename : P1_benchmark.c
* Original Author : Felipe Gohring de Magalhaes
*
* This module contains partition 1 main function.
*
* ASSUMPTIONS, CONSTRAINTS OR LIMITATIONS:
* ----------------------------------------
* 
*
* REFERENCES:
* -----------
* None.
* __________________________________________________________________________
*/
/* INCLUDE SECTION */
/*Include here any system specific lib that is not with performance_lib.h*/

#include "performance_lib.h"

#ifndef NB_ITER
#define NB_ITER 100
#endif

#define NB_TASK 2

perf_task_retval_t monitor();
perf_task_retval_t task();
perf_task_retval_t jitter_initialize_test();

perf_task_handle_t tasks_handle[NB_TASK + 1];
char tasks_name[NB_TASK][5];

DECLARE_TIME_COUNTERS(perf_time_t, _)


/* __________________________________________________________________________
*
* FUNCTION NAME : main_process
* DESCRIPTION   : Partition 1 function.
* PARAMETERS : None.
*   INPUT :
*   OUTPUT :
* RETURN : None.
* __________________________________________________________________________
*/
void MAIN_FUNCTION()
{
  RETURN_CODE_TYPE errCode;

  perf_initialize_test(jitter_initialize_test);
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t jitter_initialize_test()
{
  int32_t i;

  for (i = 0 ; i < NB_TASK; i++)
  {
    tasks_name[i][0] = 65;
    tasks_name[i][1] = (65 + i) % 255;
    tasks_name[i][2] = (66 + i) % 255;
    tasks_name[i][3] = (67 + i) % 255;
    tasks_name[i][4] = '\0';
    tasks_handle[i] = perf_create_task(task, tasks_name[i], BASE_PRIO - 1);
  }

  tasks_handle[NB_TASK + 1] = perf_create_task(monitor, "MON", BASE_PRIO);
}

perf_task_retval_t task()
{
  int32_t i;

  for (i = 0; i < NB_ITER; i++)
  {
    perf_task_yield();
  }

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t monitor()
{
  int32_t i;
  long diff;

  perf_time_t time_t1;
  perf_time_t time_t2;
  DECLARE_TIME_STATS(uint64_t);

  for (i = 0; i < NB_ITER; i++)
  {
    time_t1 = GET_CURRENT_TICKS();
    time_t1 = perf_add_times(&time_t1, 500000000);

    perf_task_delay(400);

    time_t2 = GET_CURRENT_TICKS();
    diff = perf_time_diff(&time_t1, &time_t2);
    PERF_PRINT_NUMBER(diff);
    PERF_PRINT_EOL();

    perf_task_yield();
  }

  REPORT_BENCHMARK_RESULTS("--Monitor--");
  (void) cycles;
  perf_task_suspend_self();

  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
