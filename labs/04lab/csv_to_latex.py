#!/usr/bin/env python3

import argparse
import errno
import os
import sys

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
DESC_CSV_TO_LATEX='Converts CSV files to LaTeX'
def get_args_csv_to_latex(description=DESC_CSV_TO_LATEX):
    arg_parser = get_arg_parser(description)
    arg_parser.add_argument('filenames', nargs=argparse.REMAINDER)
    return arg_parser

class CSV(object):
    def __init__(self, filename, delim=','):
        self.filename = None
        self.header = None
        self.data = None
        self.num_cols = 0
        header = None
        data = []
        num_cols = 0
        with open(filename, 'r') as file:
            header = file.readline().split(delim)
            num_cols = len(header)
            for line in file:
                data.append(line.split(delim))

        self.filename = filename
        self.header = header
        self.data = data
        self.num_cols = num_cols
        assert(self.filename)
        assert(self.header)
        assert(self.data)
        assert(self.num_cols)
    def print_to_latex(file=sys.stdout):
        return

def main():
    args_parser = get_args_csv_to_latex()
    args = args_parser.parse_args()
    for filename in args.filenames:
        CSV(filename)
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)