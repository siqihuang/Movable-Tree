#include "FileAnalysis.h"
#include "maya\MGlobal.h"

MAReader::MAReader(string fileName){
	this->fileName=fileName;
	reader.open(fileName);
	componentNum=0;
}

MAReader::MAReader(){

}

void MAReader::readFile(){
	string str,substr;
	char buffer[256];
	while(reader.getline(buffer,256,'\n')){
		str=buffer;
		substr=str.substr(0,15);
		if(substr=="createNode mesh"){
			std::size_t found = str.find("-p");
			substr=str.substr(found+4,str.length()-6-found);
			componentName.push_back(substr);
			//MString s(substr.c_str());
			componentNum++;
			//cout<<substr<<endl;
			//MGlobal::displayInfo(s);
		}
	}
}

int MAReader::getComponentNum(){
	return componentNum;
}

string MAReader::getComponentName(int n){
	return componentName[n];
}