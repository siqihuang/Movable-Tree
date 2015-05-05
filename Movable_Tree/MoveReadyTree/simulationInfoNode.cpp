#include "simulationInfoNode.h"

MTypeId simulationInfoNode::id( 0x80010);
MObject simulationInfoNode::pointList;
MObject simulationInfoNode::pointNumList;
MObject simulationInfoNode::trigger;
MObject simulationInfoNode::output;
MObject simulationInfoNode::pointNum;
bool simulationInfoNode::dataSent;

simulationInfoNode::simulationInfoNode(){
	
}

simulationInfoNode::~simulationInfoNode(){
}

void* simulationInfoNode::creator(){
	return new simulationInfoNode();
}

MStatus simulationInfoNode::initialize(){
	MStatus status=MStatus::kSuccess;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	dataSent=false;

	output=tAttr.create("output","out",MFnData::kString,&status);
	addAttribute(output);
	//tAttr.setWritable(false);

	trigger=nAttr.create("trigger","tri",MFnNumericData::kBoolean,false,&status);
	addAttribute(trigger);
	//nAttr.setWritable(false);
	attributeAffects(trigger,output);

	pointList=tAttr.create("pointList","pl",MFnData::kString,&status);
	addAttribute(pointList);
	//tAttr.setWritable(false);
	//tAttr.setStorable(false);

	pointNumList=tAttr.create("pointNumList","pnl",MFnData::kString,&status);
	addAttribute(pointNumList);
	//tAttr.setWritable(false);

	pointNum=nAttr.create("pointNum","pN",MFnNumericData::kInt,0,&status);
	addAttribute(pointNum);
	//nAttr.setWritable(false);

	return status;
}

MStatus simulationInfoNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status;
	//MGlobal::displayInfo("#");
	bool tri=data.inputValue(trigger,&status).asBool();
	//MGlobal::displayInfo("$");
	if(plug==output&&!dataSent&&tri)
	{
		dataSent=true;
		int FDomainNum=fdomain_list.size();
		//MGlobal::displayInfo("^");
		int pN=data.inputValue(pointNum,&status).asInt();
		//MGlobal::displayInfo("^");
		MGlobal::displayInfo(MString(std::to_string((long double) FDomainNum).c_str()));
		MGlobal::displayInfo(MString(std::to_string((long double) pN).c_str()));

		int *pList=new int[pN];
		int *nList=new int[FDomainNum];
	
		std::string pointString=std::string(data.inputValue(pointList,&status).asString().asChar());
		std::string pointNumString=std::string(data.inputValue(pointNumList,&status).asString().asChar());
	
		extractList(pointString,pN,pList);
		MGlobal::displayInfo("!");
		extractList(pointNumString,FDomainNum,nList);
		MGlobal::displayInfo("@");

		for(int i=0,j=0;i<FDomainNum;i++){
			std::vector<int> tmp;
			int num=nList[i];
			for(int k=0;k<num;k++,j++){
				tmp.push_back(pList[j]);
			}
			constraintIndex.push_back(tmp);
		}
	}
	std::string tmp=std::to_string((long double)constraintIndex.size());
	MGlobal::displayInfo(MString(tmp.c_str()));
	return status;
}

void simulationInfoNode::extractList(string tagList,int componentNum,int list[]){
	string subList=tagList;
	for(int i=0;i<componentNum;i++){
		std::size_t found = subList.find("$$");
		list[i]=atoi(subList.substr(0,found).c_str());
		subList=subList.substr(found+2,subList.length()-1);
		//MGlobal::displayInfo(MString(tags[i].c_str()));
	}
}