#ifndef COMMON_DATA_H 
#define COMMON_DATA_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Domain.h"
#include <vector>
#include <map>

class Domain;

//==================4.1 union-find: all domain======================
extern std::vector<Domain*>domain_list;
//=========================================================

//=================4.2 instancing: all instance set================= 

//instance set: all domains look the same. (according to UV)
extern std::vector<std::vector<Domain*>>all_instance_set;

//=====================================================

//===============4.3 compute F-domain Graph================= 

//All domains which were taged "F"
extern std::vector<Domain*>fdomain_list;
//According to the collision detection, build the fdomain-graph.
extern std::map<Domain*, std::vector<Domain*>>fgraph;

//=====================================================

//=============4.4 connect F-domain Graph====================== 

//To find how many connected-components in fdomain graph, for later process(4.4 connect).
//A map:
//First: the representive domain of a fdomain components, each element points to this domain.
//Second: all the other domains in this components.
extern std::map<Domain*, std::vector<Domain*>>fdomain_components;

//=====================================================

extern float EPS; 

static void print_domainlist()
{
	//printf("common data: %d\n", domain_list.size());
}
#endif
