#include "connectingNode.h"

MTypeId connectingNode::id( 0x80002 );
MObject connectingNode::tmp;
MObject connectingNode::tmp1;
MObject connectingNode::index1;
MObject connectingNode::index2;
MObject connectingNode::trigger;
MObject connectingNode::rootNumber;
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
	connectingNode::rootNumber=nAttr.create("rootNum","rN",MFnNumericData::kInt,0,&returnStatus);

	addAttribute(tmp);
	addAttribute(tmp1);
	addAttribute(index1);
	addAttribute(index2);
	addAttribute(trigger);
	addAttribute(rootNumber);

	attributeAffects(tmp1,tmp);

	return MS::kSuccess;
}

MStatus connectingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	bool tri=data.inputValue(trigger,&status).asBool();
	if(plug==tmp&&tri){
		//4.3 compute F-domain graph
		if(state==0)
		{
			//jason
			//fdg.beforeInit();
			fdg.InitAnchorPoints();
			fdg.compute();
			extractBlockNum();	

			state=1;

			std::string fdNum=std::to_string(fdomain_list.size());
			MGlobal::executeCommand("$FDomainNum="+MString(fdNum.c_str())+";");
			//pass F Domain numbers to mel

			std::string preferredRoot=std::to_string(fdg.GetInitialRootDomain());
			MGlobal::executeCommand("$preferredDomainIndex="+MString(preferredRoot.c_str())+";");
			//pass preferred root index to mel
			turnOffTrigger(data);
		}
		else if(state==1){//compute new blocks num and highlight blocks
			MGlobal::executeCommand("clear($connectingBlock)");//clear the last data
			hideOtherMesh();
			setBlockGroup();
			MGlobal::executeCommand("highlightBlocks();");
			if(fdomain_components.size()>1)
				state=2;
			else 
				state=3;
			turnOffTrigger(data);
		}
		else if(state==2){//connect edge
			int in1=data.inputValue(index1,&status).asInt();
			int in2=data.inputValue(index2,&status).asInt();
			in1=domain_list[in1]->index;
			in2=domain_list[in2]->index;
			fdg.connect_edge(in1,in2);
			extractBlockNum();
			state=1;
			turnOffTrigger(data);
		}
		else if(state==3){//select root domain
			int rootIndex=data.inputValue(rootNumber,&status).asInt();
			std::string s=std::to_string(rootIndex);
			MGlobal::displayInfo(MString(s.c_str()));
			fdg.SetRootDomain(rootIndex);
			MGlobal::displayInfo("successful!");
			turnOffTrigger(data);
		}
	}
	return MS::kSuccess;
}

void connectingNode::extractBlockNum(){
	std::string num=std::to_string(fdomain_components.size());
	MString com="$unconnectedDomainNum="+MString(num.c_str())+";";
	com+="string $tmp=\"connected domains: \"+$unconnectedDomainNum;";
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