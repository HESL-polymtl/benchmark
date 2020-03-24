/* __________________________________________________________________________
 * DEVELOPMENT HISTORY:
 * --------------------
 *
 * $Author:: $: Felipe Magalhaes
 * $Rev:: $: Revision of last commit
 * $Date:: $: 11/12/2018
 * $Comments:: $: Initial version of the file.
 *  __________________________________________________________________________
 */
/* __________________________________________________________________________
 * MODULE DESCRIPTION:
 * -------------------
 * Filename : performance_lib.h
 * Original Author : Felipe Magalhaes
 *
 * TODO
 *
 * ASSUMPTIONS, CONSTRAINTS OR LIMITATIONS:
 * ----------------------------------------
 * None.
 *
 * REFERENCES:
 * -----------
 * None.
 * __________________________________________________________________________
 */

#ifndef PERFORMANCE_LIB_H_
#define PERFORMANCE_LIB_H_

#include "ARINC653.h"
#include "string.h"

#ifndef PERF_LOW_PRIO
  #define PERF_LOW_PRIO 2
#endif
#ifndef PERF_MED_PRIO
  #define PERF_MED_PRIO 5
#endif
#ifndef PERF_HIGH_PRIO
  #define PERF_HIGH_PRIO 10
#endif

#define BASE_PRIO 10

#ifndef MIN
  #define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
  #define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

uint64_t PerfGetTimeTicks(void);                   //function to read system timer
#define GET_CURRENT_TICKS() PerfGetTimeTicks()     //mapping for the function
#define MAIN_FUNCTION() main()

/*UTILS*/
int32_t rand(void);
void srand(uint32_t seed);
void perfUltoa(char_t * buf, uint64_t val);
uint64_t perfStrtoull(const uint8_t *str);
uint64_t __div64(uint64_t dividend, uint64_t divisor);
uint64_t __mod64(uint64_t dividend, uint64_t divisor);

//types definition
typedef void perf_task_retval_t;
typedef void* perf_task_argument_t;
typedef int perf_main_retval_t;
typedef int perf_main_argument_t;
typedef perf_task_retval_t (*perf_task_entry_t)(perf_task_argument_t);
typedef uint64_t perf_time_t;
typedef PROCESS_ID_TYPE perf_task_handle_t;
typedef SEMAPHORE_ID_TYPE perf_sem_t;
typedef MUTEX_ID_TYPE perf_mutex_t;
typedef void (*perf_int_handler_t)(int, void*);

#define PERF_NAME_LENGTH 32
typedef char_t PERF_NAME_TYPE[PERF_NAME_LENGTH];

#ifndef SEMAPHORE_NAME_TYPE
typedef PERF_NAME_TYPE SEMAPHORE_NAME_TYPE;
#endif

#ifndef MUTEX_NAME_TYPE
typedef PERF_NAME_TYPE MUTEX_NAME_TYPE;
#endif

#if LIBC == 1
  void ftoa(float32_t n, char_t *res, int32_t afterpoint);
  float32_t atof(const char_t *s);
  float32_t sinus(float32_t arg, int32_t quad);
  float32_t sin(float32_t arg);
  float32_t cos(float32_t arg);
  float32_t modf(float32_t val, float32_t *iptr);
  float32_t fabs(float32_t n);
  float32_t ldexp(float32_t val, int32_t expon);
  float32_t frexp(float32_t value, int32_t *eptr);
  float32_t exp(float32_t arg);
  float32_t sqrt(float32_t arg);
  float32_t pow(float32_t arg1, float32_t arg2);
  float32_t log10(float32_t arg);
  float32_t log(float32_t arg);
#endif

typedef struct{
    char_t name[18];
    uint32_t exec_time;
    uint64_t currentTime;
    uint32_t deltaTime;
    uint64_t sPrevTime;
    uint64_t average;
    uint8_t first_run;
    float32_t deviation;
    float32_t averageUS;
    float32_t worstUS;
    float32_t bestUS;
    float32_t timeNS;
    float32_t timeUS;
    float32_t timeMS;
}perf_struct;

/*TIME*/
uint32_t perf_get_time_in_ns();
uint32_t perf_get_time_in_us();
uint32_t perf_get_time_in_ms();
uint32_t perf_tick_to_ns(uint64_t ticks);
uint32_t perf_tick_to_us(uint64_t ticks);
uint64_t perf_ns_to_ticks(uint64_t ns);

void PrintString(const int8_t* string);
void PrintNumber(const int32_t number_to_print);
void PrintNumberUnsigned(const uint32_t number);
void PrintNumberUnsigned64(const uint64_t number);
void PrintNumberHex(const uint32_t number, const int8_t nibbles_to_print);
void PrintNumberHex64(const uint64_t number, const int8_t nibbles_to_print);
void PrintFloat(const float32_t input_number);
void PERF_PRINT_EOL();

/*STDIO*/
void print_perf();
#ifdef PERFORMANCE_PRINT
  #define PERF_PRINT_STRING(x) PrintString((int8_t*)x)
  #define PERF_PRINT_FLOAT(x) PrintFloat(x)
  #define PERF_PRINT_NUMBER(x) PrintNumber(x)
  #define PERF_PRINT_UNSIGNED(x) PrintNumberUnsigned(x)
  #define PERF_PRINT_UNSIGNED64(x) PrintNumberUnsigned64(x)
  #define PERF_PRINT_HEX(x, y) PrintNumberHex(x, y)
  #define PERF_PRINT_HEX64(x, y) PrintNumberHex64(x, y)

#else
  #define PERF_PRINT_STRING(x) (void)x
  #define PERF_PRINT_EOL() 
  #define PERF_PRINT_FLOAT(x) (void)x
  #define PERF_PRINT_NUMBER(x) (void)x
  #define PERF_PRINT_UNSIGNED(x) (void)x 
  #define PERF_PRINT_UNSIGNED64(x) (void)x
  #define PERF_PRINT_HEX(x, y) (void)(x), (void)(y)
#endif

/*TIME MEASURE TAGS*/
#if MEASURE_STATIC == 1
  int32_t perf_current_pos();
  int32_t perf_init(char_t name[]);
  void perf_start_measurements(int32_t pos);
  void perf_end_measurements(int32_t pos);
  void perf_validate_measurements(int32_t pos, uint8_t calc_deviation);
  perf_struct get_perf(int32_t my_pos);

  #define DECLARE_TIME_MEASURE() int32_t my_pos_perf = -1;

  #define INITIALIZE_TIME_VARS(x) \
      my_pos_perf = perf_init(x); \
      if (my_pos_perf < 0) \
          while(1); //handle lack of space here

  #define INIT_TIME_MEASURE() perf_start_measurements(my_pos_perf);
  #define FINISH_TIME_MEASURE() perf_end_measurements(my_pos_perf);
  #define VALIDATE_TIME_MEASURE(x) perf_validate_measurements(my_pos_perf, x);
  #define PRINT_PERFORMANCE_INFO() print_perf();

#else
  #define DECLARE_TIME_MEASURE()
  #define INITIALIZE_TIME_VARS(x) (void)(x);
  #define INIT_TIME_MEASURE()
  #define FINISH_TIME_MEASURE()
  #define VALIDATE_TIME_MEASURE(x)(void)(x);
  #define PRINT_PERFORMANCE_INFO()
#endif

/*Time-based framework import*/
#define PERF_INIT_MIN_TIME_VALUE 0
#define PERF_INIT_MAX_TIME_VALUE 0x7FFFFFFFFFFFFFFF
#define NO_PART_MAX_GAP 100000000
#define DO_WORKLOAD(i) \
  do { \
    uint64_t x = 9; \
    uint64_t a = 0L; \
    uint64_t r = 0L; \
    uint64_t e = 0L; \
    int _workload_i_; \
    for (_workload_i_ = 0; _workload_i_ < BITSPERLONG; _workload_i_++) \
    { \
      r = (r << 2) + TOP2BITS(x); x <<= 2; \
      a <<= 1; \
      e = (a << 1) + 1; \
      if (r >= e) \
      { \
        r -= e; \
        a++; \
      } \
    } \
    _workload_results[i] = a; \
  } while (0);

#define BITSPERLONG 32
#define TOP2BITS(x) ((x & (3UL << (BITSPERLONG-2))) >> (BITSPERLONG-2))

#define TASK_DEFAULT_RETURN

#define DECLARE_TIME_COUNTERS(TYPE, SUFFIX) \
  TYPE SUFFIX##_t1; \
  TYPE SUFFIX##_t2; 

#define DECLARE_TIME_STATS(TYPE) \
  TYPE cycles = PERF_INIT_MIN_TIME_VALUE; \
  TYPE max_cycles = PERF_INIT_MIN_TIME_VALUE; \
  TYPE min_cycles = PERF_INIT_MAX_TIME_VALUE; \
  TYPE average_cycles = PERF_INIT_MIN_TIME_VALUE; \
  float32_t __LAST_PERF_STD = 0.0; \
  float32_t __CURR_PERF_STD = 0.0; \
  float32_t __STD_DEV = 0.0; \

#define WRITE_T1_COUNTER(SUFFIX) \
  SUFFIX##_t1 = GET_CURRENT_TICKS();

#define WRITE_T2_COUNTER(SUFFIX) \
  SUFFIX##_t2 = GET_CURRENT_TICKS();

#define COMPUTE_TIME_STATS(SUFFIX, n) do { \
    cycles = perf_time_diff(&SUFFIX##_t1, &SUFFIX##_t2); \
    if (cycles < NO_PART_MAX_GAP) { \
      if (cycles > max_cycles) \
      { \
        max_cycles = cycles; \
      } \
      if (cycles <  min_cycles) \
      { \
        min_cycles = cycles; \
      } \
      average_cycles = (average_cycles == 0) ? cycles : (__div64(cycles+average_cycles, 2)); \
      __CURR_PERF_STD = perf_tick_to_ns(cycles) / 1000.0; \
      if (__LAST_PERF_STD != 0.0){ \
        __STD_DEV = (__CURR_PERF_STD + __LAST_PERF_STD)/2; \
        __STD_DEV = (((__CURR_PERF_STD-__STD_DEV)*(__CURR_PERF_STD-__STD_DEV)) + ((__LAST_PERF_STD-__STD_DEV)*(__LAST_PERF_STD-__STD_DEV))) / 2; \
        __STD_DEV = sqrt(__STD_DEV); \
      }\
      __LAST_PERF_STD = __CURR_PERF_STD; \
    }\
  } while(0);

#define RESET_TIME_STATS() \
  average_cycles = 0; \
  max_cycles = 0; \
  min_cycles = PERF_INIT_MAX_TIME_VALUE;


#define REPORT_RESULTS(max_cycles, min_cycles, average_cycles) \
  PERF_PRINT_EOL(); \
  PERF_PRINT_STRING("Max time(us): "); \
  PERF_PRINT_UNSIGNED(perf_tick_to_us(max_cycles)); \
  PERF_PRINT_EOL(); \
  PERF_PRINT_STRING("Min time(us): "); \
  PERF_PRINT_UNSIGNED(perf_tick_to_us(min_cycles)); \
  PERF_PRINT_EOL(); \
  PERF_PRINT_STRING("Average time(us): "); \
  PERF_PRINT_UNSIGNED(perf_tick_to_us(average_cycles)); \
  PERF_PRINT_EOL(); \
  PERF_PRINT_STRING("Standard Deviation: "); \
  PERF_PRINT_FLOAT(__STD_DEV); \
  PERF_PRINT_EOL(); \
  (void)__LAST_PERF_STD;\
  (void)__CURR_PERF_STD;\


#define REPORT_BENCHMARK_RESULTS(STR_PTR) do { \
      PERF_PRINT_STRING(STR_PTR); \
      REPORT_RESULTS(max_cycles, min_cycles, average_cycles); \
    } while (0);

/* Porting API */
/* @brief Call to initialize the a test case. */
void perf_initialize_test(perf_task_entry_t init_test_fct);
/* @brief Creates a task. The task must be in a suspended or dormant state after creation. */
perf_task_handle_t perf_create_task(perf_task_entry_t task_entry, char task_name[4], uint32_t prio);
/* @brief Changes the state of the running task to ready. */
void perf_task_yield();
/* @brief Suspends the calling task */
void perf_task_suspend_self();
/* @brief Delays the calling task by the specified number of nanoseconds */
void perf_task_delay(uint32_t nanoseconds);
/* @brief Reads the current value of a time. */
perf_time_t perf_time_get();
/* @brief Prints the results on the serial output */
void perf_result_report(int64_t max, int64_t min, int64_t average);
/* @brief Write a single integer value with a prefix */
void perf_single_result_report(char* prefix, int64_t value);
/* @brief Computes the addition of a time value and a number of tick */
perf_time_t perf_add_times(const perf_time_t* base, uint32_t ticks);
/* @brief Computes the difference between two time values (t2 -t1) */
perf_time_t perf_time_diff(const perf_time_t* t1, const perf_time_t* t2);
/* @brief Creates a semaphore. */
void perf_sem_create(perf_sem_t* sem, int current_value);
/* @brief Wait on a semaphore. */
void perf_sem_wait(perf_sem_t* sem);
/* @brief Signal a semaphore. */
void perf_sem_signal(perf_sem_t* sem);
/* @brief Creates a mutex. */
void perf_mutex_create(perf_mutex_t* mutex);
/* @brief Acquire a mutex . */
void perf_mutex_acquire(perf_mutex_t* mutex);
/* @brief Release a mutex. */
void perf_mutex_release(perf_mutex_t* mutex);
/* @brief Writes a string on the serial output */
void perf_serial_write(const char_t* string);

#endif /* PERFORMANCE_LIB_H_ */
/* __________________________________________________________________________
* END OF FILE:
* -------------
* ___________________________________________________________________________
*/
