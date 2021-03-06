#!/usr/bin/env python3
"""
    Jeff McGovern
    CPE 315, Winter 2016
    Lab 04: Performance Measurement
    Runs the rpistat and gprof command on the raspberry pi benchmarks, saving
    their output into a txt file.
"""
import errno
import os
import sys
import subprocess
from subprocess import run

sys.path.append(os.getcwd())
import lab4
from lab4 import FOLD_CHSTONE
from lab4 import EXE_RPISTAT
from lab4 import FILE_RPISTAT
from lab4 import file_to_compile

def main():
    os.chdir(FOLD_CHSTONE)
    for file in os.scandir('./'):
        if file.is_dir():
            test_name = file.name
            print('Entering {}...'.format(test_name))
            os.chdir(file.name)
            for i in range(0,3 + 1):
                opt_level = 'O{}'.format(i)
                exe_name = './{}-{}'.format(test_name, opt_level)
                exe_prof_name = './{}-prof-O{}'.format(test_name, i)
                assert(os.path.exists(exe_name))

                # BEGIN RPISTAT
                run([EXE_RPISTAT, exe_name])
                # END   RPISTAT
                rpi_res_filename = '{}_{}_{}.txt'.format('rpistat',test_name,opt_level)
                gpf_res_filename = '{}_{}_{}.txt'.format('rpigprof',test_name,opt_level)
                os.rename(FILE_RPISTAT, rpi_res_filename)
                with open(rpi_res_filename, 'a') as file:
                    for j in range(10):
                        #run(['/usr/bin/env','time', '-f', 'Run {}: %e'.format(i), exe_name], stdout=file)
                        #run(['time', exe_name], stderr=file, shell=True)
                        subprocess.Popen(['/bin/bash', '-c', 'time {}'.format(exe_name)], stderr=file)
                with open(gpf_res_filename, 'w') as file:
                    print('Running {}'.format(exe_prof_name))
                    subprocess.run(['gprof', exe_prof_name], stdout=file)
            os.chdir('../')
    os.chdir('../')
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)
