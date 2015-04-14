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
		if(dom->instancing_repr != dom)
		{
			dom->instancing_repr = _Find(dom->instancing_repr);
		}
		return dom->instancing_repr;
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
			pb->instancing_repr = pa;
			++pa->rank;
		}
		else 
		{
			pa->instancing_repr = pb;
			++pb->rank;
		}
	}
};

#endif