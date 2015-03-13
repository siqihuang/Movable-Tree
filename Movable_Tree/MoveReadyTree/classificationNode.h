#ifndef _CLASSIFICATIONNODE_H
#define _CLASSIFICATIONNODE_H

#include "maya\MPxNode.h"
#include "maya\MFnNumericAttribute.h"
#include "maya\MFnTypedAttribute.h"
#include "maya\MGlobal.h"
#include "maya\MPxCommand.h"
#include "objLoader.h"
#include <string>

class classificationNode:MPxNode
{
public:
	classificationNode(void);
	~classificationNode(void);
	static void *creator();

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	static MTypeId id;
	static MObject MAFilePath;//the ma file path
	static MObject DomainNum;//the domain number
	static MObject selectedList;//the selected list
	static MObject trigger;//the trigger that trigger compute
	static MObject output;//the output
	objLoader loader;
};

#endif