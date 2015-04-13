#ifndef _INSTANCINGNODE_H
#define _INSTANCINGNODE_H

#include <maya/MGlobal.h>
#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>


#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>

#include <maya/MIOStream.h>
#include <string>
#include <vector>
#include "CommonData.h"
#include "mesh.h"

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
	static std::vector<MObject>outputMesh;//the output
	static MObject trigger;//the trigger
	static MObject command;//command
	static MObject x,y,z;
	static MObject instancingNum;
	static int domainNum,state;
	mesh m;
protected:
	MObject createMesh(int n,MObject& outData, MStatus& stat);
};

#endif