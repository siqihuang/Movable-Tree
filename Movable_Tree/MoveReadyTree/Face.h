#ifndef FACE_H
#define FACE_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "CommonData.h"
#include <vector>

using namespace std;


class Face 
{
public:
    inline Face()
	{
		rank = 0;
		parent = this;
		UVminDis = FLT_MAX;
		nearestNeighbor = NULL;
	}
	
	/*
	inline Face(vector<unsigned int>_vertices, vector<unsigned int>_uv_indexs, int component_idx)
	{
		rank = 0;
		parent = this;
		vertices = _vertices;
		uv_indexs = _uv_indexs; 
		UVminDis = FLT_MAX;
		nearestNeighbor = NULL;
		init_uv_coords(component_idx);
	}
	*/
	
	inline Face(vector<unsigned int>_vertices, vector<unsigned int>_uv_indexs, vector<glm::vec2>_uv_coords) 
	{
		rank = 0;
		parent = this;
		vertices = _vertices;
		uv_indexs = _uv_indexs; 
		uv_coords = _uv_coords; 
		UVminDis = FLT_MAX;
		nearestNeighbor = NULL;
		/*
		if(true)
		{
			for(int i = 0; i < _vertices.size(); ++i)
			{
				printf("%d ", _vertices[i]);
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


	void init_uv_coords(int component_idx)
	{
		/*
		for(int i = 0; i < uv_indexs.size(); ++i)
		{
			glm::vec2 tmp = UV_COORDS[component_idx][uv_indexs[i] - 1]; 
			uv_coords.push_back(tmp);
			printf("component:%d uv_idx:%d %f %f\n", component_idx, uv_indexs[i] - 1, 
				tmp.x, tmp.y);
		}
		*/
	}
	
	bool CheckShareVertex(Face *b)
	{
		for(int i = 0; i < vertices.size(); ++i)
		{
			for(int j = 0; j < b->vertices.size(); ++j)
			{
				if(vertices[i] == b->vertices[j]) //same vertex index
				{
					return true;
				}
			}
		}
		return false;
	}

	vector<unsigned int>vertices; //each vertex index
	vector<unsigned int>uv_indexs; //each vertex's uv index
	vector<glm::vec2>uv_coords; //each vertex's uv coordinates 
	
	int index; //face index

	//4.1 union-find
	int rank;	
	Face* parent; 

	//4.2 texture mapping
	float UVminDis; 
	Face* nearestNeighbor; 

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