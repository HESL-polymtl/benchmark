# SFPBench
Set of ARINC-653 applications for assessing system performance / worst-case execution time over a repetition of runs. The goal of this benchmark framework - namely Straight-forward Partion-aware benchmark (SFPBench) -  is to provide a standard set of applications, which are easily adapted to any target system. This is possible by the usage of abstraction layers, which reduce the porting complexity. 

SFPBench seeks to provide a standard set of applications for ARINC-653 based systems evaluation. The primary objective of ARINC 653 standard is to define a general-purpose APplication/EXecutive (APEX) interface (Application Program Interface (API)) between the Core Software (CSW) of an Avionics Computer Resource (ACR) and the application software. Included within ARINC 653 are the interface requirements between the application software and the CSW and the list of services which allow the application software to control the scheduling, communication, and status information of its internal processing elements. Assessing general RTOS metrics is possible using different benchmark suites. Nevertheless, ARINC-653 RTOS are a different class of RTOS, where available benchmarks may not efficiently assess metrics. SFPBench aims on defining a generic set of open access benchmark applications to be used with any ARINC-653 compliant RTOS. By defining a generic benchmark suite, it is possible to define base values for fair comparison.

The SFPBench is composed of several test applications, each one of them designed to assess a given set of metrics. These applications are either an extension of open-source frameworks (i.e., RTOSBench and SNU-RT) or are new test applications. SFPBench is organized as support_files, which contains the abstractions layers for porting it to different targets, as well as test_applications, which holds the applications.

# Reference paper
  https://arxiv.org/abs/2007.10794

# references
  1. https://github.com/gchamp20/RTOSBench/tree/dev 
  2. http://www.cprover.org/goto-cc/examples/snu.html
  3. https://en.wikipedia.org/wiki/ARINC_653
