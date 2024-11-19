#pragma once
#include <iostream>
#include <omp.h>
#include <vector>
#include <string>

class MyMatrix
{
	int n;
	int m;
	double** M;

public:
	MyMatrix(int n_, int m_, std::vector<double>& data);
	MyMatrix(int n_, int m_);
	MyMatrix(int n_);
	MyMatrix();
	MyMatrix(const MyMatrix& orig);
	~MyMatrix();

	static MyMatrix readMatrix(const std::string& filename);
	static MyMatrix createRandMatrix(int n_, int m_);

	std::pair<int, int> size() const;
	double inf_norma() const;

	MyMatrix operator - () const;
	MyMatrix operator - (const MyMatrix& that) const;
	MyMatrix operator + (const MyMatrix& that) const;
	MyMatrix operator * (const MyMatrix& that) const;

	MyMatrix inverse() const;

	friend std::istream& operator>> (std::istream& is, MyMatrix& that);
	friend std::ostream& operator<< (std::ostream& os, const MyMatrix& that);
};

