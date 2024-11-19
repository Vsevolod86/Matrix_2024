#include "MyVector.h"

MyVector::MyVector(int n_, std::vector<double>& data) : MyMatrix(n_, 1, data)
{
}

MyVector::MyVector(int n_) : MyMatrix(n_, 1)
{
}

MyVector::MyVector(const MyVector& orig) : MyMatrix(orig)
{
}

