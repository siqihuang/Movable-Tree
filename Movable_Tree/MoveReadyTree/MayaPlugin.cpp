#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>
#include "classificationNode.h"
#include "instancingNode.h"
#include "connectingNode.h"
#include "deleteEdgeNode.h"
#include "classificationCommand.h"

MStatus initializePlugin(MObject obj){
	MStatus status;

	MFnPlugin fnPlugin(obj,"Siqi Huang","1.0","CIS660");

	//MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/meshSelection.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/classification.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/instancing.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/connecting.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/rootSelection.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/deleteEdge.mel\"");
	status = fnPlugin.registerUI("createMoveReadyTreeUI", "deleteMoveReadyTreeUI");

	status=fnPlugin.registerNode("classificationNode",classificationNode::id,classificationNode::creator,classificationNode::initialize);
	status=fnPlugin.registerNode("instancingNode",instancingNode::id,instancingNode::creator,instancingNode::initialize);
	status=fnPlugin.registerNode("connectingNode",connectingNode::id,connectingNode::creator,connectingNode::initialize);
	status=fnPlugin.registerNode("deleteEdgeNode",deleteEdgeNode::id,deleteEdgeNode::creator,deleteEdgeNode::initialize);
	status=fnPlugin.registerCommand( "classificationCommand",classificationCommand::creator,classificationCommand::newSyntax );

	return status;
}

MStatus uninitializePlugin(MObject obj){
	MStatus status;
	MFnPlugin fnPlugin(obj);
	status=fnPlugin.deregisterNode(classificationNode::id);
	status=fnPlugin.deregisterNode(instancingNode::id);
	status=fnPlugin.deregisterNode(connectingNode::id);
	status=fnPlugin.deregisterNode(deleteEdgeNode::id);

	status=fnPlugin.deregisterCommand("classificationCommand");

	return status;
}
