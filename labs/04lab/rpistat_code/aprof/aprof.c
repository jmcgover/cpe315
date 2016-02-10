/**
 * @file aprof.c
 *
 * @remark Copyright 2013 P.J. Drongowski
 * @remark Read the file COPYING
 *
 * @author Paul J. Drongowski
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/workqueue.h>
#include <linux/time.h>
#include <asm/mutex.h>


//
// Read the Secure Use and Non-secure Access Validation
// Control Register and return its value.
//
  static inline unsigned long
armv6_sunsav_read(void)
{
  u32 val;
  asm volatile("mrc p15, 0, %0, c15, c9, 0" : "=r"(val));
  return val;
}

//
// Write the Secure Use and Non-secure Access Validation
// Control Register
//
  static inline void
armv6_subsav_write(unsigned long val)
{
  asm volatile("mcr p15, 0, %0, c15, c9, 0" : : "r"(val));
}



static int __init aprofile_init(void)
{
  int err = 0 ;
  unsigned long sunsav = 0 ;
  printk ("aprofile module loaded\n") ;

  // Enable user-space access to the Performance Monitor counters
  armv6_subsav_write(0x1) ;
  // Read the Secure User and Non-secure Access Validation register
  sunsav = armv6_sunsav_read() ;

  printk ("SUNSAV: %lu\n", sunsav) ;

  return( err ) ;
}

static void __exit aprofile_exit(void)
{
  unsigned long sunsav = 0 ;

  // Disable user-space access to the Performance Monitor counters
  armv6_subsav_write(0x0) ;
  // Read the Secure User and Non-secure Access Validation register
  sunsav = armv6_sunsav_read() ;

  printk ("SUNSAV: %lu\n", sunsav) ;
  printk ("aprofile module unloading...\n") ;
}

module_init(aprofile_init);
module_exit(aprofile_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paul Drongowski");
MODULE_DESCRIPTION("Simple ARMv6 profiler");
