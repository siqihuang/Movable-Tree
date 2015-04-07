#include "objLoader.h"

objLoader::objLoader(string fileName,string meshList,int componentNum){
	this->fileName=fileName;
	this->componentNum=componentNum;
	meshName=new string[componentNum];
}

void objLoader::extractList(string meshList,int componentNum){
	string subList=meshList;
	meshName=new string[componentNum];
	tempName=new string[componentNum];
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
	UV_COORDS=new vector<glm::vec2>[componentNum];
	faceInfo=new vector<string>[componentNum];
	index=new vector<vector<unsigned int>>[componentNum];
	UV_INDEXS = new vector<vector<unsigned int>>[componentNum];
}

int objLoader::GetComponentNum()
{
	return componentNum;
}

vector<glm::vec3>* objLoader::GetVertex()
{
	return vertex;	
}
vector<glm::vec2>* objLoader::GetUV()
{
	return UV_COORDS;
}

vector<string>* objLoader::GetFaceInfo()
{
	return faceInfo;
}

vector<vector<unsigned int>>* objLoader::GetIndex()
{
	return index;
}

string *objLoader::getTempName(){
	return tempName;
}

int count=0;

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
			UV_COORDS[n].push_back(vec);
		}
		else if(n<componentNum&&input==meshName[n]){
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
		else if(input=="usemtl"){
			inObj>>tempName[n-1];
			MGlobal::displayInfo(MString(tempName[n-1].c_str()));
		}
	}
	
	string sub;
	for(int i=0;i<componentNum;i++)
	{
		for(int j=0;j<faceInfo[i].size();j++)
		{
			vector<unsigned int> v;
			vector<unsigned int> uv_idx;
			sub=faceInfo[i][j];
			std::size_t found = sub.find(" ");
			while(found!=-1){
				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find("/");
				//vertex
				int idx = stoi(sub.substr(0,found));
				if(i > 0)
				{
					for(int k = 0; k < i; ++k)
					{
						idx -= vertex[k].size();
					}
				}
				v.push_back(idx);

				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find("/");
				
				//uv_idx
				idx = stoi(sub.substr(0,found));
				if(i > 0)
				{
					for(int k = 0; k < i; ++k)
					{
						idx -= UV_COORDS[k].size();
					}
				}
				uv_idx.push_back(idx);
				//
				sub=sub.substr(found+1,sub.size()-1);
				found=sub.find(" ");
			}
			index[i].push_back(v);
			UV_INDEXS[i].push_back(uv_idx);
		}
		//debug
		/*
		for(int i = 0; i < componentNum; ++i)
		{
			for(int j = 0; j < UV_INDEXS[i].size(); ++j)
			{
				for(int k = 0; k < UV_INDEXS[i][j].size(); ++k)
					printf("uv idx:%d ", UV_INDEXS[i][j][k]);
			}
		}
		*/
	}
	
	MGlobal::displayInfo("Reading complete!");

	inObj.close();
}