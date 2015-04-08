#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <vector>
#include <algorithm>

#include "Face.h"
class Face;

class Domain
{
public:
	Domain()
	{
		instance_pa = this; 
		mst_pa = this;
		fdomain_pa = this;
		rank = 0;
	}
    ~Domain() {}
	
	std::vector<Face*>face_list;
	std::string tag;
	std::string texture_file; 
	
	//domain index
	int index;

	//for instancing
	std::vector<glm::vec2>uv_coords_list;
	Domain *instance_pa;
	
	//for fgraph connected components
	Domain *fdomain_pa;

	//minimum spanning tree
	Domain *mst_pa;

	int rank;

	void AddFace(Face* f)
	{
		face_list.push_back(f);
		for(int i = 0; i < f->uv_coords.size(); ++i)
		{
			uv_coords_list.push_back(f->uv_coords[i]);
		}
	}
	void SortUVCoords()
	{
		//sort(uv_coords_list.begin(), uv_coords_list.end(), UVCmp);
		//sort(uv_coords_list.begin(), uv_coords_list.end(), uvcmp);
	}
};

#endif