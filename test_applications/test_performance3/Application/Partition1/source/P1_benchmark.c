
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
#define NB_ITER 300
#endif

perf_task_retval_t mutex_processing_initialize_tests();
perf_task_retval_t task();

perf_task_handle_t task_handle;
perf_mutex_t mutex;

/* __________________________________________________________________________
*
* FUNCTION NAME : main
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

  perf_initialize_test(mutex_processing_initialize_tests);
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t mutex_processing_initialize_tests()
{
  /* Create resources */
  perf_mutex_create(&mutex);

  /* Create task */
  task_handle = perf_create_task(task,
      "S",
      BASE_PRIO - 1 /* `task` is the only task */
    );

}

perf_task_retval_t task()
{
  int32_t i;
  DECLARE_TIME_COUNTERS(perf_time_t, acqui);
  DECLARE_TIME_COUNTERS(perf_time_t, recv);
  DECLARE_TIME_STATS(uint64_t);

  /* 1 - Measure mutex acquisition time */
  for (i = 0; i < NB_ITER; i++)
  {
    WRITE_T1_COUNTER(acqui);
    perf_mutex_acquire(&mutex);
    WRITE_T2_COUNTER(acqui);
    perf_mutex_release(&mutex);
    COMPUTE_TIME_STATS(acqui, i);

    perf_cycle_reset_counter();

  }

  REPORT_BENCHMARK_RESULTS("--- mutex acquisition ---");
  RESET_TIME_STATS();

  /* 2 - Measure mutex release time */
  for (i = 0; i < NB_ITER; i++)
  {
    perf_mutex_acquire(&mutex);
    WRITE_T1_COUNTER(recv);
    perf_mutex_release(&mutex);
    WRITE_T2_COUNTER(recv);
    COMPUTE_TIME_STATS(recv, i);

    perf_cycle_reset_counter();

  }

  REPORT_BENCHMARK_RESULTS("--- mutex release ---");

  perf_task_suspend_self();

  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
