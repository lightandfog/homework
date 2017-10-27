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
        fhr = open(str,'r')
        fhw1 = open("vgresult.txt",'w')
        for line in fhr:
          #  num1 = re.match(r'.*nest\s+(1)\s+=\s+(\S+E\S+)\s+second.*',line)
            
            #num1 = re.match(r'\s+(\d+)\s+(\S+)\s+(\S+)\s*',line)
            num1 = re.match(r'Running\s+on\s+(\d+)\s+\S+.*',line)
            if num1:
                fhw1.write(num1.group(1)+" ")
            num1 = re.match(r'Work\s+took\s+(\S+)\s+\S+.*',line)
            if num1:
                fhw1.write(num1.group(1)+'\n')
        
        fhr.close()
        fhw1.close()
    
    def do_average(self,ar):
        sum = 0
        for i in range (len(ar)):
            sum = sum + ar[i]
        avgr = sum/len(ar)
        print len(ar)
        print avgr

    def pro2(self):
        i = 1
        str = "v3519"+repr(i)+"result.txt"
        tdict = {}
        pdict = {}
        dc = {}
        while(i < 9):
            fhr = open(str,'r')
            for line in fhr:
                num1 = re.match(r'(\d+)\s+(\S+)\s+(\S+)\s*',line)
                if num1:
                    if int(num1.group(1)) not in tdict:
                        tdict[int(num1.group(1))] = float(num1.group(2))
                    else:
                        tdict[int(num1.group(1))] = tdict[int(num1.group(1))] + float(num1.group(2))
            
                    if int(num1.group(1)) not in pdict:
                        pdict[int(num1.group(1))] = float(num1.group(3))
                    else:
                        pdict[int(num1.group(1))] = pdict[int(num1.group(1))] + float(num1.group(3))

                    if int(num1.group(1)) not in dc:
                        dc[int(num1.group(1))] = 1
                    else:
                        dc[int(num1.group(1))] = dc[int(num1.group(1))] + 1
            i = i + 1
            fhr.close()
        keys = dc.keys()
        keys.sort()
        for key in keys:
            avt = tdict[key]/dc[key]
            avp = pdict[key]/dc[key]
            print repr(key)+' '+repr(avt)+' '+repr(avp)
            
        
                



if __name__ == "__main__":
    app = test1()
    app.proc(sys.argv[1])
    #app.pro2()
    #root  = Tk()
    #root.mainloop()
    #root.destroy()

