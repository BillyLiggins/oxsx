#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from glob import glob
import re


# commonAbvs =[

def processCamelCase(string):
    newString=""
    lastCharUpper = False
    lastCharLower = False
    lastChar ="d"
    for i, char in enumerate(string):
        # lastChar = char
        if char.isupper() and i==0 :
            newString += char.lower()
            lastCharLower = False
            lastCharUpper = True
            lastChar = char
        elif char.isupper() and lastCharUpper:
            newString += char.lower()
            lastCharLower = False
            lastCharUpper = True
            lastChar =char
        elif char.isupper() and lastCharLower:
            newString += "_"+char.lower()
            lastCharLower = False
            lastCharUpper = True
            lastChar = char
        elif char.islower() and lastCharUpper and i != 1:
            newString = newString[:-1]
            newString += "_"+lastChar.lower()+char
            lastCharUpper=False
            lastCharLower=True
            lastChar = char
        else:
            newString += char
            lastCharLower = True
            lastCharUpper = False
            lastChar = char

    return newString



def findFunctions(lines):
    
    functions = []
    for line in lines:
        searchObj = re.search( r'.*\s(.*)\(.*\).*', line,re.M|re.I)
        if searchObj:
            functions.append(searchObj.group(1))

    return functions 

def main():
    """
    :returns: TODO

    """
    print "BinED"
    print processCamelCase("BinED")
    print "EDManger"
    print processCamelCase("EDManger")
    print "newEDManger"
    print processCamelCase("newEDManger")
    print "PDFEDConverter"
    print processCamelCase( "PDFEDConverter")
    
    lines =[]
    with open("src/systematic/Convolution.h","r") as f:
        lines = f.readlines()

    
    print findFunctions(lines)
    print [processCamelCase(x) for x in findFunctions(lines)]

if __name__ == "__main__":
    main()
