#pragma once
#include "MyMatrix.h"
class MyVector :
	public MyMatrix
{
public:

	MyVector(int n_, std::vector<double>& data);
	MyVector(int n_);
	MyVector(const MyVector& orig);
};

