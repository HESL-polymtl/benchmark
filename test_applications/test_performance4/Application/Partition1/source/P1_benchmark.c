
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
#define NB_ITER 30
#endif

#ifndef NB_WORKLOAD_TASK
#define NB_WORKLOAD_TASK 3
#endif

#ifndef DELAY_MS
#define DELAY_MS 10
#endif

perf_task_retval_t mutex_initialize_test();
perf_task_retval_t sender();
perf_task_retval_t receiver();
perf_task_retval_t workload_task();

perf_task_handle_t tasks_handle[2];
perf_task_handle_t workload_tasks_handle[NB_WORKLOAD_TASK];
char_t workload_tasks_name[NB_WORKLOAD_TASK][4];

perf_sem_t aux_sem;
perf_mutex_t mutex;

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
  int32_t i;

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

  for (i = 0 ; i < NB_WORKLOAD_TASK; i++)
  {
    workload_tasks_name[i][0] = 65;
    workload_tasks_name[i][1] = (65 + i) % 255;
    workload_tasks_name[i][2] = 0;
    workload_tasks_name[i][3] = 0;
    /* Workoad tasks are the lowest priority */
    workload_tasks_handle[i] = perf_create_task(workload_task, workload_tasks_name[i], BASE_PRIO - 2);
  }
}

perf_task_retval_t sender()
{
  int32_t i;

  /* 1b - Benchmark. */
  for (i = 0; i < NB_ITER; i++)
  {
    perf_mutex_acquire(&mutex);
    perf_sem_signal(&aux_sem);
    WRITE_T1_COUNTER(_)
    perf_mutex_release(&mutex);
    perf_task_delay(DELAY_MS);
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
    WRITE_T2_COUNTER(_)
    perf_mutex_release(&mutex);
    COMPUTE_TIME_STATS(_, i);
    perf_cycle_reset_counter();

  }

  REPORT_BENCHMARK_RESULTS("-- Mutex workload --")

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t workload_task()
{
  int32_t i;
  int32_t j;
  uint64_t _workload_results[100];

  while (1)
  {
    for (j = 0; j < 100; j++) {
      DO_WORKLOAD(i % 100)
    }
    perf_task_yield();
    i++;
  }

  perf_task_suspend_self();
  (void) _workload_results;
  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
