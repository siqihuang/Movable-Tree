#include "CommonData.h"

float EPS = 1e-3f; 

std::vector<Domain*>domain_list;
std::vector<std::vector<Domain*>>all_instance_set;

std::vector<Domain*>fdomain_list;
std::map<Domain*, std::vector<Domain*>>fgraph;
Domain* root_domain; 

std::map<Domain*, std::vector<Domain*>>fdomain_components;
std::map<Domain*, std::vector<Domain*>>mini_tree; //minimum_tree
std::map<Domain*, std::vector<Domain*>>redundant_edges;

std::map<int, glm::vec3>repr_anchor_points; 

std::vector<std::vector<int>>constraintIndex;
std::vector<std::vector<int>>parentConstraintIndex;
std::vector<int> domainParentIndex;
std::vector<glm::dvec3> parentLastPosNew;
std::vector<glm::dvec3> parentLastPosOld;
int currentConstriant=0;
