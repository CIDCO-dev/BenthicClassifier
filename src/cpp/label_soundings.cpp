#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>


class Label{
public:
	Label(double easting,double northing,char * substrate):easting(easting),northing(northing),substrate(substrate){

	}

	~Label(){

	}

	double easting;
	double northing;
	std::string substrate;
};

void printUsage(){
	std::cerr << "Usage: label_soundings label-file radius" << std::endl;
	exit(1);
}

int main(int argc,char** argv){

	if(argc != 3){
		printUsage();
	}

	int radius;

	if(sscanf(argv[2],"%d",&radius) != 1){
		printUsage();
	}

	std::ifstream labelFile;
	labelFile.open(argv[1]);

	if(!labelFile){
		printUsage();
	}

	std::vector<Label*> labels;

	std::string labelString;
	std::string line;
	char substrat[1024];

	double easting;
	double northing;

	std::cerr << "Reading labels..." << std::endl;

	while(std::getline(labelFile,line)){
		if(sscanf(line.c_str(),"%lf,%lf,%s",&easting,&northing,substrat) == 3){
			Label * l = new Label(easting,northing,substrat);

			labels.push_back(l);
		}
		else{
			std::cerr << "Bad label line: " << line << std::endl;
		}
	}

	std::cerr << labels.size() << " labels loaded" << std::endl;

	labelFile.close();

	std::cout   << "x,"
                    << "y,"
                    << "z,"
                    << "sum,"
                    << "omnivariance,"
                    << "eigenentropy,"
                    << "anisotropy,"
                    << "planarity,"
                    << "linearity,"
                    << "surfaceVariation,"
                    << "sphericity,"
                    << "verticality,"
                    << "momentOrder1Axis1,"
                    << "momentOrder1Axis2,"
                    << "momentOrder2Axis1,"
                    << "momentOrder2Axis2,"
                    << "verticalRange,"
                    << "heightBelow,"
                    << "heightAbove,"
		    << "class"
		    << std::endl;
	//read stdin
	double depth;

	while(std::getline(std::cin,line)){
		if(sscanf(line.c_str(),"%lf,%lf,%*s",&easting,&northing)==2){

			//Holy O(m*n) time, Batman!
			for(auto i=labels.begin();i!=labels.end();i++){
				if(
					easting  >= (*i)->easting  - radius &&
					easting  <= (*i)->easting  + radius &&
					northing >= (*i)->northing - radius &&
					northing <= (*i)->northing + radius
				){
					printf("%s,%s\n",line.c_str(),(*i)->substrate.c_str());
					break;
				}
			}
		}
		else{
			std::cerr << "Bad sounding line: " << line << std::endl;
		}
	}

	return 0;
}
