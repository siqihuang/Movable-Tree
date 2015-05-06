#ifndef _SIMULATIONINFONODE_H
#define _SIMULATIONINFONODE_H

#include "maya\MPxNode.h"
#include "maya\MFnNumericAttribute.h"
#include "maya\MFnTypedAttribute.h"
#include "maya\MGlobal.h"
#include "maya\MPxCommand.h"
#include "objLoader.h"
#include "InstancingModule.h"
#include "union_find.h"
#include "mtlLoader.h"
#include <string>

class simulationInfoNode:MPxNode
{
public:
	simulationInfoNode(void);
	~simulationInfoNode(void);
	static void *creator();

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	void extractList(string tagList,int componentNum,int list[]);

	static MTypeId id;
	static MObject pointList;//the fix point list
	static MObject pointNumList;//the fix point num list
	static MObject trigger;//the trigger that trigger compute
	static MObject output;//the output
	static MObject pointNum;//the fix vertex number
	static MObject parentPointList;//the parent point list
	static MObject parentPointNumList;//the parent point num list
	static MObject parentPointNum;//the parent point num
	static MObject domainParentList;//the domain parent list
	static bool dataSent;
};


#endif