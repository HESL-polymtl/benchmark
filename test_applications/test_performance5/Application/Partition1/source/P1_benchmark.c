
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
* Measure signaling time with priority:
* 1 - TA blocks waiting on aux_sem
* 2 - TB blocks waiting on aux_sem2
* 3 - TC acquires mutex
* 4 - TC signals aux_sem
* 5 - TA signals aux_sem2
* 6a - TA reads time
* 6 - TA tries to acquire mutex
* 7 - TC reads time (PIP)
* 8 - TC release mutex
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
#define NB_ITER 50
#endif

perf_task_retval_t mutex_processing_initialize_tests();

perf_task_retval_t TA_high_prio();
perf_task_retval_t TB_med_prio();
perf_task_retval_t TC_low_prio();

perf_task_handle_t tasks_handle[3];

perf_mutex_t mutex;
perf_sem_t aux_sem;
perf_sem_t aux_sem2;

DECLARE_TIME_COUNTERS(perf_time_t, time);
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
  perf_sem_create(&aux_sem, 0);
  perf_sem_create(&aux_sem2, 0);

  /* Create task */
  tasks_handle[0] = perf_create_task(TC_low_prio,
      "L",
      BASE_PRIO - 2 /* TC_low_prio is the low priority task. */
    );

  tasks_handle[1] = perf_create_task(TB_med_prio,
      "M",
      BASE_PRIO - 1 /* TB_med_prio is the med priority task. */
    );

  tasks_handle[2] = perf_create_task(TA_high_prio,
      "H",
      BASE_PRIO /* TA_high_prio is the high priority task. */
    );

  return TASK_DEFAULT_RETURN;
}

perf_task_retval_t TC_low_prio()
{
  int32_t i;
  DECLARE_TIME_STATS(uint64_t);

  for (i = 0; i < NB_ITER; i++)
  {
    perf_mutex_acquire(&mutex);
    perf_sem_signal(&aux_sem);
    WRITE_T2_COUNTER(time);
    COMPUTE_TIME_STATS(time, i);
    perf_cycle_reset_counter();

    perf_mutex_release(&mutex);
  }

  REPORT_BENCHMARK_RESULTS("--- Mutex PIP---");

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t TB_med_prio()
{
  int32_t i;

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&aux_sem2);
  }

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t TA_high_prio()
{
  int32_t i;

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&aux_sem);
    perf_sem_signal(&aux_sem2);
    WRITE_T1_COUNTER(time);
    perf_mutex_acquire(&mutex);
    perf_mutex_release(&mutex);
  }

  perf_task_suspend_self();

  while(1);
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
