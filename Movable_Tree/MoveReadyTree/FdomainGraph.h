#ifndef FDOMAINGRAPH_H 
#define FDOMAINGRAPH_H 

#include "CommonData.h"
#include "kd_tree.h"
#include<ctime>
#include<queue>
#include<set>

#define FDG_ITER std::map<Domain*, std::vector<Domain*>>::iterator 

class FdomainGraph 
{
public: 
	//tmp redundant edge, may be add to MST after user remove loop.
	Domain* rd_a;
	Domain* rd_b;

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

		print_graph();
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
		FDG_ITER it;
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

			//can not find this set, build it 
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
			printf("%d: \n", it->first->index);
			for(int j = 0; j < it->second.size(); ++j)
			{
				printf("%d ", it->second[j]->index);
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
		FDG_ITER it; 
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
						AddEdge(fdomain_list[i], fdomain_list[j], fgraph);
						AddEdge(fdomain_list[j], fdomain_list[i], fgraph);
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
						AddEdge(fdomain_list[i], fdomain_list[j], fgraph);
						AddEdge(fdomain_list[j], fdomain_list[i], fgraph);
					}
				}
				printf("Finished The %d fdomain.......\n", i); 
			}
		}
	}

	void AddEdge(Domain* a, Domain*b, std::map<Domain*, std::vector<Domain*>>&graph)
	{	
		FDG_ITER it = graph.find(a);
		//not create
		if(it == graph.end())
		{
			std::vector<Domain*>neighbors;
			neighbors.push_back(b);
			graph.insert(std::pair<Domain*, std::vector<Domain*>>(a, neighbors));
		}
		else
		{
			//avoid copies	
			if(std::find(it->second.begin(), it->second.end(), b) == it->second.end())
			{
				it->second.push_back(b);
			}
		}
	}

	void DelEdge(Domain* a, Domain*b, std::map<Domain*, std::vector<Domain*>>&graph)
	{	
		FDG_ITER it = graph.find(a);
		if(it != graph.end())
		{
			std::remove(it->second.begin(), it->second.end(), b);	
		}
	}	

	//for user remove loop in MAYA
	void DelEdgeExternal(int index1 ,int index2)
	{
		Domain* a = GetDomainByIndex(index1);	
		Domain* b = GetDomainByIndex(index2);	
		if(a && b)
		{
			DelEdge(a, b, mini_tree);
			DelEdge(b, a, mini_tree);
			
			AddEdge(a, b, redundant_edges);
			AddEdge(b, a, redundant_edges);

			DelEdge(rd_a, rd_b, redundant_edges);
			DelEdge(rd_b, rd_a, redundant_edges);
			rd_a = rd_b = NULL;
		}
		else
		{
			if(!a)
			{
				printf("[ERROR] DelEdgeExternal: Domain is NULL: %d", index1);
			}
			else
			{
				printf("[ERROR] DelEdgeExternal: Domain is NULL: %d", index2);
			}
		}
	}

	Domain* GetDomainByIndex(int index) 
	{
		for(int i = 0; i < fdomain_list.size(); ++i)
		{
			if(fdomain_list[i]->index == index)
			{
				return fdomain_list[i];
			}
		}
		return NULL;
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
		AddEdge(a, b, fgraph);
		AddEdge(b, a, fgraph);

		//update fdomain_components 
		compute_connect_components();

		printf("F-Graph updated!\n[PRINT_FGRAPH]\n");
		for(FDG_ITER it = fgraph.begin(); it != fgraph.end(); ++it)
		{
			printf("%d:\n", it->first->index);			
			for(int j = 0; j < it->second.size(); ++j)
			{
				printf("%d: ", it->second[j]->index);
			}
			printf("\n"); 
		}
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

	void SetRootDomain(int index)
	{
		Domain * d = GetDomainByIndex(index);
		if(d)
		{
			d->SetRoot();
		}
	}
	
	void BeginRemoveLoops()
	{
		ComputeMST();
		FindRedundantEdges();
		FindLoops();
	}

	//BFS: from the root domain
	void ComputeMST()
	{
		Domain* root;
		for(int i = 0; i < fdomain_list.size(); ++i) 
		{
			if(fdomain_list[i]->IsRoot())
			{
				root = fdomain_list[i];
				break;
			}
		}
		std::queue<Domain*>q;
		q.push(root);
		while(!q.empty())
		{
			Domain* node = q.front();
			q.pop();
			FDG_ITER it = fgraph.find(node);
			if(it == fgraph.end())
			{
				printf("[ERROR]====ComputeMST, Can not find domain:%d", node->index);
				return;
			}
			else
			{
				//node's neighbors
				for(int i = 0; i < it->second.size(); ++i)
				{
					if(node->mst_pa != it->second[i]->mst_pa)
					{
						//union
						it->second[i]->mst_pa = node->mst_pa;
						//Add to mini tree
						AddEdge(node, it->second[i], mini_tree);
						AddEdge(it->second[i], node, mini_tree);
						//in queue
						q.push(it->second[i]);
					}
				}
			}
		}
	}

	bool FindLoopRecursive(Domain* src ,Domain* tar, std::vector<int>&loop_path, std::set<Domain*>&vis)	
	{
		vis.insert(src);
		FDG_ITER it = mini_tree.find(src);
		if(it == mini_tree.end())
		{
			printf("[ERROR] Mini tree can not find domain a%d:\n", src->index);
			return false;
		}
		else
		{
			//for src's neighbors
			for(int i = 0; i < it->second.size(); ++i)
			{
				Domain* cur = it->second[i];
				//find target
				if(cur == tar)
				{
					loop_path.push_back(tar->index);
					return true;	
				}
				//not visited
				else if(vis.find(cur) == vis.end())
				{
					if(FindLoopRecursive(cur, tar, loop_path, vis))	
					{
						loop_path.push_back(cur->index);
						return true;
					}
				}
			}
		}
		return false;
	}

	void FindLoops()
	{
		std::vector<int>loop_path;
		FDG_ITER re_it = redundant_edges.begin();
		for(;re_it != redundant_edges.end(); ++re_it)
		{
			Domain* key = re_it->first;
			bool found = false;
			for(int i = 0; i < re_it->second.size(); ++i)
			{
				std::set<Domain*>vis;
				//key is the source node of dfs, and re_it->second[i] is the destination
				found = FindLoopRecursive(key, re_it->second[i], loop_path, vis);
				if(found)
				{
					rd_a = key;
					rd_b = re_it->second[i];
					loop_path.push_back(key->index);
					break;
				}
			}
			if(found)
			{
				break;
			}
		}
		if(loop_path.size() == 0)
		{
			printf("[PRINT LOOP PATH]: No more loop!");
		}
		else
		{
			printf("[PRINT LOOP PATH]:\n");
			for(int i = 0; i < loop_path.size(); ++i)	
			{
				printf("%d ", loop_path[i]);
			}
			printf("\n");
		}
	}
	
	void FindRedundantEdges()	
	{
		FDG_ITER mst_it;
		for(FDG_ITER fg_it = fgraph.begin(); fg_it != fgraph.end(); ++fg_it)
		{
			Domain* key = fg_it->first;
			mst_it = mini_tree.find(key); 
			//can not find this node in mini tree
			if(mst_it == mini_tree.end())
			{
				//put this node's all edges into redundant
				redundant_edges.insert(std::pair<Domain*, std::vector<Domain*>>(key, fg_it->second)); 
			}
			else
			{
				//find all edges not in mini tree
				for(int i = 0; i < fg_it->second.size(); ++i)
				{
					Domain* tar = fg_it->second[i];
					bool found = false;
					for(int j = 0; j < mst_it->second.size(); ++j)
					{
						if(tar == mst_it->second[j])
						{
							found = true;
							break;
						}
					}
					if(!found)
					{
						//add to redundant
						AddEdge(key, tar, redundant_edges);
						AddEdge(tar, key, redundant_edges);
					}
				}
			}
		}
		//print
		printf("[PRINT REDUNDANT EDGES] size:%d\n", redundant_edges.size());
		for(FDG_ITER it = redundant_edges.begin(); it != redundant_edges.end(); ++it)
		{
			printf("Node: %d\n", it->first->index);	
			for(int i = 0; i < mst_it->second.size(); ++i)
			{
				printf("%d ", it->second[i]);
			}
			printf("\n");
		}
	}

	void print_graph()
	{
		printf("Graph structure:\n"); 
		for(FDG_ITER it = fgraph.begin(); it != fgraph.end(); ++it)
		{
			int len = it->second.size();
			printf("Domain Index: %d neighbor size: %d\n", it->first->index, len); 
			for(int i = 0; i < len; ++i) 
			{
				printf("%d ", it->second[i]->index); 
			}
			printf("\n");	
		}
		for(FDG_ITER it; it != fgraph.end(); ++it)
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
