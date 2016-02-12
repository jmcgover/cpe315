#!/usr/bin/env python3

import errno
import os
import sys
import subprocess
from subprocess import call

sys.path.append(os.getcwd())
import lab4
from lab4 import FOLD_CHSTONE
from lab4 import EXE_RPISTAT
from lab4 import FILE_RPISTAT
from lab4 import file_to_compile

def main():
    os.chdir(FOLD_CHSTONE)
    for file in os.listdir('./'):
        if os.path.isdir(file):
            test_name = file
            print('Entering {}...'.format(test_name))
            os.chdir(file)
            for i in range(0,3 + 1):
                opt_level = 'O{}'.format(i)
                exe_name = './{}-{}'.format(test_name, opt_level)
                exe_prof_name = './{}-prof-O{}'.format(test_name, i)
                assert(os.path.exists(exe_name))

                # BEGIN PERF STAT
                lab_res_filename = '{}_{}_{}.txt'.format('labstat',test_name,opt_level)
                gpf_res_filename = '{}_{}_{}.txt'.format('labgprof',test_name,opt_level)
                #os.rename(FILE_RPISTAT, rpi_res_filename)
                with open(lab_res_filename, 'w') as file:
                    call(['perf','stat', exe_name], stderr=file)
                with open(lab_res_filename, 'a') as file:
                    for j in range(10):
                        #call(['/usr/bin/env','time', '-f', 'Run {}: %e'.format(i), exe_name], stdout=file)
                        #call(['time', exe_name], stderr=file, shell=True)
                        subprocess.Popen(['/bin/bash', '-c', 'time {}'.format(exe_name)], stderr=file)
                # END   PERF STAT
                with open(gpf_res_filename, 'w') as file:
                    print('Running {}'.format(exe_prof_name))
                    subprocess.call(['gprof', exe_prof_name], stdout=file)
            os.chdir('../')
    os.chdir('../')
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)
