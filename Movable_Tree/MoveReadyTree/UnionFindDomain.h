#ifndef UnionFindDomain_H
#define UnionFindDomain_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


#include "CommonData.h"
#include <map>
#include <ctime>


class Domain;

//for instancing: combine similar domain
class UnionFindDomain
{
public:
	Domain* _Find(Domain* dom)
	{
		if(dom->instance_pa != dom)
		{
			dom->instance_pa = _Find(dom->instance_pa);
		}
		return dom->instance_pa;
	}

	void _Union(Domain* a, Domain* b) 
	{
		Domain* pa = _Find(a);
		Domain* pb = _Find(b);
		if(pa == pb)
		{
			return;
		}
		if(pa->rank >= pb->rank)
		{
			pb->instance_pa = pa;
		}
		else if (pa->rank < pb->rank)
		{
			pa->instance_pa = pb;
		}
		else if (pa->rank == pb->rank)
		{
			pa->instance_pa = pb;
			++pa->rank;
		}
	}
};

#endif