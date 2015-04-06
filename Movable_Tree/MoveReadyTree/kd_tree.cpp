#include "kd_tree.h"

KdNode::KdNode()
{
	box = NULL;
	left = NULL;
	right = NULL;
}

KdNode::KdNode(const std::vector<Face*>& in_tris, BV* pa_box, float mid, int dim, bool is_left) //for son treenode
{
	box = new BV();
	box->min_pt = pa_box->min_pt;
	box->max_pt = pa_box->max_pt;
	switch(dim)
	{
	case 0:
		if(is_left)
			box->max_pt.x = mid;
		else
			box->min_pt.x = mid;
		break;
	case 1:
		if(is_left)
			box->max_pt.y = mid;
		else
			box->min_pt.y = mid;
		break;
	case 2:
		if(is_left)
			box->max_pt.z = mid;
		else
			box->min_pt.z = mid;
		break;
	}
	face_list = in_tris;
	left = NULL;
	right = NULL;
}

KdNode::~KdNode() 
{
	face_list.clear();
	delete box;
}

struct sort_cmp
{
	sort_cmp(int dim)
	{
		this->dim = dim;
	}
	bool operator() (Face *a, Face *b)
	{
		return a->min_pt[dim] < b->min_pt[dim];
	}
	int dim;
};

inline float quick_select_mid_pt(float a[], int n, int k)
{
	int index = rand()% n;
	float pivot = a[index]; 
	float *l = new float[n];
	float *r = new float[n];
	int ln,rn;
	ln = rn = 0;
	for(int i = 0; i < n; ++i)
	{
		if(a[i] < pivot)
			l[ln++] = a[i]; 
		else if(a[i] > pivot)
			r[rn++] = a[i]; 
	}
	if(k < ln) return quick_select_mid_pt(l,  ln, k); 
	if(k > n - rn) return quick_select_mid_pt(r,  rn, k - (n - rn)); 
	delete[] l;
	delete[] r;
	return pivot;
}

float KdNode::find_mid(int dim)
{
	sort(face_list.begin(), face_list.end(), sort_cmp(dim));
	return face_list[face_list.size()/2]->min_pt[dim];
}

int KdNode::divide_node(int lev)
{
	level = lev;
	int n = face_list.size();
	if(lev >= MAX_LEVEL || n <= MIN_TRIS_NUM) return lev;	

	int dim = box->get_longest_axis(); 
	float mid = find_mid(dim);

	std::vector<Face*>left_tris;
	std::vector<Face*>right_tris;
	
	//if overlap triangles of left&right node reaches to this number, no more divide 
	int max_overlap = n / 4;  
	//overlap on left and right node
	int match = 0; 
	for(int i = 0; i < n; ++i) //each triangle	
	{
		bool l,r; 
		l = r = false; //if insert into left/right sub-tree
		float tar;
		Face* face = face_list[i];
		for(int j = 0; j < face->vertex_coords.size(); ++j) //each triangle point
		{
			if(l && r)break;
			switch(dim)
			{
			case 0:
				tar = face->vertex_coords[j].x; 
				break;
			case 1:
				tar = face->vertex_coords[j].y; 
				break;
			case 2:
				tar = face->vertex_coords[j].z; 
				break;
			}
			bool smaller = (tar <= mid);
			l |= smaller;
			r |= !smaller;
		}
		//a triangle both in left&right tree, and overlap each other over 50%. 
		if(l && r && ++match >= max_overlap) return lev;
		if(l) left_tris.push_back(face);
		if(r) right_tris.push_back(face);
	}

	left = new KdNode(left_tris, box, mid, dim, true);
	right = new KdNode(right_tris, box, mid, dim, false);
	return std::max(left->divide_node(level + 1), right->divide_node(level + 1));

	/*
	if(level == 0)
	{
		printf("left box min: %f %f %f\n", left->box->min_pt.x, left->box->min_pt.y, left->box->min_pt.z);
		printf("left box max: %f %f %f\n", left->box->max_pt.x, left->box->max_pt.y, left->box->max_pt.z);
		printf("\n");
	}
	*/
}

bool KdNode::search(const Ray &ray, std::vector<KdNode*>&nodes) //find which node the ray will hit
{
	if(!box->aabb_intersect_test(ray))
		return false;

	bool res = false;
	if(left && left->search(ray, nodes))
		res = true;
	if(right && right->search(ray, nodes))
		res = true;

	if(!res) //is leaf, insert current node and return
		nodes.push_back(this);
	return true;
}

void KdNode::print()
{
	printf("\n");
	printf("level: %d\n", level);
	for(int i = 0; i < face_list.size(); ++i)
	{
		printf("%d ", face_list[i]->index);
	}
}

//------------KdTree-------------------------------
KdTree::KdTree(){}
KdTree::KdTree(int max_depth)
{
	MAX_LEVEL = max_depth;
}

KdTree::~KdTree() {}

bool KdTree::search(const Ray &ray, std::vector<KdNode*>&nodes)
{
	return root->search(ray, nodes);	
}

KdNode* KdTree::build_tree(const std::vector<Domain*>& domain_list)
{
	for(int i = 0; i < domain_list.size(); ++i) 
	{
		Domain* d = domain_list[i];
		for(int j = 0; j < d->face_list.size(); ++j) 
		{
			root->face_list.push_back(d->face_list[j]);	
		}
	}
	root = new KdNode();
	root->box = new BV(root->face_list);

	clock_t newtime, oldtime;
	oldtime = clock();

	printf("Mesh triangles num %d\n", root->face_list.size());
	printf("KdTree start building...\n");

	level = root->divide_node(0);
	printf("KdTree finish building: \n");
	printf("Depth: %d\n", level);
	
	newtime = clock();
	float usetime = static_cast<float>(newtime - oldtime) / static_cast<float>(CLOCKS_PER_SEC);
	printf("Use time %.2f secs.\n", usetime);
	printf("-------------------------------------------------------\n");

	return root;
}
