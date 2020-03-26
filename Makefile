CC=g++
OPTIONS=-Wall -static
#INCLUDES=-I/usr/include/eigen3
VERSION=0.1.0


exec_dir=build


default: prepare BenthicClassifier

BenthicClassifier: prepare
	$(CC) $(OPTIONS) $(INCLUDES) -o $(exec_dir)/BenthicClassifier src/main.cpp 


prepare:
	mkdir -p $(exec_dir)


