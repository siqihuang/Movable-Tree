#include "classificationNode.h"

//MTypeId id(0x80000);
MTypeId classificationNode::id( 0x80000 );
MObject classificationNode::MAFilePath;
MObject classificationNode::DomainNum;
MObject classificationNode::selectedList;
MObject classificationNode::trigger;
MObject classificationNode::output;

classificationNode::classificationNode(){
	
}

classificationNode::~classificationNode(){
}

void* classificationNode::creator(){
	return new classificationNode();
}

MStatus classificationNode::initialize(){
	MStatus status=MStatus::kSuccess;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	output=tAttr.create("output","out",MFnData::kString,&status);
	addAttribute(output);
	//tAttr.setWritable(false);

	trigger=nAttr.create("trigger","tri",MFnNumericData::kBoolean,false,&status);
	addAttribute(trigger);
	//nAttr.setWritable(false);
	attributeAffects(trigger,output);

	MAFilePath=tAttr.create("path","pa",MFnData::kString,&status);
	addAttribute(MAFilePath);
	tAttr.setWritable(true);
	tAttr.setStorable(false);

	DomainNum=nAttr.create("domainNum","dN",MFnNumericData::kInt,0,&status);
	addAttribute(DomainNum);
	nAttr.setKeyable(false);

	selectedList=tAttr.create("selected","sl",MFnData::kString,&status);
	addAttribute(selectedList);
	tAttr.setWritable(false);
	//tAttr.setStorable(false);
	/*
	BackButton=nAttr.create("BackButton","Back",MFnNumericData::kBoolean,false,&status);
	addAttribute(BackButton);
	attributeAffects(BackButton,Empty);
	*/
	return status;
}

MStatus classificationNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status;
	bool triggered=data.inputValue(trigger,&status).asBool();
	if(plug==output&&triggered){
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
	}
	/*if(plug==Empty){
		int remain=data.inputValue(remainComponent,&status).asInt();
		bool back=data.inputValue(BackButton,&status).asBool();

		if(!back) remainNum=remain-1;
		else remainNum=remain+1;

		std::string s=std::to_string(remainNum);
		MString sRemainNum(s.c_str());
		MGlobal::executeCommand("setAttr \"classification1.remain\""+sRemainNum);

		MAReader reader("C:/Users/siqiHuang/Desktop/tree/tree.ma");
		reader.readFile();
		std::string mel="select -r tree:"+reader.getComponentName(remainNum-1);
		MString sMel(mel.c_str());
		MGlobal::executeCommand(sMel);
		//MGlobal::executeCommand("setAttr \"classification1.remain\" 0");
		//MDataHandle hOutput=data.outputValue(aOutput,&status);
		//hOutput.setFloat(output);
		//hOutput.setClean();

		//MGlobal::executeCommand("select classification1");

		data.setClean(plug);
	}
	*/
	return status;
}