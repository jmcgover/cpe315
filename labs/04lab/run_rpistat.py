#!/usr/bin/env python3

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
                assert(os.path.exists(exe_name))
                run([EXE_RPISTAT, exe_name])
                rpi_res_filename = '{}_{}_{}.txt'.format('rpistat',test_name,opt_level)
                os.rename(FILE_RPISTAT, rpi_res_filename)
            os.chdir('../')
    os.chdir('../')
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)
