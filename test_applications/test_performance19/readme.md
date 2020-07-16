# Test_Performance19

This synthetic application emulates the execution flow of a real application. It is composed by two (2) partitions, with two (2) processes each. For each partition, one process is periodic, and the other one is sporadic. The application uses sampling ports to synchronize processes in different partitions, and semaphores to synchronize processes in the same partition. Partition one (1) executes CRC calculations, using randomly created data. The data is them transferred to partition two (2) using a sampling port. Partition two (2) waits on the sampling port for data to be consumed, and using this data, creates matrices and performs the multiplication of these matrices. The collected time is the end-to-end execution time for each partition. 

### Partition mapping to the A653 API tested in the present test application:

The following is the mapping of the (2) partitions included in this application:

| Partition # | Tested feature    | Number of test |
| ----------- | ----------------  | -------------- |
|    P1       | Apex application  |  	 1		   |
|    P2       | Apex application  |  	 1		   |

