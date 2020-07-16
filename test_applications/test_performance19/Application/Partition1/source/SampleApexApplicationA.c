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
* Filename : SampleApexApplicationA.c
* Original Author : Felipe Magalhaes
* Description: This file contains all the logic of partition 1 application.
* __________________________________________________________________________
*/


/* INCLUDE SECTION */
#include "performance_lib.h"

/* Test specific includes */
#include "TestParameterP1.h"
#include "CalCRC32.h"

/* __________________________________________________________________________
 * PUBLIC DATA SECTION:
 * ---------------------
 */

/* IMPORTED GLOBAL INPUT VARIABLES */
extern SAMPLING_PORT_ID_TYPE SamplingTargetDstId;
extern SAMPLING_PORT_ID_TYPE SamplingCommandSrcId;
extern SAMPLING_PORT_ID_TYPE SamplingInterCRCSrcId;

/* EXPORTED GLOBAL OUTPUT VARIABLES */
/* None. */

/* __________________________________________________________________________
 * PRIVATE DATA SECTION:
 * ---------------------
 */

/* __________________________________________________________________________
 * PRIVATE SUBROUTINES PROTOTYPES:
 * -------------------------------
 */
static void mathComputation (float64_t target, float64_t * command);

/* LOCAL DATA VARIABLES */
uint32_t DataPool[DATA_POOL_ROW_SIZE][DATA_POOL_COL_SIZE];
uint64_t twoMilliSecinTicks;
static uint32_t countProcInner=0;
static uint32_t countProcMonCrc=0;

/* __________________________________________________________________________
 *
 * FUNCTION    : mathComputation
 *
 * DESCRIPTION : This functions computes an actuator command based on a target value, which shall:
 *               - Not use any CRuntime function (TDB, or we end up comparing CRuntime performance)
 *               - Simply perform CPU intensive calculations, and be adjusted to execute for around 2 ms
 *
 * PARAMETERS  :
 *   INPUTS      : const float64_t target : the target value
 *   OUTPUTS     : float64_t * command : the calculated command
 * RETURN      : None.
 * __________________________________________________________________________
 */
static void mathComputation (float64_t target, float64_t * command)
{
  float64_t val = target;
  volatile uint64_t i = 0;
  uint32_t count=0;

  for (count = 0; count < MATH_COMPUTATION_ITER; count++)
    {
      if (i++ & 0x1)
        {
          val *= (float64_t) 1.0;
        }
      else
        {
          val /= (float64_t) 1.0;
        }
    }

  *command = val;
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitTimeReferencesP1
 *
 * DESCRIPTION : This functions initializes the time references required by
 *               Application A
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitTimeReferencesP1 (void)
{
  twoMilliSecinTicks = perf_ns_to_ticks(2000000);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : StartPerformanceMeasurementsP1
 *
 * DESCRIPTION : This functions ...
 *
 * PARAMETERS  :
 *   INPUTS      : None.
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void StartPerformanceMeasurementsP1 (void)
{
  DECLARE_TIME_MEASURE();
  INITIALIZE_TIME_VARS("SAMPLE_APEX_APP_A");
  INIT_TIME_MEASURE();
}

/* __________________________________________________________________________
 *
 * FUNCTION    : ProcessMonitoring
 *
 * DESCRIPTION : This functions will execute when the InnerLoop process is not scheduled. 
 *               It will determine the CRC32 value of the a static data pool buffer filled 
 *               during the Partition initialization process. Every CRC_CHUNK bytes, 
 *               the Monitoring process will push the current CRC value to a queuing port.
 *
 * PARAMETERS  :
 *   INPUTS      : None
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void ProcessMonitoring(void)
{
  uint32_t crcResultBytes;
  RETURN_CODE_TYPE errCode = NO_ERROR;
  uint32_t i = 0, j = 0;

  while (1)
    {
      countProcMonCrc++;
      /* read chunk from data pool and generate its 32-bit CRC */
      crcResultBytes = CalcCRC32((uint8_t*) DataPool[i], (uint32_t)DATA_POOL_COL_SIZE);

      WRITE_SAMPLING_MESSAGE(SamplingInterCRCSrcId,
                           (MESSAGE_ADDR_TYPE)&crcResultBytes,
                           sizeof(crcResultBytes), &errCode);

      if (++i >= DATA_POOL_ROW_SIZE)
        {
          if (j > 40)
            {
              /* Stop time measurement */
              DECLARE_TIME_MEASURE();
              INITIALIZE_TIME_VARS("SAMPLE_APEX_APP_A");
              FINISH_TIME_MEASURE();
              VALIDATE_TIME_MEASURE(0);
              PRINT_PERFORMANCE_INFO();

              PERF_PRINT_STRING("Proc. Inner Loop count=");
              PERF_PRINT_UNSIGNED(countProcInner);
              PERF_PRINT_EOL();
              PERF_PRINT_STRING("Proc. Monitoring count=");
              PERF_PRINT_UNSIGNED(countProcMonCrc);
              PERF_PRINT_EOL();
              break;

            }
          else
            {
              j++;
              i = 0;
            }
        }
    }

  STOP_SELF();
}

/* __________________________________________________________________________
 *
 * FUNCTION    : ProcessInnerLoop
 *
 * DESCRIPTION : This functions computes an actuator command based on a target value determine by Application B. 
 *               The command value is sent on an APEX sampling port, which would be sent to the Remote Data Concentrator
 *               responsible for controlling the actuator.
 *
 * PARAMETERS  :
 *   INPUTS      : None
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void ProcessInnerLoop(void)
{
  float64_t rxTargetMsg=0;
  float64_t txCommandMsg;
  MESSAGE_SIZE_TYPE rxTargetMsgSize = 0;
  VALIDITY_TYPE isMsgValid = INVALID;
  RETURN_CODE_TYPE errCode = NO_ERROR;
  uint64_t urxTargetMsg;

  while (1)
    {
      countProcInner++;
      rxTargetMsg = 0;
      READ_SAMPLING_MESSAGE(SamplingTargetDstId,
                            (MESSAGE_ADDR_TYPE) &urxTargetMsg, &rxTargetMsgSize,
                            &isMsgValid, &errCode);
      mathComputation((float64_t) rxTargetMsg, &txCommandMsg);
      WRITE_SAMPLING_MESSAGE(SamplingCommandSrcId,
                             (MESSAGE_ADDR_TYPE) &txCommandMsg,
                             sizeof(txCommandMsg), &errCode);

      PERIODIC_WAIT(&errCode);
    }
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
