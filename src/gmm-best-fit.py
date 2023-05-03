from dask import dataframe as dd
import numpy as np
import pickle
import sys

from sklearn import preprocessing

from sklearn import mixture

from sklearn.model_selection import train_test_split


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
features = reader[["3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18"]]
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
while n <= maxClusters :
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

#Save model
pickle.dump(best_model,open("gmm_trained.model","wb"))

