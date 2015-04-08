#include "CommonData.h"

float EPS = 1e-3f; 

std::vector<Domain*>domain_list;
std::vector<std::vector<Domain*>>all_instance_set;

std::vector<Domain*>fdomain_list;
std::map<Domain*, std::vector<Domain*>>fgraph;

std::map<Domain*, std::vector<Domain*>>fdomain_components;



