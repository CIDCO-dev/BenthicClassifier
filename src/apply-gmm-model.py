import os
import sys
import csv
import numpy as np
import pickle


if len(sys.argv) != 3:
	sys.stderr.write("Usage: apply-gmm-model.py model-file hackelDataFile\n")
	#sys.stderr.write("{}\n".format(sys.argv))
	sys.exit(1)


modelFilename = os.fsencode(sys.argv[1])
dataFilename = os.fsencode(sys.argv[2])
model = pickle.load(open(modelFilename,"rb"))

features = []
classes = []

#Load training data
if model:
	with open(dataFilename,"r") as dataFile:
		reader = csv.reader(dataFile, delimiter=",")
		next(reader)
		data = list(reader)
		sys.stderr.write("[+] Loaded {} soundings\n".format(len(data)))
		
		for line in data:
			y = [float(x) for x in line[3:]]
			features.append(y)
		
		classes = model.predict(features);

else:
	sys.stderr.write("Couldn't load model\n")
	sys.exit(1)


for i in range(0,len(classes)):
	sys.stdout.write("{},{},{},{}\n".format(data[i][0],data[i][1],data[i][2], classes[i]))





