# Test_Performance10

This application assess the semaphore workload time. Three pre-compiler macros define the number iterations times, the number of workload processes and the workload delay: NB_ITER, NB_WORKLOAD_TASK and DELAY_MS. 
If nothing is passed to the compiler, standard values are used: 50 iterations, 2 workload processes with 10 ms delay each.

### Partition mapping to the A653 API tested in the present test application:

The following is the mapping of the (1) partition included in this application:

| Partition # | Tested feature    | Number of test |
| ----------- | ----------------  | -------------- |
|    P1       | semaphore  		  |  	 1		   |
