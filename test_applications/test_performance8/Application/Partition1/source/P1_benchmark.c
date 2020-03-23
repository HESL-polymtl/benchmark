
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
* 2 - TB blocks waiting on sem
* 3 - TC signals aux_sem
* 4a - TA reads t1    
* 4b - TA signals aux
*    - OS moves TB to ready
* 4c - TA reads t2
* 5 - Go back to 1
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

#ifndef NB_TASK
#define NB_TASK 1
#endif

perf_task_retval_t sem_prio_initialize_test();
perf_task_retval_t TA_high_prio();
perf_task_retval_t TB_med_prio();
perf_task_retval_t TC_low_prio();

perf_task_handle_t tasks_handle[2];

perf_task_handle_t med_tasks_handle[NB_TASK];
char workload_tasks_name[NB_TASK][4];

perf_sem_t aux_sem;
perf_sem_t sem;

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

  perf_initialize_test(sem_prio_initialize_test);
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t sem_prio_initialize_test()
{
  int32_t i;

  perf_sem_create(&sem, 0);
  perf_sem_create(&aux_sem, 0);

  tasks_handle[0] = perf_create_task(TC_low_prio,
      "L",
      BASE_PRIO - 2 /* TC_low_prio is the low priority task. */
    );

  for (i = 0; i < NB_TASK; i++)
  {
    workload_tasks_name[i][0] = 65;
    workload_tasks_name[i][1] = (65 + i) % 255;
    workload_tasks_name[i][2] = 0;
    workload_tasks_name[i][3] = 0;

    med_tasks_handle[i] = perf_create_task(TB_med_prio,
        workload_tasks_name[i],
        BASE_PRIO - 1
      );
  }

  tasks_handle[1] = perf_create_task(TA_high_prio,
      "H",
      BASE_PRIO /* TA_high_prio is the high priority task. */
    );

  return TASK_DEFAULT_RETURN;
}

perf_task_retval_t TC_low_prio()
{
  int i;

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_signal(&aux_sem);
  }

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t TB_med_prio()
{
  int i;

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&sem);
  }

  perf_task_suspend_self();

  while(1);
}

perf_task_retval_t TA_high_prio()
{
  int i;
  DECLARE_TIME_COUNTERS(perf_time_t, time);
  DECLARE_TIME_STATS(uint64_t);

  for (i = 0; i < NB_ITER; i++)
  {
    perf_sem_wait(&aux_sem);

    WRITE_T1_COUNTER(time);
    perf_sem_signal(&sem);
    WRITE_T2_COUNTER(time);

    COMPUTE_TIME_STATS(time, i);
    perf_cycle_reset_counter();
  }

  REPORT_BENCHMARK_RESULTS("--- sem singaling with prio ---");

  perf_task_suspend_self();

  while(1);
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
