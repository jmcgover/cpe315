//
// ARM Performance Monitor support
//

//
// Author: P.J. Drongowski
// Date: 17 May 2013
//
// Copyright (c) 2013 P.J. Drongowski
//

#include "../test_common/rpi_pmu.h"

//
// Remember requested Performance Monitor events and the last counts
// that were read.
//
static int event_0 = 0 ;
static int event_1 = 0 ;
static uint64_t ccr = 0 ;
static uint64_t ctr0 = 0 ;
static uint64_t ctr1 = 0 ;
static int ccr_overflow = 0 ;
static int ctr0_overflow = 0 ;
static int ctr1_overflow = 0 ;


//
// These routines directly access the ARM V6 PMU from user space. User space
// access must first be enabled by loading the "aprofile" kernel module.
//

//
// Return a pointer to the ASCII event name. Return NULL if the event
// identifier is not valid.
//
static char* lookup_event(int event)
{
  switch( event ) {
    case ARMV6_EVENT_ICACHE_MISS :
      return( "Instruction cache miss" ) ;
    case ARMV6_EVENT_IBUF_STALL:
      return( "Instruction buffer stall" ) ;
    case ARMV6_EVENT_DDEP_STALL:
      return( "Data dependency stall" ) ;
    case ARMV6_EVENT_ITLB_MISS:
      return( "Instruction MicroTLB miss" ) ;
    case ARMV6_EVENT_DTLB_MISS:
      return( "Data MicroTLB miss" ) ;
    case ARMV6_EVENT_BR_EXEC:
      return( "Executed branch instruction" ) ;
    case ARMV6_EVENT_BR_MISPREDICT:
      return( "Mispredicted branch" ) ;
    case ARMV6_EVENT_INSTR_EXEC:
      return( "Executed instruction" ) ;
    case ARMV6_EVENT_DCACHE_CACCESS:
      return( "Data cache access (cacheable)" ) ;
    case ARMV6_EVENT_DCACHE_ACCESS:
      return( "Data cache access" ) ;
    case ARMV6_EVENT_DCACHE_MISS:
      return( "Data cache miss" ) ;
    case ARMV6_EVENT_DCACHE_WBACK:
      return( "Data cache writeback" ) ;
    case ARMV6_EVENT_SW_PC_CHANGE:
      return( "Software PC change" ) ;
    case ARMV6_EVENT_MAIN_TLB_MISS:
      return( "Main TLB miss" ) ;
    case ARMV6_EVENT_EXPL_D_ACCESS:
      return( "Explicit external data access" ) ;
    case ARMV6_EVENT_LSU_FULL_STALL:
      return( "Load Store Unit request queue full stall" ) ;
    case ARMV6_EVENT_WBUF_DRAINED:
      return( "Write buffer drained" ) ;
    case ARMV6_EVENT_CPU_CYCLES:
      return( "CPU cycles") ;
    case ARMV6_EVENT_NOP:
      return( "Nop" ) ;
  }
  return( NULL ) ;
}

//
// Clear the performance counters and start counting. Always count
// process cycles. event_a and event_b must specify a valid ARM V6
// performance event (e.g., ARMV6_EVENT_INSTR_EXEC).
//
void start_counting(int evt0, int evt1)
{
  ccr = 0 ;
  ctr0 = 0 ;
  ctr1 = 0 ;
  ccr_overflow = 0 ;
  ctr0_overflow = 0 ;
  ctr1_overflow = 0 ;
  event_0 = evt0 ;
  event_1 = evt1 ;

  if (lookup_event(evt0) == NULL) {
    fprintf(stderr, "*warning* Unknown PM event was requested (counter 0)\n") ;
    event_0 = ARMV6_EVENT_NOP ;
  }

  if (lookup_event(evt1) == NULL) {
    fprintf(stderr, "*warning* Unknown PM event was requested (counter 1)\n") ;
    event_1 = ARMV6_EVENT_NOP ;
  }

  // Clear the sticky overflow bits
  armv6_pmcr_write(ARMV6_PMCR_CCOUNT_OVERFLOW |
      ARMV6_PMCR_COUNT0_OVERFLOW |
      ARMV6_PMCR_COUNT1_OVERFLOW
      ) ;

  // Clear and start the performance counters
  armv6_pmcr_write(ARMV6_PMCR_ENABLE |
      ARMV6_PMCR_CCOUNT_RESET |
      ARMV6_PMCR_CCOUNT_DIV |
      ARMV6_PMCR_CTR01_RESET |
      (event_0 << ARMV6_PMCR_EVT_COUNT0_SHIFT) |
      (event_1 << ARMV6_PMCR_EVT_COUNT1_SHIFT)
      ) ;
}

void stop_counting(void)
{
  uint32_t pmcr = 0 ;

  // Stop the performance counters
  armv6_pmcr_write(0) ;

  // Read the PMU control register
  pmcr = armv6_pmcr_read() ;
#ifdef _DEBUG
  fprintf(stderr, "PMCR: %lx\n", pmcr) ;
#endif

  // Extract counter overflow flags
  ccr_overflow = (pmcr & ARMV6_PMCR_CCOUNT_OVERFLOW) != 0 ;
  ctr0_overflow = (pmcr & ARMV6_PMCR_COUNT0_OVERFLOW) != 0 ;
  ctr1_overflow = (pmcr & ARMV6_PMCR_COUNT1_OVERFLOW) != 0 ;

  // Read and accumulate the event counts
  ccr += armv6pmu_read_counter(ARMV6_CYCLE_COUNTER) ;
  ctr0 += armv6pmu_read_counter(ARMV6_COUNTER0) ;
  ctr1 += armv6pmu_read_counter(ARMV6_COUNTER1) ;
}

void get_counts(uint64_t* cycles, uint64_t* evt0, uint64_t* evt1)
{
  if (cycles != NULL) {
    *cycles = ccr ;
  }

  if (evt0 != NULL) {
    *evt0 = ctr0 ;
  }

  if (evt1 != NULL) {
    *evt1 = ctr1 ;
  }
}

//
// Temporarily stop the counters. Read the event counts.
// Accumulate the counts into the static event variables.
// Clear the counters and restart counting. Assume that
// the overflow bits are sticky and remain set/valid.
//
void accumulate_counts(void)
{
  uint32_t pmcr = 0 ;

  // Stop the performance counters
  armv6_pmcr_write(0) ;

  // Read and accumulate the event counts
  ccr += armv6pmu_read_counter(ARMV6_CYCLE_COUNTER) ;
  ctr0 += armv6pmu_read_counter(ARMV6_COUNTER0) ;
  ctr1 += armv6pmu_read_counter(ARMV6_COUNTER1) ;

  // Clear and start the performance counters
  armv6_pmcr_write(ARMV6_PMCR_ENABLE |
      ARMV6_PMCR_CCOUNT_RESET |
      ARMV6_PMCR_CCOUNT_DIV |
      ARMV6_PMCR_CTR01_RESET |
      (event_0 << ARMV6_PMCR_EVT_COUNT0_SHIFT) |
      (event_1 << ARMV6_PMCR_EVT_COUNT1_SHIFT)
      ) ;
}

//
// Print the event counts (as saved/accumulated in the
// static event variables) to the specified file.
//
void print_counts(FILE* result_file)
{
  char* event_name_0 = NULL ;
  char* event_name_1 = NULL ;

  fprintf(result_file, " Cycle Counter: %llu ", ccr) ;
  if (ccr_overflow) {
    fprintf(result_file, "(OVERFLOW) ") ;
  }
  fprintf(result_file, " Processor cycles (scaled by 64)\n") ;

  if ((event_name_0 = lookup_event(event_0)) == NULL) {
    fprintf(stderr, "*warning* Unknown PM event (0) when printing\n") ;
    fprintf(result_file, " Counter 0: *warning*\n") ;
    return ;
  } else {
    fprintf(result_file, " Counter 0: %llu ", ctr0) ;
    if (ctr0_overflow) {
      fprintf(result_file, "(OVERFLOW) ") ;
    }
    fprintf(result_file, " %s\n", event_name_0) ;
  }

  if ((event_name_1 = lookup_event(event_1)) == NULL) {
    fprintf(stderr, "*warning* Unknown PM event (1) when printing\n") ;
    fprintf(result_file, " Counter 1: *warning*\n") ;
    return ;
  } else {
    fprintf(result_file, " Counter 1: %llu ", ctr1) ;
    if (ctr1_overflow) {
      fprintf(result_file, "(OVERFLOW) ") ;
    }
    fprintf(result_file, " %s\n", event_name_1) ;
  }
}
