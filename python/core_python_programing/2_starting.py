#! /usr/bin/env python

##2-5:
#print "while: "
#var = 0
#while var <= 10:
#    print var,
#    var = var + 1
#print
#
#print "for: "
#
#for var in range(11):
#    print var, 

##2-6:
##var = 10
#var = input("Please input num:")
#
#if var < 0:
#    print "%d < 0" % var
#elif var == 0:
#    print "%d = 0" % var
#else:
#    print "%d > 0" % var

##2-7:
#string = raw_input("Please input string:")
#
#idx = 0
#while idx < len(string):
#    print string[idx]
#    idx = idx + 1
#print
#
#for ch in string:
#    print ch
#print

###2-8:
##li = ["zero", "one", 2, "three",4]
#li = []
#for i in range(5):
#    li.append(input("Input Num[%d]:" % i))
#
#s = 0
#i = 0
#while i < len(li): 
#    print li[i]
#    s = s + li[i]
#    i = i + 1
#print "SUM by while is %d" % s
#
#s=0
#for i in li:
#    print i
#    s = s + i
#print "SUM by for is %d" % s

##2-9:
#li = []
#for i in range(5):
#    li.append(input("Input Num[%d]:" % i))
#
#s= 0
#for i in li:
#    s = s + i
#avg = float(s)/5
#print "avg is %f" % avg

##2-10:
#var = int(raw_input("Input num between 1 and 100:"))
#while var < 1 or var > 100:
#    var = int(raw_input("Input num between 1 and 100:"))
#print "var is %d" % var

##2-11:
#while True:
#    print "[1]: sum of five numbers"
#    print "[2]: avg of five numbers"
#    print "[X]: quit"
#    cmd = raw_input("Please input choice:")
#
#    if cmd == "X":
#        print "quit!"
#        break
#
#    s = 0
#    for i in range(5):
#        var = input("Please input num[%d]" % i)
#        s = s + var
#
#    if cmd == "1":
#        print "sum is %d" % s
#    elif cmd == "2":
#        avg = float(s) / 5
#        print "avg is %f" % avg
#    else:
#        print "unsupported cmd!"



