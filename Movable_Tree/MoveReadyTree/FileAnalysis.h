#ifndef _FILEANALYSIS_H
#define _FILEANALYSIS_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class MAReader{
private:
	ifstream reader;
	string fileName;
	int componentNum;
	vector<string> componentName;

public:
	MAReader(string fileName);

	MAReader();

	int getComponentNum();

	string getComponentName(int n);

	void readFile();
};

#endif