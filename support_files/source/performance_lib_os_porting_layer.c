/* __________________________________________________________________________
* DEVELOPMENT HISTORY:
* --------------------
*
* $Author:: $: Felipe Magalhaes
* $Rev:: $: Revision of last commit
* $Date:: $: 13/12/2018
* $Comments (Refer to applicable SCR/PR) $: Initial version of the file.
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* MODULE DESCRIPTION:
* -------------------
* Filename : performance_lib_os_porting_layer.c
* Original Author : Felipe Magalhaes
* Comments: This file contains the OS abstraction layer. In this file,
* JetOS ARINC-653 RTOS is used and target. If a different RTOS is aimed,
* this file should be updated accordingly.
*
* __________________________________________________________________________
*/

#include "performance_lib.h"
#include "performance_lib_mapping.h"

SEMAPHORE_NAME_TYPE SemaphoreName = "Semaphore";
const char* semValue = "0";

MUTEX_NAME_TYPE MutexName = "Mutex";
const char* mutexValue = "0";   

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_initialize_test
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_initialize_test(perf_task_entry_t init_test_fct)
{
  init_test_fct(NULL);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_create_task
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
perf_task_handle_t perf_create_task(perf_task_entry_t task_entry,
                                    char_t task_name[4],
                                    uint32_t prio)
{
  perf_task_handle_t thandle;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  PROCESS_ATTRIBUTE_TYPE processAttributes;
  processAttributes.BASE_PRIORITY = prio;
  processAttributes.DEADLINE = SOFT;
  processAttributes.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)task_entry;
  strncpy(processAttributes.NAME, task_name, MAX_NAME_LENGTH);
  processAttributes.PERIOD = INFINITE_TIME_VALUE;
  processAttributes.STACK_SIZE = 4096;
  processAttributes.TIME_CAPACITY = INFINITE_TIME_VALUE;
  CREATE_PROCESS(&processAttributes, &thandle, &errCode);

  if(errCode != NO_ERROR)
  {
    PERF_PRINT_STRING("Just had an error creating the task:  ");
    PERF_PRINT_NUMBER(errCode);
    PERF_PRINT_EOL();
  }

  else
  {
    START(thandle, &errCode);
    if(errCode != NO_ERROR)
    {
      PERF_PRINT_STRING("Just had an error starting the task:  ");
      PERF_PRINT_NUMBER(errCode);
      PERF_PRINT_EOL();
    }
  }

  return thandle;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_task_yield
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_task_yield()
{
  RETURN_CODE_TYPE errCode = NO_ERROR;
  TIMED_WAIT(0, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_task_suspend
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_task_suspend(perf_task_handle_t task)
{
  RETURN_CODE_TYPE errCode = NO_ERROR;
  SUSPEND(task, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_task_suspend_self
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_task_suspend_self()
{
  RETURN_CODE_TYPE errCode = NO_ERROR;
  SUSPEND_SELF(0, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_task_resume
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_task_resume(perf_task_handle_t task)
{
  RETURN_CODE_TYPE errCode = NO_ERROR;
  RESUME(task, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_task_delay
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_task_delay(uint32_t milliseconds)
{
  RETURN_CODE_TYPE errCode = NO_ERROR;
  uint32_t nano = milliseconds * 1000000;
  TIMED_WAIT(nano, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_time_diff
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
perf_time_t perf_time_diff(const perf_time_t* t1, const perf_time_t* t2)
{
  perf_time_t diff = ((perf_time_t)*t2 - (perf_time_t)*t1);
  return diff;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_sem_create
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_sem_create(perf_sem_t* sem, int current_value)
{
  RETURN_CODE_TYPE   err_code;

  strcat(SemaphoreName, semValue);
  CREATE_SEMAPHORE (SemaphoreName, current_value, 50, (QUEUING_DISCIPLINE_TYPE) FIFO, sem, &err_code);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_sem_wait
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_sem_wait(perf_sem_t* sem)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  WAIT_SEMAPHORE(*sem, INFINITE_TIME_VALUE, &errCode);
  if(errCode != NO_ERROR)
  {
    perf_serial_write("SEM T ER");
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_sem_signal
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_sem_signal(perf_sem_t* sem)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  SIGNAL_SEMAPHORE(*sem, &errCode);
  if(errCode != NO_ERROR)
  {
    perf_serial_write("SEM G ER");
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_mutex_create
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_mutex_create(perf_mutex_t* mutex)
{
  RETURN_CODE_TYPE   err_code;

  strcat(MutexName, mutexValue);
  CREATE_MUTEX (MutexName, 10, (QUEUING_DISCIPLINE_TYPE) FIFO, mutex, &err_code);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_mutex_acquire
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_mutex_acquire(perf_mutex_t* mutex)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  ACQUIRE_MUTEX(*mutex, INFINITE_TIME_VALUE, &errCode);
  if(errCode != NO_ERROR)
  {
    perf_serial_write("Mutex acquire fail");
  }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_mutex_release
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_mutex_release(perf_mutex_t* mutex)
{
  RETURN_CODE_TYPE errCode = TIMED_OUT;
  RELEASE_MUTEX(*mutex, &errCode);
  if(errCode != NO_ERROR)
  {
    perf_serial_write("Mutex release fail");
  }
}
/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_serial_write
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_serial_write(const char* string)
{
  PERF_PRINT_STRING(string);
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : perf_cycle_reset_counter
 * DESCRIPTION   : TODO
 *
 * PARAMETERS : -in/out TODO
 *              -in/out TODO
 * RETURN : None.
 * __________________________________________________________________________
 */
void perf_cycle_reset_counter()
{
  // Do Nothing
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
