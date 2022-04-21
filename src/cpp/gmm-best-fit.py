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

if len(sys.argv) != 2:
	sys.stderr.write("Usage: gmm-best-fit.py training-data.txt\n")
	sys.exit(1)

trainingFile = sys.argv[1]

#Load training data
with open(trainingFile) as f:
	reader = csv.reader(f)
	next(reader)
	labeled_data = list(reader)
	sys.stderr.write("[+] Loaded {} training samples\n".format(len(labeled_data)))

	features = []
	labels = []

	#Parse features and labels
	for x in labeled_data:
		y = [ float(h) for h in x[3:-2]]
		features.append( y )
		labels.append( x[-2] )


	#Preprocess labels
	labelEncoder = preprocessing.LabelEncoder()
	labels_encoded = labelEncoder.fit_transform(labels)

	labelDictionary = dict(zip(labelEncoder.transform(labelEncoder.classes_),labelEncoder.classes_))

	featuresTrain, featuresTest, labelsTrain, labelsTest = train_test_split(features,labels_encoded, test_size=0.2,random_state=109) #80/20 split
	
	#print(features)
	
	#features = np.array(features)
	bic = []
	lowest_bic = np.infty
	
	sys.stderr.write("[+] Finding optimal parameters...\n")
	n = 1
	while True :
		print(n)
		model = mixture.GaussianMixture(n, covariance_type = "full")
		n += 1
		model.fit(features,labelsTrain)
		bic.append(model.bic(np.array(featuresTrain)))
		if bic[-1] < lowest_bic:
			lowest_bic = bic[-1]
			best_model = model
		else:
			break;
		if n > 101:
			break;

	
	print(bic)
	print(min(bic))
	bestFit = 1 + bic.index(min(bic))
	print(bestFit)
	
	predictions = model.predict(featuresTest)
	print(predictions)
	print(labelsTest)
	
	
	y = np .arange(bestFit)
	x = np.array(bic)
	plt.plot(x,y)
	plt.show()

