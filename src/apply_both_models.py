import os
import sys
import csv
import numpy as np
import pickle


if len(sys.argv) != 4:
	sys.stderr.write("Usage: apply-both-model.py model-file gmm-model-file hackelDataFile\n")
	sys.exit(1)


boostingModelFilename = os.fsencode(sys.argv[1])
gmmModelFilename = os.fsencode(sys.argv[2])
hackelDataFile = os.fsencode(sys.argv[3])
modelBoosting = pickle.load(open(boostingModelFilename,"rb"))
modelGmm = pickle.load(open(gmmModelFilename,"rb"))

features = []
classesBoosting = []
classesgmm = []


#Load training data
if modelGmm and modelBoosting:
	with open(hackelDataFile,"r") as dataFile:
		reader = csv.reader(dataFile, delimiter=",")
		next(reader)
		data = list(reader)
		sys.stderr.write("[+] Loaded {} soundings\n".format(len(data)))
		
		for line in data:
			
			y = [float(x) for x in line[3:]]
			features.append(y)
		
		classesBoosting = modelBoosting.predict(features)
		classesgmm = modelGmm.predict(features)

else:
	sys.stderr.write("Couldn't load model\n")
	sys.exit(1)


if len(classesBoosting) != len(classesgmm):
	sys.exit(1)

#sys.stdout.write("{},{},{},{},{}\n".format("x", "y", "z", "boosting class", "gmm class"))
for i in range(0,len(classesBoosting)):
	sys.stdout.write("{},{},{},{},{}\n".format(data[i][0],data[i][1],data[i][2], classesBoosting[i], classesgmm[i]))


