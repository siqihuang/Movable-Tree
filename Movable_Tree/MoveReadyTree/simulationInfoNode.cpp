#include "simulationInfoNode.h"

MTypeId simulationInfoNode::id( 0x80010);
MObject simulationInfoNode::pointList;
MObject simulationInfoNode::pointNumList;
MObject simulationInfoNode::trigger;
MObject simulationInfoNode::output;
MObject simulationInfoNode::pointNum;
MObject simulationInfoNode::parentPointList;
MObject simulationInfoNode::parentPointNumList;
MObject simulationInfoNode::parentPointNum;
MObject simulationInfoNode::domainParentList;
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

	pointNumList=tAttr.create("pointNumList","pnl",MFnData::kString,&status);
	addAttribute(pointNumList);

	pointNum=nAttr.create("pointNum","pN",MFnNumericData::kInt,0,&status);
	addAttribute(pointNum);

	parentPointList=tAttr.create("parentPointList","ppl",MFnData::kString,&status);
	addAttribute(parentPointList);

	parentPointNumList=tAttr.create("parentPointNumList","ppnl",MFnData::kString,&status);
	addAttribute(parentPointNumList);

	parentPointNum=nAttr.create("parentPointNum","ppN",MFnNumericData::kInt,0,&status);
	addAttribute(parentPointNum);

	domainParentList=tAttr.create("domainParentList","dpl",MFnData::kString,&status);
	addAttribute(domainParentList);

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
		int ppN=data.inputValue(parentPointNum,&status).asInt();
		//MGlobal::displayInfo("^");
		//MGlobal::displayInfo(MString(std::to_string((long double) FDomainNum).c_str()));
		//MGlobal::displayInfo(MString(std::to_string((long double) pN).c_str()));

		int *pList=new int[pN];
		int *nList=new int[FDomainNum];
		int *ppList=new int[ppN];
		int *pnList=new int[FDomainNum];
		int *dpList=new int[FDomainNum];
	
		std::string pointString=std::string(data.inputValue(pointList,&status).asString().asChar());
		std::string pointNumString=std::string(data.inputValue(pointNumList,&status).asString().asChar());
		std::string parentPointString=std::string(data.inputValue(parentPointList,&status).asString().asChar());
		std::string parentPointNumString=std::string(data.inputValue(parentPointNumList,&status).asString().asChar());
		std::string domainParentString=std::string(data.inputValue(domainParentList,&status).asString().asChar());

		extractList(pointString,pN,pList);
		//MGlobal::displayInfo("!");
		extractList(pointNumString,FDomainNum,nList);
		extractList(parentPointString,ppN,ppList);
		extractList(parentPointNumString,FDomainNum,pnList);
		extractList(domainParentString,FDomainNum,dpList);

		MGlobal::displayInfo("@");

		for(int i=0,j=0;i<FDomainNum;i++){
			std::vector<int> tmp;
			int num=nList[i];
			for(int k=0;k<num;k++,j++){
				tmp.push_back(pList[j]);
			}
			constraintIndex.push_back(tmp);
		}//for constraint on self

		for(int i=0,j=0;i<FDomainNum;i++){
			std::vector<int> tmp;
			int num=pnList[i];
			for(int k=0;k<num;k++,j++){
				tmp.push_back(ppList[j]);
			}
			parentConstraintIndex.push_back(tmp);
		}

		for(int i=0;i<FDomainNum;i++){
			domainParentIndex.push_back(dpList[i]);
		}

		for(int i=0;i<FDomainNum;i++){
			parentLastPosOld.push_back(glm::dvec3(0,0,0));
			parentLastPosNew.push_back(glm::dvec3(0,0,0));
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