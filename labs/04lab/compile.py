#!/usr/bin/env python3

import errno
import os
import sys
import subprocess
from subprocess import run

sys.path.append(os.getcwd())
import lab4
from lab4 import CC
from lab4 import FOLD_CHSTONE
from lab4 import file_to_compile
from lab4 import get_args_compile

def main():
    os.chdir(FOLD_CHSTONE)
    arg_parser = get_args_compile()
    args = arg_parser.parse_args()
    for file in os.scandir('./'):
        if file.is_dir():
            test_name = file.name
            c_file = file_to_compile[test_name]
            print('Entering {}({})...'.format(test_name, c_file))
            os.chdir(file.name)
            for i in range(0,3 + 1):
                exe_name = '{}-O{}'.format(test_name, i)
                opt_flag = '-O{}'.format(i)
                if args.clean:
                    if os.path.exists(exe_name):
                        print('\tFound {} --- removing...'.format(exe_name))
                        os.remove(exe_name)
                    continue
                if not args.force and os.path.exists(exe_name):
                    print('\tFound {} --- skipping {}.'.format(exe_name, opt_flag))
                    continue
                print('\tBuilding {}...'.format(exe_name))
                # BEGIN COMPILE
                run([CC, opt_flag, '-o', exe_name, c_file])
                # END   COMPILE
            os.chdir('../')
    os.chdir('../')
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)
