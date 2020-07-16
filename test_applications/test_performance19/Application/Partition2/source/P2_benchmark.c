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
* Filename : P2_benchmark.c
* Original Author : Felipe Magalhaes
* Description: This file initializes and executes all tests.
* __________________________________________________________________________
*/

/* INCLUDE SECTION */
#include "performance_lib.h"

/* Test specific includes */
#include "commonDefn.h"
#include "InitializeSampleApexP2.h"
#include "SampleApexApplicationB.h"

/* __________________________________________________________________________
 *
 * FUNCTION NAME : main_process
 * DESCRIPTION   : Partition 2 main function.
 * PARAMETERS :
 *   INPUT  : None
 *   OUTPUT : None
 * RETURN : None.
 * __________________________________________________________________________
 */
void MAIN_FUNCTION()
{
  RETURN_CODE_TYPE errCode = NO_ERROR;

  InitFibbonacciSequenceMatrix();
  InitSquareSequenceMatrix();
  InitTimeReferencesP2();
  InitializeSampleApexP2Resources();
  InitializeSampleApexP2Processes();
  InitSyncWithApplicationA();
  StartPerformanceMeasurementsP2();

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
