

import os
import sys
import csv
import numpy as np
import pickle
from sklearn import preprocessing
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import AdaBoostClassifier

from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split

from sklearn import metrics

if len(sys.argv) != 3:
	sys.stderr.write("Usage: apply-model.py model-file data-with-hackel-features\n")
	sys.exit(1)


modelFilename = os.fsencode(sys.argv[1])
dataFilename = os.fsencode(sys.argv[2])

model = pickle.load(open(modelFilename,"rb"))

#Load training data
if model:
	with open(dataFilename,"r") as dataFile:
		reader = csv.reader(dataFile)
		next(reader)
		data = list(reader)
		print("[+] Loaded {} soundings".format(len(data)))

		features = [x[3:] for x in data]

		classes = model.predict(features);

		for i in range(0,len(classes)):
			print("{},{},{},{}".format(data[i][0],data[i][1],data[i][2],classes[i]))

else:
	sys.stderr.write("Couldn't load model\n")
