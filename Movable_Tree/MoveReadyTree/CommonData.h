#ifndef COMMON_DATA_H 
#define COMMON_DATA_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Domain.h"
#include <vector>

class Domain;

//after 4.1 union-find: all domain
extern std::vector<Domain*>domain_list;

//after 4.2 instancing: all instance set 
extern std::vector<std::vector<Domain*>>all_instance_set;

extern float EPS; 
static void print_domainlist()
{
	//printf("common data: %d\n", domain_list.size());
}
#endif
