#ifndef COMMON_DATA_H 
#define COMMON_DATA_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Domain.h"
#include <vector>


extern std::vector<Domain*>domain_list;
extern std::vector<vector<Domain*>>all_instance_set;

/*
extern std::vector<glm::vec3> *vertex;
extern std::vector<glm::vec2> *UV_COORDS; 
extern std::vector<string> *faceInfo;
extern std::vector<vector<unsigned int>> *index;
extern std::vector<vector<unsigned int>> *UV_INDEXS;
*/


static void print_domainlist()
{
	//printf("common data: %d\n", domain_list.size());
}
#endif
