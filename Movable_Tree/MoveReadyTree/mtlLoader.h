#ifndef _MTLLOADER_
#define _MTLLOADER_

#include <maya\MGlobal.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<glm/glm.hpp>
using namespace std;

class mtlLoader{
private:
	string fileName,*textureFile,*tempFile;
	int componentNum;
	
public:
	mtlLoader();
	void setComponentNum(int componentNum);
	void setFileName(string fileName);
	void readFile();
	void getTempFile(string *tempFile) ;
	string *getTextureFile() const;
};

#endif