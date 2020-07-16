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
* Filename : TestParameterP1.h
* Original Author : Felipe Magalhaes
* Description: This file contains partition 1 parameters.
* __________________________________________________________________________
*/

/* __________________________________________________________________________
* PREPROCESSOR DIRECTIVES:
* ------------------------
*/
#ifndef _INC_TEST_PARAMETER_P1_H
#define _INC_TEST_PARAMETER_P1_H

// Must be adjusted as a function of the CRC_CHUNK value and the reader
// process period to ensure the buffer is never more than 75 % full
#define MAX_INTERCRC_QUEUE_SIZE 8

// Data pool size be simply adjusted to a value that ensure OS differences
// are highlighted while minimizing the test duration
#define DATA_POOL_ROW_SIZE 1500
#define DATA_POOL_COL_SIZE MAX_INTERCRC_QUEUE_SIZE // Must be adjusted as a function of the "Intermediate CRC Queuing Port Max Number of Messages" value and the reader process period to ensure the buffer is never more than 75 % full

#define MATH_COMPUTATION_ITER 88878

#define TARGET_TIME_OUT 1000000

#endif /* _INC_TEST_PARAMETER_P1_H */

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
