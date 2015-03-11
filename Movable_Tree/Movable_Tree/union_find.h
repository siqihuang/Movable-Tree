#ifndef UnionFind_H
#define UnionFind_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Triangle.h"

Triangle* Find(Triangle* tri)
{
	if(tri->parent != tri)
	{
		tri->parent = Find(tri->parent);
	}
	return tri->parent;
}

void Union(Triangle* a, Triangle* b)
{
	Triangle* pa = Find(a);
	Triangle* pb = Find(b);
	if(pa == pb)
	{
		return;
	}
	if(pa->rank >= pb->rank)
	{
		pb->parent = pa;
	}
	else if (pa->rank < pb->rank)
	{
		pa->parent = pb;
	}
	else if (pa->rank == pb->rank)
	{
		pa->parent = pb;
		++pa->rank;
	}
}



#endif
