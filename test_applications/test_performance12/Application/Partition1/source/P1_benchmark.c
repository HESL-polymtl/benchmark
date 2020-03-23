
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

/*******************************************************************************
 * TESTS SETTINGS
 ******************************************************************************/
#define BASE_TESTS
#define EXTENDED_TESTS
#define INTERPARTITION_TESTS
/*******************************************************************************
 * PARTITION SPECIFIC VARIABLES
 ******************************************************************************/
#ifdef BASE_TESTS
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

    BUFFER_NAME_TYPE            buff_name1 = "BUFFER1";
    BUFFER_NAME_TYPE            buff_name2 = "BUFFER2";
    BUFFER_NAME_TYPE            buff_name3 = "BUFFER3";
    BUFFER_NAME_TYPE            buff_name4 = "BUFFER4";
    MESSAGE_SIZE_TYPE           max_size;
    MESSAGE_RANGE_TYPE          msg_range;
    QUEUING_DISCIPLINE_TYPE     buffer_queue;

    MESSAGE_ADDR_TYPE msgToSend16 = NULL;

    BLACKBOARD_NAME_TYPE        black_name;
    MESSAGE_SIZE_TYPE           max_size;
    BLACKBOARD_ID_TYPE          black_id1, black_id2;

#endif


#ifdef EXTENDED_TESTS
    EVENT_ID_TYPE eventID1 = 0;
    EVENT_ID_TYPE eventID2 = 0;
    EVENT_ID_TYPE eventID3 = 0;
    EVENT_ID_TYPE eventID4 = 0;

    EVENT_NAME_TYPE      eventName1 = "Event Name1";
    EVENT_NAME_TYPE      eventName2 = "Event Name2";
    EVENT_NAME_TYPE      eventName3 = "Event Name3";
    EVENT_NAME_TYPE      eventName4 = "Event Name4";


    uint8_t BIG_ARRAY[10000] = {0};
#endif


#ifdef INTERPARTITION_TESTS
    QUEUING_PORT_ID_TYPE sDestPort1  = 0;
    QUEUING_PORT_ID_TYPE sDestPort2  = 0;
    QUEUING_PORT_ID_TYPE sDestPort3  = 0;
    QUEUING_PORT_ID_TYPE sDestPort4 = 0;

    SAMPLING_PORT_ID_TYPE sSamplingID1   = 0;
    SAMPLING_PORT_ID_TYPE sSamplingID2   = 0;
    SAMPLING_PORT_ID_TYPE sSamplingID3   = 0;
    SAMPLING_PORT_ID_TYPE sSamplingID4   = 0;

#endif

PROCESS_NAME_TYPE  process1Name = "PRINT";
/*******************************************************************************
 * TESTS FUNCTIONS
 ******************************************************************************/
#ifdef BASE_TESTS
    void test_partition_status(uint8_t calc_dev);
    void test_create_sem(uint8_t calc_dev);
    void test_create_buff(uint8_t calc_dev); 
    void test_create_blackboard(uint8_t calc_dev);
    void test_lock_preem(uint8_t calc_dev); 
    void test_unlock_preem(uint8_t calc_dev);
    void test_display_black(int size, uint8_t calc_dev);
    void test_read_black(int size, uint8_t calc_dev);
    void test_send_buffer(int size, uint8_t calc_dev);
    void test_receive_buffer(int size, uint8_t calc_dev);
    void test_signal_sem(uint8_t calc_dev);
    void test_wait_on_sem(uint8_t calc_dev);
#endif

#ifdef EXTENDED_TESTS
    void test_set_prio(uint8_t calc_dev);
    void test_my_id(uint8_t calc_dev);
    void test_process_id(uint8_t calc_dev);
    void test_process_status(uint8_t calc_dev);
    void test_sem_id(uint8_t calc_dev);
    void test_sem_status(uint8_t calc_dev);
    void test_create_event(uint8_t calc_dev);
    void test_set_event(uint8_t calc_dev);
    void test_event_id(uint8_t calc_dev);
    void test_event_status(uint8_t calc_dev);
    void test_malloc(uint8_t calc_dev);
    void test_wait_event(uint8_t calc_dev);
    void test_serial_print(uint8_t calc_dev);
    void test_write_memory(uint8_t calc_dev);
    void test_read_memory(uint8_t calc_dev);
    void test_get_ticks(uint8_t calc_dev);
#endif

#ifdef INTERPARTITION_TESTS
    void test_sampling_ports(uint8_t calc_dev);
    void test_create_sampling_ports(uint8_t calc_dev);
    void test_queuing_ports_status(uint8_t calc_dev);
    void test_queuing_ports_ids(uint8_t calc_dev);
    void test_create_queuing_ports(uint8_t calc_dev);
    void test_write_queuing_ports(uint8_t calc_dev);
    void test_read_queuing_ports(uint8_t calc_dev);
    void test_write_sampling_ports(uint8_t calc_dev);
    void test_read_sampling_ports(uint8_t calc_dev);
#endif

void printResults(void);
void dummyProcess();

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
void  MAIN_FUNCTION()
{
  PROCESS_ID_TYPE        process_id, process_id2;
  PROCESS_ATTRIBUTE_TYPE process_att;
  PARTITION_STATUS_TYPE partitionStatus;
  
  RETURN_CODE_TYPE       ret_type;
  DECLARE_TIME_MEASURE();

  PERF_PRINT_STRING("Init P1 partition");

  GET_PARTITION_STATUS(&partitionStatus, &ret_type);
  
  INITIALIZE_TIME_VARS("PRINT_INIT")
  /* Set processes  */
  process_att.ENTRY_POINT   = printResults;
  process_att.DEADLINE      = SOFT;
  process_att.PERIOD        = partitionStatus.PERIOD;
  process_att.STACK_SIZE    = 16384;
  process_att.TIME_CAPACITY = partitionStatus.PERIOD;
  process_att.BASE_PRIORITY = 2;
  strncpy(process_att.NAME, process1Name, MAX_NAME_LENGTH);
  
  INIT_TIME_MEASURE()
  CREATE_PROCESS(&process_att, &process_id, &ret_type);
  FINISH_TIME_MEASURE()

  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot create PRINT process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }
  else{
      VALIDATE_TIME_MEASURE(0);
  }

  INITIALIZE_TIME_VARS("DUMMY_INIT1")
  process_att.ENTRY_POINT   = dummyProcess;
  process_att.DEADLINE      = SOFT;
  process_att.PERIOD        = partitionStatus.PERIOD;
  process_att.STACK_SIZE    = 4096;
  process_att.TIME_CAPACITY = partitionStatus.PERIOD;
  process_att.BASE_PRIORITY = 2;
  memcpy(process_att.NAME, "DUMMY1\0", strlen("DUMMY1\0") * sizeof(char));
  
  INIT_TIME_MEASURE()
  CREATE_PROCESS(&process_att, &process_id2, &ret_type);
  FINISH_TIME_MEASURE()

  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot create DUMMY1 process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }
  else{
      VALIDATE_TIME_MEASURE(0);
  }

  #ifdef BASE_TESTS
      PERF_PRINT_STRING("Running APEX analysis");
      test_create_sem(0);
      test_create_buff(0); 
      test_create_blackboard(0);

      msgToSend16 = (MESSAGE_ADDR_TYPE) malloc(16*sizeof(T_int8));
  #endif

  #ifdef EXTENDED_TESTS
      test_malloc(0);
      test_create_event(0);
  #endif

  #ifdef INTERPARTITION_TESTS
      test_create_queuing_ports(0);
      test_create_sampling_ports(0);
  #endif

  START(process_id, &ret_type);
  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot start PRINT process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }
  else{
      VALIDATE_TIME_MEASURE(0);
  }

  START(process_id2, &ret_type);
  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot start DUMMY process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }

  PERF_PRINT_STRING("P1 partition switching to normal mode");
  SET_PARTITION_MODE(NORMAL, &ret_type);

  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot switch P1 partition to NORMAL state "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }

  while(1);

}

void dummyProcess()
{
  RETURN_CODE_TYPE retCode = NO_ERROR;
  
  while(1){
      PERIODIC_WAIT(&retCode);
  }
}

void printResults(void)
{
RETURN_CODE_TYPE retCode = NO_ERROR;

while(TRUE){
  #ifdef BASE_TESTS
      test_partition_status(1);
      test_lock_preem(1); 
      test_unlock_preem(1);
      test_display_black(16,1);
      test_read_black(16,1);
      test_display_black(64,1);
      test_read_black(64,1);
      test_send_buffer(16,1);
      test_receive_buffer(16,1);
      test_signal_sem(1);
      test_wait_on_sem(1);
  #endif

  #ifdef EXTENDED_TESTS
      test_malloc(1);
      test_set_prio(1);
      test_my_id(1);
      test_process_id(1);
      test_process_status(1);
      test_sem_id(1);
      test_sem_status(1);
      test_set_event(1);
      test_event_id(1);
      test_event_status(1);
      test_wait_event(1);
      test_write_memory(1);
      test_read_memory(1);
      test_get_ticks(1);
  #endif

  #ifdef INTERPARTITION_TESTS
      test_sampling_ports(1);
      test_queuing_ports_status(1);
      test_queuing_ports_ids(1);
      test_write_queuing_ports(1);
      test_read_queuing_ports(1);
      test_write_sampling_ports(1);
      test_read_sampling_ports(1);
  #endif
  PRINT_PERFORMANCE_INFO();
  PERIODIC_WAIT(&retCode);
}
}

#ifdef BASE_TESTS
void test_partition_status(uint8_t calc_dev)
{
  PARTITION_STATUS_TYPE partition_status;
  RETURN_CODE_TYPE      return_code;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("PART_STATUS");

  INIT_TIME_MEASURE();
  GET_PARTITION_STATUS (&partition_status,&return_code);
  FINISH_TIME_MEASURE();
  if(return_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("PART STATUS: ");
      PERF_PRINT_NUMBER(return_code);
      PERF_PRINT_EOL();
  } 
  
  INIT_TIME_MEASURE();
  GET_PARTITION_STATUS (&partition_status,&return_code);
  FINISH_TIME_MEASURE();
  if(return_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PART STATUS: ");
      PERF_PRINT_NUMBER(return_code);
      PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  GET_PARTITION_STATUS (&partition_status,&return_code);
  FINISH_TIME_MEASURE();
  if(return_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PART STATUS: ");
      PERF_PRINT_NUMBER(return_code);
      PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  GET_PARTITION_STATUS (&partition_status,&return_code);
  FINISH_TIME_MEASURE();
  if(return_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PART STATUS: ");
      PERF_PRINT_NUMBER(return_code);
      PERF_PRINT_EOL();
  } 
}

void test_create_sem(uint8_t calc_dev)
{
  RETURN_CODE_TYPE   err_code;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SEM_CREATE");

  sem_queue = FIFO;

  INIT_TIME_MEASURE();
  CREATE_SEMAPHORE (Semaphore1Name, CurrentValue, MaxValue, sem_queue, &sem_id1, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("CREATE SEM: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  CREATE_SEMAPHORE (Semaphore2Name, CurrentValue, MaxValue, sem_queue, &sem_id2, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("CREATE SEM: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  CREATE_SEMAPHORE (Semaphore3Name, CurrentValue, MaxValue, sem_queue, &sem_id3, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE SEM: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_SEMAPHORE (Semaphore4Name, CurrentValue, MaxValue, sem_queue, &sem_id4, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("CREATE SEM: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  } 
}

void test_create_buff(uint8_t calc_dev)
{
  RETURN_CODE_TYPE   err_code;
  BUFFER_ID_TYPE     buff_id1, buff_id2, buff_id3, buff_id4;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("BUFFER_CREATE");

  max_size = 16;
  msg_range = 2;
  buffer_queue = FIFO;

  INIT_TIME_MEASURE();
  CREATE_BUFFER (buff_name1, max_size, msg_range, buffer_queue, &buff_id1, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Buffer: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_BUFFER (buff_name2, max_size, msg_range, buffer_queue, &buff_id2, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Buffer: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_BUFFER (buff_name3, max_size, msg_range, buffer_queue, &buff_id3, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Buffer: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  CREATE_BUFFER (buff_name4, max_size, msg_range, buffer_queue, &buff_id4, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Buffer: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
} 
void test_create_blackboard(uint8_t calc_dev)
{
  RETURN_CODE_TYPE   err_code;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("BLACKBOARD");

  strcpy(black_name, "BB");
  max_size = 64;

  INIT_TIME_MEASURE();
  CREATE_BLACKBOARD (black_name, max_size, &black_id1, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("BB Create 1: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
  strcpy(black_name, "BB2");
  INIT_TIME_MEASURE();
  CREATE_BLACKBOARD (black_name, max_size, &black_id2, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("BB Create 2: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
}

void test_lock_preem(uint8_t calc_dev)
{
  LOCK_LEVEL_TYPE out;
  RETURN_CODE_TYPE   err_code;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("LOCK");

  INIT_TIME_MEASURE();
  LOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("LOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  

  INIT_TIME_MEASURE();
  LOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("LOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
}
      

  INIT_TIME_MEASURE();
  LOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("LOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
}
      

  INIT_TIME_MEASURE();
  LOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("LOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
      
}

void test_unlock_preem(uint8_t calc_dev)
{
  LOCK_LEVEL_TYPE out;
  RETURN_CODE_TYPE   err_code;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("UNLOCK");

  LOCK_PREEMPTION(&out, &err_code);
  INIT_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("UNLOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
  LOCK_PREEMPTION(&out, &err_code);
  INIT_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("UNLOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
  LOCK_PREEMPTION(&out, &err_code);
  INIT_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("UNLOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
  LOCK_PREEMPTION(&out, &err_code);
  INIT_TIME_MEASURE();
  UNLOCK_PREEMPTION(&out, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("UNLOCK: ");
      PERF_PRINT_NUMBER(err_code);
      PERF_PRINT_EOL();
  }
  
}

void test_display_black(int size, uint8_t calc_dev)
{
RETURN_CODE_TYPE   err_code;
  char message16[16] = {'0'};
  char message64[64] = {'B'};

  DECLARE_TIME_MEASURE()

  if (size == 16){
      INITIALIZE_TIME_VARS("BBoard16");

      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id1,(MESSAGE_ADDR_TYPE)message16, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
  
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id1, (MESSAGE_ADDR_TYPE)message16, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
  
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id1, (MESSAGE_ADDR_TYPE)message16, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }

      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id1, (MESSAGE_ADDR_TYPE)message16, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
  }
  else if (size == 64){
      INITIALIZE_TIME_VARS("BBoard64");
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id2, (MESSAGE_ADDR_TYPE)message64, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }

      CLEAR_BLACKBOARD(black_id2, &err_code);
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id2, (MESSAGE_ADDR_TYPE)message64, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }

      CLEAR_BLACKBOARD(black_id2, &err_code);
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id2, (MESSAGE_ADDR_TYPE)message64, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }

      CLEAR_BLACKBOARD(black_id2, &err_code);
      INIT_TIME_MEASURE();
      DISPLAY_BLACKBOARD(black_id2, (MESSAGE_ADDR_TYPE)message64, size, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Display64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
  }
  
  else{

  }
  
}

void test_read_black(int size, uint8_t calc_dev)
{
  RETURN_CODE_TYPE   err_code;
  char message16[16];
  char message64[64];
  SYSTEM_TIME_TYPE    time_out = 0;
  MESSAGE_SIZE_TYPE   readSize;
  DECLARE_TIME_MEASURE()

  if (size == 16){
      INITIALIZE_TIME_VARS("ReadBBoard16");

      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id1, time_out, (MESSAGE_ADDR_TYPE)message16, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }

      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id1, time_out,  (MESSAGE_ADDR_TYPE)message16, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id1, time_out,  (MESSAGE_ADDR_TYPE)message16, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id1, time_out, (MESSAGE_ADDR_TYPE) message16, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read16: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
  }
  else if (size == 64){
      INITIALIZE_TIME_VARS("ReadBBoard64");
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id2, time_out,  (MESSAGE_ADDR_TYPE)message64, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id2, time_out,  (MESSAGE_ADDR_TYPE)message64, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id2, time_out,  (MESSAGE_ADDR_TYPE)message64, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
      INIT_TIME_MEASURE();
      READ_BLACKBOARD(black_id2, time_out,  (MESSAGE_ADDR_TYPE)message64, &readSize, &err_code);
      FINISH_TIME_MEASURE();
      if(err_code == NO_ERROR)
          VALIDATE_TIME_MEASURE(calc_dev)
      else{
          PERF_PRINT_STRING("BB Read64: ");
          PERF_PRINT_NUMBER(err_code);
          PERF_PRINT_EOL();
      }
      
  }
  
  else{

  }
}

void test_send_buffer(int size, uint8_t calc_dev)
{
  DECLARE_TIME_MEASURE()
  RETURN_CODE_TYPE errCode   = 0;
  BUFFER_ID_TYPE   buffer2Id;
  SYSTEM_TIME_TYPE timeOut   = 800000000;

  strcpy(((char*)msgToSend16), "0123456789ABCDEF");


  INITIALIZE_TIME_VARS("SendBuffer16");

  GET_BUFFER_ID(buff_name1, &buffer2Id, &errCode);
  
  INIT_TIME_MEASURE();
  SEND_BUFFER(buffer2Id, msgToSend16, size, timeOut, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR)
      VALIDATE_TIME_MEASURE(calc_dev)
  else{
      PERF_PRINT_STRING("Send Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_BUFFER_ID(buff_name2, &buffer2Id, &errCode);
  
  INIT_TIME_MEASURE();
  SEND_BUFFER(buffer2Id, msgToSend16, size, timeOut, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR)
      VALIDATE_TIME_MEASURE(calc_dev)
  else{
      PERF_PRINT_STRING("Send Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
          
  GET_BUFFER_ID(buff_name3, &buffer2Id, &errCode);
  
  INIT_TIME_MEASURE();
  SEND_BUFFER(buffer2Id, msgToSend16, size, timeOut, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR)
      VALIDATE_TIME_MEASURE(calc_dev)
  else{
      PERF_PRINT_STRING("Send Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
          
  GET_BUFFER_ID(buff_name4, &buffer2Id, &errCode);
  
  INIT_TIME_MEASURE();
  SEND_BUFFER(buffer2Id, msgToSend16, size, timeOut, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR)
      VALIDATE_TIME_MEASURE(calc_dev)
  else{
      PERF_PRINT_STRING("Send Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
}

void test_receive_buffer(int size, uint8_t calc_dev)
{
  RETURN_CODE_TYPE   errCode        = 0;
  BUFFER_ID_TYPE     buffer1Id     = 0;
  MESSAGE_SIZE_TYPE  length         = 0;
  SYSTEM_TIME_TYPE   timeOut        = 15000000;
  uint8_t            receivedMsg    = 0;
  DECLARE_TIME_MEASURE()

  if (size == 16){
      INITIALIZE_TIME_VARS("ReceiveBuffer16");
  }

  GET_BUFFER_ID(buff_name1, &buffer1Id, &errCode);
  
  INIT_TIME_MEASURE();
  RECEIVE_BUFFER(buffer1Id, timeOut, &receivedMsg, &length, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Receive Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  timeOut        = 15000000; 
  receivedMsg    = 0;
  length         = 0;
  buffer1Id     = 0; 
  GET_BUFFER_ID(buff_name2, &buffer1Id, &errCode);
  
  INIT_TIME_MEASURE();
  RECEIVE_BUFFER(buffer1Id, timeOut, &receivedMsg, &length, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Receive Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  timeOut        = 15000000; 
  receivedMsg    = 0;
  length         = 0;
  buffer1Id     = 0;
  GET_BUFFER_ID(buff_name3, &buffer1Id, &errCode);
  
  INIT_TIME_MEASURE();
  RECEIVE_BUFFER(buffer1Id, timeOut, &receivedMsg, &length, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Receive Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  timeOut        = 15000000; 
  receivedMsg    = 0;
  length         = 0;
  buffer1Id     = 0;
  GET_BUFFER_ID(buff_name4, &buffer1Id, &errCode);
  
  INIT_TIME_MEASURE();
  RECEIVE_BUFFER(buffer1Id, timeOut, &receivedMsg, &length, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Receive Buffer: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
}

void test_signal_sem(uint8_t calc_dev)
{
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SEMAPHORE_SIGNAL")

  RETURN_CODE_TYPE errCode = TIMED_OUT;
  SEMAPHORE_ID_TYPE SemaphoreId;

  GET_SEMAPHORE_ID(Semaphore1Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  SIGNAL_SEMAPHORE(SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ERROR: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_SEMAPHORE_ID(Semaphore2Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  SIGNAL_SEMAPHORE(SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ERROR: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
}
  

  GET_SEMAPHORE_ID(Semaphore3Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  SIGNAL_SEMAPHORE(SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ERROR: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
}
  
  GET_SEMAPHORE_ID(Semaphore4Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  SIGNAL_SEMAPHORE(SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ERROR: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
}

}

void test_wait_on_sem(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  SEMAPHORE_ID_TYPE SemaphoreId;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SEMAPHORE_WAIT")

  GET_SEMAPHORE_ID(Semaphore1Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  WAIT_SEMAPHORE(SemaphoreId,INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Sem WAIT: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_SEMAPHORE_ID(Semaphore2Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  WAIT_SEMAPHORE(SemaphoreId,INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Sem WAIT: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  

  GET_SEMAPHORE_ID(Semaphore3Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  WAIT_SEMAPHORE(SemaphoreId,INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Sem WAIT: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  

  GET_SEMAPHORE_ID(Semaphore4Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  WAIT_SEMAPHORE(SemaphoreId,INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Sem WAIT: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
     
}
#endif

#ifdef EXTENDED_TESTS
void test_malloc(uint8_t calc_dev)
{
  T_int32 *test1, *test2, *test3, *test4;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("MALLOC")
  INIT_TIME_MEASURE();
  test1 = (T_int32*) malloc(10*sizeof(T_int32));
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
  (void)(test1);

  INIT_TIME_MEASURE();
  test2 = (T_int32*) malloc(50*sizeof(T_int32));
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
  (void)(test2);

  INIT_TIME_MEASURE();
  test3 = (T_int32*) malloc(200*sizeof(T_int32));
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
  (void)(test3);

  INIT_TIME_MEASURE();
  test4 = (T_int32*) malloc(500*sizeof(T_int32));
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
  (void)(test4);
}

void test_set_prio(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode   = 0;
  PROCESS_ID_TYPE process1Id = 0;
  PRIORITY_TYPE   newPrio = 2;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SET_PRIORITY")
  
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  
  INIT_TIME_MEASURE();
  SET_PRIORITY (process1Id, newPrio, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Prio: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  

  INIT_TIME_MEASURE();
  SET_PRIORITY (process1Id, newPrio, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Prio: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  SET_PRIORITY (process1Id, newPrio, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Prio: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  SET_PRIORITY (process1Id, newPrio, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("Prio: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
}

void test_my_id(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode   = 0;
  PROCESS_ID_TYPE myId       = 0;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("MY_ID")
  
  INIT_TIME_MEASURE();
  GET_MY_ID(&myId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("MY ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_MY_ID(&myId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("MY ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_MY_ID(&myId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("MY ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_MY_ID(&myId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("MY ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
}

void test_process_id(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode   = 0;
  PROCESS_ID_TYPE process1Id = 0;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("PROCESS_ID")
  
  INIT_TIME_MEASURE();
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

}

void test_process_status(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode   = 0;
  PROCESS_ID_TYPE process1Id = 0;
  PROCESS_STATUS_TYPE status;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("PROCESS_STATUS")
  
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  INIT_TIME_MEASURE();
  GET_PROCESS_STATUS (process1Id, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PROCESS STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  INIT_TIME_MEASURE();
  GET_PROCESS_STATUS (process1Id, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PROCESS STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  INIT_TIME_MEASURE();
  GET_PROCESS_STATUS (process1Id, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("PROCESS STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_PROCESS_ID(process1Name, &process1Id, &errCode);
  INIT_TIME_MEASURE();
  GET_PROCESS_STATUS (process1Id, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("PROCESS STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
}

void test_sem_id(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  SEMAPHORE_ID_TYPE SemaphoreId;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SEMAPHORE_ID")

  INIT_TIME_MEASURE();
  GET_SEMAPHORE_ID(Semaphore1Name, &SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("SEM ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_ID(Semaphore2Name, &SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("SEM ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_ID(Semaphore3Name, &SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("SEM ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_ID(Semaphore4Name, &SemaphoreId, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("SEM ID: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
}

void test_sem_status(uint8_t calc_dev)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  SEMAPHORE_ID_TYPE SemaphoreId;
  SEMAPHORE_STATUS_TYPE   status;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SEM_STATUS")

  GET_SEMAPHORE_ID(Semaphore1Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_STATUS (SemaphoreId, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("SEM STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_SEMAPHORE_ID(Semaphore2Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_STATUS (SemaphoreId, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
       PERF_PRINT_STRING("SEM STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  GET_SEMAPHORE_ID(Semaphore3Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_STATUS (SemaphoreId, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("SEM STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  GET_SEMAPHORE_ID(Semaphore4Name, &SemaphoreId, &errCode);
  INIT_TIME_MEASURE();
  GET_SEMAPHORE_STATUS (SemaphoreId, &status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
      }
  else{
      PERF_PRINT_STRING("SEM STATUS: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
}

void test_create_event(uint8_t calc_dev)
{
  RETURN_CODE_TYPE     errCode = 0;

  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("CREATE_EVENT")
  
  INIT_TIME_MEASURE();
  CREATE_EVENT(eventName1, &eventID1, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT CREATE1: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_EVENT(eventName2, &eventID2, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT CREATE2: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_EVENT(eventName3, &eventID3, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT CREATE3: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_EVENT(eventName4, &eventID4, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT CREATE4: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
}

void test_set_event(uint8_t calc_dev)
{
  RETURN_CODE_TYPE     errCode = 0;

  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("EVENT_SET")
  
  INIT_TIME_MEASURE();
  SET_EVENT (eventID1, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT SET1: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  SET_EVENT (eventID2, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT SET2: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  SET_EVENT (eventID3, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT SET3: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  SET_EVENT (eventID4, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT SET4: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
}

void test_event_id(uint8_t calc_dev)
{
  EVENT_ID_TYPE event_id1 = 0;
  EVENT_ID_TYPE event_id2 = 0;
  EVENT_ID_TYPE event_id3 = 0;
  EVENT_ID_TYPE event_id4 = 0;
  RETURN_CODE_TYPE     errCode = 0;
  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("EVENT_ID")

  INIT_TIME_MEASURE();
  GET_EVENT_ID (eventName1, &event_id1, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT ID1: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_ID (eventName2, &event_id2, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT ID2: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_ID (eventName3, &event_id3, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT ID3: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_ID (eventName4, &event_id4, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT ID4: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

}

void test_event_status(uint8_t calc_dev)
{
  EVENT_STATUS_TYPE event_status;
  RETURN_CODE_TYPE     errCode = 0;
  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("STATUS_EVENT")

  INIT_TIME_MEASURE();
  GET_EVENT_STATUS (eventID1, &event_status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT STATUS1: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_STATUS (eventID2, &event_status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT STATUS2: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_STATUS (eventID3, &event_status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT STATUS3: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  GET_EVENT_STATUS (eventID4, &event_status, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT STATUS4: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
}

void test_wait_event(uint8_t calc_dev)
{
  RETURN_CODE_TYPE     errCode = 0;
  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("EVENT_WAIT")

  INIT_TIME_MEASURE();
  WAIT_EVENT (eventID1, INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT WAIT1: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  RESET_EVENT (eventID1, &errCode);

  INIT_TIME_MEASURE();
  WAIT_EVENT (eventID2, INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT WAIT2: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  RESET_EVENT (eventID2, &errCode);

  INIT_TIME_MEASURE();
  WAIT_EVENT (eventID3, INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT WAIT3: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  RESET_EVENT (eventID3, &errCode);

  INIT_TIME_MEASURE();
  WAIT_EVENT (eventID4, INFINITE_TIME_VALUE, &errCode);
  FINISH_TIME_MEASURE();
  if(errCode == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("EVENT WAIT4: ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
  }
  
  RESET_EVENT (eventID4, &errCode);
}

void test_serial_print(uint8_t calc_dev)
{
  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("SERIAL_PORT_64")

  INIT_TIME_MEASURE();
  PERF_PRINT_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  PERF_PRINT_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  PERF_PRINT_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  PERF_PRINT_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  PERF_PRINT_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
}

void test_read_memory(uint8_t calc_dev)
{
  T_int32 pos;
  uint8_t read;

  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("READ_MEM")
  
  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      read = BIG_ARRAY[pos];
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      read = BIG_ARRAY[pos];
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      read = BIG_ARRAY[pos];
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      read = BIG_ARRAY[pos];
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  (void)read;
}

void test_write_memory(uint8_t calc_dev)
{
  T_int32 pos;

  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("WRITE_MEM")
  
  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      BIG_ARRAY[pos] = 0;
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      BIG_ARRAY[pos] = 1;
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      BIG_ARRAY[pos] = 2;
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)

  INIT_TIME_MEASURE();
  for (pos = 0; pos < 10000; pos++){
      BIG_ARRAY[pos] = 3;
  }
  FINISH_TIME_MEASURE();
  VALIDATE_TIME_MEASURE(calc_dev)
}


void test_get_ticks(uint8_t calc_dev)
{
  const T_uint64 compare = 0xFFFFFFFFFFFFF;
  T_uint64 ticks = compare;

  DECLARE_TIME_MEASURE() 
  INITIALIZE_TIME_VARS("GET_TICKS")

  INIT_TIME_MEASURE();
  ticks = GET_CURRENT_TICKS();
  FINISH_TIME_MEASURE();
  if(ticks != compare){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("GET_TICK ERROR");
      PERF_PRINT_EOL();
  }
  ticks = compare;

  INIT_TIME_MEASURE();
  ticks = GET_CURRENT_TICKS();
  FINISH_TIME_MEASURE();
  if(ticks != compare){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("GET_TICK ERROR");
      PERF_PRINT_EOL();
  }
  ticks = compare;

  INIT_TIME_MEASURE();
  ticks = GET_CURRENT_TICKS();
  FINISH_TIME_MEASURE();
  if(ticks != compare){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("GET_TICK ERROR");
      PERF_PRINT_EOL();
  }
  ticks = compare;

  INIT_TIME_MEASURE();
  ticks = GET_CURRENT_TICKS();
  FINISH_TIME_MEASURE();
  if(ticks != compare){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("GET_TICK ERROR");
      PERF_PRINT_EOL();
  }
  ticks = compare;        
}

#endif

#ifdef INTERPARTITION_TESTS

void test_create_queuing_ports(uint8_t calc_dev)
{
  char portName[32];
  RETURN_CODE_TYPE rc = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("CREATE_QUEUING_PORT")

  strcpy(portName, "P1");
  INIT_TIME_MEASURE()
  CREATE_QUEUING_PORT(portName, 16, 8, SOURCE, FIFO, &sDestPort1, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_QUEUE_1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P2");
  INIT_TIME_MEASURE()
  CREATE_QUEUING_PORT(portName, 16, 8, SOURCE, FIFO, &sDestPort2, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_QUEUE_2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P3");
  INIT_TIME_MEASURE()
  CREATE_QUEUING_PORT(portName, 16, 8, DESTINATION, FIFO, &sDestPort3, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_QUEUE_3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P4");
  INIT_TIME_MEASURE()
  CREATE_QUEUING_PORT(portName, 16, 8, DESTINATION, FIFO, &sDestPort4, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_QUEUE_4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_queuing_ports_ids(uint8_t calc_dev)
{
  char portName[32];
  RETURN_CODE_TYPE rc = NO_ERROR;
  QUEUING_PORT_ID_TYPE sGetPortID    = 0;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("TEST_QUEUE_ID")

  strcpy(portName, "P1");
  INIT_TIME_MEASURE()        
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_ID1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P2");
  INIT_TIME_MEASURE()        
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_ID2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P3");
  INIT_TIME_MEASURE()        
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_ID3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P4");
  INIT_TIME_MEASURE()        
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_ID4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_queuing_ports_status(uint8_t calc_dev)
{
  QUEUING_PORT_STATUS_TYPE portStatus;
  char portName[32];
  RETURN_CODE_TYPE rc = NO_ERROR;
  QUEUING_PORT_ID_TYPE sGetPortID    = 0;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("TEST_QUEUE_STATUS")

  strcpy(portName, "P1");
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  INIT_TIME_MEASURE()  
  GET_QUEUING_PORT_STATUS(sGetPortID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_STATUS1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P2");
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  INIT_TIME_MEASURE()  
  GET_QUEUING_PORT_STATUS(sGetPortID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_STATUS2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P3");
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  INIT_TIME_MEASURE()  
  GET_QUEUING_PORT_STATUS(sGetPortID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_STATUS3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "P4");
  GET_QUEUING_PORT_ID(portName, &sGetPortID, &rc);
  INIT_TIME_MEASURE()  
  GET_QUEUING_PORT_STATUS(sGetPortID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("QUEUE_STATUS4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_create_sampling_ports(uint8_t calc_dev)
{
  char portName[32];
  RETURN_CODE_TYPE rc = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("CREATE_SAMPLING_PORT")

  strcpy(portName, "SP1");
  INIT_TIME_MEASURE()
  CREATE_SAMPLING_PORT(portName, 16, SOURCE, 200000000, &sSamplingID1, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_SAMPLING_1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP2");
  INIT_TIME_MEASURE()
  CREATE_SAMPLING_PORT(portName, 16, SOURCE, 200000000, &sSamplingID2, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_SAMPLING_2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP3");
  INIT_TIME_MEASURE()
  CREATE_SAMPLING_PORT(portName, 16, DESTINATION, 200000000, &sSamplingID3, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_SAMPLING_3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP4");
  INIT_TIME_MEASURE()
  CREATE_SAMPLING_PORT(portName, 16, DESTINATION, 200000000, &sSamplingID4, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("CREATE_SAMPLING_4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }


}

void test_sampling_ports(uint8_t calc_dev)
{
  char portName[32];
  SAMPLING_PORT_STATUS_TYPE portStatus;
  SAMPLING_PORT_ID_TYPE getSamplingID;
  RETURN_CODE_TYPE rc = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SAMPLING_PORT_ID")

  strcpy(portName, "SP1");
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }


  strcpy(portName, "SP2");
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }


  strcpy(portName, "SP3");
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP4");
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  INITIALIZE_TIME_VARS("SAMPLING_PORT_STATUS")

  strcpy(portName, "SP1");
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_STATUS(getSamplingID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING_ID1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP2");
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_STATUS(getSamplingID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING_ID2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP3");
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_STATUS(getSamplingID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING_ID3: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy(portName, "SP4");
  GET_SAMPLING_PORT_ID(portName, &getSamplingID, &rc);
  INIT_TIME_MEASURE()
  GET_SAMPLING_PORT_STATUS(getSamplingID, &portStatus, &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("TEST_SAMPLING_ID4: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

}

void test_write_queuing_ports(uint8_t calc_dev)
{
  RETURN_CODE_TYPE rc = NO_ERROR;
  uint8_t  txMessageBuf[16];
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("WRITE_QUEUE")

  strcpy((char*)txMessageBuf, "P1_MESSAGE");
  INIT_TIME_MEASURE()
  SEND_QUEUING_MESSAGE(sDestPort1,
                     (MESSAGE_ADDR_TYPE)txMessageBuf,
                     strlen((const char *)txMessageBuf) + 1,
                     0,
                     &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("WRITE_QUEUE1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy((char*)txMessageBuf, "P2_MESSAGE");
  INIT_TIME_MEASURE()
  SEND_QUEUING_MESSAGE(sDestPort2,
                     (MESSAGE_ADDR_TYPE)txMessageBuf,
                     strlen((const char *)txMessageBuf) + 1,
                     0,
                     &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("WRITE_QUEUE2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_read_queuing_ports(uint8_t calc_dev)
{
  RETURN_CODE_TYPE rc = NO_ERROR;
  T_uint32 messageBufSize = 16;
  uint8_t  rxMessageBuf[16];
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("READ_QUEUE")

  strcpy((char*)rxMessageBuf, "GOT IT");
  INIT_TIME_MEASURE()
  RECEIVE_QUEUING_MESSAGE(sDestPort3,
                    0,
                    (MESSAGE_ADDR_TYPE)rxMessageBuf,
                    (MESSAGE_SIZE_TYPE*)&messageBufSize,
                    &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("READ_QUEUE1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy((char*)rxMessageBuf, "GOT IT");
  INIT_TIME_MEASURE()
  RECEIVE_QUEUING_MESSAGE(sDestPort4,
                    0,
                    (MESSAGE_ADDR_TYPE)rxMessageBuf,
                    (MESSAGE_SIZE_TYPE*)&messageBufSize,
                    &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("READ_QUEUE2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_write_sampling_ports(uint8_t calc_dev)
{
  uint8_t txSamplingMessageBuf[16];
  RETURN_CODE_TYPE rc = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("WRITE_SAMPLING")

  strcpy((char*)txSamplingMessageBuf, "P1_SAMPLING");
  INIT_TIME_MEASURE()
  WRITE_SAMPLING_MESSAGE(sSamplingID1,
      (MESSAGE_ADDR_TYPE)txSamplingMessageBuf,
      strlen((const char *)txSamplingMessageBuf) + 1,
      &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("P1_SAMPLING1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  strcpy((char*)txSamplingMessageBuf, "P2_SAMPLING");
  INIT_TIME_MEASURE()
  WRITE_SAMPLING_MESSAGE(sSamplingID2,
      (MESSAGE_ADDR_TYPE)txSamplingMessageBuf,
      strlen((const char *)txSamplingMessageBuf) + 1,
      &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("P1_SAMPLING2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}

void test_read_sampling_ports(uint8_t calc_dev)
{
  RETURN_CODE_TYPE rc = NO_ERROR;
  uint8_t rxSamplingMessageBuf[32];
  T_uint32 validity = 0;
  T_uint32 messageBufSize = 32;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("READ_SAMPLING")

  INIT_TIME_MEASURE()
  READ_SAMPLING_MESSAGE(sSamplingID3,
      (MESSAGE_ADDR_TYPE)rxSamplingMessageBuf,
      (MESSAGE_SIZE_TYPE*)&messageBufSize,
      (VALIDITY_TYPE*)&validity,
      &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("READ_SAMPLING1: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE()
  READ_SAMPLING_MESSAGE(sSamplingID4,
      (MESSAGE_ADDR_TYPE)rxSamplingMessageBuf,
      (MESSAGE_SIZE_TYPE*)&messageBufSize,
      (VALIDITY_TYPE*)&validity,
      &rc);
  FINISH_TIME_MEASURE()
  if(rc == NO_ERROR){
      VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
      PERF_PRINT_STRING("READ_SAMPLING2: ");
      PERF_PRINT_NUMBER(rc);
      PERF_PRINT_EOL();
  }
}
#endif
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
