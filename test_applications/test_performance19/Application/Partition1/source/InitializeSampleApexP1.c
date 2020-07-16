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
* Filename : InitializeSampleApexP1.c
* Original Author : Felipe Magalhaes
* Description: This file contains test initializations.
* __________________________________________________________________________
*/
/* INCLUDE SECTION */
#include <string.h>
#include "TestParameterP1.h"
#include "CalCRC32.h"

/* __________________________________________________________________________
 * PUBLIC DATA SECTION:
 * ---------------------
 */

/* IMPORTED GLOBAL INPUT VARIABLES */
extern void ProcessMonitoring(void);
extern void ProcessInnerLoop(void);
extern uint32_t DataPool[DATA_POOL_ROW_SIZE][DATA_POOL_COL_SIZE];

/* EXPORTED GLOBAL OUTPUT VARIABLES */
PROCESS_ID_TYPE ProcessMonitoringId = 0;
PROCESS_ID_TYPE ProcessInnerLoopId = 0;

SAMPLING_PORT_ID_TYPE SamplingCommandSrcId = 0;
SAMPLING_PORT_ID_TYPE SamplingCommandDstId = 0;
SAMPLING_PORT_ID_TYPE SamplingTargetDstId = 0;
SAMPLING_PORT_ID_TYPE SamplingInitSyncASrcId = 0;
SAMPLING_PORT_ID_TYPE SamplingInitSyncBDstId = 0;

SAMPLING_PORT_ID_TYPE SamplingInterCRCSrcId = 0;


/* __________________________________________________________________________
 *
 * FUNCTION    : InitializeSampleApexP1Resources
 *
 * DESCRIPTION : This function initializes resources for SampleApex Application A
 *
 * PARAMETERS  :
 *   INPUTS      : void
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitializeSampleApexP1Resources(void)
{
  RETURN_CODE_TYPE errCode = NO_ERROR;

  CREATE_SAMPLING_PORT("Command_Src", 8, SOURCE, 30000000, &SamplingCommandSrcId, &errCode);

  CREATE_SAMPLING_PORT("Command_Dst", 8, DESTINATION, 30000000, &SamplingCommandDstId, &errCode);

  CREATE_SAMPLING_PORT("Target_Dst", 8, DESTINATION, TARGET_TIME_OUT, &SamplingTargetDstId, &errCode);

  CREATE_SAMPLING_PORT("InitSyncA_Src", 8, SOURCE, 30000000, &SamplingInitSyncASrcId, &errCode);

  CREATE_SAMPLING_PORT("InitSyncB_Dst", 8, DESTINATION, 30000000, &SamplingInitSyncBDstId, &errCode);

  CREATE_SAMPLING_PORT("InterCRC_Src", 4, SOURCE, 30000000, &SamplingInterCRCSrcId, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitializeSampleApexP1Processes
 *
 * DESCRIPTION : This function initializes processes for SampleApex Application A
 *
 * PARAMETERS  :
 *   INPUTS      : void
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitializeSampleApexP1Processes(void)
{
  PROCESS_ATTRIBUTE_TYPE process1Attrib;
  PROCESS_ATTRIBUTE_TYPE process2Attrib;

  PROCESS_STATUS_TYPE process1Status;
  PROCESS_STATUS_TYPE process2Status;

  RETURN_CODE_TYPE errCode = NO_ERROR;

  strncpy(process1Attrib.NAME, "Monitoring", MAX_NAME_LENGTH);
  process1Attrib.BASE_PRIORITY = 50;
  process1Attrib.DEADLINE = SOFT;
  process1Attrib.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)&ProcessMonitoring;
  process1Attrib.PERIOD = INFINITE_TIME_VALUE;
  process1Attrib.STACK_SIZE = 4*1024; // 4 Kilobytes
  process1Attrib.TIME_CAPACITY = INFINITE_TIME_VALUE;

  strncpy(process2Attrib.NAME, "InnerLoop", MAX_NAME_LENGTH);
  process2Attrib.BASE_PRIORITY = 100;
  process2Attrib.DEADLINE = SOFT;
  process2Attrib.ENTRY_POINT = (SYSTEM_ADDRESS_TYPE)&ProcessInnerLoop;
  process2Attrib.PERIOD = 30000000; // 30ms
  process2Attrib.STACK_SIZE = 4*1024; // 4 Kilobytes
  process2Attrib.TIME_CAPACITY = 25000000; // expected to be 25ms

  // Create and check Monitoring process
  CREATE_PROCESS(&process1Attrib, &ProcessMonitoringId, &errCode);

  GET_PROCESS_STATUS(ProcessMonitoringId, &process1Status, &errCode);

  START(ProcessMonitoringId, &errCode);

  // Create and check InnerLoop process
  CREATE_PROCESS(&process2Attrib, &ProcessInnerLoopId, &errCode);

  GET_PROCESS_STATUS(ProcessInnerLoopId, &process2Status, &errCode);

  START(ProcessInnerLoopId, &errCode);
}

/* __________________________________________________________________________
 *
 * FUNCTION    : InitSyncWithApplicationB
 *
 * DESCRIPTION : This function synchronize the initial start of Application B
 *
 * PARAMETERS  :
 *   INPUTS      : void
 *   OUTPUTS     : None.
 * RETURN      : None.
 * __________________________________________________________________________
 */
void InitSyncWithApplicationB(void)
{
  char_t tempMsg[8];
  MESSAGE_SIZE_TYPE rxMsgSize = 0;
  VALIDITY_TYPE isMsgValid = INVALID;
  RETURN_CODE_TYPE errCode = NO_ERROR;

  strncpy(tempMsg, "A_READY", sizeof(tempMsg));
  WRITE_SAMPLING_MESSAGE(SamplingInitSyncASrcId, (MESSAGE_ADDR_TYPE)tempMsg, sizeof(tempMsg), &errCode);

  while (1)
    {
      READ_SAMPLING_MESSAGE(SamplingInitSyncBDstId, (MESSAGE_ADDR_TYPE)tempMsg, &rxMsgSize, &isMsgValid, &errCode);
      if(errCode == NO_ERROR)
      {
          if (!strcmp("B_READY", tempMsg))
          {
            break;
          }
      }
    }
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : InitializeSampleApexP1DataPool
 * DESCRIPTION : This function initializes the data pool.
 *               The Data Pool Content must minimize repetition while not being random (at each run, the content shall be the same)
 *
 * PARAMETERS : None
 * RETURN : None.
 * __________________________________________________________________________
 */
void InitializeSampleApexP1DataPool(void)
{
  uint32_t i, j;

  for (i = 0; i < DATA_POOL_ROW_SIZE; i++)
  {
    for(j = 0; j < DATA_POOL_COL_SIZE; j++)
    {
      DataPool[i][j] = i*j;
    }    
  }
  InitializeLookupTable();
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
