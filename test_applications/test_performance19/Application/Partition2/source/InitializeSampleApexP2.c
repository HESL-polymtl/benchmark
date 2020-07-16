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
* Filename : InitializeSampleApexP2.c
* Original Author : Felipe Magalhaes
* Description: This file contains initialization functions.
* __________________________________________________________________________
*/

/* INCLUDE SECTION */
#include <string.h>

/* Test specific includes */
#include "TestParametersP2.h"

/* __________________________________________________________________________
 * PUBLIC DATA SECTION:
 * ---------------------
 */

/* IMPORTED GLOBAL INPUT VARIABLES */
extern void ProcessLogger(void);
extern void ProcessCRCMonitor(void);
extern void ProcessMatrixMultiplier(void);

/* EXPORTED GLOBAL OUTPUT VARIABLES */
PROCESS_ID_TYPE ProcessLoggerId = 0;
PROCESS_ID_TYPE ProcessCRCMonitorId = 0;
PROCESS_ID_TYPE ProcessMatrixMultiplierId = 0;

SAMPLING_PORT_ID_TYPE SamplingTargetSrcId = 0;
SAMPLING_PORT_ID_TYPE SamplingInitSyncADstId = 0;
SAMPLING_PORT_ID_TYPE SamplingInitSyncBSrcId = 0;

SAMPLING_PORT_ID_TYPE SamplingInterCRCDstId = 0;

SEMAPHORE_ID_TYPE SemaMutexId = 0;
SEMAPHORE_ID_TYPE SemaEmptyId = 0;
SEMAPHORE_ID_TYPE SemaFullId = 0;


/* __________________________________________________________________________
 *
 * FUNCTION    : InitializeSampleApexP2Resources
 *
 * DESCRIPTION : This function initializes resources for Sample Apex
 *               Application B
 *
 * PARAMETERS  :
 *   INPUTS      : pMTestAssertData
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitializeSampleApexP2Resources(void)
{
  RETURN_CODE_TYPE errCode = NO_ERROR;

  CREATE_SAMPLING_PORT("Target_Src", 8, SOURCE, TARGET_TIME_OUT, &SamplingTargetSrcId, &errCode);

  CREATE_SAMPLING_PORT("InitSyncA_Dst", 8, DESTINATION, 30000000, &SamplingInitSyncADstId, &errCode);

  CREATE_SAMPLING_PORT("InitSyncB_Src", 8, SOURCE, 30000000, &SamplingInitSyncBSrcId, &errCode);

  CREATE_SAMPLING_PORT("InterCRC_Dst", 4, DESTINATION, 30000000, &SamplingInterCRCDstId, &errCode);

  CREATE_SEMAPHORE("mutex", 1, 1, FIFO, &SemaMutexId, &errCode);

  CREATE_SEMAPHORE("empty", MAX_INTERCRC_QUEUE_SIZE, MAX_INTERCRC_QUEUE_SIZE, FIFO, &SemaEmptyId, &errCode);

  CREATE_SEMAPHORE("full", 0, MAX_INTERCRC_QUEUE_SIZE, FIFO, &SemaFullId, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitializeSampleApexP2Processes
 *
 * DESCRIPTION : This function initializes processes for Sample Apex
 *               Application B
 *
 * PARAMETERS  :
 *   INPUTS      : pMTestAssertData
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitializeSampleApexP2Processes(void)
{
  PROCESS_ATTRIBUTE_TYPE process1Attrib;
  PROCESS_ATTRIBUTE_TYPE process2Attrib;
  PROCESS_ATTRIBUTE_TYPE process3Attrib;

  PROCESS_STATUS_TYPE process1Status;
  PROCESS_STATUS_TYPE process2Status;
  PROCESS_STATUS_TYPE process3Status;

  RETURN_CODE_TYPE errCode = NO_ERROR;

  strncpy(process1Attrib.NAME, "Logger", MAX_NAME_LENGTH);
  process1Attrib.BASE_PRIORITY = 150;
  process1Attrib.DEADLINE = SOFT;
  process1Attrib.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)&ProcessLogger;
  process1Attrib.PERIOD = INFINITE_TIME_VALUE;
  process1Attrib.STACK_SIZE = 4*1024; // 4 Kilobytes
  process1Attrib.TIME_CAPACITY = INFINITE_TIME_VALUE;

  strncpy(process2Attrib.NAME, "CRCMonitor", MAX_NAME_LENGTH);
  process2Attrib.BASE_PRIORITY = 100;
  process2Attrib.DEADLINE = SOFT;
  process2Attrib.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)&ProcessCRCMonitor;
  process2Attrib.PERIOD = 30000000; // 30ms
  process2Attrib.STACK_SIZE = 4*1024; // 4 Kilobytes
  process2Attrib.TIME_CAPACITY = INFINITE_TIME_VALUE;

  strncpy(process3Attrib.NAME, "MatrixMultiplier", MAX_NAME_LENGTH);
  process3Attrib.BASE_PRIORITY = 50;
  process3Attrib.DEADLINE = SOFT;
  process3Attrib.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)&ProcessMatrixMultiplier;
  process3Attrib.PERIOD = INFINITE_TIME_VALUE;
  process3Attrib.STACK_SIZE = 4*1024; // 4 Kilobytes
  process3Attrib.TIME_CAPACITY = INFINITE_TIME_VALUE;

  // Create and check Logger process
  CREATE_PROCESS(&process1Attrib, &ProcessLoggerId, &errCode);

  GET_PROCESS_STATUS(ProcessLoggerId, &process1Status, &errCode);

  START(ProcessLoggerId, &errCode);

  // Create and check CRCMonitor process
  CREATE_PROCESS(&process2Attrib, &ProcessCRCMonitorId, &errCode);

  GET_PROCESS_STATUS(ProcessCRCMonitorId, &process2Status, &errCode);

  START(ProcessCRCMonitorId, &errCode);

  // Create and check MatrixMultiplier process
  CREATE_PROCESS(&process3Attrib, &ProcessMatrixMultiplierId, &errCode);

  GET_PROCESS_STATUS(ProcessMatrixMultiplierId, &process3Status, &errCode);

  START(ProcessMatrixMultiplierId, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitSyncWithApplicationA
 *
 * DESCRIPTION : This function synchronize the initial start of Application A
 *
 * PARAMETERS  :
 *   INPUTS      : pMTestAssertData
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitSyncWithApplicationA(void)
{
  char_t tempMsg[8];
  MESSAGE_SIZE_TYPE rxMsgSize = 0;
  VALIDITY_TYPE     isMsgValid = INVALID;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  while(1)
    {
      READ_SAMPLING_MESSAGE(SamplingInitSyncADstId, (MESSAGE_ADDR_TYPE)tempMsg, &rxMsgSize, &isMsgValid, &errCode);
      if(errCode == NO_ERROR)
      {
        if (!strcmp("A_READY", tempMsg))
          {
            break;
          }
      }
    }

  strncpy(tempMsg, "B_READY", sizeof(tempMsg));
  WRITE_SAMPLING_MESSAGE(SamplingInitSyncBSrcId, (MESSAGE_ADDR_TYPE)tempMsg, sizeof(tempMsg), &errCode);
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
