#include "instancingNode.h"

//MTypeId id(0x80000);
MTypeId instancingNode::id( 0x80001 );
MObject instancingNode::repInstancingNum;
MObject instancingNode::output;

instancingNode::instancingNode(){
	
}

instancingNode::~instancingNode(){
}

void* instancingNode::creator(){
	return new instancingNode();
}

MStatus instancingNode::initialize(){
	MStatus status=MStatus::kSuccess;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	output=tAttr.create("output","out",MFnData::kString,&status);
	addAttribute(output);
	
	repInstancingNum=nAttr.create("represent","rep",MFnNumericData::kInt,0,&status);
	addAttribute(repInstancingNum);
	
	//tAttr.setStorable(false);
	/*
	BackButton=nAttr.create("BackButton","Back",MFnNumericData::kBoolean,false,&status);
	addAttribute(BackButton);
	attributeAffects(BackButton,Empty);
	*/
	return status;
}

MStatus instancingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	//bool triggered=data.inputValue(trigger,&status).asBool();
	/*if(plug==output&&triggered){
		int componentNum=data.inputValue(DomainNum,&status).asInt();
		MString MmeshList=data.inputValue(selectedList,&status).asString();
		MString MfilePath=data.inputValue(MAFilePath,&status).asString();
		string meshList=string(MmeshList.asChar());
		string filePath=string(MfilePath.asChar());
		loader.setComponentNum(componentNum);
		loader.setFileName(filePath);
		loader.setMeshList(meshList);
		loader.extractList(meshList,componentNum);
		loader.readObj();

		MGlobal::executeCommand("createInstancingGUI()");
		MGlobal::executeCommand("$reprensentativeInstanceNum=10");
	}*/
	return status;
}