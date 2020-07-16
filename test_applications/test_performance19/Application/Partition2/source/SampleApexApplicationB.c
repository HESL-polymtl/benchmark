/* __________________________________________________________________________
* DEVELOPMENT HISTORY:
* --------------------
*
* $Author:: $: Felipe Magalhaes
* $Rev:: $: Revision of last commit
* $Date:: $: 07/16/2020
* $Comments (Refer to applicable SCR/PR) $: Initial version of the file.
* __________________________________________________________________________
*/
/* __________________________________________________________________________
* MODULE DESCRIPTION:
* -------------------
* Filename : SampleApexApplicationB.c
* Original Author : Felipe Magalhaes
* Description: This file contains partition's 2 application.
* __________________________________________________________________________
*/

/* INCLUDE SECTION */
#include <string.h>
#include "performance_lib.h"

/* Test specific includes */
#include "TestParametersP2.h"

/* __________________________________________________________________________
 * PUBLIC DATA SECTION:
 * ---------------------
 */

/* IMPORTED GLOBAL INPUT VARIABLES */
extern SAMPLING_PORT_ID_TYPE SamplingTargetSrcId;
extern SAMPLING_PORT_ID_TYPE SamplingInterCRCDstId;
extern SEMAPHORE_ID_TYPE SemaMutexId;
extern SEMAPHORE_ID_TYPE SemaEmptyId;
extern SEMAPHORE_ID_TYPE SemaFullId;

static int32_t computeGreatestCommonDivider (int32_t a, int32_t b);
static void simulateWriteToDisk (float32_t data_in, float32_t * data_out);
static uint32_t countProcLog=0;
static uint32_t countProcCrc=0;
static uint32_t countProcMatrix=0;

/* LOCAL DATA VARIABLES */
uint32_t logBuffer[MAX_LOGBUFFER_SIZE];
uint64_t matrixA[MAX_MAXTRIX_ROW][MAX_MAXTRIX_COL];
uint64_t matrixB[MAX_MAXTRIX_ROW][MAX_MAXTRIX_COL];
uint64_t tenMilliSecinTicks;
uint64_t countWriteToBuffer = 0;

/* __________________________________________________________________________
 *
 * FUNCTION    : computeGreatestCommonDivider
 *
 * DESCRIPTION : This function calculates the CGC of two integer values using
 *               Recursive Euclid algorithm
 *
 * PARAMETERS  :
 *   INPUTS      : a: First integer value
 *                 b: Second  integer value
 *   OUTPUTS     : None.
 * RETURN      : The CGC of two input integer values.
 * __________________________________________________________________________
 */
static int32_t computeGreatestCommonDivider (int32_t a, int32_t b)
{
  return (b != 0) ? computeGreatestCommonDivider(b, a % b) : a;
}

/* __________________________________________________________________________
 *
 * FUNCTION    : simulateWriteToDisk
 *
 * DESCRIPTION : This function simulates a write to disk operation: it must be
 *               CPU intensive it requires around 10 ms.
 *
 * PARAMETERS  :
 *   INPUTS      : data_in: data input
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
static void simulateWriteToDisk (float32_t data_in, float32_t * data_out)
{
  float32_t val = data_in;
  volatile uint64_t i = 0;
  uint32_t count=0;

  for (count = 0; count < WRITE_DISK_ITER; count++)
    {
      if (i++ & 0x1)
        {
          val += (float32_t) 1.0;
        }
      else
        {
          val -= (float32_t) 1.0;
        }
    }

  *data_out = val;
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitFibbonacciSequenceMatrix
 *
 * DESCRIPTION : This function populates the Fibbonacci sequence matrix.
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitFibbonacciSequenceMatrix (void)
{
  uint64_t current = 0, next = 1;
  uint32_t i = 0, j = 0;

  matrixA[i][0] = current;
  matrixA[i][1] = next;
  for (i = 0; i < MAX_MAXTRIX_ROW; i++)
    {
      if (0 == i)
        {
          j = 2;
        }
      else
        {
          j = 0;
        }
      for (; j < MAX_MAXTRIX_COL; j++)
        {
          matrixA[i][j] = current + next;
          current = next;
          next = matrixA[i][j];
        }
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitSquareSequenceMatrix
 *
 * DESCRIPTION : This functions populates the square number sequence matrix.
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitSquareSequenceMatrix (void)
{
  uint64_t k = 0;
  uint32_t i = 0, j = 0;

  for (i = 0; i < MAX_MAXTRIX_ROW; i++)
    {
      for (j = 0; j < MAX_MAXTRIX_COL; j++)
        {
          matrixB[i][j] = k * k;
          k++;
        }
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitTimeReferencesP2
 *
 * DESCRIPTION : This functions initializes the time references required by
 *               Application B
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitTimeReferencesP2 (void)
{
  tenMilliSecinTicks = perf_ns_to_ticks(10000000);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : StartPerformanceMeasurementsP2
 *
 * DESCRIPTION : This functions ...
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void StartPerformanceMeasurementsP2 (void)
{
  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("SAMPLE_APEX_APP_B");
  INIT_TIME_MEASURE();
}

/* __________________________________________________________________________
 *
 * FUNCTION    : ProcessLogger
 *
 * DESCRIPTION : The logger process consumes data from the semaphore
 *               protected log buffer and write it to disk. This process has
 *               the highest priority and will be executed each time there
 *               is new data in the log buffer. It also update the target
 *               value used by Application A inner loop process.
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void ProcessLogger (void)
{
  SEMAPHORE_STATUS_TYPE semaFullStatus;
  uint32_t count;
  uint32_t txTargetMsg;
  float32_t ret;
  SYSTEM_TIME_TYPE timeOut = INFINITE_TIME_VALUE;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  while (1)
    {
      countProcLog++;
      WAIT_SEMAPHORE(SemaFullId, timeOut, &errCode);
      WAIT_SEMAPHORE(SemaMutexId, timeOut, &errCode);

      GET_SEMAPHORE_STATUS(SemaFullId, &semaFullStatus, &errCode);
      count = (uint32_t)semaFullStatus.CURRENT_VALUE;
      txTargetMsg = logBuffer[count];

      SIGNAL_SEMAPHORE(SemaMutexId, &errCode);
      SIGNAL_SEMAPHORE(SemaEmptyId, &errCode);

      simulateWriteToDisk((float32_t)txTargetMsg,&ret);

      WRITE_SAMPLING_MESSAGE(SamplingTargetSrcId,
                             (MESSAGE_ADDR_TYPE)&ret,
                             sizeof(ret), &errCode);
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION    : ProcessCRCMonitor
 *
 * DESCRIPTION : This process computes the greatest common divider between a
 *               CRC data from Application A and a specific GDC value.
 *               The GDC result will then be pushed into the logger buffer.
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void ProcessCRCMonitor (void)
{
  int32_t gcd;
  uint32_t count;
  uint32_t rxCrcDataMsg;
  MESSAGE_SIZE_TYPE rxCrcDataMsgSize = 0;
  SEMAPHORE_STATUS_TYPE semaFullStatus;
  SYSTEM_TIME_TYPE timeOut = INFINITE_TIME_VALUE;
  VALIDITY_TYPE isMsgValid = INVALID;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  while (1)
    {
      countProcCrc++;
      READ_SAMPLING_MESSAGE(SamplingInterCRCDstId,
                            (MESSAGE_ADDR_TYPE) &rxCrcDataMsg,
                            &rxCrcDataMsgSize, &isMsgValid, &errCode);

      if (errCode == NO_ERROR)
        {
          RETURN_CODE_TYPE semaRC = NO_ERROR;

          gcd = computeGreatestCommonDivider((int32_t) rxCrcDataMsg,
                                             (int32_t) GCD_VALUE);

          WAIT_SEMAPHORE(SemaEmptyId, timeOut, &semaRC);
          WAIT_SEMAPHORE(SemaMutexId, timeOut, &semaRC);

          GET_SEMAPHORE_STATUS(SemaFullId, &semaFullStatus, &semaRC);
          count = (uint32_t) semaFullStatus.CURRENT_VALUE;
          logBuffer[count] = (uint32_t) gcd;
          countWriteToBuffer++;

          SIGNAL_SEMAPHORE(SemaMutexId, &semaRC);
          SIGNAL_SEMAPHORE(SemaFullId, &semaRC);
        }

      PERIODIC_WAIT(&errCode);
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION    : ProcessMatrixMultiplier
 *
 * DESCRIPTION : This process executes if the logging operation is not in
 *               progress and no CRC data from application is processed. It
 *               multiplies 2 matrices one row at a time and  push the sum in
 *               the log buffer unless it is full.
 *
 * PARAMETERS  :
 *   INPUTS      : None
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void ProcessMatrixMultiplier(void)
{
  SEMAPHORE_STATUS_TYPE semaFullStatus;
  SYSTEM_TIME_TYPE timeOut = INFINITE_TIME_VALUE;
  uint64_t sum = 0, result = 0;
  uint32_t i = 0, j = 0, count = 0;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  while (1)
    {
      countProcMatrix++;
      sum = 0;
      for (j = 0; j < MAX_MAXTRIX_COL; j++)
        {
          result = matrixA[i][j] * matrixB[i][j];
          sum += result;
        }

      WAIT_SEMAPHORE(SemaEmptyId, timeOut, &errCode);
      WAIT_SEMAPHORE(SemaMutexId, timeOut, &errCode);

      GET_SEMAPHORE_STATUS(SemaFullId, &semaFullStatus, &errCode);
      count = (uint32_t)semaFullStatus.CURRENT_VALUE;
      logBuffer[count] = (uint32_t)sum;

      SIGNAL_SEMAPHORE(SemaMutexId, &errCode);
      SIGNAL_SEMAPHORE(SemaFullId, &errCode);

      if (++i >= MAX_MAXTRIX_ROW)
        {
          /* Stop time measurement */
          DECLARE_TIME_MEASURE();
          INITIALIZE_TIME_VARS("SAMPLE_APEX_APP_B");
          FINISH_TIME_MEASURE();
          VALIDATE_TIME_MEASURE(0);
          PRINT_PERFORMANCE_INFO();

          PERF_PRINT_STRING("Proc. Logger count=");
          PERF_PRINT_UNSIGNED(countProcLog);
          PERF_PRINT_EOL();
          PERF_PRINT_STRING("Proc. CRCMonitor count=");
          PERF_PRINT_UNSIGNED(countProcCrc);
          PERF_PRINT_EOL();
          PERF_PRINT_STRING("Proc. MatrixMultiplier count=");
          PERF_PRINT_UNSIGNED(countProcMatrix);
          PERF_PRINT_EOL();
          break;
        }
    }

  STOP_SELF();
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
