#include "objLoader.h"

objLoader::objLoader(string fileName,string meshList,int componentNum){
	this->fileName=fileName;
	this->componentNum=componentNum;
	meshName=new string[componentNum];
}

void objLoader::extractList(string meshList,int componentNum){
	string subList=meshList;
	meshName=new string[componentNum];
	for(int i=0;i<componentNum;i++){
		std::size_t found = subList.find("$$");
		meshName[i]=subList.substr(0,found);
		subList=subList.substr(found+2,subList.length()-1);
		//MGlobal::displayInfo(MString(meshName[i].c_str()));
	}
}

void objLoader::setFileName(string fileName){
	this->fileName=fileName;
}

void objLoader::setMeshList(string meshList){
	this->meshList=meshList;
}

void objLoader::setComponentNum(int componentNum){
	this->componentNum=componentNum;
	vertex=new vector<glm::vec3>[componentNum];
	uv=new vector<glm::vec2>[componentNum];
	faceInfo=new vector<string>[componentNum];
	index=new vector<vector<unsigned int>>[componentNum];
}

void objLoader::readObj(){
	ifstream inObj;
	inObj.open(fileName);
	int n=0;
	string input;

	MGlobal::displayInfo("Reading meshes from file, please wait!");

	while(!inObj.eof()){
		inObj>>input;
		if(input=="v"){
			glm::vec3 vec;
			inObj>>vec.x;
			inObj>>vec.y;
			inObj>>vec.z;
			vertex[n].push_back(vec);
		}
		else if(input=="vn"){
			glm::vec3 vec;
			inObj>>vec.x;
			inObj>>vec.y;
			inObj>>vec.z;
			//m->saveNormal(vec);
		}
		else if(input=="vt"){
			glm::vec2 vec;
			inObj>>vec.x;
			inObj>>vec.y;
			uv[n].push_back(vec);
		}
		else if(n<3&&input==meshName[n]){
			n++;
			MString num(std::to_string(n).c_str());
			MString s="reading the "+num+" mesh";
			MGlobal::displayInfo(s);
		}
		else if(input=="f"){
			char buffer[256];
			inObj.getline(buffer,256,'\n');
			string str=buffer;
			faceInfo[n-1].push_back(str);
		}
	}
	
	string sub;
	for(int i=0;i<componentNum;i++){
		for(int j=0;j<faceInfo[i].size();j++){
			vector<unsigned int> v;
			sub=faceInfo[i][j];
			std::size_t found = sub.find(" ");
			while(found!=-1){
				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find("/");
				v.push_back(stoi(sub.substr(0,found)));
				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find("/");
				//
				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find(" ");
				//
			}
			index[i].push_back(v);
		}
	}
	
	MGlobal::displayInfo("Reading complete!");

	inObj.close();
}