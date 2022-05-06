import csv
from dask import dataframe as dd
import pandas as pd
import numpy as np
#import pickle
import sys
#import matplotlib.pyplot as plt
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

features = []
points = []


reader = dd.read_csv(trainingFile)
#print(len(reader))
#print(reader.head())
features = reader[["3","4","5","6","7","8","9","10","11","12","13","14","15","16"]]
points = reader[["0","1","2"]]
#print(features.head())
features_pd = features.compute()
points_pd = points.compute()
points = points_pd.to_numpy()


sys.stderr.write("[+] Loaded {} training samples\n".format(len(reader)))	
#features = np.array(features)
bic = []
lowest_bic = np.infty

sys.stderr.write("[+] Finding optimal parameters...\n")
n = 1
while n < maxClusters :
	s = "progress: " + str(n) + "/" + str(maxClusters)
	sys.stderr.write(s+"\n")
	model = mixture.GaussianMixture(n, covariance_type = "full")
	n += 1
	model.fit(features_pd.to_numpy())
	performance = model.bic(features_pd.to_numpy())
	bic.append(performance)
	if performance < lowest_bic:
		lowest_bic = performance
		best_model = model
	else:
		break;


bestFit = 1 + bic.index(min(bic))
sys.stderr.write("best fit: " + str(bestFit) + "\n")

predictions = model.predict(features_pd.to_numpy())

for i in range(len(features)):
	xyz = points[i]
	klass = predictions[i]
	print(str(xyz[0]),str(xyz[1]),str(xyz[2]) , klass)

"""
y = np.arange(1, len(bic)+1)
x = np.array(bic)
plt.gca().invert_yaxis()
plt.gca().invert_xaxis()
plt.plot(x,y)
plt.xlabel("best fit")
plt.xlabel("N clusters")
plt.ylabel("BIC")
plt.show()
"""
