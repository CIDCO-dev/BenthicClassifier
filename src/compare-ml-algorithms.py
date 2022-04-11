import os
import sys
import csv
import numpy as np
from sklearn import preprocessing
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import AdaBoostClassifier

from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split

from sklearn import metrics

def generateMap(surfaceDataPath,inputFilename,model,lbl):
	outputFolder = os.fsdecode(surfaceDataPath) + "/output/"

	if not os.path.exists(outputFolder):
		os.mkdir(outputFolder)

	with open(os.path.join(surfaceDataPath,inputFilename)) as input:
		outputFilename= outputFolder + os.fsdecode(inputFilename) + "." + type(model).__name__ + ".txt"
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

if len(sys.argv) != 3:
	sys.stderr.write("Usage: train.py training-data.txt surface-folder-path\n")
	sys.exit(1)



trainingDataPath = sys.argv[1]
surfaceDataPath  = os.fsencode(sys.argv[2])
#TODO: get from command line parameter
doParametricOptimization= False


#Load training data
with open(trainingDataPath) as trainingDataFile:
	reader = csv.reader(trainingDataFile)
	next(reader)
	labeled_data = list(reader)
	print("[+] Loaded {} classified soundings".format(len(labeled_data)))

	features = []
	labels = []

	#Parse features and labels from training data
	for x in labeled_data:
		y = [ float(h) for h in x[3:-2]]
		features.append( y )
		labels.append( x[-2] )

	#Preprocess labels
	labelEncoder = preprocessing.LabelEncoder()
	labels_encoded = labelEncoder.fit_transform(labels)

	labelDictionary = dict(zip(labelEncoder.transform(labelEncoder.classes_),labelEncoder.classes_))

#	print(labelDictionary)

	#Use a 80/20 training/test split
	featuresTrain, featuresTest, labelsTrain, labelsTest = train_test_split(features,labels_encoded, test_size=0.2,random_state=109)

	print("[+] Training models...")

	models = [
			#KNeighborsClassifier(n_neighbors=20),
			#GaussianNB(),
			#SVC(kernel='rbf',C=100000)
			GradientBoostingClassifier() #,
			#AdaBoostClassifier()
		]

	# Train each model, measure accuracy metrics, and generate output maps
	for model in models:
		modelName = type(model).__name__

		if not doParametricOptimization:
			print("[+] Training {}".format(modelName))

			model.fit(featuresTrain,labelsTrain)

			print("[+] Evaluation performance...")

			predictions = model.predict(featuresTest)

			#predictions = np.asarray([np.argmax(line) for line in predictions])

			print("Totals: ")
			print("[*] {} accuracy: {}".format(modelName,metrics.accuracy_score(labelsTest,predictions)))
			print(metrics.classification_report(labelsTest, predictions))
			print(metrics.confusion_matrix(labelsTest,predictions))

		elif doParametricOptimization:
			print("[+] Optimizing {}".format(modelName))
			param_grid = []

			if modelName == "KNeighborsClassifier":
				print("KNN: {}".format(model.get_params()))

				param_grid = [ { 'n_neighbors' : [3,5,7,10,20,40,80,150]}]

			elif modelName == "GaussianNB":
				print("Naive Bayes")
				print("Nothing to optimize for naive bayes")
				continue;

			elif modelName == "SVC":
				print("SVM")

				param_grid = [ #{ 'kernel':['linear'],'C':[1,10,100,1000] },
					       { 'kernel':['rbf'],'C':[1000,10000,100000,1000000,10000000] }
					     ]
			elif modelName == "GradientBoostingClassifier":
				print("GBX")
			elif modelName == "AdaBoostClassifier":
				print("Adaboost")

			optimizingModel = GridSearchCV(model,param_grid,'accuracy') #can also use 'recall'
			optimizingModel.fit(featuresTrain,labelsTrain)
			print("Best model: {}".format(optimizingModel.best_params_))

		# Generate maps with it
		#for filename in os.listdir(surfaceDataPath):
		#	if filename.endswith(b'.Hackel'):
		#		generateMap(surfaceDataPath,filename,model,labelDictionary)
		#	else:
		#		sys.stderr.write("Skipping {}\n".format(filename))

