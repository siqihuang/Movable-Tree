#include "mesh.h"
#include <maya/MGlobal.h>
#include <string>
#include <iostream>

mesh::mesh(std::vector<Domain *>domain){
	this->domain=domain;
	//initMesh();
}

mesh::mesh(){}

mesh::~mesh(){
}

void mesh::setDomain(std::vector<Domain *>domain){
	this->domain=domain;
}

void mesh::appendToMesh(MPointArray& points, MIntArray& faceCounts,MIntArray& faceConnects){
	for(int i=0;i<gPoints.length();i++){
		points.append(gPoints[i]);
	}
	for(int i=0;i<gFaceCounts.length();i++){
		faceCounts.append(gFaceCounts[i]);
	}
	for(int i=0;i<gFaceConnects.length();i++){
		faceConnects.append(gFaceConnects[i]);
	}
	if(gPoints.length()==0){
		std::string s=std::to_string(gPoints.length());
		MGlobal::displayInfo(MString(s.c_str()));
		s=std::to_string(gFaceCounts[0]);
		MGlobal::displayInfo(MString(s.c_str()));
		s=std::to_string(gFaceConnects.length());
		MGlobal::displayInfo(MString(s.c_str()));
	}
}

void mesh::initMesh(int num){
	gPoints.clear();
	gNormals.clear();
	gFaceCounts.clear();
	gFaceConnects.clear();

	Domain *d;
	std::vector<unsigned int > index;
	
	d=domain[num];
	for(int j=0;j<d->face_list.size();j++){
		Face *f=d->face_list[j];
		gFaceCounts.append(f->vertex_coords.size());//vertex count in face
		for(int k=0;k<f->vertex_coords.size();k++){
			if(checkPoints(f->vertex_indexs[k],index)){//ensure not replicate points
				index.push_back(f->vertex_indexs[k]);
				MPoint p(f->vertex_coords[k].x,f->vertex_coords[k].y,f->vertex_coords[k].z);
				gPoints.append(p);
			}//if
			int n=findIndex(f->vertex_indexs[k],index);
			gFaceConnects.append(n);
		}//for k
	}//for j
}

bool mesh::checkPoints(unsigned int index,std::vector<unsigned int> indices){
	int length=indices.size();
	for(int i=0;i<length;i++){
		if(indices[i]==index) return false;
	}
	return true;
}

int mesh::findIndex(unsigned int index,std::vector<unsigned int> indices){
	int length=indices.size();
	for(int i=0;i<length;i++){
		if(indices[i]==index) return i;
	}
	return 0;
}