#!/usr/bin/env python3

import argparse
import os
import sys

CC='gcc'
FOLD_CHSTONE='./CHStone_v1.8_130128/'
FOLD_RPISTAT='../../rpistat_code/'
EXE_RPISTAT=FOLD_RPISTAT + 'RPistat/rpistat'
FILE_RPISTAT='rpistat.txt'

file_to_compile = {
'adpcm':    'adpcm.c',
'aes':      'aes.c',
'blowfish': 'bf.c',
'dfadd':    'dfadd.c',
'dfdiv':    'dfdiv.c',
'dfmul':    'dfmul.c',
'dfsin':    'dfsin.c',
'gsm':      'gsm.c',
'jpeg':     'main.c',
'mips':     'mips.c',
'motion':   'mpeg2.c',
'sha':      'sha_driver.c'
}

class SmartFormatter(argparse.HelpFormatter):
    def _split_lines(self, text, width):
        # this is the RawTextHelpFormatter._split_lines
        if text.startswith('R|'):
            return text[2:].splitlines()
        return argparse.HelpFormatter._split_lines(self, text, width)

def get_arg_parser(description):
    return argparse.ArgumentParser(
            prog=sys.argv[0],
            description=description,
            formatter_class=SmartFormatter
        )
DESC_COMPILE='Compiles the files in the CHStone folder to optimization levels 0 thru 3.'
def get_args_compile(description=DESC_COMPILE):
    arg_parser = get_arg_parser(description)
    group = arg_parser.add_mutually_exclusive_group()
    group.add_argument('-f', '--force', action='store_true', default=False, help='forces recompilation')
    group.add_argument('-c', '--clean', action='store_true', default=False, help='removes the executables and exits')
    return arg_parser
