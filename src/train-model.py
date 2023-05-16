import csv
import numpy as np
import pickle
import sys

from sklearn import preprocessing
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import AdaBoostClassifier


from sklearn.model_selection import train_test_split
from sklearn import metrics

if len(sys.argv) != 2:
	sys.stderr.write("Usage: train.py training-data.txt\n")
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

	print(labelDictionary)

	featuresTrain, featuresTest, labelsTrain, labelsTest = train_test_split(features,labels_encoded, test_size=0.2,random_state=109) #80/20 split

	sys.stderr.write("[+] Training model...\n")

	model = GradientBoostingClassifier()

	modelName = type(model).__name__ 

	model.fit(featuresTrain,labelsTrain)

	sys.stderr.write("[+] Validating models...\n")

	predictions = model.predict(featuresTest)

	#predictions = np.asarray([np.argmax(line) for line in predictions])

	#Save model
	pickle.dump(model,open("trained.model","wb"))


	sys.stderr.write("Totals: \n")
	sys.stderr.write("[*] {} accuracy: {}\n".format(modelName,metrics.accuracy_score(labelsTest,predictions)))
#	sys.stderr.write(metrics.classification_report(labelsTest, predictions))
#	sys.stderr.write("\n")
#	sys.stderr.write(metrics.confusion_matrix(labelsTest,predictions))
#	sys.stderr.write("\n")

