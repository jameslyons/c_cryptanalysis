from itertools import product 
from math import log10
ngrams = {}
for line in file('english_quadgrams.txt'):
    key,count = line.split() 
    ngrams[key] = int(count)
N = sum(ngrams.itervalues())

print '''float qgram[] = {'''

for c,let1 in enumerate(product('ABCDEFGHIJKLMNOPQRSTUVWXYZ......',repeat=4)):
    key = ''.join(let1)
    if c >= 32*32*32*26: 
        print '''0 };'''
        break
    if '.' in key:
        print str(0)+','
    else:
      if key in ngrams:
        print str(log10(float(ngrams[key])/N))+','
      else:
        print str(log10(1./N))+','

