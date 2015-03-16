#include "mtlLoader.h"

mtlLoader::mtlLoader(){}

void mtlLoader::setComponentNum(int componentNum){
	this->componentNum=componentNum;
}

void mtlLoader::setFileName(string fileName){
	this->fileName=fileName;
}

void mtlLoader::getTempFile(string *tempFile){
	this->textureFile=new string[componentNum];
	this->tempFile=new string[componentNum];
	for(int i=0;i<componentNum;i++) this->tempFile[i]=tempFile[i];
}

string *mtlLoader::getTextureFile(){
	return textureFile;
}

void mtlLoader::readFile(){
	ifstream inObj;
	//MGlobal::displayInfo(MString(textureFile[0].c_str()));
	for(int i=0;i<componentNum;i++){
		inObj.open(fileName);
		string input;

		inObj>>input;
		while(input!=tempFile[i]&&!inObj.eof()){
			inObj>>input;
		}
		
		inObj>>input;
		while(input!="map_Kd"){
			inObj>>input;
		}
		inObj>>textureFile[i];

		MGlobal::displayInfo(MString(textureFile[i].c_str()));
		inObj.close();
	}
	/*inObj.open(fileName);
	while(!inObj.eof()){
		string input;
		inObj>>input;
		MGlobal::displayInfo(MString(input.c_str()));
	}*/
}