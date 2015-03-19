#include "InstancingModule.h"

//void InstancingModule::Instancing(vector<Domain*>&domain_list)
void InstancingModule::Instancing()
{
//	ShapeMatching();
	//TextureMap(domain_list);
	//print(domain_list);
	TextureMap();
	print();
}

//void InstancingModule::TextureMap(vector<Domain*>&domain_list)
void InstancingModule::TextureMap()
{
	clock_t start = clock();
	size_t len = domain_list.size();

	UnionFindDomain ufd;
	
	int cnt1 = 0;
	int cnt2 = 0;
	//find instance copies
	for(int i = 0; i < len; ++i)
	{
		for(int j = i + 1; j < len; ++j)
		{
			bool res = _TextureMap(domain_list[i], domain_list[j]);
			//is instanced copy
			if(res)
			{
				//++cnt1;
				ufd._Union(domain_list[i], domain_list[j]);
			}
			else
			{
				//++cnt2;
			}
		}
	}
	//printf("same copies instance: %d\n", cnt1);
	//printf("not same copies instance: %d\n", cnt2);
	
	map<Domain*, vector<Domain*>>instance_map;
	map<Domain*, vector<Domain*>>::iterator it;
	
	//organize instance set
	for(int i = 0; i < len; ++i) 
	{
		Domain* dom = domain_list[i];
		it  = instance_map.find(dom->instance_pa);
		//cannot find
		if(it == instance_map.end())
		{
			vector<Domain*>instance_set;
			instance_set.push_back(dom);
			instance_map.insert(std::pair<Domain*, vector<Domain*>>(dom->instance_pa, instance_set)); 
		}
		else
		{
			it->second.push_back(dom);
		}
	}
	
	//store data to global variable
	for(it = instance_map.begin(); it != instance_map.end(); ++it)
	{
		all_instance_set.push_back(it->second);
		printf("each instance set size: %d\n", it->second.size());
	}

	printf("\nAfter 4.2 Instancing combine same_uv domains into each instance set.\n");
	printf("Instance set size: %d\n", all_instance_set.size());

	clock_t end = clock();
	double time = (double) (end-start) / CLOCKS_PER_SEC;
	printf("Time use: %lf secs\n", time); 
}

//void InstancingModule::print(vector<Domain*>&domain_list)
void InstancingModule::print()
{
	/*
	int total_faces = 0;
	int cnt = 0;
	for(int i = 0; i < domain_list.size(); ++i)
	{
		
	}
	printf("Total faces: %d\n", total_faces);
	printf("Instancing faces: %d\n", cnt);
	*/
}

bool InstancingModule::_TextureMap(Domain *a, Domain *b)
{
	//face size not match
	if(a->face_list.size() != b->face_list.size())
		return false;

	if(a->uv_coords_list.size() != b->uv_coords_list.size())
		return false;
	
	size_t len = a->face_list.size();
	//debug
	//len = a->uv_coords_list.size();

	bool *vis = new bool[len];
	for(int i = 0; i < len; ++i)
	{
		vis[i] = false;
	}
	
	//from domain a, pick nearest face from domain b.
	for(int i = 0; i < len; ++i)
	{
		bool found = false;
		float dis = FLT_MAX;
		int tar_idx = -1;
		for(int j = 0; j < len; ++j)
		{
			if(vis[j]) continue;
			//calc uv distance
			float tmp_dis = a->face_list[i]->GetUVdistance(b->face_list[j]);
			//float tmp_dis = glm::distance(a->uv_coords_list[i], b->uv_coords_list[j]);
			//find nearest face
			if(tmp_dis <= EPS && tmp_dis < dis)
			{
				//greedy: if found one neighbour near enough, pick it!	
				dis = tmp_dis;
				//vis[j] = true;
				tar_idx = j;
				found = true;
			}
		}
		if(found)
		{
			vis[tar_idx] = true;
		}
		else
		{
			delete[] vis;
			return false;
		}
	}
	delete[] vis;
	return true;
}
