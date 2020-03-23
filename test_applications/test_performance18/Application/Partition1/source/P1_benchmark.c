
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

EVENT_ID_TYPE eventID1 = 0;
EVENT_ID_TYPE eventID2 = 0;
EVENT_ID_TYPE eventID3 = 0;
EVENT_ID_TYPE eventID4 = 0;

EVENT_NAME_TYPE      eventName1 = "Event Name1";
EVENT_NAME_TYPE      eventName2 = "Event Name2";
EVENT_NAME_TYPE      eventName3 = "Event Name3";
EVENT_NAME_TYPE      eventName4 = "Event Name4";

void create_events();

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

  create_events();
  SET_EVENT (eventID1, &retCode);
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
  LOCK_LEVEL_TYPE out;
  while(1){
    WAIT_EVENT(eventID1,INFINITE_TIME_VALUE, &retCode);
    RESET_EVENT (eventID1, &retCode);
    INIT_TIME_MEASURE();
    LOCK_PREEMPTION(&out, &retCode);
    UNLOCK_PREEMPTION(&out, &retCode);
    SET_EVENT (eventID2, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}


void task2(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ID_TYPE my;
  PROCESS_STATUS_TYPE status;

  while(1){
    WAIT_EVENT(eventID2,INFINITE_TIME_VALUE, &retCode);
    RESET_EVENT (eventID2, &retCode);
    GET_MY_ID (&my,&retCode);
    GET_PROCESS_STATUS (my, &status, &retCode);
    SET_EVENT (eventID3, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}

void task3(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESSOR_CORE_ID_TYPE my;
  while(1){
    WAIT_EVENT(eventID3,INFINITE_TIME_VALUE, &retCode);
    RESET_EVENT (eventID3, &retCode);
    GET_MY_PROCESSOR_CORE_ID (&my, &retCode);
    SET_EVENT (eventID4, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}


void task4(void){
  RETURN_CODE_TYPE retCode = NO_ERROR;
  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("APEX APP");
  while(1){
    WAIT_EVENT(eventID4,INFINITE_TIME_VALUE, &retCode);
    FINISH_TIME_MEASURE();
    VALIDATE_TIME_MEASURE(1);
    PRINT_PERFORMANCE_INFO();
    RESET_EVENT (eventID4, &retCode);
    SET_EVENT (eventID1, &retCode);
    PERIODIC_WAIT(&retCode);
  }
}


void create_events()
  {

  RETURN_CODE_TYPE     errCode = 0;

  CREATE_EVENT(eventName1, &eventID1, &errCode);
  if(errCode != NO_ERROR){
    PERF_PRINT_STRING("EVENT CREATE1: ");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  RESET_EVENT(eventID1, &errCode);
  CREATE_EVENT(eventName2, &eventID2, &errCode);
  if(errCode != NO_ERROR){
    PERF_PRINT_STRING("EVENT CREATE2: ");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  RESET_EVENT(eventID2, &errCode);
  CREATE_EVENT(eventName3, &eventID3, &errCode);
  if(errCode != NO_ERROR){
    PERF_PRINT_STRING("EVENT CREATE3: ");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  RESET_EVENT(eventID3, &errCode);
  CREATE_EVENT(eventName4, &eventID4, &errCode);
  if(errCode != NO_ERROR){
    PERF_PRINT_STRING("EVENT CREATE4: ");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }
  RESET_EVENT(eventID4, &errCode);
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
