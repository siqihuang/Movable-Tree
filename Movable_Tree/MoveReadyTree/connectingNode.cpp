#include "connectingNode.h"

MTypeId connectingNode::id( 0x80002 );

connectingNode::connectingNode(){
	
}

connectingNode::~connectingNode(){
}

void* connectingNode::creator(){
	return new connectingNode();
}

MStatus connectingNode::initialize()
{
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	
	MStatus returnStatus;
	
	return MS::kSuccess;
}

MStatus connectingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	return MS::kSuccess;
}