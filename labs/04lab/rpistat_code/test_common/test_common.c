//
// Test common code
//

//
// Author: P.J. Drongowski
// Date: 18 April 2013
//
// Copyright (c) 2013 Paul J. Drongowski
//

#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>

#include <sched.h>

#include "../test_common/test_common.h"

//
// Common command line options
//
int d_flag = 0 ; // Display debugging output when TRUE
int h_flag = 0 ; // Display usage/help information then TRUE
int v_flag = 0 ; // Display more information when TRUE

//
// Write test run results to this file
//
FILE* result_file = NULL ;

//
// Display this divider in headers
//
static char* hline = "***************************************************************" ;


unsigned long long int getTimeStamp()
{
  return( 0 ) ;
}


//
// Display the processor affinity mask for the process. This function is
// used mainly for debugging purposes.
//
void display_process_affinity()
{
  cpu_set_t set ;
  int cpu_set_size = sizeof(cpu_set_t) ;
  int number_of_cpus = -1 ;
  int i = 0 ;
  unsigned long long int affinity_mask ;

  // Get the number of CPUs in the platform
  number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN ) ;

  // Display a warning if there are more than 64 processors since the
  // mask will exceed the 64-bit affinity_mask.
  if (number_of_cpus > 64) {
    fprintf(stderr, "display_process_affinity: More than 64 cores\n") ;
  }

  // Get the current process scheduling affinity set (mask)
  sched_getaffinity(0, cpu_set_size, &set) ;

  // Convert the CPU set to a true affinity mask
  affinity_mask = 0 ;
  for (i = number_of_cpus-1 ; i >= 0 ; i--) {
    affinity_mask = affinity_mask << 1 ;
    if (CPU_ISSET(i, &set)) affinity_mask |= 1 ;
  }

  // Display the affinity mask in hex
  fprintf(stderr, "Affinity mask: %16llx\n", affinity_mask) ;
}


// Currently stubbed out
// Return TRUE on success, otherwise FALSE
int set_process_affinity(int core)
{
  cpu_set_t set ;
  int cpu_set_size = sizeof(cpu_set_t) ;
  int number_of_cpus = -1 ;
  int i = 0 ;
  unsigned long long int affinity_mask ;

  number_of_cpus = sysconf( _SC_NPROCESSORS_ONLN ) ;
  if ((core < 0) || (core >= number_of_cpus)) {
    fprintf(stderr, "set_process_affinity: Invalid core number: %d\n", core) ;
  }

  if (d_flag) {
    fprintf(stderr, "Initial process affinity mask\n") ;
    display_process_affinity() ;
  }

  CPU_ZERO(&set) ;
  CPU_SET(core, &set) ;
  sched_setaffinity(0, cpu_set_size, &set) ;

  if (d_flag) {
    fprintf(stderr, "Final process affinity mask\n") ;
    display_process_affinity() ;
  }

  return( 1 ) ;
}

//
// Display usage information
//
void display_usage_info(char* prog, char* usage_strings[])
{
  char** msg ;

  fprintf(stderr, "Usage: %s\n", prog) ;

  for (msg = usage_strings ; *msg != NULL ; msg++) {
    fprintf(stderr, "%s\n", *msg) ;
  }
  exit( EXIT_FAILURE ) ;
}

//
// Return CPU speed in MHz
//
unsigned int get_cpu_speed()
{
  return( 700 ) ;
}

//
// Write system information into the results file
//
void print_system_info()
{
  int numberOfCPUs = -1 ;

  numberOfCPUs = sysconf( _SC_NPROCESSORS_ONLN ) ;

  fprintf(result_file, "System information\n") ;
  fprintf(result_file, " Number of processors: %d\n", numberOfCPUs) ;
}

//
// Get the elapsed time from the OS and return the time after converting
// to seconds.
//
double get_elapsed_time()
{
  return( ((double)clock() / (double)CLOCKS_PER_SEC) ) ;
}

//
// Obtain and print the elapsed time to the result file
//
void print_elapsed_time()
{
  double elapsed ;
  double resolution ;

  // Obtain and display elapsed execution time
  elapsed = get_elapsed_time() ;
  resolution = 1.0 / CLOCKS_PER_SEC ;

  fprintf(result_file,
      " Elapsed time: %9.3f (%8.6f sec resolution)\n",
      elapsed, 
      resolution) ;
}

//
// Obtain and print system and user time into the result file
//
void print_process_times()
{
  struct tms pt ;
  int ticks_per_second = 1 ;
  double user_time_sec, system_time_sec ;

  if (times(&pt) == -1) {
    fprintf(stderr, "Couldn't get process times\n") ;
  }

  ticks_per_second = sysconf(_SC_CLK_TCK) ;

  user_time_sec = (double)pt.tms_utime / (double)ticks_per_second ;
  system_time_sec = (double)pt.tms_stime / (double)ticks_per_second ;

  fprintf(result_file," User time: %9.3f sec\n", user_time_sec) ;
  fprintf(result_file," System time: %9.3f sec\n", system_time_sec) ;
}

//
// Print heading into the result file
//
void print_heading(char* heading)
{
  time_t current_time ;

  time(&current_time) ;

  fprintf(result_file, "%s\n", hline) ;
  fprintf(result_file, "%s\n", heading) ;
  fprintf(result_file, "%s\n", ctime(&current_time)) ;
  fprintf(result_file, "%s\n", hline) ;
}

//
// Create the result file and return TRUE if successful
//
int create_result_file(char* filename)
{
  if ((result_file = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "Couldn't open result file\n") ;
    return( 0 ) ;
  }
  return( 1 ) ;
}

//
// Close the result file
//
void close_result_file()
{
  fclose( result_file ) ;
}
