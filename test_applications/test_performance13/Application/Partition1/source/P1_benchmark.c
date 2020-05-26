
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
* Filename : P1_benchmark.c
* Original Author : Felipe Gohring de Magalhaes
*
* This module contains partition 1 main function.
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
/*Include here any system specific lib that is not with performance_lib.h*/

#include "performance_lib.h"

#ifndef NUMBER_OF_ITERATIONS
#define NUMBER_OF_ITERATIONS 50
#endif

void partition_switch_test(void);
void partition_switch_loop(void);

uint64_t time1 = 0, time2 = 0, diff = 0, wcet = 0, bcet = (uint64_t)INFINITE_TIME_VALUE, average = 0;
float32_t deviation = 0.0, averageNS = 0.0, diffNS = 0.0, variance = 0.0, sumTimeNSSQ = 0.0;
uint64_t WINDOW_TIME_TICKS = 0;
uint32_t noSamples = 0;
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
void  MAIN_FUNCTION()
{
    PROCESS_ID_TYPE        process_id;
    PROCESS_ATTRIBUTE_TYPE process_att;
    RETURN_CODE_TYPE       ret_type;
    PARTITION_STATUS_TYPE  partitionStatus;

    GET_PARTITION_STATUS(&partitionStatus, &ret_type);
    WINDOW_TIME_TICKS = perf_ns_to_ticks(partitionStatus.DURATION);

    process_att.ENTRY_POINT   = partition_switch_test;
    process_att.DEADLINE      = SOFT;
    process_att.PERIOD        = INFINITE_TIME_VALUE;
    process_att.STACK_SIZE    = 4096;
    process_att.TIME_CAPACITY = INFINITE_TIME_VALUE;
    process_att.BASE_PRIORITY = 10;
    strncpy(process_att.NAME, "PartitionSwitch", MAX_NAME_LENGTH);
    
    CREATE_PROCESS(&process_att, &process_id, &ret_type);

    if(ret_type != NO_ERROR)
    {
        PERF_PRINT_STRING("Cannot create partition_switch_test process 1"); 
        PERF_PRINT_NUMBER(ret_type);
        PERF_PRINT_EOL();
    }

    DELAYED_START(process_id, partitionStatus.DURATION*NUMBER_OF_ITERATIONS, &ret_type);
    if(ret_type != NO_ERROR)
    {
        PERF_PRINT_STRING("Cannot start partition_switch_test process 1"); 
        PERF_PRINT_NUMBER(ret_type);
        PERF_PRINT_EOL();
    }

    process_att.ENTRY_POINT   = partition_switch_loop;
    process_att.DEADLINE      = SOFT;
    process_att.PERIOD        = INFINITE_TIME_VALUE;
    process_att.STACK_SIZE    = 4096;
    process_att.TIME_CAPACITY = INFINITE_TIME_VALUE;
    process_att.BASE_PRIORITY = 2;
    strncpy(process_att.NAME, "PartitionSwitchLoop", MAX_NAME_LENGTH);
    
    CREATE_PROCESS(&process_att, &process_id, &ret_type);

    if(ret_type != NO_ERROR)
    {
        PERF_PRINT_STRING("Cannot create partition_switch_loop process 1"); 
        PERF_PRINT_NUMBER(ret_type);
        PERF_PRINT_EOL();
    }

    START(process_id, &ret_type);

    PERF_PRINT_STRING("P1 partition switching to normal mode");
    PERF_PRINT_EOL();
    SET_PARTITION_MODE(NORMAL, &ret_type);

    if(ret_type != NO_ERROR)
    {
        PERF_PRINT_STRING("Cannot switch P1 partition to NORMAL state "); 
        PERF_PRINT_NUMBER(ret_type);
        PERF_PRINT_EOL();
    }

    while(1);
}

void partition_switch_loop(void){
    while(1)
    {
      time1 = GET_CURRENT_TICKS();
      if((time2 + WINDOW_TIME_TICKS) < time1)
        {
          noSamples++;
          diff = time1 - time2 - WINDOW_TIME_TICKS;
          diffNS = (float32_t)perf_tick_to_ns(diff);
          average = ((noSamples - 1) * average + diff) / noSamples;
          averageNS = ((float32_t)(noSamples - 1) * averageNS + diffNS) / (float32_t)noSamples;
          wcet = (diff > wcet) ? diff : wcet;
          bcet = (diff < bcet) ? diff : bcet;

          sumTimeNSSQ += diffNS * diffNS;
          variance = (sumTimeNSSQ / (float32_t)noSamples) - (averageNS * averageNS);
          if (variance < 0)
            {
              deviation = (float32_t)((float32_t)(perf_tick_to_us(wcet) - perf_tick_to_us(bcet)) / 4.0);
            }
          else
            {
              deviation = (float32_t)(sqrt(variance)/ 1000.0);
            }
        }
      time2 = time1;
    }
}

void partition_switch_test(void)
{
    PERF_PRINT_STRING("--PARTITION SWITCH--");
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("BCET (tick): ");
    PERF_PRINT_UNSIGNED64(bcet);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("WCET (tick): ");
    PERF_PRINT_UNSIGNED64(wcet);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Average (tick): ");
    PERF_PRINT_UNSIGNED64(average);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Last Execution Time (us): ");
    PERF_PRINT_FLOAT(diffNS / 1000.0);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Average (us): ");
    PERF_PRINT_FLOAT(averageNS / 1000.0);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("WCET (us): ");
    PERF_PRINT_FLOAT((float32_t)perf_tick_to_ns(wcet) / 1000.0);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("BCET (us): ");
    PERF_PRINT_FLOAT((float32_t)perf_tick_to_ns(bcet) / 1000.0);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Standard Deviation (us): ");
    PERF_PRINT_FLOAT(deviation);
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Number of samples: ");
    PERF_PRINT_UNSIGNED(noSamples);
    PERF_PRINT_EOL();
    while(1);
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
