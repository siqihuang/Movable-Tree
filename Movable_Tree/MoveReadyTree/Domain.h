#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <vector>
#include <algorithm>
#include "Face.h"

class Domain
{
public:
	Domain()
	{
		instance_pa = this; 
		rank = 0;
	}
    ~Domain() {}
	
	vector<Face*>face_list;
	string tag;

	//for instancing
	vector<glm::vec2>uv_coords_list;
	Domain *instance_pa;
	int rank;

	void AddFace(Face* face)
	{
		face_list.push_back(face);
		for(int i = 0; i < face->uv_coords.size(); ++i)
		{
			uv_coords_list.push_back(face->uv_coords[i]);
		}
	}
	void SortUVCoords()
	{
		//sort(uv_coords_list.begin(), uv_coords_list.end(), UVCmp);
		//sort(uv_coords_list.begin(), uv_coords_list.end(), uvcmp);
	}
};

#endif