
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

#ifndef SOBEL_RUNS_PER_MEASURE
  #define SOBEL_RUNS_PER_MEASURE 10
#endif

#define width 48
#define height 38

uint8_t image[height * width];

uint8_t sobel_img[height * width]={0}, sobel_img2[height * width]={0};
void RunSobelTests ();
uint8_t sobel_gaussian(uint8_t buffer[5][5]);
uint32_t isqrt(uint32_t a);
uint8_t sobel(uint8_t buffer[3][3]);
void do_gaussian();
void do_sobel();
void printResults(void);

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
void MAIN_FUNCTION()
{
  RETURN_CODE_TYPE retCode = NO_ERROR;
  PROCESS_ATTRIBUTE_TYPE processAttrib;
  PROCESS_ID_TYPE processId;

  PARTITION_STATUS_TYPE partitionStatus;
  GET_PARTITION_STATUS(&partitionStatus, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&RunSobelTests;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_MED_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Sobel process", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Sobel PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);

  processAttrib.PERIOD            = partitionStatus.PERIOD;
  processAttrib.TIME_CAPACITY     = partitionStatus.PERIOD;
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&printResults;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_LOW_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "PRINT process", MAX_NAME_LENGTH);


  CREATE_PROCESS(&processAttrib, &processId, &retCode);
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("PRINT PROBLEM: ");
    PERF_PRINT_NUMBER(retCode);
    PERF_PRINT_EOL();
  }

  START(processId, &retCode);
  SET_PARTITION_MODE(NORMAL, &retCode);
  while(1)
  {
    PERF_PRINT_STRING("* main_process Running in IDLE mode ");
  }
}

void printResults()
{
  RETURN_CODE_TYPE retCode = NO_ERROR;
  while(TRUE){
    PRINT_PERFORMANCE_INFO()
    PERIODIC_WAIT(&retCode);
  }
}

void  RunSobelTests ()
{
  int32_t runs, j;
  RETURN_CODE_TYPE retCode = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("SOBEL")
  while(TRUE){
    INIT_TIME_MEASURE()
    for (runs = 0; runs < SOBEL_RUNS_PER_MEASURE; runs++){
      for (j = 0; j < height * width; j++){
          image[j] = (uint8_t)rand();
        }

      do_gaussian();
      do_sobel();
    }
    FINISH_TIME_MEASURE()
    VALIDATE_TIME_MEASURE(1)
    PERIODIC_WAIT(&retCode); 
  }
}

uint8_t sobel_gaussian(uint8_t buffer[5][5])
{
  int32_t sum = 0, mpixel;
  uint8_t i, j;

  int16_t kernel[5][5] =  {
    {2, 4, 5, 4, 2},
    {4, 9, 12, 9, 4},
    {5, 12, 15, 12, 5},
    {4, 9, 12, 9, 4},
    {2, 4, 5, 4, 2}
  };
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
    sum += ((int32_t)buffer[i][j] * (int32_t)kernel[i][j]);

  mpixel = (int32_t)(sum / 159);

  return (uint8_t)mpixel;
}

uint32_t isqrt(uint32_t a)
{
  uint32_t i, rem = 0, root = 0, divisor = 0;

  for (i = 0; i < 16; i++){
    root <<= 1;
    rem = ((rem << 2) + (a >> 30));
    a <<= 2;
    divisor = (root << 1) + 1;
    if (divisor <= rem){
      rem -= divisor;
      root++;
    }
  }
  return root;
}

uint8_t sobel(uint8_t buffer[3][3])
{
  int32_t sum = 0, gx = 0, gy = 0;
  uint8_t i, j;

  int16_t kernelx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1},
  };

  int16_t kernely[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1},
  };

  for (i = 0; i < 3; i++){
    for (j = 0; j < 3; j++){
      gx += ((int32_t)buffer[i][j] * (int32_t)kernelx[i][j]);
      gy += ((int32_t)buffer[i][j] * (int32_t)kernely[i][j]);
    }
  }

  sum = isqrt(gy * gy + gx * gx);

  if (sum > 255)
    sum = 255;
  if (sum < 0)
    sum = 0;

  return (uint8_t)sum;
}

void do_gaussian()
{
  int32_t i = 0, j = 0, k, l;
  uint8_t image_buf[5][5];

  for(i = 0; i < height; i++){
    if (i > 1 && i < height-2){
      for(j = 0; j < width; j++){
        if (j > 1 && j < width-2){
          for (k = 0; k < 5; k++)
            for(l = 0; l < 5; l++)
              image_buf[k][l] = image[(((i + l-2) * width) + (j + k-2))];

          sobel_img[((i * width) + j)] = sobel_gaussian(image_buf);
        }
        else{
          sobel_img[((i * width) + j)] = image[((i * width) + j)];
        }
      }
    }
    else{
      sobel_img[((i * width) + j)] = image[((i * width) + j)];
    }
  }
}

void do_sobel()
{
  int32_t i = 0, j = 0, k, l;
  uint8_t image_buf[3][3];

  for(i = 0; i < height; i++){
    if (i > 2 && i < height-3){
      for(j = 0; j < width-1; j++){
        if (j > 2 && j < width-3){
          for (k = 0; k < 3; k++)
            for(l = 0; l < 3; l++)
              image_buf[k][l] = sobel_img[(((i + l-1) * width) + (j + k-1))];

          sobel_img2[((i * width) + j)] = sobel(image_buf);
        }
        else{
          sobel_img2[((i * width) + j)] = 0;
        }
      }
    }
    else{
      sobel_img2[((i * width) + j)] = 0;
    }
  }
}
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
