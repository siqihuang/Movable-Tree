#ifndef _CLASSIFICATIONNODE_H
#define _CLASSIFICATIONNODE_H

#include "maya\MPxNode.h"
#include "maya\MFnNumericAttribute.h"
#include "maya\MFnTypedAttribute.h"
#include "maya\MGlobal.h"
#include "maya\MPxCommand.h"
#include "objLoader.h"
#include "InstancingModule.h"
#include "FdomainGraph.h"
#include "union_find.h"
#include "mtlLoader.h"
#include <string>

class classificationNode:MPxNode
{
public:
	classificationNode(void);
	~classificationNode(void);
	static void *creator();

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	void extractList(string tagList,int componentNum);

	static MTypeId id;
	static MObject ObjFilePath;//the obj file path
	static MObject MtlFilePath;//the mtl file path
	static MObject DomainNum;//the domain number
	static MObject selectedList;//the selected list
	static MObject trigger;//the trigger that trigger compute
	static MObject tagList;//tag list
	static MObject output;//the output
	
	UnionFind uf;
	InstancingModule im;
	FdomainGraph fdg;

	string *tags;
	objLoader Objloader;
	mtlLoader Mtlloader;
};

#endif