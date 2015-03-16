#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <maya\MGlobal.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<glm/glm.hpp>

#include "CommonData.h"

using namespace std;

class objLoader{
public:
	int componentNum;
	string fileName,meshList;
	string *meshName;
	vector<glm::vec3> *vertex;
	vector<glm::vec2> *UV_COORDS;
	vector<string> *faceInfo;
	vector<vector<unsigned int>> *index;
	vector<vector<unsigned int>> *UV_INDEXS;

	objLoader(){};
	objLoader(string fileName,string meshList,int componentNum);
	void setFileName(string fileName);
	void setMeshList(string meshList);
	void setComponentNum(int componentNum);
	void readObj();
	void extractList(string meshList,int componentNum);
	
	vector<glm::vec3>* GetVertex();
	vector<glm::vec2>* GetUV();
	vector<string>* GetFaceInfo();
	vector<vector<unsigned int>>* GetIndex();
	int GetComponentNum();
};

#endif