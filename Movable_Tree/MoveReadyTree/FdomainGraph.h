#ifndef FDOMAINGRAPH_H 
#define FDOMAINGRAPH_H 

#include "CommonData.h"
#include "kd_tree.h"
#include<ctime>

class FdomainGraph 
{
public: 
	void compute()
	{
		//build kd-tree
		//ktree = new KdTree();
		//ktree->build_tree(domain_list);
		init();

		clock_t start = clock();
			
		compute_graph();

		clock_t end = clock();
		double time = (double) (end-start) / CLOCKS_PER_SEC; 
		printf("\nAfter 4.3 Compute F-domain graph\n"); 
		printf("Time used: %lf secs\n\n", time); 

		print();
	}

	void init()
	{
		int total_faces = 0;
		fdomain_list.clear();
		for(int i = 0; i < domain_list.size(); ++i)
		{
			//init fdomain_list
			if(domain_list[i]->tag == "F")
			{
				fdomain_list.push_back(domain_list[i]);
				total_faces += domain_list[i]->face_list.size();
			}	
		}
		printf("fdomain_list size: %d\n", fdomain_list.size());	
		printf("fdomain_list face size: %d\n", total_faces); 
	}

	//brute_force version
	void compute_graph()
	{
		int n = fdomain_list.size();	
		int shape = fdomain_list[0]->face_list[0]->uv_coords.size();

		if(shape == 3) //triangle
		{
			for(int i = 0; i < n; ++i)
			{
				for(int j = i + 1; j < n; ++j)
				{
					if(IsCollidedTri(fdomain_list[i], fdomain_list[j]))
					{
						AddEdge(fdomain_list[i]->index, fdomain_list[j]->index);
						AddEdge(fdomain_list[j]->index, fdomain_list[i]->index);
					}
				}
				printf("Finished The %d fdomain.......\n", fdomain_list[i]->index);
			}
		}
		else if(shape == 4) //quad
		{
			for(int i = 0; i < n; ++i)
			{
				for(int j = i + 1; j < n; ++j)
				{
					if(IsCollidedQuad(fdomain_list[i], fdomain_list[j]))
					{
						AddEdge(fdomain_list[i]->index, fdomain_list[j]->index);
						AddEdge(fdomain_list[j]->index, fdomain_list[i]->index);
					}
				}
				printf("Finished The %d fdomain.......", i); 
			}
		}
	}

	void AddEdge(int a, int b)
	{	
		std::map<int , std::vector<int>>::iterator it;
		it = graph.find(a);
		if(it == graph.end())
		{
			std::vector<int>v;
			v.push_back(b);
			graph.insert(std::pair<int, std::vector<int>>(a, v));
		}
		else
		{
			it->second.push_back(b);
		}
	}

	bool IsCollidedTri(Domain* a, Domain* b)
	{
		for(int i = 0; i < a->face_list.size(); ++i)
		{
			Face* a_face = a->face_list[i];
			for(int j = 0; j < b->face_list.size(); ++j)
			{
				if(Face::TriangleIntersectionTest(a_face->vertex_coords, b->face_list[j]->vertex_coords))
				{
					return true;
				}
			}
		}
		return false;
	}
	
	bool IsCollidedQuad(Domain* a, Domain* b)
	{
		for(int i = 0; i < a->face_list.size(); ++i)
		{
			Face* a_face = a->face_list[i];	
			for(int j = 0; j < b->face_list.size(); ++j)
			{
				Face* b_face = b->face_list[j];	
				bool res = Face::TriangleIntersectionTest(a_face->C1, b_face->C1) ||
					Face::TriangleIntersectionTest(a_face->C1, b_face->C2) ||
					Face::TriangleIntersectionTest(a_face->C2, b_face->C1) ||
					Face::TriangleIntersectionTest(a_face->C2, b_face->C2); 
				if(res)					
				{
					return true;
				}
			}
		}
		return false;
	}

	void print()
	{
		printf("Graph structure:\n"); 

		std::map<int , std::vector<int>>::iterator it = graph.begin();
		for(; it != graph.end(); ++it)
		{
			int len = it->second.size();
			printf("Domain Index: %d neighbor size: %d\n", it->first, len); 
			for(int i = 0; i < len; ++i) 
			{
				printf("%d ", it->second[i]); 
			}
			printf("\n");	
		}

		it = graph.begin();
		for(; it != graph.end(); ++it)
		{
			if(it->second.size() == 0)
			{
				printf("Index %d Has no neighbor: \n", it->first);
			}
		}
	}

	//edge table
	std::map<int , std::vector<int>>graph;
	
	KdTree* ktree;	
};

#endif
