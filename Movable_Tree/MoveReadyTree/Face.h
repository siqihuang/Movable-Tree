#ifndef FACE_H
#define FACE_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Face 
{
public:
    inline Face()
	{
		rank = 0;
		parent = this;
		//UVminDis = FLT_MAX;
		//nearestNeighbor = NULL;
	}
	
	/*
	inline Face(std::vector<unsigned int>_vertex_indexs, std::vector<unsigned int>_uv_indexs, int component_idx)
	{
		rank = 0;
		parent = this;
		vertex_indexs = _vertex_indexs;
		uv_indexs = _uv_indexs; 
		UVminDis = FLT_MAX;
		nearestNeighbor = NULL;
		init_uv_coords(component_idx);
	}
	*/
	
	inline Face(const std::vector<unsigned int>&_vertex_indexs, const std::vector<glm::vec3>&_vertex_coords,
		std::vector<unsigned int>&_uv_indexs, std::vector<glm::vec2>&_uv_coords) 
	{
		rank = 0;
		parent = this;
		vertex_coords = _vertex_coords;
		vertex_indexs = _vertex_indexs;
		uv_indexs = _uv_indexs; 
		uv_coords = _uv_coords; 
		//UVminDis = FLT_MAX;
		//nearestNeighbor = NULL;
		/*
		if(true)
		{
			for(int i = 0; i < _vertex_indexs.size(); ++i)
			{
				printf("%d ", _vertex_indexs[i]);
			}
			printf("\n"); 
			for(int i = 0; i < _uv_indexs.size(); ++i)
			{
				printf("%d ", _uv_indexs[i]);
			}
			printf("\n"); 
			for(int i = 0; i < _uv_coords.size(); ++i)
			{
				printf("%f %f", _uv_coords[i].x, _uv_coords[i].y);
			}
			printf("\n"); 
		}
		*/
	}
	
	bool CheckShareVertex(Face *b)
	{
		for(int i = 0; i < vertex_indexs.size(); ++i)
		{
			for(int j = 0; j < b->vertex_indexs.size(); ++j)
			{
				if(vertex_indexs[i] == b->vertex_indexs[j]) //same vertex index
				{
					return true;
				}
			}
		}
		return false;
	}

	std::vector<unsigned int>vertex_indexs; //each vertex index
	std::vector<glm::vec3>vertex_coords; //each vertex coordinates 
	std::vector<unsigned int>uv_indexs; //each vertex's uv index
	std::vector<glm::vec2>uv_coords; //each vertex's uv coordinates 
	
	int index; //face index

	glm::vec3 min_pt;
	glm::vec3 max_pt;

	//4.1 union-find
	int rank;	
	Face* parent; 

	//4.2 texture mapping
	//float UVminDis; 
	//Face* nearestNeighbor; 

	float GetUVdistance(Face* b)
	{
		float dis = 0.f;
		if(uv_coords.size() != b->uv_coords.size())
		{
			//printf("jason: %d %d\n", uv_coords.size(), b->uv_coords.size());
			return FLT_MAX;
		}
		//todo: uv coordinates is in order?
		bool *vis = new bool[uv_coords.size()];
		memset(vis, false, sizeof(vis));
		for(int i = 0; i < uv_coords.size(); ++i)
		{
			for(int j = 0; i < uv_coords.size(); ++j)
			{
				//dis += glm::distance(uv_coords[i], b->uv_coords[i]);
				dis += fabs(uv_coords[i].x - b->uv_coords[i].x) +  fabs(uv_coords[i].y - b->uv_coords[i].y); 
				printf("%f ", dis);
			}
		}
		return dis;
	}
};
#endif