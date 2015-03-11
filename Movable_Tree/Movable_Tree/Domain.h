#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <string>
#include <vector>
#include <map>
#include <string>
#include "triangle.h"

class Domain
{
public:
	std::vector<Triangle>& triangle_list;
	std::string tag;

public:
    Domain();
    ~Domain() {}

    //void loadProgram(const std::string& fileName);
    //void loadProgramFromString(const std::string& program);
protected:
};

#endif