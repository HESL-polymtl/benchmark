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
* Filename : TestParametersP2.h
* Original Author : Felipe Magalhaes
* Description: This file contains partition 2 application's parameters.
* __________________________________________________________________________
*/


/* __________________________________________________________________________
* PREPROCESSOR DIRECTIVES:
* ------------------------
*/
#ifndef _INC_TEST_PARAMETER_P2_H
#define _INC_TEST_PARAMETER_P2_H

// Must be adjusted as a function of the CRC_CHUNK value and the reader
// process period to ensure the buffer is never more than 75 % full
#define MAX_INTERCRC_QUEUE_SIZE 8

// Must be set equal to intermediate CRC Queuing Port Max Number of Message
#define MAX_LOGBUFFER_SIZE MAX_INTERCRC_QUEUE_SIZE

// Must be adjusted to minimize stack usage while ensuring CRCMonitor is in
// competition with MatrixMultiplier
#define GCD_VALUE 3

// Matrix Row and Col count shall be adjusted so that CRCMonitor
// pushes data on the LogBuffer at least a 10 times before the matrix
// multiplication completes
#define MAX_MAXTRIX_ROW 5
#define MAX_MAXTRIX_COL 8

#define WRITE_DISK_ITER 1333170 

#define TARGET_TIME_OUT 1000000

#endif /* _INC_TEST_PARAMETER_P2_H */

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
