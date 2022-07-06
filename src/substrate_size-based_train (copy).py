import csv
import numpy as np
import sys
import math

from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingRegressor
from sklearn.metrics import mean_squared_error

# size in milimeters
typeOfSubstrate = {'sands':1, 'blocks':300, 'rocky':600, 'cobble':150, 'gravels':30, 'sandy mud':0.05}


def evaluate_substrate(substrate_type):
	for x in typeOfSubstrate.keys():
		if x == substrate_type:
			return typeOfSubstrate[x]
	
	sys.stderr.write("type of substrate not found")
	return -1




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
	print(typeOfSubstrate)
	
featuresTrain, featuresTest, labelsTrain, labelsTest = train_test_split(features,substrateSizes, test_size=0.2,random_state=110)

params = {
    "n_estimators": 400,
    "max_depth": 4,
    "min_samples_split": 5,
    "learning_rate": 0.005,
    "loss": "squared_error",
}

model = GradientBoostingRegressor(**params)

modelName = type(model).__name__ 

model.fit(featuresTrain, labelsTrain)

sys.stderr.write("[+] Validating models...\n")

"""
for i in range(len(predictions)):
	print(predictions[i], " : ", test_labels[i] )
"""
rmse = math.sqrt(mean_squared_error(labelsTest, model.predict(featuresTest)))
print("The root mean squared error (RMSE) on test set: {:.4f}".format(rmse))

def try_param(param):
	model = GradientBoostingRegressor(**param)
	model.fit(featuresTrain, labelsTrain)
	rmse = math.sqrt(mean_squared_error(labelsTest, model.predict(featuresTest)))
	print("try: ", rmse)
	return rmse
	

RMSE = []
RMSE.append(rmse+1)
RMSE.append(rmse)
temp_params = params

loop = 0

while(RMSE[len(RMSE) - 2 ] - RMSE[len(RMSE) - 1 ] >= 1):
	#print("loop", RMSE[len(RMSE) - 2 ] ," ? ", RMSE[len(RMSE) - 1 ])
	#loop = loop + 1 
	
	
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
	up_params.update({"learning_rate": temp_params["learning_rate"] + 0.0005})
	up_rmse = try_param(up_params)
	down_params = temp_params.copy()
	down_params.update({"learning_rate": temp_params["learning_rate"] - 0.0005})
	down_rmse = try_param(down_params)
	
	if up_rmse > down_rmse:
		temp_params.update({"learning_rate": down_params["learning_rate"]})
		RMSE.append(down_rmse)
	elif up_rmse < down_rmse:
		temp_params.update({"learning_rate": up_params["learning_rate"]})
		RMSE.append(up_rmse)
	
	print(temp_params)
	print("\n")


