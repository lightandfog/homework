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
            
    def pro3(self,str):
        fhr = open(str,'r')
        fhw = open(str+".re",'w')
        for line in fhr:
            #num1 = re.match(r'.*nest\s+(1)\s+=\s+(\S+E\S+)\s+second.*',line    )
            ma1 = re.match(r'(.*time)\s+=\s+(\S+)\s+seconds',line)
            #print line
            if ma1:
                fhw.write(ma1.group(1)+'\t'+ma1.group(2)+'\n')
            else:
                ma1 = re.match(r'(.*time)\s+=\s+(\S+)\s*',line)
                if ma1:
                    fhw.write(ma1.group(1)+'\t'+ma1.group(2)+'\n')
        fhr.close()
        fhw.close()



if __name__ == "__main__":
    app = test1()
    #app.proc(sys.argv[1])
    app.pro3(sys.argv[1])
    #root  = Tk()
    #root.mainloop()
    #root.destroy()

