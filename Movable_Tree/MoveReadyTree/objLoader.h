#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <maya\MGlobal.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<glm/glm.hpp>

using namespace std;

class objLoader{
private:
	int componentNum;
	string fileName,meshList;
	string *meshName;
	vector<glm::vec3> *vertex;
	vector<glm::vec2> *uv;
	vector<string> *faceInfo;
	vector<vector<unsigned int>> *index;

public:
	objLoader(){};
	objLoader(string fileName,string meshList,int componentNum);
	void setFileName(string fileName);
	void setMeshList(string meshList);
	void setComponentNum(int componentNum);
	void readObj();
	void extractList(string meshList,int componentNum);
};

#endif