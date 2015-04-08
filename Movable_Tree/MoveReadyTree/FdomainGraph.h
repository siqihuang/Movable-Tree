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
		
		//4.3 Compute Fdomain graph
		compute_graph();

		//copy to global data
		//fgraph = graph;
		
		//4.4 Compute each fdomain components according to the fgraph.
		compute_connect_components();

		clock_t end = clock();
		double time = (double) (end-start) / CLOCKS_PER_SEC; 
		printf("\nAfter 4.3 Compute F-domain graph\n"); 
		printf("Time used: %lf secs\n\n", time); 

		print();
	}
	
	Domain* _Find(Domain* dom)
	{
		if(dom->fdomain_pa != dom)
		{
			dom->fdomain_pa = _Find(dom->fdomain_pa);
		}
		return dom->fdomain_pa;
	}

	void _Union(Domain* a, Domain* b) 
	{
		Domain* pa = _Find(a);
		Domain* pb = _Find(b);
		if(pa != pb)
		{
			pb->fdomain_pa = pa;
		}
	}

	void compute_connect_components()
	{
		std::map<Domain*, std::vector<Domain*>>::iterator it;
		for(it = fgraph.begin(); it != fgraph.end(); ++it)
		{
			for(int i = 0; i < it->second.size(); ++i)
			{
				_Union(it->first, it->second[i]);
			}
		}

		//rebuild
		fdomain_components.clear();
		//build fdomain components
		for(int i = 0; i < fdomain_list.size(); ++i) 
		{
			//find which component this domain belongs to 
			Domain* pa = _Find(fdomain_list[i]);
			it = fdomain_components.find(pa);	

			//can not find, build one
			if(it == fdomain_components.end())
			{
				std::vector<Domain*>domain_set;
				domain_set.push_back(fdomain_list[i]);
				fdomain_components.insert(std::pair<Domain*, std::vector<Domain*>>(pa, domain_set));
			}
			else
			{
				it->second.push_back(fdomain_list[i]);
			}
		}
		
		printf("\nTotal components:%d\n=======Each fdomain components:\n", fdomain_components.size());
		printf("[	PRINT_FDOMAIN_COMPONENTS]");
		for(it = fdomain_components.begin(); it != fdomain_components.end(); ++it)
		{
			printf("%d\n", it->first->index);
			for(int j = 0; j < it->second.size(); ++j)
			{
				printf("%d: ", it->second[j]->index);
			}
			printf("\n");
		}
		printf("\n");
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
		printf("\n\n==============================\n");
		printf("fdomain_list size: %d\n", fdomain_list.size());	
		printf("fdomain_list face size: %d\n", total_faces); 
	}

	//Compute Fdomain-graph.
	//brute_force version
	void compute_graph()
	{
		int n = fdomain_list.size();	
		fgraph.clear();
		std::map<Domain*, std::vector<Domain*>>::iterator it;
		//init fgraph
		for(int i = 0; i < n; ++i)
		{
			std::vector<Domain*>neighbors;
			fgraph.insert(std::pair<Domain*, std::vector<Domain*>>(fdomain_list[i], neighbors)); 
		}

		int shape = fdomain_list[0]->face_list[0]->uv_coords.size();
		if(shape == 3) //triangle
		{
			for(int i = 0; i < n; ++i)
			{
				for(int j = i + 1; j < n; ++j)
				{
					if(IsCollidedTri(fdomain_list[i], fdomain_list[j]))
					{
						AddEdge(fdomain_list[i], fdomain_list[j]);
						AddEdge(fdomain_list[j], fdomain_list[i]);
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
						AddEdge(fdomain_list[i], fdomain_list[j]);
						AddEdge(fdomain_list[j], fdomain_list[i]);
					}
				}
				printf("Finished The %d fdomain.......\n", i); 
			}
		}
	}

	void AddEdge(Domain* a, Domain*b)
	{	
		std::map<Domain*, std::vector<Domain*>>::iterator it;
		it = fgraph.find(a);
		//not create
		if(it == fgraph.end())
		{
			std::vector<Domain*>neighbors;
			neighbors.push_back(b);
			fgraph.insert(std::pair<Domain*, std::vector<Domain*>>(a, neighbors));
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
	
	//arguments: domain_1 index, domain_2 index
	void connect_edge(int index1 ,int index2)
	{
		Domain* a; 
		Domain* b;
		int cnt = 0;
		//find these two domains' pointers
		for(int i = 0; i < fdomain_list.size(); ++i)
		{
			if(fdomain_list[i]->index == index1)
			{
				a = fdomain_list[i];
				++cnt;
				if(cnt == 2) break;
			}
			else if(fdomain_list[i]->index == index2)
			{
				b = fdomain_list[i];
				++cnt;
				if(cnt == 2) break;
			}
		}

		//update fgraph
		std::map<Domain*, std::vector<Domain*>>::iterator it; 
		//a
		it = fgraph.find(a);
		if(it != fgraph.end())
		{
			//avoid copies
			if(std::find(it->second.begin(), it->second.end(), b) == it->second.end())
			{
				it->second.push_back(b);
			}
		}
		else
		{
			printf("ERROR!!!! Can not find Domain:%d in fgraph\n", a->index);
		}
		//b
		it = fgraph.find(b);
		if(it != fgraph.end())
		{
			//avoid copies
			if(std::find(it->second.begin(), it->second.end(), a) == it->second.end())
			{
				it->second.push_back(a);
			}
		}
		else
		{
			printf("ERROR!!!! Can not find Domain:%d in fgraph\n", b->index);
		}

		printf("F-Graph updated!\n[PRINT_FGRAPH]\n");
		for(it = fgraph.begin(); it != fgraph.end(); ++it)
		{
			printf("%d:\n", it->first->index);			
			for(int j = 0; j < it->second.size(); ++j)
			{
				printf("%d: ", it->second[j]->index);
			}
			printf("\n"); 
		}
		
		//update fdomain_components 
		compute_connect_components();
	}
	
	//4.5 
	int GetInitialRootDomain()
	{
		int cnt = 0;
		int idx = 0;
		for(int i = 0; i < fdomain_list.size(); ++i) 
		{
			if(cnt < fdomain_list[i]->face_list.size())
			{
				cnt = fdomain_list[i]->face_list.size();
				idx = fdomain_list[i]->index;
			}
		}
		return idx;
	}

	void print()
	{
		printf("Graph structure:\n"); 

		std::map<Domain*, std::vector<Domain*>>::iterator it = fgraph.begin();
		for(; it != fgraph.end(); ++it)
		{
			int len = it->second.size();
			printf("Domain Index: %d neighbor size: %d\n", it->first->index, len); 
			for(int i = 0; i < len; ++i) 
			{
				printf("%d ", it->second[i]->index); 
			}
			printf("\n");	
		}

		it = fgraph.begin();
		for(; it != fgraph.end(); ++it)
		{
			if(it->second.size() == 0)
			{
				printf("Index %d Has no neighbor: \n", it->first->index);
			}
		}


	}

	//edge table
	//std::map<Domain*, std::vector<Domain*>>graph;
	
	KdTree* ktree;	
};

#endif
