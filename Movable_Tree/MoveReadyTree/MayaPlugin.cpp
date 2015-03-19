#include "maya\MGlobal.h"
#include "maya\MFnPlugin.h"
#include "classificationNode.h"
#include "instancingNode.h"
#include "classificationCommand.h"

MStatus initializePlugin(MObject obj){
	MStatus status;

	MFnPlugin fnPlugin(obj,"Siqi Huang","1.0","CIS660");
	status=fnPlugin.registerNode("classificationNode",classificationNode::id,classificationNode::creator,classificationNode::initialize);

	status = fnPlugin.registerCommand( "classificationCommand",classificationCommand::creator,classificationCommand::newSyntax );

	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/meshSelection.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/classification.mel\"");
	MGlobal::executeCommand("source \"" + fnPlugin.loadPath() + "/instancing.mel\"");
	status = fnPlugin.registerUI("createMoveReadyTreeUI", "deleteMoveReadyTreeUI");

	return status;
}

MStatus uninitializePlugin(MObject obj){
	MStatus status;
	MFnPlugin fnPlugin(obj);
	status=fnPlugin.deregisterNode(classificationNode::id);
	status=fnPlugin.deregisterNode(instancingNode::id);

	status=fnPlugin.deregisterCommand("classificationCommand");

	return status;
}
