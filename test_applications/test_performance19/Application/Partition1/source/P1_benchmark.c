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
* Filename : P1_benchmark.c
* Original Author : Felipe Magalhaes
* Description: This file initializes and executes all tests.
* __________________________________________________________________________
*/

/* INCLUDE SECTION */
#include "performance_lib.h"

/* Test specific includes */
#include "InitializeSampleApexP1.h"
#include "commonDefn.h"
#include "SampleApexApplicationA.h"


/* __________________________________________________________________________
 *
 * FUNCTION NAME : main_process
 * DESCRIPTION   : Partition 1 main function.
 * PARAMETERS :
 *   INPUT  : None
 *   OUTPUT : None
 * RETURN : None.
 * __________________________________________________________________________
 */
void MAIN_FUNCTION()
{
  RETURN_CODE_TYPE errCode = NO_ERROR;

  InitializeSampleApexP1DataPool();
  InitTimeReferencesP1();
  InitializeSampleApexP1Resources();
  InitializeSampleApexP1Processes();
  InitSyncWithApplicationB();
  StartPerformanceMeasurementsP1();

  SET_PARTITION_MODE(NORMAL, &errCode);

  while (1)
  {
    // should never reach this point.
  }
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
