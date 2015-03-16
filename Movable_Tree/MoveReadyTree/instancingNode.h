#ifndef _INSTANCINGNODE_H
#define _INSTANCINGNODE_H

#include "maya\MPxNode.h"
#include "maya\MFnNumericAttribute.h"
#include "maya\MFnTypedAttribute.h"
#include "maya\MGlobal.h"
#include "maya\MPxCommand.h"
#include <string>

class instancingNode:MPxNode
{
public:
	instancingNode(void);
	~instancingNode(void);
	static void *creator();

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	static MTypeId id;
	static MObject repInstancingNum;
	static MObject output;//the output
};

#endif