
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
#define NB_ITER 1000
#endif

perf_task_retval_t sem_processing_initialize_test();
perf_task_retval_t task();

perf_task_handle_t task_handle;
perf_sem_t sem;

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
  perf_initialize_test(sem_processing_initialize_test);
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t sem_processing_initialize_test()
{
  perf_sem_create(&sem, 0);

  task_handle = perf_create_task(task,
      "S",
      BASE_PRIO - 1 /* `task` is the only task */
    );

  return TASK_DEFAULT_RETURN;
}

perf_task_retval_t task()
{
  int32_t i;
  DECLARE_TIME_COUNTERS(perf_time_t, inc);
  DECLARE_TIME_COUNTERS(perf_time_t, dec);
  DECLARE_TIME_STATS(uint64_t);
  uint64_t _workload_results[NB_ITER];

  /* 1b - Measure semaphore signaling time */
  for (i = 0; i < NB_ITER; i++)
  {
    WRITE_T1_COUNTER(inc);
    perf_sem_signal(&sem);
    WRITE_T2_COUNTER(inc);
    perf_sem_wait(&sem);
    COMPUTE_TIME_STATS(inc, i);
    DO_WORKLOAD(i)
    perf_cycle_reset_counter();
  }

  REPORT_BENCHMARK_RESULTS("--- Sem: Signal ---");

  RESET_TIME_STATS();

  /* 2b - Measure semaphore acquisition time */
  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_signal(&sem);
    WRITE_T1_COUNTER(dec);
    perf_sem_wait(&sem);
    WRITE_T2_COUNTER(dec);
    COMPUTE_TIME_STATS(dec, i);
    DO_WORKLOAD(i)
    perf_cycle_reset_counter();
  }

  REPORT_BENCHMARK_RESULTS("--- Sem: Wait ---");
  (void)_workload_results;
  perf_task_suspend_self();

  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
