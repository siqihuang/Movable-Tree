#include "InstancingModule.h"

//void InstancingModule::Instancing(std::vector<Domain*>&domain_list)
void InstancingModule::Instancing()
{
//	ShapeMatching();
	//TextureMap(domain_list);
	//print(domain_list);
	TextureMap();
	//print();
}

void InstancingModule::TextureMap()
{
	clock_t start = clock();
	size_t len = domain_list.size();

	UnionFindDomain ufd;
	//find instance copies
	for(int i = 0; i < len; ++i)
	{
		for(int j = i + 1; j < len; ++j)
		{
			if(domain_list[i]->instance_pa == domain_list[j]->instance_pa)
				continue;
			bool res = _TextureMap(domain_list[i], domain_list[j]);
			//is instanced copy
			if(res)
			{
				ufd._Union(domain_list[i], domain_list[j]);
			}
		}
	}
	//printf("same copies instance: %d\n", cnt1);
	//printf("not same copies instance: %d\n", cnt2);
	
	std::map<Domain*, std::vector<Domain*>>instance_map;
	std::map<Domain*, std::vector<Domain*>>::iterator it;
	
	//organize instance set
	for(int i = 0; i < len; ++i) 
	{
		Domain* d = domain_list[i];
		//find parent domain in map
		it  = instance_map.find(ufd._Find(d));
		//cannot find parent
		if(it == instance_map.end())
		{
			std::vector<Domain*>instance_set;
			instance_set.push_back(d);
			instance_map.insert(std::pair<Domain*, std::vector<Domain*>>(d->instance_pa, instance_set)); 
		}
		else
		{
			it->second.push_back(d);
		}
	}
	
	//store data to global variable
	int total = 0;
	for(it = instance_map.begin(); it != instance_map.end(); ++it)
	{
		all_instance_set.push_back(it->second);
		printf("each instance set size: %d ", it->second.size()); 
		std::cout<<it->second[0]->tag<<std::endl;
		//debug
		/*
		if(it->second.size() < 10)
		{
			printf("====================\n");
			for(int i = 0; i < it->second.size(); ++i)
			{
				printf("face size: %d\n", it->second[i]->face_list.size());
				for(int j = 0; j < it->second[i]->face_list.size(); ++j) 
				{
					Face* face = it->second[i]->face_list[j]; 
					printf("uv:");
					for(int k = 0; k < face->uv_coords.size(); ++k) 
					{
						printf("%f %f\n", face->uv_coords[k].x, face->uv_coords[k].y); 
					}
					printf("\n");
				}
			}
			printf("====================\n");
		}
		total += it->second.size();
		*/
	}

	printf("\nAfter 4.2 Instancing combine same_uv domains into each instance set.\n");
	printf("Instance set size: %d\n", all_instance_set.size());
	//printf("All Instance size: %d\n", total); 

	clock_t end = clock();
	double time = (double) (end-start) / CLOCKS_PER_SEC;
	printf("Time use: %lf secs\n", time); 
}

//void InstancingModule::print(std::vector<Domain*>&domain_list)
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
	if(a->texture_file != b->texture_file)
		return false;

	//face size not match
	if(a->face_list.size() != b->face_list.size())
		return false;

	//uv point size not match
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
		for(int j = 0; j < len; ++j)
		{
			if(vis[j]) continue;
			//calc uv distance
			float tmp_dis = a->face_list[i]->GetUVdistance(b->face_list[j]);
			if(tmp_dis <= EPS)
			{
				vis[j] = true;
				found = true;
				//greedy 
				break;
			}
		}
		if(!found)
		{
			delete[] vis;
			return false;
		}
	}
	delete[] vis;
	return true;
}
