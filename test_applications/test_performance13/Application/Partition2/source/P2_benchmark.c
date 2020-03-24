
/* __________________________________________________________________________
* DEVELOPMENT HISTORY:
* --------------------
*
* $Author:: $: Felipe Gohring de Magalhaes
* $Rev:: $: Revision of last commit
* $Date:: $: 10/25/2018
* $Comments (Refer to applicable SCR/PR) $: Initial version of the file.
* __________________________________________________________________________
*/
/* __________________________________________________________________________
* MODULE DESCRIPTION:
* -------------------
* Filename : P2_benchmark.c
* Original Author : Felipe Gohring de Magalhaes
*
* This module contains partition 2 main function.
*
* ASSUMPTIONS, CONSTRAINTS OR LIMITATIONS:
* ----------------------------------------
* 
*
* REFERENCES:
* -----------
* None.
* __________________________________________________________________________
*/
/* INCLUDE SECTION */

#include "P2.h"

void dummy(void);
/* __________________________________________________________________________
*
* FUNCTION NAME : main_process
* DESCRIPTION   : Partition 1 function.
* PARAMETERS : None.
*   INPUT :
*   OUTPUT :
* RETURN : None.
* __________________________________________________________________________
*/
void  main_process(void)
{

    RETURN_CODE_TYPE       ret_type;
    
    PROCESS_ID_TYPE        process_id;
    PROCESS_ATTRIBUTE_TYPE process_att;

    process_att.ENTRY_POINT   = dummy;
    process_att.DEADLINE      = SOFT;
    process_att.PERIOD        = INFINITE_TIME_VALUE;
    process_att.STACK_SIZE    = 4096;
    process_att.TIME_CAPACITY = INFINITE_TIME_VALUE;
    process_att.BASE_PRIORITY = 10;
    strncpy(process_att.NAME, "dummy", MAX_NAME_LENGTH);
    
    CREATE_PROCESS(&process_att, &process_id, &ret_type);
    START(process_id, &ret_type);
    SET_PARTITION_MODE(NORMAL, &ret_type);

    while(1);
}

void dummy(void)
{
	uint32_t a;
	while(1){
		a++;
	}
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
