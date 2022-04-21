import csv
import numpy as np
#import pickle
import sys
import matplotlib.pyplot as plt
from sklearn import preprocessing
#from sklearn.naive_bayes import GaussianNB
#from sklearn.svm import SVC
#from sklearn.ensemble import GradientBoostingClassifier
from sklearn import mixture
#from sklearn.neighbors import KNeighborsClassifier
#from sklearn.ensemble import AdaBoostClassifier
from sklearn.model_selection import train_test_split
#from sklearn import metrics

if len(sys.argv) != 3:
	sys.stderr.write("Usage: gmm-best-fit.py training-data.txt nb-max-class \n")
	sys.exit(1)

trainingFile = sys.argv[1]
maxClusters = int(sys.argv[2])

#Load training data
with open(trainingFile) as f:
	reader = csv.reader(f)
	next(reader)
	labeled_data = list(reader)
	sys.stderr.write("[+] Loaded {} training samples\n".format(len(labeled_data)))

	features = []
	labels = []
	
	for x in labeled_data:
		y = [ float(h) for h in x[3:-2]]
		features.append( y )
	
	features = np.array(features)
	bic = []
	lowest_bic = np.infty
	
	sys.stderr.write("[+] Finding optimal parameters...\n")
	n = 1
	while True :
		print(n)
		model = mixture.GaussianMixture(n, covariance_type = "full")
		n += 1
		model.fit(features)
		bic.append(model.bic(features))
		if bic[-1] < lowest_bic:
			lowest_bic = bic[-1]
			best_model = model
		else:
			break;
		if n > maxClusters:
			break;

	
	#print(bic)
	print("bic min: ", min(bic))
	bestFit = 1 + bic.index(min(bic))
	print("best fit: ", bestFit)
	
	predictions = model.predict(features)
	print(predictions)
	
	for i in range(len(features)):
		xyz = features[i][:3]
		klass = predictions[i]
		print(str(xyz[0]),str(xyz[1]),str(xyz[2]) , klass)

	y = np.arange(1, len(bic)+1)
	x = np.array(bic)
	plt.gca().invert_yaxis()
	plt.gca().invert_xaxis()
	plt.plot(x,y)
	plt.xlabel("best fit")
	plt.xlabel("N clusters")
	plt.ylabel("BIC")
	plt.show()

