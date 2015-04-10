#ifndef _DELETEEDGENODE_H
#define _DELETEEDGENODE_H

#include <maya/MGlobal.h>
#include <maya/MPxNode.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include "FdomainGraph.h"
#include "CommonData.h"

class deleteEdgeNode:MPxNode{
public:
	deleteEdgeNode(void);
	~deleteEdgeNode(void);
	static void *creator();
	void setLoop(std::vector<int> loop);
	void setRedundantEdgeNum();
	void turnOffTrigger(MDataBlock &data);

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	static MTypeId id;
	static MObject tmp;
	static MObject tmp1;
	static MObject index1;
	static MObject index2;
	static MObject trigger;
	static int state;
	FdomainGraph fdg;
};

#endif