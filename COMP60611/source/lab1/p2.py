#!/usr/bin/python
import os
import socket
import struct
import binascii
import thread
import sys
import re
#from Tkinter import *



class test1:
    def __init__(self):
        pass
    def proc(self,str):
        sum1 = 0
        sum2 = 0
        c1 = 0
        c2 = 0
        rlst = [i for i in range(2),[]]
        fhr = open(str,'r')
        num1 = re.match(r'\D+(\d+).*',str)
        if num1:
            str0 = num1.group(1)
            print str0
        fhw1 = open("v"+str0+"result.txt",'w')
        for line in fhr:
          #  num1 = re.match(r'.*nest\s+(1)\s+=\s+(\S+E\S+)\s+second.*',line)
            
            num1 = re.match(r'\s+(\d+)\s+(\S+)\s+(\S+)\s*',line)
            if num1:
                fhw1.write(num1.group(1)+" "+num1.group(2)+" "+num1.group(3)+"\n")
        
        fhr.close()
        fhw1.close()
    
    def do_average(self,ar):
        sum = 0
        for i in range (len(ar)):
            sum = sum + ar[i]
        avgr = sum/len(ar)
        print len(ar)
        print avgr

if __name__ == "__main__":
    app = test1()
    app.proc(sys.argv[1])
    #root  = Tk()
    #root.mainloop()
    #root.destroy()

