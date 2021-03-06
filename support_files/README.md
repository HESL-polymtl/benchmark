# support_files
These files are used as abstraction layers for porting the framework for different targets (RTOS and HW). The framework is organized 
as it follows:
  - performance_lib_hw_porting_layer: hardware abstraction layer
  - performance_lib_libc: standard libC functions - these can be enable using pre-compiler macro LIBC=1
  - performance_lib_os_porting_layer: operating system abstraction layer
  - performance_lib_static: framework timing control structures - enable using pre-compiler macro MEASURE_STATIC=1
  - performance_lib_stdio: functions to control stdio (print) - these can be enable using pre-compiler macro PERFORMANCE_PRINT

Also, two library files are included: performance_lib and performance_lib_mapping. These files are used for types definitions and 
overal system definitions.

In order for the benchmark framework to be ported to a target system, the HW and OS porting layers should be adapted. Also, library 
files should reflect needed libraries and functions (i.e., if it should use <stdlib.h> or provide a RTOS specific lib). 

As an example, the files included here use JetOS as target RTOS. For the hardware platform, PowerPC MPC5777C and ARM TMS570 boards are used.
If a different target is needed, one can rely in the examples already in place and modify only what is needed.

# references
 - http://www.mathnet.ru/php/archive.phtml?wshow=paper&jrnid=tisp&paperid=227&option_lang=eng
 - http://www.ti.com/tool/TMDX570LC43HDK
 - https://www.nxp.com/products/processors-and-microcontrollers/power-architecture/mpc55xx-5xxx-mcus/ultra-reliable-mpc57xx-mcus/ultra-reliable-mpc5777c-mcu-for-automotive-and-industrial-engine-management:MPC5777C 
