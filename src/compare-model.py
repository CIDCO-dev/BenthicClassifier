import os
import sys
import csv
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits import mplot3d

def read_classification_file(filename):
	
	classifiedData = []		
		
	with open(filename,"r") as dataFile:
		reader = csv.reader(dataFile, delimiter=",")
		next(reader)
		data = list(reader)
		sys.stderr.write("[+] Loaded {} classified points\n".format(len(data)))
		
		for line in data:
			row = [float(line[0]), float(line[1]), float(line[2]), int(line[3]), int(line[4])]
			classifiedData.append(row)
	
	return classifiedData
	



if len(sys.argv) != 2:
	sys.stderr.write("Usage: compare-model.py classifiedData \n")
	sys.exit(1)


print(["0 : 19", "1 : blocks", "2 : cobble", "3 : gravels", "4 : rocky", "5 : sands", "6 : sandy mud"]) 

classifiedData = read_classification_file(sys.argv[1])

classMatch = []
classMatchingCounter = dict()
gmm = []
boosting = []

for i in range(len(classifiedData)):
	classGBX = str(classifiedData[i][3])
	classGMM = str(classifiedData[i][4])
	match = classGBX + "-" + classGMM
	classMatch.append(match)
	gmm.append(classifiedData[i][4])
	boosting.append(classifiedData[i][3])
	
	x = classMatchingCounter.get(match)
	if x == None:
		classMatchingCounter[match] = 1
	else:
		classMatchingCounter[match] += 1


""" Histogram """
#plt.hist(classMatch,30, edgecolor='red')
#plt.show()

""" scatter with color"""
#x = np.arange(len(classifiedData), dtype=int)
#plt.scatter(x, gmm, color='red', s=20)
#plt.scatter(x, boosting, color='blue', s=2)
#plt.show()

""" scatter """
#plt.scatter(boosting, gmm)
#plt.show()

""" 3D scatter """
#z = []
#for match in classMatch:
#	x = classMatchingCounter.get(match)
#	z.append(x)


#ax = plt.axes(projection='3d')
#ax.set_xlabel('GBX class')
#ax.set_ylabel('GMM class')
#ax.set_zlabel('Occurence')
#index = np.arange(len(classifiedData), dtype=int)
#boosting = np.array(boosting, dtype=int)
#gmm = np.array(gmm, dtype=int)
#ax.scatter3D(boosting, gmm, z)
#plt.show()


""" 3D bars """
z = list(classMatchingCounter.values())
boostingBar = []
gmmBar = []
for match in classMatchingCounter.keys():
	classes = match.split("-")
	boostingBar.append(int(classes[0]))
	gmmBar.append(int(classes[1]))


ax = plt.axes(projection='3d')
bottom = np.zeros_like(z)
width = depth = 0.1
ax.set_xlabel('GBX class')
ax.set_ylabel('GMM class')
ax.set_zlabel('Occurence')

ax.bar3d(boostingBar, gmmBar, bottom, width, depth, z, shade=True)
plt.show()




