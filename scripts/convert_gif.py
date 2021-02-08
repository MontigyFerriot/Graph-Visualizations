#!/usr/bin/env python3

import sys, getopt
import glob, os, re
import subprocess

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]

def parse_opts(argv):
    ifile = ''
    ofile = ''
    extension = ''
    delay = ''

    help = 'convert_gif.py --extension=[] -i <inputfile> -o <outputfile>'

    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["help", "delay=", "ofile=", "extension="])
    except getopt.GetoptError:
        print(help)
        sys.exit(2)

    for opt, arg in opts:
        if opt in ('-h', '--help'):
            print(help)
            sys.exit()
        elif opt in ("--delay="):
            delay = arg
        elif opt in ("-o", "--ofile"):
            ofile = arg
        elif opt in ("--extension="):
            extension = '.' + arg

    create_gif(extension, ofile, delay)

def create_gif(extension, ofile, delay):
    files = []

    for file in glob.glob('*' + extension):
        files.append(file)

    files.sort(key=natural_keys)

    cmdargs = ''

    for file in files:
        cmdargs += file
        cmdargs += ' '

    os.system('convert -delay ' + delay + ' -loop 0 ' + cmdargs + ofile)

if __name__ == "__main__":
    parse_opts(sys.argv[1:])

