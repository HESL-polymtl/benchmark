
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

#ifndef DIJKSTRA_RUNS_PER_MEASURE
  #define DIJKSTRA_RUNS_PER_MEASURE 50
#endif

#ifndef EDGES
  #define EDGES 28
#endif

#define INFINITY 0xFFFFFFFFFFFFF

int32_t edges;                      /// The number of nonzero edges in the graph
int32_t nodes;                      /// The number of nodes in the graph

float32_t prev[EDGES][EDGES];                   /// prev[i] is the node that comes right before i in the shortest path from the source to i
float32_t  d[EDGES][EDGES];                     /// d[s][i] is the number of hopes between the source (s) and node i
float32_t fd[EDGES][EDGES];                     /// fd[s][i] is the length of the shortest path between the source (s) and node i
float32_t dist[EDGES][EDGES];                   /// dist[i][j] is the distance between node i and j; or 0.0 if there is no direct connection

int32_t GRAPHSIZE;
int32_t vector_u[] = {0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8};
int32_t vector_v[] = {1,3,4,2,0,4,1,4,5,0,4,6,1,3,5,7,2,4,8,3,4,7,4,6,8,4,5,7};
float32_t vector_fw[] = {2.5,2.1,3.85,2.05,2.33,2.77,2.69,3.31,2.258,2.001,2.002,2.9,2.87,2.01,2.23,2.200008,2.36,2.85,2.96,2.45,3.71,2.8,2.41,2.64,2.19,3.82,2.25,2.65};



void API_dijkstra(int32_t s);
void RunDijkstraTests ();

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
  processAttrib.ENTRY_POINT       = (SYSTEM_ADDRESS_TYPE)&RunDijkstraTests;
  processAttrib.STACK_SIZE        = 4096; // in bytes
  processAttrib.BASE_PRIORITY     = PERF_MED_PRIO;
  processAttrib.DEADLINE          = SOFT;
  strncpy(processAttrib.NAME, "Dijkstra process", MAX_NAME_LENGTH);

  CREATE_PROCESS(&processAttrib, &processId, &retCode);
 
  if (NO_ERROR != retCode)
  {
    PERF_PRINT_EOL();
    PERF_PRINT_STRING("Dijkstra PROBLEM: ");
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


void  RunDijkstraTests ()
{
  int32_t i, j;
  int32_t u, v;
  float32_t fw;
  edges = EDGES;
  RETURN_CODE_TYPE retCode = NO_ERROR;
  DECLARE_TIME_MEASURE()
  INITIALIZE_TIME_VARS("DIJKSTRA")

  for (i = 0; i < edges; i++) {
      for (j = 0; j < edges; j++) {
          dist[i][j] = 0.0;
          d[i][j] = 0.0;
          fd[i][j] = 0.0;
      }
  }

  nodes = -1;
  for (i = 0; i < edges; i++) {
      u = vector_u[i];
      v = vector_v[i];
      fw = vector_fw[i];
      dist[u][v] = fw;
      nodes = MAX(u, MAX(v, nodes));
  }

  GRAPHSIZE = nodes + 1;
  while (TRUE){
      INIT_TIME_MEASURE();
      for (j = 0; j < DIJKSTRA_RUNS_PER_MEASURE; j++){
          for (i = 0; i < EDGES; i++){
              vector_fw[i] *= i + 0.15;
          }

          for (i = 0; i <= nodes;i++){
              API_dijkstra(i);
          }
      }
      FINISH_TIME_MEASURE()
      VALIDATE_TIME_MEASURE(1)
      PERIODIC_WAIT(&retCode); 
  }
  
}  


void API_dijkstra(int32_t s)
{
  int32_t i, k, mini;
  int32_t visited[20];

  for (i = 0; i <= nodes; i++) {
    d[s][i] = INFINITY;
    fd[s][i] = INFINITY;
    prev[s][i] = -1;                    /// no path has yet been found to i
    visited[i] = 0;                     /// the i-th element has not yet been visited
  }

  d[s][s] = 0;
  fd[s][s] = 0.0;

  for (k = 0; k <= nodes; k++) {
    mini = -1;
    for (i = 0; i <= nodes; i++) {
      if (!visited[i] && ((mini == -1) || (fd[s][i] < fd[s][mini]))) {
        mini = i;
      }
    }

    visited[mini] = 1;

    for (i = 0; i <= nodes; i++) {
      if (dist[mini][i] >= 0.1) {
        if (fd[s][mini] + dist[mini][i] < fd[s][i]) {
          d[s][i] = d[s][mini] + 1;
          fd[s][i] = fd[s][mini] + dist[mini][i];
          prev[s][i] = mini;
        }
      }
    }
  }
}

/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
