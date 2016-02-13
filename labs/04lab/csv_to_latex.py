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
        self.shortname = None
        self.basename = None
        self.header = None
        self.data = None
        self.num_cols = 0

        shortname = filename.split('/')[-1].replace('.csv','')
        basename = filename.replace('.csv','')
        header = None
        data = []
        num_cols = 0
        with open(filename, 'r') as file:
            header = file.readline().strip().split(delim)
            num_cols = len(header)
            for line in file:
                data.append(line.strip().split(delim))

        self.filename = filename
        self.shortname = shortname
        self.basename = basename
        self.header = header
        self.data = data
        self.num_cols = num_cols
        assert(self.filename)
        assert(self.shortname)
        assert(self.basename)
        assert(self.header)
        assert(self.data)
        assert(self.num_cols)
    def print_to_latex(self, file=sys.stdout):
        delim = '\t&\t'
        print('\\begin{table}[ht!]',file=file)
        print('\\centering',file=file)
        print('\\caption{{\\texttt{{{}}}}}'.format(self.shortname),file=file)
        print('\\label{{tab:{}}}'.format(self.shortname),file=file)
        print('\\begin{{tabular}}{{|{}|}}'.format('|'.join(map(lambda x: 'l', self.header))),file=file)
        print('\\hline',file=file)
        print('{}\t\\\\\\hline\\hline'.format(delim.join(map(lambda x:'\\textbf{{{}}}'.format(x),self.header))), file=file)
        for datum in self.data:
            print('{}\t\\\\\\hline'.format(delim.join(datum)), file=file)
        print('\\end{tabular}',file=file)
        print('\\end{table}',file=file)

def main():
    args_parser = get_args_csv_to_latex()
    args = args_parser.parse_args()
    for filename in args.filenames:
        csv = CSV(filename)
        tex_name = '{}.tex'.format(csv.basename)
        print('Converting {} to {}...'.format(csv.filename, tex_name))
        with open(tex_name, 'w') as file:
            csv.print_to_latex(file)
    return 0

if __name__ == '__main__':
    rtn = main()
    sys.exit(rtn)
