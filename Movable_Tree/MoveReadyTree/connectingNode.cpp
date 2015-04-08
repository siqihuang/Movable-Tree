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
			extractBlockNum();	
			state=1;
		}
		else if(state==1){
			MGlobal::executeCommand("clear($connectingBlock)");//clear the last data
			hideOtherMesh();
			setBlockGroup();
			state=2;
		}
		else if(state==2){
			//fdg.connect_edge();

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
	it=fgraph.begin();//once at a time, no need to loop, recompute after every operation
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