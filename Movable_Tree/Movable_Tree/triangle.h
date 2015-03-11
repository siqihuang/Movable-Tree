#ifndef TRIANGLE_H
#define TRIANGLE_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct Triangle 
{
    inline Triangle()
	{
		rank = 0;
		parent = this;
	}
    inline Triangle(const glm::vec3 &a_in, const glm::vec3 &b_in, const glm::vec3 &c_in, int id)
	{
		p[0] = a_in;
		p[1] = b_in;
		p[2] = c_in;
		index = id;
		rank = 0;
		parent = this;
	}
	glm::vec3 p[3];
	int index;

	//for union-find
	int rank;	
	Triangle* parent; 
};

#endif
