//
// ARMv6 Performance Monitor support
//

//
// Author: P.J. Drongowski
// Date: 17 May 2013
//
// This module uses code from linux/arch/arm/kernel/perf_event_v6.c.
// Copyright (C) 2009 picoChip Designs, Ltd., Jamie Iles
//

#ifndef _RPI_PMU_H_
#define _RPI_PMU_H_

#include <stdint.h>
#include <stdio.h>

#define ARMV6_EVENT_ICACHE_MISS 0x0
#define ARMV6_EVENT_IBUF_STALL 0x1
#define ARMV6_EVENT_DDEP_STALL 0x2
#define ARMV6_EVENT_ITLB_MISS 0x3
#define ARMV6_EVENT_DTLB_MISS 0x4
#define ARMV6_EVENT_BR_EXEC 0x5
#define ARMV6_EVENT_BR_MISPREDICT 0x6
#define ARMV6_EVENT_INSTR_EXEC 0x7
#define ARMV6_EVENT_DCACHE_CACCESS 0x9
#define ARMV6_EVENT_DCACHE_ACCESS 0xA
#define ARMV6_EVENT_DCACHE_MISS 0xB
#define ARMV6_EVENT_DCACHE_WBACK 0xC
#define ARMV6_EVENT_SW_PC_CHANGE 0xD
#define ARMV6_EVENT_MAIN_TLB_MISS 0xF
#define ARMV6_EVENT_EXPL_D_ACCESS 0x10
#define ARMV6_EVENT_LSU_FULL_STALL 0x11
#define ARMV6_EVENT_WBUF_DRAINED 0x12
#define ARMV6_EVENT_NOP 0x20
#define ARMV6_EVENT_CPU_CYCLES 0xFF

//
// Only the ARM1176 supports the following four events
//
#define ARMV6_EVENT_CALL_EXEC 0x23
#define ARMV6_EVENT_RET_EXEC 0x24
#define ARMV6_EVENT_RET_PREDICT 0x25
#define ARMV6_EVENT_RET_MISPREDICT 0x26


enum armv6_counters {
  ARMV6_CYCLE_COUNTER = 0,
  ARMV6_COUNTER0,
  ARMV6_COUNTER1,
};

//
// Read the Performance Monitor Control Register and return its value.
//
  static inline unsigned long
armv6_pmcr_read(void)
{
  uint32_t val;
  asm volatile("mrc p15, 0, %0, c15, c12, 0" : "=r"(val));
  return val;
}

//
// Write the Performance Monitor Control Register using the specified value.
//
  static inline void
armv6_pmcr_write(unsigned long val)
{
  asm volatile("mcr p15, 0, %0, c15, c12, 0" : : "r"(val));
}

//
// Define bit fields within the Performance Monitor Control Register (PMCR).
//
#define ARMV6_PMCR_ENABLE (1 << 0)
#define ARMV6_PMCR_CTR01_RESET (1 << 1)
#define ARMV6_PMCR_CCOUNT_RESET (1 << 2)
#define ARMV6_PMCR_CCOUNT_DIV (1 << 3)
#define ARMV6_PMCR_COUNT0_IEN (1 << 4)
#define ARMV6_PMCR_COUNT1_IEN (1 << 5)
#define ARMV6_PMCR_CCOUNT_IEN (1 << 6)
#define ARMV6_PMCR_COUNT0_OVERFLOW (1 << 8)
#define ARMV6_PMCR_COUNT1_OVERFLOW (1 << 9)
#define ARMV6_PMCR_CCOUNT_OVERFLOW (1 << 10)
#define ARMV6_PMCR_EVT_COUNT0_SHIFT 20
#define ARMV6_PMCR_EVT_COUNT0_MASK (0xFF << ARMV6_PMCR_EVT_COUNT0_SHIFT)
#define ARMV6_PMCR_EVT_COUNT1_SHIFT 12
#define ARMV6_PMCR_EVT_COUNT1_MASK (0xFF << ARMV6_PMCR_EVT_COUNT1_SHIFT)

#define ARMV6_PMCR_OVERFLOWED_MASK \
  (ARMV6_PMCR_COUNT0_OVERFLOW | ARMV6_PMCR_COUNT1_OVERFLOW | \
   ARMV6_PMCR_CCOUNT_OVERFLOW)

  static inline int
armv6_pmcr_has_overflowed(unsigned long pmcr)
{
  return pmcr & ARMV6_PMCR_OVERFLOWED_MASK;
}

//
// Test the specified Performance Monitor counter for an overflow.
//
  static inline int
armv6_pmcr_counter_has_overflowed(unsigned long pmcr,
    enum armv6_counters counter)
{
  int ret = 0;

  if (ARMV6_CYCLE_COUNTER == counter)
    ret = pmcr & ARMV6_PMCR_CCOUNT_OVERFLOW;
  else if (ARMV6_COUNTER0 == counter)
    ret = pmcr & ARMV6_PMCR_COUNT0_OVERFLOW;
  else if (ARMV6_COUNTER1 == counter)
    ret = pmcr & ARMV6_PMCR_COUNT1_OVERFLOW;
  else
    fprintf(stderr, "Invalid counter number (%d)\n", counter) ;

  return ret;
}

//
// Read the specified Performance Monitor counter and return its value.
//
  static inline uint32_t
armv6pmu_read_counter(int counter)
{
  unsigned long value = 0;

  if (ARMV6_CYCLE_COUNTER == counter)
    asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(value));
  else if (ARMV6_COUNTER0 == counter)
    asm volatile("mrc p15, 0, %0, c15, c12, 2" : "=r"(value));
  else if (ARMV6_COUNTER1 == counter)
    asm volatile("mrc p15, 0, %0, c15, c12, 3" : "=r"(value));
  else
    fprintf(stderr, "Invalid counter number (%d)\n", counter) ;

  return value;
}

//
// Write a value into the specified Performance Monitor counter.
//
  static inline void
armv6pmu_write_counter(int counter,
    uint32_t value)
{
  if (ARMV6_CYCLE_COUNTER == counter)
    asm volatile("mcr p15, 0, %0, c15, c12, 1" : : "r"(value));
  else if (ARMV6_COUNTER0 == counter)
    asm volatile("mcr p15, 0, %0, c15, c12, 2" : : "r"(value));
  else if (ARMV6_COUNTER1 == counter)
    asm volatile("mcr p15, 0, %0, c15, c12, 3" : : "r"(value));
  else
    fprintf(stderr, "Invalid counter number (%d)\n", counter) ;
}


extern void start_counting(int evt0, int evt1) ;
extern void stop_counting(void) ;
extern void get_counts(uint64_t* cycles, uint64_t* evt0, uint64_t* evt1) ;
extern void accumulate_counts(void) ;
extern void print_counts(FILE* result_file) ;

#endif
