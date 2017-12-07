#! /usr/bin/env python

import os

def write_txt():
    ''' 
Write each line from stdin to make a txt file. 
    '''
    file_name = raw_input("Please input file name:").lstrip()
    if os.path.exists(file_name):
        print "file %s already exists!" % file_name
        return False

    fp = file(file_name, "w")
    
    lines = []
    while True:
        line = raw_input(">")
        if line == ".":
            break
        #print >> fp, line
        lines.append(line)

    fp.writelines(["%s%s" % (x, os.linesep) for x in lines])

    fp.close()

def read_txt():
    '''
    Read strings from a txt file.
    '''
    file_name = raw_input("Please input file name: ").lstrip()
    #if not os.path.isfile(file_name):
    #    print "file %s is not exists!" % file_name
    #    quit()

    try:
        fp = file(file_name, "r")
    except IOError, e:
        print "file %s open error:\n %s" %  (file_name, e)
    else:
        #for i in fp.readlines():
        for i in fp:
            print i,
        fp.close()

if __name__ == "__main__":
    #write_txt()
    #read_txt()
