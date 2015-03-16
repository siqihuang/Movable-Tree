#ifndef INSTANCING_H 
#define INSTANCING_H 

#include "CommonData.h"
#include "UnionFindDomain.h"
#include<ctime>
#include<iostream>

const float EPS = 1e-3f;

class InstancingModule 
{
public: 
	void Instancing();
	//void Instancing(vector<Domain*>&domain_list);
	void TextureMap();
	//void TextureMap(vector<Domain*>&domain_list);
	bool _TextureMap(Domain *a, Domain *b);
	void print(); //for test
	//void print(vector<Domain*>&domain_list);

};

#endif
