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
* Filename : CalCRC32.h
* Original Author : Felipe Magalhaes
* Description: This file contains partition 1 parameters.
* __________________________________________________________________________
*/

#ifndef _CAL_CRC32_H
#define _CAL_CRC32_H

#define LOOKUP_TABLE_SIZE 256

void InitializeLookupTable(void);
uint32_t CalcCRC32(uint8_t *buffer, uint32_t bufferSize);

#endif//_CAL_CRC32_H
/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
