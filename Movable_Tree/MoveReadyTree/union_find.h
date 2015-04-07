#ifndef UnionFind_H
#define UnionFind_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


#include "CommonData.h"
#include "objLoader.h"
#include "mtlLoader.h"
#include<algorithm>
#include <map>
#include <ctime>


class UnionFind
{
public:
	int domain_idx;
	UnionFind()
	{
		domain_idx = 0;
	}

	Face* _Find(Face* face)
	{
		if(face->parent != face)
		{
			face->parent = _Find(face->parent);
		}
		return face->parent;
	}

	void _Union(Face* a, Face* b) //union each faces
	{
		Face* pa = _Find(a);
		Face* pb = _Find(b);
		if(pa == pb)
		{
			return;
		}
		if(pa->rank >= pb->rank)
		{
			pb->parent = pa;
			++pa->rank;
		}
		else 
		{
			pa->parent = pb;
			++pb->rank;
		}
	}
	
	void Union(const objLoader& objloader, const mtlLoader& mtlloader, const string* tags, int componentNum) 
	{
		clock_t start = clock();
		map<Face*, Domain*>domain_map;
		map<Face*, Domain*>::iterator it;
		for(int i = 0; i < componentNum; i++)
		{
			printf("%d component\n", i);
			vector<Face*>face_list;
			printf("%d index[i] size\n", objloader.index[i].size());
			string texture_file = mtlloader.getTextureFile()[i];
			//Each faces
			for(int j = 0; j < objloader.index[i].size(); j++) 
			{
				if(objloader.index[i][j].size() <= 0) continue;
				//vertices
				vector<glm::vec3>_vertex_coords;
				for(int k = 0; k < objloader.index[i][j].size(); ++k)
				{
					int idx = objloader.index[i][j][k] - 1;
					_vertex_coords.push_back(objloader.vertex[i][idx]);
				}

				//UV
				vector<glm::vec2>_uv_coords;
				for(int k = 0; k < objloader.UV_INDEXS[i][j].size(); ++k)
				{
					int uv_idx = objloader.UV_INDEXS[i][j][k] - 1;
					_uv_coords.push_back(objloader.UV_COORDS[i][uv_idx]);
					
					/*
					//printf("uv_coords: %d %f %f\n", uv_idx, 
						//objloader.UV_COORDS[i][uv_idx].x, objloader.UV_COORDS[i][uv_idx].y);
					*/
				}
				Face *face = new Face(objloader.index[i][j], _vertex_coords, objloader.UV_INDEXS[i][j], _uv_coords);
				face_list.push_back(face);
			}

			printf("face_list size %d\n", face_list.size()); 
			for(int j = 0; j < face_list.size(); j++) //each faces
			{
				for(int k = j + 1; k < face_list.size(); k++) 
				{
					if(_Find(face_list[j]) == _Find(face_list[k])) continue;
					bool res = face_list[j]->CheckShareVertex(face_list[k]);
					if(res)
					{
						_Union(face_list[j], face_list[k]);
					}
				}
				if(j % 500 == 0)
				{
					printf("Union-find processing %d face... \n", j);
				}
			}
			
			for(int j = 0; j < face_list.size(); j++) //each faces
			{
				Face* face = face_list[j];
				it  = domain_map.find(_Find(face));
				//new domain not created 
				if(it == domain_map.end())
				{
					//new a domain
					Domain* d = new Domain();
					d->texture_file = texture_file;
					d->tag = tags[i];
					d->AddFace(face);
					domain_map.insert(std::pair<Face*, Domain*>(face->parent, d)); 
				}
				else
				{
					(*it).second->AddFace(face);
				}
			}
		}
		for(it = domain_map.begin(); it != domain_map.end(); ++it)
		{
			Domain* d = it->second;
			d->index = domain_list.size(); 
			domain_list.push_back(it->second);
		}
		
		//for test
		/*
		int total_faces = 0;
		for(int i = 0; i < domain_list.size(); ++i) 
		{
			total_faces += domain_list[i]->face_list.size();
			if(domain_list[i]->face_list.size() <= 1) 
			{
				printf("===================jason %d %d \n", domain_list[i]->face_list.size(), domain_list[i]->index); 
				Face* face = domain_list[i]->face_list[0];
				for(int j = 0; j < face->vertex_indexs.size(); ++j) 
				{
					printf("%f ", face->vertex_indexs[j]);
				}
			}
		}
		printf("total face==============: %d\n", total_faces);
		*/

		clock_t end = clock();
		double time = (double) (end-start) / CLOCKS_PER_SEC; 
		
		
		printf("\nAfter 4.1 Union_Find and combine share-vertex faces into each domains.\n");
		printf("Domain list length: %d \n", domain_list.size());
		printf("Time use: %lf secs\n\n", time); 

		//return domain_list;
	}
};

#endif