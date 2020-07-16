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
* Filename : CalCRC32.c
* Original Author : Felipe Magalhaes
* Description: This file contains CRC functions.
* __________________________________________________________________________
*/

/* INCLUDE SECTION */
#include "CalCRC32.h"

/* __________________________________________________________________________
 * PRIVATE TYPES SECTION:
 * ---------------------
 */

/* __________________________________________________________________________
 * LOCAL FUNCTION PROTOTYPE:
 * -------------------------------
 */
static uint32_t calLookUpElem(uint32_t index);
/* __________________________________________________________________________
 * PRIVATE DATA SECTION:
 * ---------------------
 */
static uint32_t sCRCLookupTable[LOOKUP_TABLE_SIZE];  // Look-up Table

/* __________________________________________________________________________
 * LOCAL FUNCTION :
 * -------------------------------
 */

/* __________________________________________________________________________
 *
 * FUNCTION NAME : __LookUpElem
 * DESCRIPTION   : Calculates element of the look-up table.
 *
 * PARAMETERS : Index of the element
 * RETURN : Value of the look-up table element.
 * __________________________________________________________________________
 */
#define G	0x04C11DB7
static uint32_t calLookUpElem(uint32_t index) 
{
	uint32_t generator = G; // CRC generator
	uint32_t leading_bit; 	// MSB of shift_reg before shift

	uint32_t shift_reg = (index << 24);
	for (int32_t k = 7; k >= 0; k--) {
		leading_bit = shift_reg & 0x80000000;
		shift_reg = shift_reg << 1;
		if (0 != leading_bit) {
			shift_reg = shift_reg ^ generator;
		}
	}
	return shift_reg;
}

/* __________________________________________________________________________
 *
 * FUNCTION NAME : InitializeLookupTable
 * DESCRIPTION : This functions initialize the lookup table.
 *               
 *
 * PARAMETERS : None
 * RETURN : None.
 * __________________________________________________________________________
 */
void InitializeLookupTable(void)
{
	for (int32_t i = 0; i < LOOKUP_TABLE_SIZE; i++) {
		sCRCLookupTable[i] = calLookUpElem((uint32_t)i);
	}	
}


/* __________________________________________________________________________
 *
 * FUNCTION NAME : CalcCRC32
 * DESCRIPTION   : Returns 32 bit value of the buffer's crc.
 *
 * PARAMETERS : -in uint8_t  *buffer
 *              -in uint32_t bufferSize
 * RETURN : 32 bit CRC value.
 * __________________________________________________________________________
 */
#define Init 0xFFFFFFFF
#define XorOut 0xFFFFFFFF
uint32_t CalcCRC32(uint8_t *buffer, uint32_t bufferSize) {
	uint32_t i; // Byte index into buffer
	uint32_t k; // Index into look-up table
	uint32_t crc = Init;

	// Loop for each byte in Buffer
	for (i = 0; i < bufferSize; i++) {
		// Compute index into look-up table for the current byte
		k = ((crc >> 24) ^ (int32_t) buffer[i]) & 0xFF;
		// Update the running CRC_value for the current byte
		crc = (crc << 8) ^ sCRCLookupTable[k];
	}
	return (crc ^ XorOut);
}

/* __________________________________________________________________________
 * END OF FILE:
 * -------------
 * ___________________________________________________________________________
 */
