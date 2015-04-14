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
		instancing_repr = this; 
		mst_repr = this;
		components_repr = this;
		rank = 0;
		is_root = false;
	}
    ~Domain() {}
	
	std::vector<Face*>face_list;
	std::string tag;
	std::string texture_file; 
	int index;
	
	//anchor point vertex coordinate
	glm::vec3 anchor_point;

	//anchor point uv coordinate
	glm::vec2 anchor_uv;

	//instancing
	std::vector<glm::vec2>uv_coords_list;
	//instancing: union-find, set representative
	Domain *instancing_repr;
	int rank;
	
	//for fgraph connected components: union-find, set representative
	Domain *components_repr;

	//minimum spanning tree: union-find, set representative
	Domain *mst_repr;

	//Fdomain graph, root mark
	bool is_root;
	
	//Fdomain graph_parent, root mark
	Domain* graph_parent;

	void AddFace(Face* f)
	{
		face_list.push_back(f);
		for(int i = 0; i < f->uv_coords.size(); ++i)
		{
			uv_coords_list.push_back(f->uv_coords[i]);
		}
	}

	void SetRoot()
	{
		is_root = true;
	}

	bool IsRoot()
	{
		return is_root;
	}
	
	//for instance copies: (which means except representative instance)
	//compute anchor point: according to its' representative instance's anchor point's uv coord
	void ComputeAnchorPoint(const glm::vec2& uv_coord)
	{
		const float EPS = 0.005f;
		int tmp_idx = -1;
		for(int i = 0; i < face_list.size(); ++i)
		{
			for(int j = 0; j < face_list[i]->uv_coords.size(); ++j)
			{
				glm::vec2 a = face_list[i]->uv_coords[j];
				if(fabs(uv_coord.x - a.x) < EPS && fabs(uv_coord.y - a.y) < EPS)
				{
					printf("[ComputeAnchorPoint] Repr's Index:%d=========uv: %f %f\n", 
						instancing_repr->index, uv_coord.x, uv_coord.y);
					tmp_idx = j;
					break;
				}
			}
			if(tmp_idx != -1)
			{
				anchor_point = face_list[i]->vertex_coords[tmp_idx]; 
				return;
			}
		}
		printf("[ERROR] ComputeAnchorPoint can not find anchor point:%d uv: %f %f\n", 
			index, uv_coord.x, uv_coord.y);
	}
	
	//for representative instance: 
	//Find uv coord from accord point vertex coord
	void ComputeAnchorPointUV()
	{
		//int uv_coord_idx = -1;
		int tmp_idx = -1;
		for(int i = 0; i < face_list.size(); ++i)
		{
			//for all vertex coords
			for(int j = 0; j < face_list[i]->vertex_coords.size(); ++j)
			{
				//find coord
				if(anchor_point == face_list[i]->vertex_coords[j])
				{
					tmp_idx = j;	
					break;
				}
			}
			if(tmp_idx != -1)
			{
				//find vertex index
				//uv_coord_idx = face_list[i]->uv_indexs[tmp_idx];
				//uv_idx = vertex idx, just use it
				anchor_uv = face_list[i]->uv_coords[tmp_idx]; 
				printf("[ComputeAnchorPointUV] : Index: %d ====x:%f y:%f\n", index, anchor_uv.x ,anchor_uv.y);
				return;
			}
		}	
	}
};

#endif