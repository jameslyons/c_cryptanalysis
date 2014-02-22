import itertools

perms = []
i = 0
for a in itertools.permutations(range(0,16), 5):

    if (0 in a) and (8 in a): continue
    if (1 in a) and (9 in a): continue
    if (2 in a) and (10 in a): continue
    if (3 in a) and (11 in a): continue
    if (4 in a) and (12 in a): continue
    if (5 in a) and (13 in a): continue
    if (6 in a) and (14 in a): continue
    if (7 in a) and (15 in a): continue
    
    #print a,',',
    #i = i + 1
    #if i%10 == 0: print ''
    perms.append(a)
    
print len(perms)
