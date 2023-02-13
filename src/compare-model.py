import os
import sys
import csv
import matplotlib.pyplot as plt

def read_classification_file(filename):
	
	classifiedData = []		
		
	with open(filename,"r") as dataFile:
		reader = csv.reader(dataFile, delimiter=",")
		#next(reader)
		data = list(reader)
		sys.stderr.write("[+] Loaded {} classified points\n".format(len(data)))
		#count = 0
		for line in data:
			#sys.stderr.write("{}\n".format(count))
			#count += 1
			y = [float(x) for x in line]
			classifiedData.append(y)
	
	return classifiedData
	

if len(sys.argv) != 2:
	sys.stderr.write("Usage: compare-model.py classifiedData \n")
	sys.exit(1)


classifiedData = read_classification_file(sys.argv[1])
#classifiedData2 = read_classification_file(sys.argv[2])

classMatchingCounter = dict()
classMatch = []
for i in range(len(classifiedData)):
	class1 = str(int(classifiedData[i][3]))
	class2 = str(int(classifiedData[i][4]))
	match = class1+"-"+class2
	classMatch.append(match)
	
	"""
	x = classMatchingCounter.get(match)
	if x == None:
		classMatchingCounter[match] = 1
	else:
		classMatchingCounter[match] += 1
	"""
	
plt.hist(classMatch)
plt.show()


