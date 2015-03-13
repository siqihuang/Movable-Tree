#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include "maya/MSelectionList.h"
#include <string>

class classificationCommand : public MPxCommand 
{
public:
	virtual MStatus	doIt ( const MArgList& );
	static void *creator() { return new classificationCommand; }
	static MSyntax newSyntax();
};
