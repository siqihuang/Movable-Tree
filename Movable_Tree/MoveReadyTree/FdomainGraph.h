#ifndef FDOMAINGRAPH_H 
#define FDOMAINGRAPH_H 

#include "CommonData.h"
#include "kd_tree.h"
#include<ctime>

class FdomainGraph 
{
public: 
	void compute()
	{
		ktree = new KdTree();
		ktree->build_tree(domain_list);
	}
	//edge: domain_index1 & domain_index2
	std::vector<glm::vec2>graph;
	void print(); //for test

	KdTree* ktree;
};

#endif
