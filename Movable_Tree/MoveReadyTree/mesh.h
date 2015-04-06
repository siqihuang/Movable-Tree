#ifndef cylinder_H_
#define cylinder_H_

#include <maya/MPoint.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <vector>

#include "Domain.h"
#include "Face.h"

class mesh
{
public:
	mesh();
	mesh(std::vector<Domain *>domain);
    ~mesh();

    void getMesh(MPointArray& points,MIntArray& faceCounts,MIntArray& faceConnects);
    
    void appendToMesh(MPointArray& points,MIntArray& faceCounts,MIntArray& faceConnects);

	void setDomain(std::vector<Domain *>domain);

	void initMesh(int num);

protected:
    std::vector<Domain *> domain;

    // Creates a unit cylinder from (0,0,0) to (1,0,0) with radius r
	bool checkPoints(unsigned int index,std::vector<unsigned int> indices);
	int findIndex(unsigned int index,std::vector<unsigned int> indices);
    MPointArray gPoints;
    MVectorArray gNormals;
    MIntArray gFaceCounts;
    MIntArray gFaceConnects;
};

#endif