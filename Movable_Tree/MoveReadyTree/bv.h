#ifndef BV_H 
#define BV_H 

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "ray.h"
#include "Face.h"
#include "Domain.h"

class Face;
class Domain;

#ifndef EPS
#define EPS 1e-6f
#endif

class BV 
{
public:
	BV();
	BV(std::vector<glm::vec3>&);
	BV(std::vector<Face*>&);
	BV(std::vector<Domain*>&all_domains);
	virtual ~BV(); 
    
	bool is_intersect(const Ray &);
	inline bool aabb_intersect_test(const Ray &);
	bool sphere_intersect_test(const Ray &);

	void init_box(std::vector<glm::vec3>&); 
	void init_sphere(std::vector<glm::vec3>&); 

	//bounding sphere
    glm::vec3 center_;
    float radius_;

	//bounding box: AABB
	glm::vec3 max_pt;
	glm::vec3 min_pt;
	int get_longest_axis();

	bool use_aabb;
};

#endif