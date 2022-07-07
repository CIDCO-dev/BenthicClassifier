import csv
import numpy as np
import sys
import math

from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingRegressor
from sklearn.metrics import mean_squared_error

# size in milimeters
# blocks = (1000 - 254)/2 + 254
# rocky = (inf - blocks)/2 + blocks ~= (627 - 10 000)/2 + 627
typeOfSubstrate = {'sands':1.031, 'blocks':627, 'rocky':5313, 'cobble':160, 'gravels':33, 'sandy mud':0.032}


def evaluate_substrate(substrate_type):
	for x in typeOfSubstrate.keys():
		if x == substrate_type:
			return typeOfSubstrate[x]
	
	sys.stderr.write("type of substrate not found")
	return -1

def try_param(param):
	model = GradientBoostingRegressor(**param)
	model.fit(featuresTrain, labelsTrain)
	rmse = math.sqrt(mean_squared_error(labelsTest, model.predict(featuresTest)))
	print("try: ", rmse)
	return rmse



# ==========MAIN========== #
if len(sys.argv) != 2:
	sys.stderr.write("Usage: substrate_size-based_train.py mbes-training-data.txt\n")
	sys.exit(1)

trainingFile = sys.argv[1]
features = []
substrateSizes = []
points = []


#Load training data
with open(trainingFile) as f:
	reader = csv.reader(f)
	#next(reader)
	labeled_data = list(reader)
	sys.stderr.write("[+] Loaded {} training samples\n".format(len(labeled_data)))

	#Parse features and labels
	for x in labeled_data:
		y = [ float(h) for h in x[:-2]]
		substrateSize = evaluate_substrate(x[len(x)-2])
		if substrateSize > 0 :
			points.append( y[:2] )
			features.append(y[3:-2])
			substrateSizes.append(substrateSize)

if(len(features) == len(points) and len(features) == len(substrateSizes)):
	print("Parsing ok")
	
featuresTrain, featuresTest, labelsTrain, labelsTest = train_test_split(features,substrateSizes, test_size=0.2,random_state=110)

params = {
    "n_estimators": 100,
    "max_depth": 2,
    "min_samples_split": 2,
    "learning_rate": 0.01,
    "loss": "squared_error",
}
print("model param : ", params)
sys.stderr.write("[+] Testing model param...\n")
model = GradientBoostingRegressor(**params)
model.fit(featuresTrain, labelsTrain)
rmse = math.sqrt(mean_squared_error(labelsTest, model.predict(featuresTest)))
print("The root mean squared error (RMSE) on test set: {:.4f}".format(rmse))
	

print("\noptimisation start ...")
RMSE = [rmse+1,rmse]
temp_params = params.copy()

estimator = 500
bestEstimator = 0
estimatorRmse = [rmse+10, rmse]
depthRmse = [rmse+10, rmse]
splitRmse = [rmse+10, rmse]
learningRmse = [rmse+10, rmse]

print("estimator param")
while (estimator <= 10000 and estimatorRmse[len(estimatorRmse) - 2] - estimatorRmse[len(estimatorRmse) - 1] >= 10 ):
	temp_params.update({"n_estimators": estimator})
	rmse = try_param(temp_params)
	estimatorRmse.append(rmse)
	estimator = estimator + 500
print("estimator" , estimatorRmse)
bestEstimator = estimator - 500


temp_params = params.copy()
depth = 3
bestDepth = 0
print("max depth param")
while (depth <= 20 and depthRmse[len(depthRmse) - 2] - depthRmse[len(depthRmse) - 1] >= 10 ):
	temp_params.update({"max_depth": depth})
	rmse = try_param(temp_params)
	depthRmse.append(rmse)
	depth = depth + 2
print("depth", depthRmse)
bestDepth = depth - 2

"""
temp_params = params.copy()
split = 5
bestSplit = 0
print("min sample split param")
while (split <= 20 and splitRmse[len(splitRmse) - 2] - splitRmse[len(splitRmse) - 1] >= 10 ):
	temp_params.update({"min_samples_split": split})
	rmse = try_param(temp_params)
	splitRmse.append(rmse)
	split = split + 2
print("split", splitRmse)
"""

temp_params = params.copy()
learning = [0.1, 0.01, 0.001, 0.5, 0.05, 0.005]
bestRate = 0
print("learning rate param")
for rate in learning:
	subLearningRmse = learningRmse.copy()
	while (rate <= 0.5 and learningRmse[len(learningRmse) - 2] - learningRmse[len(learningRmse) - 1] >= 10 ):
		temp_params.update({"learning_rate": rate})
		rmse = try_param(temp_params)
		subLearningRmse.append(rmse)
		rate = rate + rate
	print(subLearningRmse)
	


temp_params = {
    "n_estimators": int(bestEstimator/2),
    "max_depth": int(bestDepth/2),
    "min_samples_split": 5,
    "learning_rate": 0.01,
    "loss": "squared_error",
}
#print(temp_params)

print("fine tune param")
while(RMSE[len(RMSE) - 2 ] - RMSE[len(RMSE) - 1 ] >= 1):
	up_params = temp_params.copy()
	up_params.update({"n_estimators": temp_params["n_estimators"]*2})
	up_rmse = try_param(up_params)
	down_params = temp_params.copy()
	down_params.update({"n_estimators": int(temp_params["n_estimators"]/2)})
	down_rmse = try_param(down_params)
	
	if up_rmse > down_rmse:
		temp_params.update({"n_estimators": down_params["n_estimators"]})
	elif up_rmse < down_rmse:
		temp_params.update({"n_estimators": up_params["n_estimators"]})

		
	up_params = temp_params.copy()
	up_params.update({"max_depth": temp_params["max_depth"] + 1})
	up_rmse = try_param(up_params)
	down_params = temp_params.copy()
	down_params.update({"max_depth": temp_params["max_depth"] - 1})
	down_rmse = try_param(down_params)
	
	if up_rmse > down_rmse:
		temp_params.update({"max_depth": down_params["max_depth"]})
	elif up_rmse < down_rmse:
		temp_params.update({"max_depth": up_params["max_depth"]})
		
	up_params = temp_params.copy()
	up_params.update({"learning_rate": temp_params["learning_rate"]*2})
	up_rmse = try_param(up_params)
	down_params = temp_params.copy()
	down_params.update({"learning_rate": temp_params["learning_rate"]/2})
	down_rmse = try_param(down_params)
	
	if up_rmse > down_rmse:
		temp_params.update({"learning_rate": down_params["learning_rate"]})
		RMSE.append(down_rmse)
	elif up_rmse < down_rmse:
		temp_params.update({"learning_rate": up_params["learning_rate"]})
		RMSE.append(up_rmse)
	
	print(temp_params)
	print("\n")


