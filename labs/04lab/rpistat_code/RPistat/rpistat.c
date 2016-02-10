//
// rpistat: Run command and measure PMU events
//

//
// Author: Paul J. Drongowski
// Date: 27 June 2013
//
// Copyright (c) 2013 Paul J. Drongowski
//

#include <sys/types.h>
#include <sys/wait.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../test_common/test_common.h"
#include "../test_common/rpi_pmu.h"

#define RESULT_FILE_NAME "rpistat.txt"
char heading[256] = "rpistat: " ;

int event_set = 0 ;

//
// Number of measurement periods
//
uint64_t periods = 0 ; // All periods
uint64_t iperiods = 0 ; // Instruction periods
uint64_t dperiods = 0 ; // DC periods
uint64_t tperiods = 0 ; // TLB periods
uint64_t bperiods = 0 ; // Branch periods

//
// Cumulative event counts
//
uint64_t cycles = 0 ;
uint64_t instructions = 0 ;
uint64_t ibuf_stalls = 0 ;
uint64_t branches = 0 ;
uint64_t mispredicts = 0 ;
uint64_t data_cache_accesses = 0 ;
uint64_t data_cache_misses = 0 ;
uint64_t micro_tlb_misses = 0 ;
uint64_t main_tlb_misses = 0 ;

//
// Scale the event count by the reciprocal of the
// active event fraction. Return the scaled event
// count as a double.
//
double dscale(uint64_t count, uint64_t event_period)
{
  double factor = ((double)periods/(double)event_period) ;
  return( factor * (double)count ) ;
}

//
// Scale the event count by the reciprocal of the
// active event fraction. Return the scaled event
// count as an unsigned 64-bit int.
//
uint64_t scale(uint64_t count, uint64_t event_period)
{
  double factor = ((double)periods/(double)event_period) ;
  return( (uint64_t)(factor * (double)count) ) ;
}

//
// Switch between event sets. Accumulate current
// event counts, then reconfigure the performance
// counters for the next event set.
//
static void handle_event_sets()
{
  int event_0, event_1 ;

  // Increment the number of measurement periods
  periods++ ;

  // Accumulate the Cycle Counter Register
  cycles += armv6pmu_read_counter(ARMV6_CYCLE_COUNTER) ;

  switch( event_set ) {
    case 0:
      instructions += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      ibuf_stalls += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      iperiods++ ;
      //Advance to next event set
      event_set = 1 ;
      event_0 = ARMV6_EVENT_DCACHE_CACCESS ;
      event_1 = ARMV6_EVENT_DCACHE_MISS ;
      break ;
    case 1:
      data_cache_accesses += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      data_cache_misses += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      dperiods++ ;
      //Advance to next event set
      event_set = 2 ;
      event_0 = ARMV6_EVENT_DTLB_MISS ;
      event_1 = ARMV6_EVENT_MAIN_TLB_MISS ;
      break ;
    case 2:
      micro_tlb_misses += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      main_tlb_misses += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      tperiods++ ;
      //Advance to next event set
      event_set = 3 ;
      event_0 = ARMV6_EVENT_BR_EXEC ;
      event_1 = ARMV6_EVENT_BR_MISPREDICT ;
      break ;
    default:
      branches += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      mispredicts += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      bperiods++ ;
      //Advance to next event set
      event_set = 0 ;
      event_0 = ARMV6_EVENT_INSTR_EXEC ;
      event_1 = ARMV6_EVENT_IBUF_STALL ;
      break ;
  }

  // Clear and start the performance counters
  armv6_pmcr_write(ARMV6_PMCR_ENABLE |
      ARMV6_PMCR_CCOUNT_RESET |
      // ARMV6_PMCR_CCOUNT_DIV |
      ARMV6_PMCR_CTR01_RESET |
      (event_0 << 
       ARMV6_PMCR_EVT_COUNT0_SHIFT) |
      (event_1 << 
       ARMV6_PMCR_EVT_COUNT1_SHIFT)
      ) ;
}

//
// Initialize the period counts and the ARM1176
// performance counters with the first event set.
// Start the counters.
//
static void handle_first_event_set()
{
  int event_0, event_1 ;

  periods = 0 ;
  iperiods = 0 ;
  dperiods = 0 ;
  tperiods = 0 ;
  bperiods = 0 ;

  event_set = 0 ;
  event_0 = ARMV6_EVENT_INSTR_EXEC ;
  event_1 = ARMV6_EVENT_IBUF_STALL ;

  // Clear the sticky PMU overflow bits
  armv6_pmcr_write(ARMV6_PMCR_CCOUNT_OVERFLOW |
      ARMV6_PMCR_COUNT0_OVERFLOW |
      ARMV6_PMCR_COUNT1_OVERFLOW
      ) ;

  // Clear and start the performance counters
  armv6_pmcr_write(ARMV6_PMCR_ENABLE |
      ARMV6_PMCR_CCOUNT_RESET |
      // ARMV6_PMCR_CCOUNT_DIV |
      ARMV6_PMCR_CTR01_RESET |
      (event_0 << 
       ARMV6_PMCR_EVT_COUNT0_SHIFT) |
      (event_1 << 
       ARMV6_PMCR_EVT_COUNT1_SHIFT)
      ) ;
}

//
// Handle the last event set after the workload is
// finished. Accumulate the event counts, and
// check for counter overflow. Do not restart the
// counters.
//
static void handle_last_event_set()
{
  uint32_t pmcr = 0 ;

  // Increment the number of measurement periods
  periods++ ;

  // Accumulate the Cycle Counter Register
  cycles += armv6pmu_read_counter(ARMV6_CYCLE_COUNTER) ;

  switch( event_set ) {
    case 0:
      instructions += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      ibuf_stalls += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      iperiods++ ;
      break ;
    case 1:
      data_cache_accesses += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      data_cache_misses += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      dperiods++ ;
      break ;
    case 2:
      micro_tlb_misses += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      main_tlb_misses += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      tperiods++ ;
      break ;
    default:
      branches += armv6pmu_read_counter(ARMV6_COUNTER0) ;
      mispredicts += armv6pmu_read_counter(ARMV6_COUNTER1) ;
      bperiods++ ;
      break ;
  }

  pmcr = armv6_pmcr_read() ;
  if (armv6_pmcr_has_overflowed(pmcr)) {
    fprintf(result_file, "**********************************\n") ;
    fprintf(result_file, " WARNING! Overflow detected!\n") ;
    fprintf(result_file, "**********************************\n") ;
  }
}

int main(int argc, char* argv[])
{
  int status = -1 ;
  pid_t child = -1 ;
  pid_t pid = -1 ;
  double scaled_instructions = 0 ;

  if (argc < 2) {
    fprintf(stderr, "*fatal* Need the command to be exected\n") ;
    fprintf(stderr, "Usage: pmustat command [arguments]\n") ;
  }

  // Store final event counts in the result file
  if (create_result_file(RESULT_FILE_NAME) == 0) {
    exit( EXIT_FAILURE ) ;
  }
  setlocale(LC_NUMERIC, "") ;
  print_heading(strcat(heading, argv[1])) ;
  fprintf(result_file,"\n") ;
  print_system_info() ;

  // Fork/exec the workload in a child process
  if ((child = fork()) == 0) {
    // Child
    if ((status = execvp(argv[1], &argv[1])) == -1) {
      fprintf(stderr, "*fatal* execvp() failed\n") ;
      exit( EXIT_FAILURE ) ;
    }
  } else {
    // Parent
    if (child == -1) {
      fprintf(stderr, "*fatal* fork() failed\n") ;
      exit( EXIT_FAILURE ) ;
    }

    handle_first_event_set() ;
    for(;;) {
      if ((pid = waitpid(child, 0, WNOHANG)) == 0) {
        // Child is still running
        usleep(100000) ;
        // Stop the performance counters
        armv6_pmcr_write(0) ;
        handle_event_sets() ;
      } else if (pid == child) {
        // Child terminated
        // Stop the performance counters
        armv6_pmcr_write(0) ;
        handle_last_event_set() ;
        // fprintf(stdout, "Workload terminated\n") ;
        break ;
      } else if (pid == -1) {
        // Error
        fprintf(stderr, "*error* waitpid failed\n") ;
        perror("waitpid") ;
      } else {
        armv6_pmcr_write(0) ;
        fprintf(stderr, "*fatal* Unexpected return from waitpid()\n") ;
        exit( EXIT_FAILURE ) ;
      }
    }
  }

  // Write event counts to the results file
  fprintf(result_file, "\nPerformance events\n") ;
  fprintf(result_file, " [ ... ] = scaled event count\n") ;
  fprintf(result_file, " PTI = per thousand instructions\n") 
    ;
  fprintf(result_file, " Total periods: %lld\n", periods) 
    ;

  // Cycles are counted during all measurement periods
  scaled_instructions = dscale(instructions,iperiods) ;
  fprintf(result_file, "\n") ;
  fprintf(result_file, " Cycles: %'lld\n", 
      cycles) ;
  fprintf(result_file, " Instructions: %'lld", 
      instructions) ;
  fprintf(result_file, " [%'lld]\n", scale(instructions, iperiods)) 
    ;
  fprintf(result_file, " IBUF stall cycles: %'lld", ibuf_stalls) ;
  fprintf(result_file, " [%'lld]\n", scale(ibuf_stalls, iperiods)) ;
  fprintf(result_file, " Instr periods: %lld\n", iperiods) 
    ;
  if (instructions != 0) {
    fprintf(result_file, " CPI: %-7.3f\n",
        (double)cycles / scaled_instructions) ;
  }
  fprintf(result_file, " IBUF stall percent: %-7.3f\%\n",
      (dscale(ibuf_stalls,iperiods) / (double)cycles) * 
      100.0) ;

  fprintf(result_file, "\n") ;
  fprintf(result_file, " DC cached accesses: %'lld", 
      data_cache_accesses) ;
  fprintf(result_file, " [%'lld]\n", scale(data_cache_accesses, 
        dperiods)) ;
  fprintf(result_file, " DC misses: %'lld", 
      data_cache_misses) ;
  fprintf(result_file, " [%'lld]\n", scale(data_cache_misses, 
        dperiods)) ;
  fprintf(result_file, " DC periods: %lld\n", 
      dperiods) ;
  if (data_cache_accesses != 0) {
    fprintf(result_file, " DC miss ratio: %-7.3f\%\n",
        ((double)data_cache_misses / 
         (double)data_cache_accesses) * 100.0) ;
  }

  fprintf(result_file, "\n") ;
  fprintf(result_file, " MicroTLB misses: %'lld", 
      micro_tlb_misses) ;
  fprintf(result_file, " [%'lld]\n", scale(micro_tlb_misses, 
        tperiods)) ;
  fprintf(result_file, " Main TLB misses: %'lld", 
      main_tlb_misses) ;
  fprintf(result_file, " [%'lld]\n", scale(main_tlb_misses, 
        tperiods)) ;
  fprintf(result_file, " TLB periods: %lld\n", 
      tperiods) ;
  if (instructions != 0) {
    fprintf(result_file, " Micro miss rate: %-7.3f PTI\n",
        1000.0 * dscale(micro_tlb_misses,tperiods) / 
        scaled_instructions) ;
    fprintf(result_file, " Main miss rate: %-7.3f PTI\n",
        1000.0 * dscale(main_tlb_misses,tperiods) / 
        scaled_instructions) ;
  }

  fprintf(result_file, "\n") ;
  fprintf(result_file, " Branches: %'lld", 
      branches) ;
  fprintf(result_file, " [%'lld]\n", scale(branches, bperiods)) ;
  fprintf(result_file, " Mispredicted BR: %'lld", mispredicts) 
    ;
  fprintf(result_file, " [%'lld]\n", scale(mispredicts, bperiods)) ;
  fprintf(result_file, " BR periods: %lld\n", 
      bperiods) ;
  if (instructions != 0) {
    fprintf(result_file, " Branch rate: %-7.3f PTI\n",
        1000.0 * dscale(branches,bperiods) / 
        scaled_instructions) ;
  }
  if (branches != 0) {
    fprintf(result_file, " Mispredict ratio: %-7.3f\%\n",
        ((double)mispredicts / (double)branches) * 
        100.0) ;
  }

  close_result_file() ;

  return( EXIT_SUCCESS ) ;
}
