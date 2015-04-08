#include "connectingNode.h"

MTypeId connectingNode::id( 0x80002 );
MObject connectingNode::tmp;
MObject connectingNode::tmp1;
int connectingNode::state;

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

	connectingNode::state=0;

	connectingNode::tmp=nAttr.create("tmp","t",MFnNumericData::kInt,0,&returnStatus);
	connectingNode::tmp1=nAttr.create("tmp1","t1",MFnNumericData::kInt,0,&returnStatus);
	
	addAttribute(tmp);
	addAttribute(tmp1);

	attributeAffects(tmp1,tmp);

	return MS::kSuccess;
}

MStatus connectingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	if(plug==tmp){
		//4.3 computer F-domain graph
		if(state==0){
			fdg.compute();
			
			state=1;
		}

	}
	return MS::kSuccess;
}