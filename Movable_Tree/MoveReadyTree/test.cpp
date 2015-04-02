#ifndef TEST_H 
#define TEST_H 

#include "InstancingModule.h"
#include "FdomainGraph.h"
#include "union_find.h"
#include "mtlLoader.h"
#include <string>


class Test
{
public:
	UnionFind uf;
	InstancingModule im;
	FdomainGraph fdg;

	string *tags;
	objLoader Objloader;
	mtlLoader Mtlloader;

	void unit_test()
	{
		int componentNum = 3;
		
		string meshList;
		string ObjPath;
		string MtlPath;
		string tag;

		meshList = "tree:AS12_Leaf2$$tree:AS12_Leaf1$$tree:AS12_Branch1$$";
		ObjPath = "C:/Users/Wingszero/Desktop/660 Adv CG/Movable-Tree/Movable_Tree/tree/tree.obj";
		MtlPath = "C:/Users/Wingszero/Desktop/660 Adv CG/Movable-Tree/Movable_Tree/tree/tree.mtl";
		tag = "F$$F$$F$$";
		
		Objloader.setComponentNum(componentNum);
		Objloader.setFileName(ObjPath);
		Objloader.setMeshList(meshList);
		Objloader.extractList(meshList,componentNum);
		Objloader.readObj();

		Mtlloader.setComponentNum(componentNum);
		Mtlloader.setFileName(MtlPath);
		Mtlloader.getTempFile(Objloader.getTempName());
		Mtlloader.readFile();
		
		MGlobal::executeCommand("createInstancingGUI()");
		MGlobal::executeCommand("$reprensentativeInstanceNum=10");
		
		//4.1 union find
		uf.Union(Objloader, componentNum);
		//4.2 instancing
		im.Instancing();
		//4.3 compute F-domain graph
		fdg.compute();
	}
};

#endif