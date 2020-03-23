
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


MUTEX_ID_TYPE mut_id1;
MUTEX_ID_TYPE mut_id2;
MUTEX_ID_TYPE mut_id3;
MUTEX_ID_TYPE mut_id4;
MUTEX_NAME_TYPE Mutex1Name = "Mutex1";
MUTEX_NAME_TYPE Mutex2Name = "Mutex2";
MUTEX_NAME_TYPE Mutex3Name = "Mutex3";
MUTEX_NAME_TYPE Mutex4Name = "Mutex4";
PRIORITY_TYPE priorityValue = MAX_PRIORITY_VALUE;
QUEUING_DISCIPLINE_TYPE     mut_queue;

void test_create_sem(T_uint8 calc_dev);
void test_signal_sem(T_uint8 calc_dev);
void test_wait_on_sem(T_uint8 calc_dev);
void test_sem_id(T_uint8 calc_dev);
void test_sem_status(T_uint8 calc_dev); 


void test_create_mut(T_uint8 calc_dev);
void test_acquire_mut(T_uint8 id, T_uint8 calc_dev);
void test_release_mut(T_uint8 id, T_uint8 calc_dev);
void test_id_mut(T_uint8 calc_dev);
void test_status_mut(T_uint8 calc_dev); 
void test_reset_mut(T_uint8 calc_dev); 

void printResults(void);

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
  PROCESS_ID_TYPE        process_id;
  PROCESS_ATTRIBUTE_TYPE process_att;
  RETURN_CODE_TYPE       ret_type;

  PARTITION_STATUS_TYPE partitionStatus;
  GET_PARTITION_STATUS(&partitionStatus, &ret_type);

  PERF_PRINT_STRING("Init P1 partition\n\r");

  /* Set processes  */
  process_att.ENTRY_POINT   = printResults;
  process_att.DEADLINE      = SOFT;
  process_att.PERIOD        = partitionStatus.PERIOD;
  process_att.STACK_SIZE    = 16384;
  process_att.TIME_CAPACITY = partitionStatus.PERIOD;
  process_att.BASE_PRIORITY = MIN_PRIORITY_VALUE;
  strncpy(process_att.NAME, "PRINT", MAX_NAME_LENGTH);
 
  CREATE_PROCESS(&process_att, &process_id, &ret_type);

  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot create PRINT process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }

  PERF_PRINT_STRING("Running APEX analysis\n\r");
  test_create_sem(1);
  test_create_mut(1);

  START(process_id, &ret_type);
  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot start PRINT process  "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }

  PERF_PRINT_STRING("P1 partition switching to normal mode\n\r");
  SET_PARTITION_MODE(NORMAL, &ret_type);

  if(ret_type != NO_ERROR)
  {
      PERF_PRINT_STRING("Cannot switch P1 partition to NORMAL state "); 
      PERF_PRINT_NUMBER(ret_type);
      PERF_PRINT_EOL();
  }

  while(1);
}

void printResults(void)
{
  RETURN_CODE_TYPE retCode = NO_ERROR;

  while(TRUE){
    test_signal_sem(1);
    test_wait_on_sem(1);
    test_sem_id(1);
    test_sem_status(1); 
    test_acquire_mut(0,1);
    test_release_mut(0,1);
    test_acquire_mut(1,1);
    test_release_mut(1,1);
    test_acquire_mut(2,1);
    test_release_mut(2,1);
    test_acquire_mut(3,1);
    test_release_mut(3,1);
    test_id_mut(1);
    test_status_mut(1); 
    test_reset_mut(1); 

    PRINT_PERFORMANCE_INFO();
    PERIODIC_WAIT(&retCode);
  }
}

/*SEMPAHORES*/
 void test_create_sem(T_uint8 calc_dev)
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

void test_signal_sem(T_uint8 calc_dev)
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

void test_wait_on_sem(T_uint8 calc_dev)
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

void test_sem_id(T_uint8 calc_dev)
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

void test_sem_status(T_uint8 calc_dev)
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

/*MUTEX*/
void test_create_mut(T_uint8 calc_dev)
  {
  RETURN_CODE_TYPE   err_code;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("MUTEX_CREATE");

  mut_queue = FIFO;

  INIT_TIME_MEASURE();
  CREATE_MUTEX (Mutex1Name, priorityValue, mut_queue, &mut_id1, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
    VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
    PERF_PRINT_STRING("CREATE MUTEX: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  CREATE_MUTEX (Mutex2Name, priorityValue, mut_queue, &mut_id2, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
    VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
    PERF_PRINT_STRING("CREATE MUTEX: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  } 

  INIT_TIME_MEASURE();
  CREATE_MUTEX (Mutex3Name, priorityValue, mut_queue, &mut_id3, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
    VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
    PERF_PRINT_STRING("CREATE MUTEX: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  }

  INIT_TIME_MEASURE();
  CREATE_MUTEX (Mutex4Name, priorityValue, mut_queue, &mut_id4, &err_code);
  FINISH_TIME_MEASURE();
  if(err_code == NO_ERROR){
    VALIDATE_TIME_MEASURE(calc_dev)
  }
  else{
    PERF_PRINT_STRING("CREATE MUTEX: ");
    PERF_PRINT_NUMBER(err_code);
    PERF_PRINT_EOL();
  } 
}

void test_acquire_mut(T_uint8 id, T_uint8 calc_dev)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  MUTEX_ID_TYPE MutexId;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("MUTEX_ACQUIRE")

  if (id == 0){
    GET_MUTEX_ID(Mutex1Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("ACQUIRE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }
  else if (id == 1){   
    GET_MUTEX_ID(Mutex2Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("ACQUIRE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }

  else if (id == 2){
    GET_MUTEX_ID(Mutex3Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("ACQUIRE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }

  else{
    GET_MUTEX_ID(Mutex4Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("ACQUIRE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }
}

void test_release_mut(T_uint8 id, T_uint8 calc_dev)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  MUTEX_ID_TYPE MutexId;

  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("MUTEX_RELEASE")

  if (id == 0){
    GET_MUTEX_ID(Mutex1Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    RELEASE_MUTEX(MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RELEASE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }
  else if (id == 1){   
    GET_MUTEX_ID(Mutex2Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    RELEASE_MUTEX(MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RELEASE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }

  else if (id == 2){
    GET_MUTEX_ID(Mutex3Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    RELEASE_MUTEX(MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RELEASE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }
  else{
    GET_MUTEX_ID(Mutex4Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    RELEASE_MUTEX(MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RELEASE_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  }
}


void test_id_mut(T_uint8 calc_dev)
{
    RETURN_CODE_TYPE errCode = TIMED_OUT;
    MUTEX_ID_TYPE MutexId;

    DECLARE_TIME_MEASURE()
    INITIALIZE_TIME_VARS("MUTEX_ID")
    
    INIT_TIME_MEASURE();
    GET_MUTEX_ID(Mutex1Name, &MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_ID: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }

    INIT_TIME_MEASURE();
    GET_MUTEX_ID(Mutex2Name, &MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_ID: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }

    INIT_TIME_MEASURE();
    GET_MUTEX_ID(Mutex3Name, &MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_ID: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }

    INIT_TIME_MEASURE();
    GET_MUTEX_ID(Mutex4Name, &MutexId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_ID: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
}

void test_status_mut(T_uint8 calc_dev)
{
    RETURN_CODE_TYPE errCode = TIMED_OUT;
    MUTEX_ID_TYPE MutexId;
    MUTEX_STATUS_TYPE status;

    DECLARE_TIME_MEASURE()
    INITIALIZE_TIME_VARS("MUTEX_STATUS")

    GET_MUTEX_ID(Mutex1Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    GET_MUTEX_STATUS(MutexId, &status, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_STATUS: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
    
    GET_MUTEX_ID(Mutex2Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    GET_MUTEX_STATUS(MutexId, &status, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_STATUS: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }

    GET_MUTEX_ID(Mutex3Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    GET_MUTEX_STATUS(MutexId, &status,  &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_STATUS: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }

    GET_MUTEX_ID(Mutex4Name, &MutexId, &errCode);
    INIT_TIME_MEASURE();
    GET_MUTEX_STATUS(MutexId, &status, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("GET_MUTEX_STATUS: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
}

void test_reset_mut(T_uint8 calc_dev)
{
    RETURN_CODE_TYPE errCode = TIMED_OUT;
    MUTEX_ID_TYPE MutexId;
    PROCESS_ID_TYPE processId;

    DECLARE_TIME_MEASURE()
    INITIALIZE_TIME_VARS("RESET_MUTEX")

    GET_PROCESS_ID("PRINT", &processId, &errCode);
    GET_MUTEX_ID(Mutex1Name, &MutexId, &errCode);
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    INIT_TIME_MEASURE();
    RESET_MUTEX(MutexId, processId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
       VALIDATE_TIME_MEASURE(calc_dev)
       }
    else{
       PERF_PRINT_STRING("RESET_MUTEX: ");
       PERF_PRINT_NUMBER(errCode);
       PERF_PRINT_EOL();
    }

    GET_MUTEX_ID(Mutex2Name, &MutexId, &errCode);
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    INIT_TIME_MEASURE();
    RESET_MUTEX(MutexId, processId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RESET_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
  


    GET_MUTEX_ID(Mutex3Name, &MutexId, &errCode);
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    INIT_TIME_MEASURE();
    RESET_MUTEX(MutexId, processId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RESET_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
    

    GET_MUTEX_ID(Mutex4Name, &MutexId, &errCode);
    ACQUIRE_MUTEX(MutexId,INFINITE_TIME_VALUE, &errCode);
    INIT_TIME_MEASURE();
    RESET_MUTEX(MutexId, processId, &errCode);
    FINISH_TIME_MEASURE();
    if(errCode == NO_ERROR){
        VALIDATE_TIME_MEASURE(calc_dev)
        }
    else{
        PERF_PRINT_STRING("RESET_MUTEX: ");
        PERF_PRINT_NUMBER(errCode);
        PERF_PRINT_EOL();
    }
       
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
