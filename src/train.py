import csv
import numpy as np
from sklearn import preprocessing
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import AdaBoostClassifier


from sklearn.model_selection import train_test_split
from sklearn import metrics

def generateMap(inputFilename,model,lbl):
	with open(inputFilename) as input:
		outputFilename=inputFilename[:-4] + "_" + type(model).__name__ + ".txt"
		reader = csv.reader(input)
		next(reader)
		unlabeled_data = list(reader)
		print("[+] Loaded {} soundings to classify".format(len(unlabeled_data)))

		features = []

		#Parse features and labels
		for x in unlabeled_data:
			y = [ float(h) for h in x[3:]]
			features.append( y )


		encoded_labels = model.predict(features)

		#Format output
		with open(outputFilename,'w') as output:
			writer = csv.writer(output,delimiter=',',quotechar='"')
			for i in range(len(encoded_labels)):
				# We will use a numeric ID that equals the encoded value + 1 to avoid conflict with the 0 value which is used for "nothingness" in rasters
				writer.writerow([unlabeled_data[i][0],unlabeled_data[i][1],unlabeled_data[i][2],lbl[encoded_labels[i]],encoded_labels[i] + 1 ]) #encoded_labels[i]] )

###############################################

########
# Main #
########

if len(sys.argv) != 2:
	sys.stderr.write("Usage: train.py training-data.txt")
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
		y = [ float(h) for h in x[3:-1]]
		features.append( y )
		labels.append( x[-1] )


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

	sys.stderr.write("Totals: \n")
	sys.stderr.write("[*] {} accuracy: {}\n".format(modelName,metrics.accuracy_score(labelsTest,predictions)))
	sys.stderr.write(metrics.classification_report(labelsTest, predictions))
	sys.stderr.write("\n")
	sys.stderr.write(metrics.confusion_matrix(labelsTest,predictions))
	sys.stderr.write("\n")

else:
	sys.stderr.write("Cannot open training data file")
	sys.exit(1)
