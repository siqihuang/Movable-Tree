#include "deleteEdgeNode.h"

MTypeId deleteEdgeNode::id( 0x80003 );
MObject deleteEdgeNode::tmp;
MObject deleteEdgeNode::tmp1;
MObject deleteEdgeNode::index1;
MObject deleteEdgeNode::index2;
MObject deleteEdgeNode::trigger;
int deleteEdgeNode::state;

deleteEdgeNode::deleteEdgeNode(){
	
}

deleteEdgeNode::~deleteEdgeNode(){
}

void* deleteEdgeNode::creator(){
	return new deleteEdgeNode();
}

MStatus deleteEdgeNode::initialize()
{
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	
	MStatus returnStatus;

	deleteEdgeNode::state=0;

	deleteEdgeNode::tmp=nAttr.create("tmp","t",MFnNumericData::kInt,0,&returnStatus);
	deleteEdgeNode::tmp1=nAttr.create("tmp1","t1",MFnNumericData::kInt,0,&returnStatus);
	deleteEdgeNode::index1=nAttr.create("index1","in1",MFnNumericData::kInt,-1,&returnStatus);
	deleteEdgeNode::index2=nAttr.create("index2","in2",MFnNumericData::kInt,-1,&returnStatus);
	deleteEdgeNode::trigger=nAttr.create("trigger","tri",MFnNumericData::kBoolean,true,&returnStatus);

	addAttribute(tmp);
	addAttribute(tmp1);
	addAttribute(index1);
	addAttribute(index2);
	addAttribute(trigger);

	attributeAffects(tmp1,tmp);

	return MS::kSuccess;
}

MStatus deleteEdgeNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	bool tri=data.inputValue(trigger,&status).asBool();
	if(plug==tmp&&tri){
		//4.3 computer F-domain graph
		if(state==0){
			std::vector<int> loop;
			loop=fdg.BeginRemoveLoops();
			setLoop(loop);
			setRedundantEdgeNum();
			if(fdg.GetRedundantEdgesSize()==0){
				state=2;
				MGlobal::executeCommand("button -edit -enable true $deleteEdgeOKButton");
			}
			else state=1;
			turnOffTrigger(data);
		}
		else if(state==1){//delete edges
			int in1=data.inputValue(index1,&status).asInt();
			int in2=data.inputValue(index2,&status).asInt();
			in1=domain_list[in1]->index;
			in2=domain_list[in2]->index;
			fdg.DelEdgeExternal(in1,in2);
			MGlobal::displayInfo("!");

			std::vector<int> loop;
			loop=fdg.FindLoops();
			setLoop(loop);
			setRedundantEdgeNum();
			if(fdg.GetRedundantEdgesSize()==0){
				state=2;
				MGlobal::executeCommand("button -edit -enable true $deleteEdgeOKButton");
			}

			turnOffTrigger(data);
		}
		else if(state==2){//connect edge
			MGlobal::displayInfo("@");
			fdg.AddRdomainToGraph();
			turnOffTrigger(data);
		}
		else if(state==3){//select root domain

			turnOffTrigger(data);
		}
	}
	return MS::kSuccess;
}


void deleteEdgeNode::turnOffTrigger(MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	MDataHandle triggerHandle=data.outputValue(trigger,&status);
	triggerHandle.setBool(false);
}

void deleteEdgeNode::setLoop(std::vector<int> loop){
	MGlobal::executeCommand("clear($loopEdge);");
	for(int i=0;i<loop.size();i++){
		std::string com;
		com="$loopEdge["+std::to_string(i)+"]="+std::to_string(loop[i])+";";
		MGlobal::executeCommand(MString(com.c_str()));
	}
	MGlobal::executeCommand("highlightLoops()");
}

void deleteEdgeNode::setRedundantEdgeNum(){
	std::string com="$redundantEdgeNum=";
	com+=std::to_string(fdg.GetRedundantEdgesSize())+";";
	std::cout<<fdg.GetRedundantEdgesSize()<<std::endl;
	MGlobal::executeCommand(MString(com.c_str()));
	MGlobal::executeCommand("getRedundantNum()");
}