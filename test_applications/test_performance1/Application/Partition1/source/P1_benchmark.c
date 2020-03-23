
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
#include "P1.h"

#include "performance_lib.h"

#ifndef NB_ITER
#define NB_ITER 1000
#endif

#ifndef NB_TASK
#define NB_TASK 15
#endif

perf_task_retval_t task();
perf_task_retval_t round_robin_stress_initialize_test();

perf_task_handle_t tasks_handle[NB_TASK];
char_t tasks_name[NB_TASK][5];


volatile int32_t tasks_idx;

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
  perf_initialize_test(round_robin_stress_initialize_test); 
  SET_PARTITION_MODE (NORMAL, &errCode);
  if (errCode != NO_ERROR){
    PERF_PRINT_STRING("\nError changing partition mode");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  while(1);
}

perf_task_retval_t round_robin_stress_initialize_test()
{
  int32_t i;

  tasks_idx = 0;

  for (i = 0 ; i < NB_TASK; i++)
  {
    tasks_name[i][0] = 65;
    tasks_name[i][1] = (65 + i) % 255;
    tasks_name[i][2] = (66 + i) % 255;
    tasks_name[i][3] = (67 + i) % 255;
    tasks_name[i][4] = '\0';
    tasks_handle[i] = perf_create_task(task, tasks_name[i], BASE_PRIO);
  }
  PERF_PRINT_STRING("Process Context Switch. No of processes: ");
  PERF_PRINT_NUMBER(NB_TASK);
  PERF_PRINT_EOL();

}

perf_task_retval_t task()
{
  int32_t i;
  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("CNTX SWITCH");
  int32_t id =  tasks_idx++;

  for (i = 0; i < NB_ITER; i++)
  {
    INIT_TIME_MEASURE();
    perf_task_yield();
    FINISH_TIME_MEASURE();
    VALIDATE_TIME_MEASURE(1);
  }
    
  if (id == 0){
    PRINT_PERFORMANCE_INFO();
  }
  else{
    PERF_PRINT_STRING("Process ");
    PERF_PRINT_NUMBER(id);
    PERF_PRINT_STRING(" done.");
    PERF_PRINT_EOL();
  }

  perf_task_suspend_self();

  while(1);

}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
