#ifndef FACE_H
#define FACE_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


class Face 
{
public:
	int index; 
	std::vector<unsigned int>vertex_indexs; //each vertex index
	std::vector<glm::vec3>vertex_coords; //each vertex coordinates 
	std::vector<unsigned int>uv_indexs; //each vertex's uv index
	std::vector<glm::vec2>uv_coords; //each vertex's uv coordinates 

	glm::vec3 min_pt;
	glm::vec3 max_pt;

	//4.1 union-find
	int rank;	
	Face* parent; 

    Face()
	{
		rank = 0;
		parent = this;
	}

	Face(const std::vector<unsigned int>&_vertex_indexs, const std::vector<glm::vec3>&_vertex_coords,
		std::vector<unsigned int>&_uv_indexs, std::vector<glm::vec2>&_uv_coords) 
	{
		rank = 0;
		parent = this;
		vertex_coords = _vertex_coords;
		vertex_indexs = _vertex_indexs;
		uv_indexs = _uv_indexs; 
		uv_coords = _uv_coords; 
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

	bool IsCollided(const Face* b)
	{
		int vn = vertex_coords.size(); 
		if(vn != b->vertex_coords.size())
		{
			printf("ERROR! Face a vertex num is different from face b");
			return false;
		}
		//quad
		if(vn == 4)
		{
			return QuadIntersectionTest(b);
		}
		//triangle
		else if(vn == 3)
		{
			return TriangleIntersectionTest(b);
		}
		return true;
	}

	bool QuadIntersectionTest(const Face *b)
	{
		return false;
	}

	bool TriangleIntersectionTest(const Face *b)
	{
		return false;
	}

	float GetUVdistance(const Face* b)
	{
		int n = uv_coords.size();
		if(n != b->uv_coords.size())
		{
			//printf("jason: %d %d\n", uv_coords.size(), b->uv_coords.size());
			return FLT_MAX;
		}
		bool *vis = new bool[n];
		for(int i = 0; i < n; ++i)
		{
			vis[i] = false;
		}
		for(int i = 0; i < n; ++i)
		{
			bool found = false;
			for(int j = 0;  j < n; ++j)
			{
				if(vis[j])continue;
				float tmp_dis = fabs(uv_coords[i].x - b->uv_coords[j].x) + fabs(uv_coords[i].y - b->uv_coords[j].y); 
				//greedy
				if(tmp_dis < 0.01f) 
				{
					found = true;
					vis[j] = true;
					break;
				}
			}
			if(!found)
			{
				delete[ ] vis;
				return FLT_MAX;
			}
		}
		delete[ ] vis;
		return 0;
	}
};
#endif