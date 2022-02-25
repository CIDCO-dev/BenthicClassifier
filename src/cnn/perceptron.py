import torch;
import torch.nn as nn
import torch.nn.functional as F
import csv
import random
from sklearn import preprocessing

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

class BathyData(torch.utils.data.Dataset):
	def __init__(self,features,labels):
		super(BathyData,self).__init__()

		self.features = features
		self.labels   = labels

	def __getitem__(self,idx):
		return self.features[idx],self.labels[idx]

	def __len__(self):
		return len(self.features)


class BathyNet(torch.nn.Module):
	def __init__(self):
		super(BathyNet, self).__init__()

		self.nbInputs = 15
		self.nbOutputs = 7
		self.nbHidden = int( 0.6666 * self.nbInputs + self.nbOutputs )

		self.fc1 = nn.Linear(self.nbInputs, self.nbHidden)

		self.fc2 = nn.Linear(self.nbHidden,self.nbHidden)

		self.dropout  = nn.Dropout( 0.1, False)

		self.fc3 = nn.Linear(self.nbHidden, self.nbHidden)

		self.fc4 = nn.Linear(self.nbHidden, self.nbHidden)

		self.fcLast = nn.Linear(self.nbHidden, self.nbOutputs)

		self.sigmoid = torch.nn.Sigmoid()

	def forward(self, x):
		# Pass data through conv1
		x = self.fc1(x)

		#x = F.relu(x)

		x = self.fc2(x)
		#x = F.relu(x)

		x = self.dropout(x)

		x = self.fc3(x)

		x = self.dropout(x)

		x = self.fc4(x)

		#output = F.relu(x)

		x = self.fcLast(x)

		output = self.sigmoid(x)

		return output

########################

model = BathyNet()

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=0.01, momentum=0.75)

# Load training data
with open('../labeled-clean.txt') as f:
	reader = csv.reader(f)
	next(reader)
	labeled_data = list(reader)

	print("{} samples loaded".format(len(labeled_data)))

	#grab labels
	all_labels = list(set([i[19] for i in labeled_data]))
	all_labels = [ [i] for i in all_labels]

	le = preprocessing.OneHotEncoder(handle_unknown='ignore')

	encoded_labels= le.fit(all_labels)

	#print(encoded_labels.toarray())
	print("Classes: {}".format(list(le.categories_)))

	#preprocess data
	features = []
	labels = []

	for x in labeled_data:
		feats = x[3:18]
		feats = torch.FloatTensor( [ float(y) for y in feats] )
		features.append(feats)

		label = torch.FloatTensor(le.transform([[x[19]]])[0].toarray()[0])
		labels.append(label)

	#shuffle and split labeled data into training/test sets
	#random.shuffle(labeled_data)

	split_index = int(len(labeled_data) * 0.8) #use 80% to train

	trainingset = BathyData(features[:split_index],labels[:split_index])
	trainloader = torch.utils.data.DataLoader(trainingset, batch_size=4, shuffle=False, num_workers=2)

	testingset  = BathyData(features[split_index:],labels[split_index:])
	testloader = torch.utils.data.DataLoader(testingset, batch_size=4, shuffle=False, num_workers=2)

	# Train
	for epoch in range(100):
		running_loss = 0.0

		for i, data in enumerate(trainloader, 0):

			# get the inputs; data is a list of [inputs, labels]
			inputs, labels = data
			#print("Inputs:{}".format(inputs))

			#print("Labels:{}".format(labels))

			# zero the parameter gradients
			optimizer.zero_grad()

			# forward + backward + optimize
			outputs = model(inputs)
			loss = criterion(outputs,  torch.max(labels, 1)[1])
			loss.backward()
			optimizer.step()

			# print statistics
			running_loss += loss.item()
			if i % 2000 == 1999:    # print every 2000 mini-batches
				print('[%d, %5d] loss: %.3f' % (epoch + 1, i + 1, running_loss / 2000))
				running_loss = 0.0

print('Finished Training')

print("Testing...")

correct = 0
total = 0
with torch.no_grad():
	for data in testloader:
		inputs, labels = data
		outputs = model(inputs)
		#print(outputs)
		_, predicted = torch.max(outputs.data, 1)
		#print("Prediction: {}".format(predicted))
		_,decoded_labels = torch.max(labels, 1)
		#print("Inputs: {}".format(decoded_labels))
		total += labels.size(0)
		correct += (predicted == decoded_labels).sum().item()

print('Accuracy: %d %%' % (100 * correct / total))
