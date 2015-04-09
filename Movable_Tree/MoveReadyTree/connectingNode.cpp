#include "connectingNode.h"

MTypeId connectingNode::id( 0x80002 );
MObject connectingNode::tmp;
MObject connectingNode::tmp1;
MObject connectingNode::index1;
MObject connectingNode::index2;
MObject connectingNode::trigger;
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
	connectingNode::index1=nAttr.create("index1","in1",MFnNumericData::kInt,-1,&returnStatus);
	connectingNode::index2=nAttr.create("index2","in2",MFnNumericData::kInt,-1,&returnStatus);
	connectingNode::trigger=nAttr.create("trigger","tri",MFnNumericData::kBoolean,true,&returnStatus);

	addAttribute(tmp);
	addAttribute(tmp1);
	addAttribute(index1);
	addAttribute(index2);
	addAttribute(trigger);

	attributeAffects(tmp1,tmp);

	return MS::kSuccess;
}

MStatus connectingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	bool tri=data.inputValue(trigger,&status).asBool();
	if(plug==tmp&&tri){
		//4.3 computer F-domain graph
		if(state==0){
			fdg.compute();
			extractBlockNum();	
			state=1;
			turnOffTrigger(data);
		}
		else if(state==1){
			MGlobal::executeCommand("clear($connectingBlock)");//clear the last data
			hideOtherMesh();
			setBlockGroup();
			MGlobal::executeCommand("highlightBlocks();");
			state=2;
			turnOffTrigger(data);
		}
		else if(state==2){
			int in1=data.inputValue(index1,&status).asInt();
			int in2=data.inputValue(index2,&status).asInt();
			in1=domain_list[in1]->index;
			in2=domain_list[in2]->index;
			fdg.connect_edge(in1,in2);
			extractBlockNum();
			state=1;
			turnOffTrigger(data);
		}
	}
	return MS::kSuccess;
}

void connectingNode::extractBlockNum(){
	std::string num=std::to_string(fdomain_components.size());
	MString com="$unconnectedDomainNum="+MString(num.c_str())+";";
	com+="string $tmp=\"unconnected domains: \"+$unconnectedDomainNum;";
	com+="text -edit -label $tmp $domainsToBeConnected;";
	MGlobal::executeCommand(com);
}

void connectingNode::hideOtherMesh(){
	MGlobal::executeCommand("hide -all");//hide all domain first;
	std::string com;
	com="select ";
	for(int i=0;i<fdomain_list.size();i++){
		com+="instancing"+std::to_string(fdomain_list[i]->index)+" ";
	}
	std::string s=std::to_string(fdomain_list.size());
	MGlobal::displayInfo(MString(s.c_str()));
	com+=";showHidden -above;";
	MGlobal::executeCommand(MString(com.c_str()));
}

void connectingNode::setBlockGroup(){
	std::map<Domain*, std::vector<Domain*>>::iterator it;
	int n=0;
	std::string com;
	com="";
	it=fdomain_components.begin();//once at a time, no need to loop, recompute after every operation
	com="$connectingBlock["+std::to_string(n)+"]="+std::to_string(it->first->index)+";";
	MGlobal::executeCommand(MString(com.c_str()));
	n++;
	for(int i = 0; i < it->second.size(); ++i)
	{
		com="$connectingBlock["+std::to_string(n)+"]="+std::to_string(it->second[i]->index)+";";
		MGlobal::executeCommand(MString(com.c_str()));
		n++;
		MGlobal::displayInfo(MString(com.c_str()));
	}
	state=2;
}

void connectingNode::turnOffTrigger(MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	MDataHandle triggerHandle=data.outputValue(trigger,&status);
	triggerHandle.setBool(false);
}