
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

void task1(void);
void task2(void);
void task3(void);
void task4(void);

SEMAPHORE_ID_TYPE sem_id1;
SEMAPHORE_ID_TYPE sem_id2;
SEMAPHORE_ID_TYPE sem_id3;
SEMAPHORE_ID_TYPE sem_id4;

SEMAPHORE_NAME_TYPE Semaphore1Name = "Semaphore1";
SEMAPHORE_NAME_TYPE Semaphore2Name = "Semaphore2";
SEMAPHORE_NAME_TYPE Semaphore3Name = "Semaphore3";
SEMAPHORE_NAME_TYPE Semaphore4Name = "Semaphore4";

SEMAPHORE_VALUE_TYPE CurrentValue = 0;
SEMAPHORE_VALUE_TYPE MaxValue = 1;
QUEUING_DISCIPLINE_TYPE     sem_queue;

void create_sem();

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
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ATTRIBUTE_TYPE processAttrib;
  PROCESS_ID_TYPE processId;

  PARTITION_STATUS_TYPE partitionStatus;
  GET_PARTITION_STATUS(&partitionStatus, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&task1;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_LOW_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Task1", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Task1 PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&task2;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_LOW_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Task2", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Task2 PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&task3;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_LOW_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Task3", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Task3 PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&task4;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_LOW_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Task4", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Task4 PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);

  create_sem();

  SET_PARTITION_MODE(NORMAL, &retCode);
  while(1)
  {
    PERF_PRINT_STRING("* main_process Running in IDLE mode ");
  }
}

void task1(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("APEX APP");

  while(1){
    WAIT_SEMAPHORE(sem_id1,INFINITE_TIME_VALUE, &retCode);
    INIT_TIME_MEASURE();
    SIGNAL_SEMAPHORE(sem_id2, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}


void task2(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ID_TYPE myId       = 0;
  while(1){
    WAIT_SEMAPHORE(sem_id2,INFINITE_TIME_VALUE, &retCode);
    GET_MY_ID(&myId, &retCode);
    SIGNAL_SEMAPHORE(sem_id3, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}

void task3(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ID_TYPE process1Id = 0;
  while(1){
    WAIT_SEMAPHORE(sem_id3,INFINITE_TIME_VALUE, &retCode);
    GET_PROCESS_ID("Task1", &process1Id, &retCode);
    SIGNAL_SEMAPHORE(sem_id4, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}


void task4(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ID_TYPE process1Id = 0;
  PROCESS_STATUS_TYPE status;

  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("APEX APP");
  while(1){
    WAIT_SEMAPHORE(sem_id4,INFINITE_TIME_VALUE, &retCode);
    GET_PROCESS_ID("Task1", &process1Id, &retCode);
    GET_PROCESS_STATUS (process1Id, &status, &retCode);
    FINISH_TIME_MEASURE();
    VALIDATE_TIME_MEASURE(1);
    PRINT_PERFORMANCE_INFO();
    SIGNAL_SEMAPHORE(sem_id1, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}

void create_sem()
  {
  RETURN_CODE_TYPE   err_code;

  sem_queue = FIFO;

  CREATE_SEMAPHORE (Semaphore1Name, (SEMAPHORE_VALUE_TYPE) 1, MaxValue, sem_queue, &sem_id1, &err_code);
  if(err_code != NO_ERROR){
    PERF_PRINT_STRING("CREATE SEM: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  } 

  CREATE_SEMAPHORE (Semaphore2Name, CurrentValue, MaxValue, sem_queue, &sem_id2, &err_code);
  if(err_code != NO_ERROR){
    PERF_PRINT_STRING("CREATE SEM: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  } 

  CREATE_SEMAPHORE (Semaphore3Name, CurrentValue, MaxValue, sem_queue, &sem_id3, &err_code);
  if(err_code != NO_ERROR){
    PERF_PRINT_STRING("CREATE SEM: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  }

  CREATE_SEMAPHORE (Semaphore4Name, CurrentValue, MaxValue, sem_queue, &sem_id4, &err_code);
  if(err_code != NO_ERROR){
    PERF_PRINT_STRING("CREATE SEM: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  }

}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
