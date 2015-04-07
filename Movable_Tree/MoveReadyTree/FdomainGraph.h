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
			
			//init graph data structure
			std::vector<int>v;
			graph.push_back(v);
		}
		printf("fdomain_list size: %d\n", fdomain_list.size());	
		printf("fdomain_list face size: %d\n", total_faces); 
	}

	//brute_force version
	void compute_graph()
	{
		for(int i = 0; i < domain_list.size(); ++i)
		{
			for(int j = i + 1; j < domain_list.size(); ++j)
			{
				if(IsCollided(domain_list[i], domain_list[j]))
				{
					AddEdge(domain_list[i]->index, domain_list[j]->index);
					AddEdge(domain_list[j]->index, domain_list[i]->index);
				}
			}
		}
	}

	void AddEdge(int a, int b)
	{
		graph[a].push_back(b);
	}

	bool IsCollided(Domain* a, Domain* b)
	{
		for(int i = 0; i < a->face_list.size(); ++i)
		{
			Face* a_face = a->face_list[i];
			for(int j = 0; j < b->face_list.size(); ++j)
			{
				if(a_face->IsCollided(b->face_list[j]))
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
		for(int i = 0; i <= graph.size(); ++i)
		{
			printf("Domain Index: %d\n", i);	
			for(int j = 0; j <= graph[i].size(); ++j)
			{
				printf("%d ", j);	
			}
			printf("\n");	
		}
	}

	//edge table
	std::vector<std::vector<int>>graph;
	KdTree* ktree;
	
	
};

#endif
