#!/usr/bin/env python3
"""
    Jeff McGovern
    CPE 315, Winter 2016
    Lab 04: Performance Measurement
    Parses the lab benchmark result files and outputs a csv file as per the lab
    instructions. Leaves the function empty, 'cause I don't wanna parse that.
"""
import errno
import os
import sys
import subprocess
#from subprocess import run

import collections
from collections import defaultdict

sys.path.append(os.getcwd())
import lab4
from lab4 import FOLD_CHSTONE

def main():
    os.chdir(FOLD_CHSTONE)
    for file in os.listdir('./'):
        if os.path.isdir(file):
            test_name = file
            print('Entering {}...'.format(test_name))
            os.chdir(file)
            results = {}
            for i in range(0,3 + 1):
                opt_level = 'O{}'.format(i)
                exe_name = './{}-{}'.format(test_name, opt_level)
                lab_res_filename = '{}_{}_{}.txt'.format('labstat',test_name,opt_level)
                print('\tParsing {}...'.format(lab_res_filename))
                results[opt_level] = {}
                results[opt_level]['time'] = 0.0
                results[opt_level]['runs'] = 0
                with open(lab_res_filename) as file:
                    for line in file:
                        tuple = [x.strip().strip(':[]') for x in line.replace('#','').split()]
                        if not len(tuple):
                            continue
                        if tuple[0] == 'real':
                            minutes, seconds = tuple[1].replace('s','').split('m')
                            time = 60 * float(minutes) + float(seconds)
                            results[opt_level]['time'] += time
                            results[opt_level]['runs'] += 1
                            results[opt_level]['avg'] = results[opt_level]['time'] / results[opt_level]['runs']
                        if tuple[1] == 'cycles':
                            results[opt_level]['cycles'] = int(tuple[0].replace(',',''))
                            results[opt_level]['calculated'] = results[opt_level]['cycles'] * \
                                    1/(float(tuple[2]) * 1000000000)
                        if tuple[1] == 'instructions':
                            results[opt_level]['instructions'] = int(tuple[0].replace(',',''))
                            results[opt_level]['cpi'] = 1 / float(tuple[2])
                cycles_per_second = 700 * 1000000000
            with open('{}-lab.csv'.format(test_name), 'w') as file:
                print('{},{},{},{},{}'.format(test_name + '-lab', 'O0', 'O1', 'O2','O3'),file=file)
                key = 'cpi'
                print('{},{:.6f},{:.6f},{:.6f},{:.6f}'.format('Average CPI',results['O0'][key],results['O1'][key],results['O2'][key],results['O3'][key]),file=file)
                key = 'instructions'
                print('{},{},{},{},{}'.format('Instructions',results['O0'][key],results['O1'][key],results['O2'][key],results['O3'][key]),file=file)
                key = 'avg'
                print('{},{:.6f},{:.6f},{:.6f},{:.6f}'.format('Runtime(measured)',results['O0'][key],results['O1'][key],results['O2'][key],results['O3'][key]),file=file)
                key = 'calculated'
                print('{},{:.6f},{:.6f},{:.6f},{:.6f}'.format('Runtime(calculated)',results['O0'][key],results['O1'][key],results['O2'][key],results['O3'][key]),file=file)
                print('{},,,,'.format('Function',results['O0'][key],results['O1'][key],results['O2'][key],results['O3'][key]),file=file)
            os.chdir('../')
    os.chdir('../')
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)

