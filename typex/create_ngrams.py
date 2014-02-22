
import re
import math
import sys

f = open('all.txt','r')

L  =2
ng = {}
for line in f:
  line = re.sub(r'[^A-Z]','',line.upper())
  for i in range(len(line)-L+1):
    ng[line[i:i+L]] = ng.get(line[i:i+L], 0) + 1
f.close()  
for ngram in ng.keys():
    print ngram,ng[ngram]

