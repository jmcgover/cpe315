//
// Common test infrastructure routines
//

//
// Author: P.J. Drongowski
// Date: 22 April 2013
//
// Copyright (c) 2013 P.J. Drongowski
//

#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

#ifdef WIN32
#define FMT64 "I64"
#else
#define FMT64 "ll"
#endif

//
// Common command line options
//
extern int d_flag ; // Display debugging output when TRUE
extern int h_flag ; // Display usage/help information then TRUE
extern int v_flag ; // Display more information when TRUE

//
// Write test summary to this file
//
extern FILE *result_file ;

//
// Test utility functions
//
extern void display_usage_info(char* prog, char* usage_strings[]) ;
extern int set_process_affinity(int core) ;
extern unsigned long long int getTimeStamp() ;
extern void print_system_info() ;
extern void print_process_times() ;
extern double get_elapsed_time() ;
extern void print_elapsed_time() ;
extern void print_heading(char* heading) ;
extern int create_result_file(char* filename) ;
extern void close_result_file() ;

#endif
