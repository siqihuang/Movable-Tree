#include "classificationNode.h"
#include "CommonData.h"

//MTypeId id(0x80000);
MTypeId classificationNode::id( 0x80000 );
MObject classificationNode::ObjFilePath;
MObject classificationNode::MtlFilePath;
MObject classificationNode::DomainNum;
MObject classificationNode::selectedList;
MObject classificationNode::trigger;
MObject classificationNode::tagList;
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

	ObjFilePath=tAttr.create("pathObj","paO",MFnData::kString,&status);
	addAttribute(ObjFilePath);
	tAttr.setWritable(true);
	tAttr.setStorable(false);

	MtlFilePath=tAttr.create("pathMtl","paM",MFnData::kString,&status);
	addAttribute(MtlFilePath);
	tAttr.setWritable(true);
	tAttr.setStorable(false);

	DomainNum=nAttr.create("domainNum","dN",MFnNumericData::kInt,0,&status);
	addAttribute(DomainNum);
	nAttr.setKeyable(false);

	tagList=tAttr.create("tags","ta",MFnData::kString,&status);
	addAttribute(tagList);
	tAttr.setKeyable(false);

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
	if(plug==output&&triggered)
	{
		int componentNum=data.inputValue(DomainNum,&status).asInt();
		MString MmeshList=data.inputValue(selectedList,&status).asString();
		MString MObjPath=data.inputValue(ObjFilePath,&status).asString();
		MString MMtlPath=data.inputValue(MtlFilePath,&status).asString();
		MString Mtag=data.inputValue(tagList,&status).asString();
		string meshList=string(MmeshList.asChar());		
		string ObjPath=string(MObjPath.asChar());
		string MtlPath=string(MMtlPath.asChar());
		string tag = string(Mtag.asChar());		

		
		extractList(tag,componentNum);

		Objloader.setComponentNum(componentNum);
		Objloader.setFileName(ObjPath);
		Objloader.setMeshList(meshList);
		Objloader.extractList(meshList,componentNum);
		Objloader.readObj();

		Mtlloader.setComponentNum(componentNum);
		Mtlloader.setFileName(MtlPath);
		Mtlloader.getTempFile(Objloader.getTempName());
		Mtlloader.readFile();
		
		//MGlobal::executeCommand("createInstancingGUI()");
		MGlobal::executeCommand("$reprensentativeInstanceNum=10");
		
		//4.1 union find
		uf.Union(Objloader, Mtlloader, tags, componentNum);
		//4.2 instancing
		im.Instancing();
		//4.3 computer F-domain graph
		//fdg.compute();
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

void classificationNode::extractList(string tagList,int componentNum){
	string subList=tagList;
	tags=new string[componentNum];
	for(int i=0;i<componentNum;i++){
		std::size_t found = subList.find("$$");
		tags[i]=subList.substr(0,found);
		subList=subList.substr(found+2,subList.length()-1);
		MGlobal::displayInfo(MString(tags[i].c_str()));
	}
}