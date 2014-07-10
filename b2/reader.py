import csv 
import sys

filename=sys.argv[1]
fo = open(filename, "rw+")
lines = fo.readlines()
csum=0
hsum=0 
count=0
nofl=len(lines)-1
alt=0
for line in lines: 
	if count==0:
		print line 
		count =1 
	else : 
		line=line.split("\t") 
		#print line[len(line)-1].strip()+"---"+line[len(line)-2].strip() 
		if line[len(line)-1].strip() != '' and len(line) == 10:
			csum = csum + int(line[len(line)-1].strip()) 
			hsum = hsum + int(line[len(line)-2].strip())

print nofl
print nofl/2
cmsum=(nofl/2)-(csum/2)
print csum/2 
print hsum
print cmsum
