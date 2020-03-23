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

perf_task_retval_t mutex_initialize_test();
perf_task_retval_t sender();
perf_task_retval_t receiver();

perf_task_handle_t tasks_handle[2];
perf_sem_t aux_sem;
perf_mutex_t mutex;

DECLARE_TIME_COUNTERS(perf_time_t, s_to_r)
DECLARE_TIME_COUNTERS(perf_time_t, r_to_s)

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

  perf_initialize_test(mutex_initialize_test);
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t mutex_initialize_test()
{
  perf_sem_create(&aux_sem, 0);
  perf_mutex_create(&mutex);

  tasks_handle[0] = perf_create_task(sender,
      "S",
      BASE_PRIO - 1 /* sender is the low priority task. */
    );

  tasks_handle[1] = perf_create_task(receiver,
      "R",
      BASE_PRIO /* receiver is the high priority task. */
    );

  return TASK_DEFAULT_RETURN;
}

perf_task_retval_t sender()
{
  int32_t i;

  /* 1b - Benchmark. */
  for (i = 0; i < NB_ITER * 2; i++)
  {
    perf_mutex_acquire(&mutex);
    perf_sem_signal(&aux_sem);
    WRITE_T2_COUNTER(r_to_s)
    WRITE_T1_COUNTER(s_to_r)
    perf_mutex_release(&mutex);
  }

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t receiver()
{
  int32_t i;
  DECLARE_TIME_STATS(uint64_t)

  /* 1a - Benchmark. */
  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&aux_sem);
    perf_mutex_acquire(&mutex);
    WRITE_T2_COUNTER(s_to_r)
    perf_mutex_release(&mutex);
    COMPUTE_TIME_STATS(s_to_r, i);
    perf_cycle_reset_counter();
  }

  REPORT_BENCHMARK_RESULTS("-- Mutex release unblock --")
  RESET_TIME_STATS()

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&aux_sem);
    WRITE_T1_COUNTER(r_to_s)
    perf_mutex_acquire(&mutex);
    perf_mutex_release(&mutex);
    COMPUTE_TIME_STATS(r_to_s, i)
    perf_cycle_reset_counter();

  }

  REPORT_BENCHMARK_RESULTS("-- Mutex request block --")

  perf_task_suspend_self();

  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
