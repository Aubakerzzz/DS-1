
#pragma once

#include <iostream>
#include <opencv2/opencv.hpp> 
#include <string>
#include <fstream>
using namespace cv;
using namespace std;


class CCL
{
private:
	int** array;
	int** check;
	int rows;
	int cols;
	int depend;
	int current;


public:
	CCL();
	CCL(string);
	void addDependencies(int**&);
	void firstScan(int, int, int&);
	bool find(int**, int&, int&);
	void order(int&, int&);
	void reduceDependencies(int**&);
	void checkDependency(int**&, int, int);
	void compute();
	int** getArray();
	int getRows();
	int getCols();
};

