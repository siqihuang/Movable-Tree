#include "instancingNode.h"
#define DOMAINS 409//tree1 409,tree2 517, tree3 4516
MTypeId instancingNode::id( 0x80001 );
MObject instancingNode::repInstancingNum;
std::vector<MObject> instancingNode::outputMesh;//the output
MObject instancingNode::trigger;//the trigger
MObject instancingNode::command;//command
MObject instancingNode::x;
MObject instancingNode::y;
MObject instancingNode::z;
MObject instancingNode::instancingNum;
int instancingNode::state;
int instancingNode::domainNum;

MStatus returnStatus;

instancingNode::instancingNode(){
	
}

instancingNode::~instancingNode(){
}

void* instancingNode::creator(){
	return new instancingNode();
}

MStatus instancingNode::initialize()
{
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	
	MStatus returnStatus;

	instancingNode::state=0;

	instancingNode::repInstancingNum=nAttr.create("represent","rep",MFnNumericData::kInt,0,&returnStatus);

	instancingNode::trigger=nAttr.create("trigger","tri",MFnNumericData::kBoolean,false,&returnStatus);

	instancingNode::command=tAttr.create("command","com",MFnData::kString,&returnStatus);

	instancingNode::x=nAttr.create("PosX","x",MFnNumericData::kFloat,0,&returnStatus);

	instancingNode::y=nAttr.create("PosY","y",MFnNumericData::kFloat,0,&returnStatus);

	instancingNode::z=nAttr.create("PosZ","z",MFnNumericData::kFloat,0,&returnStatus);

	instancingNode::instancingNum=nAttr.create("instNum","iNum",MFnNumericData::kInt,-1,&returnStatus);

	MString longFlag,shortFlag;
	for(int i=0;i<DOMAINS;i++){
		longFlag="outputMesh"+MString(std::to_string(i).c_str());
		shortFlag="out"+MString(std::to_string(i).c_str());
		MObject tempMesh=tAttr.create( longFlag, shortFlag, MFnData::kMesh, &returnStatus ); 
		instancingNode::outputMesh.push_back(tempMesh);
	}
	
	returnStatus=addAttribute(instancingNode::repInstancingNum);

	returnStatus=addAttribute(instancingNode::trigger);
	
	returnStatus=addAttribute(instancingNode::command);

	returnStatus=addAttribute(instancingNode::x);

	returnStatus=addAttribute(instancingNode::y);

	returnStatus=addAttribute(instancingNode::z);

	returnStatus=addAttribute(instancingNode::instancingNum);

	for(int i=0;i<DOMAINS;i++)
		returnStatus=addAttribute(instancingNode::outputMesh[i]);
	
	attributeAffects(trigger,outputMesh[0]);
	
	return MS::kSuccess;
}

MStatus instancingNode::compute(const MPlug &plug,MDataBlock &data){
	MStatus status=MStatus::kSuccess;
	if(plug==outputMesh[0]&&state==0){
		int length=domain_list.size();
		m.setDomain(domain_list);

		std::string s=std::to_string(length);
		MGlobal::displayInfo(s.c_str());

		MStatus returnStatus;

		for(int i=0;i<length;i++){
			MDataHandle outputHandle=data.outputValue(outputMesh[i],&returnStatus);
			MFnMeshData dataCreator;
			MObject newOutputData = dataCreator.create(&returnStatus);

			createMesh(i,newOutputData,returnStatus);
			outputHandle.set(newOutputData);
			outputHandle.setClean();
			std::string transform,shape,node,out;
			MString Mtransform,Mshape,Mnode,Mout;
			transform="instancing"+std::to_string(i);Mtransform=MString(transform.c_str());
			shape="instancingShape"+std::to_string(i);Mshape=MString(shape.c_str());
			node="instancingNode"+std::to_string(2015);Mnode=MString(node.c_str());
			out="outputMesh"+std::to_string(i);Mout=MString(out.c_str());
			MGlobal::executeCommand("createNode transform -n "+Mtransform);
			//createNode transform -n instancing1;
			MGlobal::executeCommand("createNode mesh -n "+Mshape+" -p "+Mtransform);
			//createNode mesh -n instancingShape1 -p instancing1;
			MGlobal::executeCommand("sets -add initialShadingGroup "+Mshape);
			//sets -add initialShadingGroup instancingShape1;
		}

		length=all_instance_set.size();
		for(int i=0;i<length;i++){
			Domain *tmp=all_instance_set[i][0];
			std::string s=std::to_string(tmp->index);
			std::string com="$RepSurfaces["+std::to_string(i)+"]="+s+";";
			MGlobal::executeCommand(MString(com.c_str()));
			MGlobal::displayInfo(s.c_str());
		}
		state=1;
		data.setClean(plug);
	}
	else if(state==1){
		float posX,posY,posZ;
		int iNum;
		posX=data.inputValue(instancingNode::x,&status).asFloat();
		posY=data.inputValue(instancingNode::y,&status).asFloat();
		posZ=data.inputValue(instancingNode::z,&status).asFloat();
		iNum=data.inputValue(instancingNode::instancingNum,&status).asInt();
		std::map<int, glm::vec3>::iterator  it = repr_anchor_points.find(iNum);
		//not create
		if(it == repr_anchor_points.end())
		{
			repr_anchor_points.insert(std::pair<int, glm::vec3>(iNum, glm::vec3(posX,posY,posZ)));
		}
		else
		{
			it->second = glm::vec3(posX,posY,posZ);
		}
	}
	else
		return MS::kUnknownParameter;
	return MS::kSuccess;
}

MObject instancingNode::createMesh(int n,MObject& outData, MStatus& stat){
	MFnMesh	myMesh;
	MPointArray points;
	MIntArray faceCounts;
	MIntArray faceConnects;

	m.initMesh(n);
	m.appendToMesh(points,faceCounts,faceConnects);
	
	/*for(int i=0;i<faceConnects.length();i++){
		std::string s;
		s=std::to_string(faceConnects[i]);
		MGlobal::displayInfo(MString(s.c_str()));
	}*/
	MObject newMesh = myMesh.create(points.length(), faceCounts.length(), points, faceCounts, faceConnects, outData, &stat);
	return newMesh;
}
