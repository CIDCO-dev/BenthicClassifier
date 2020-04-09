CC=g++
OPTIONS=-Wall -static
INCLUDES=-I/usr/include/eigen3
VERSION=0.1.0

FILES=	src/geometry/Point.cpp \
	src/geometry/Cell.cpp \
	src/geometry/Grid.cpp \
	src/machinelearning/DBSCAN.cpp

root=$(shell pwd)

exec_dir=build

test_exec_dir=build/test/bin
test_work_dir=build/test/work
test_result_dir=build/reports

default: prepare BenthicClassifier

BenthicClassifier: prepare
	$(CC) $(OPTIONS) $(INCLUDES) -o $(exec_dir)/BenthicClassifier src/main.cpp
	
debug:
	mkdir -p $(test_exec_dir)
	$(CC) $(OPTIONS) -g $(INCLUDES) -o $(test_exec_dir)/tests test/main.cpp $(FILES)
	


test:
	mkdir -p $(test_exec_dir)
	$(CC) $(OPTIONS) $(INCLUDES) -o $(test_exec_dir)/tests test/main.cpp
	mkdir -p $(test_result_dir)
	mkdir -p $(test_work_dir)
	$(root)/$(test_exec_dir)/tests -r junit -o $(test_result_dir)/BenthicClassifier-report.xml

prepare:
	mkdir -p $(exec_dir)


.PHONY: test
