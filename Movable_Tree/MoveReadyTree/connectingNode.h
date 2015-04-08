#ifndef _CONNECTINGNODE_H
#define _CONNECTINGNODE_H

#include <maya/MGlobal.h>
#include <maya/MPxNode.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

class connectingNode{
public:
	connectingNode(void);
	~connectingNode(void);
	static void *creator();

	static MStatus initialize();
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	static MTypeId id;
};

#endif