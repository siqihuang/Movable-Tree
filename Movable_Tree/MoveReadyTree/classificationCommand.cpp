#include "classificationCommand.h"

const char *selectedFlag = "-se", *selectedLongFlag = "-seld";
const char *pathFlag = "-p", *pathLongFlag = "-path";
const char *selectedNumFlag="-sn",*selectedNumLongFlag="-snld";

MSyntax classificationCommand::newSyntax()
{
    MSyntax syntax;

	syntax.addFlag( selectedFlag, selectedLongFlag, MSyntax::kString );
    syntax.addFlag( pathFlag, pathLongFlag, MSyntax::kString );
	syntax.addFlag( selectedNumFlag, selectedNumLongFlag, MSyntax::kUnsigned);
	return syntax;
}



MStatus classificationCommand::doIt( const MArgList& args ){
	MStatus stat = MS::kSuccess;

	MString filePath,selectedList;
	unsigned int selectedNum;
	MArgDatabase argData( syntax(), args );

	if( argData.isFlagSet( selectedNumFlag ) )
	argData.getFlagArgument( selectedNumFlag, 0, selectedNum );

    if( argData.isFlagSet( pathFlag ) )
	argData.getFlagArgument( pathFlag, 0, filePath );

	if( argData.isFlagSet( selectedFlag ) )
	argData.getFlagArgument( selectedFlag, 0, selectedList );

	MGlobal::displayInfo(selectedList);
	
	

	std::string s = std::to_string(selectedNum);
	MString ms(s.c_str());
	MGlobal::displayInfo(ms);
	

	return stat;
}