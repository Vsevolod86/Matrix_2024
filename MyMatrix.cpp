#include "MyMatrix.h"
#include <cassert>
#include <fstream>

MyMatrix::MyMatrix(int n_, int m_, std::vector<double>& data)
{//ÊÎÍÑÒĞÓÊÒÎĞ ÌÀÒĞÈÖÛ ÏÎ ÂÅÊÒÎĞÓ
	n = n_;
	m = m_;
	assert((m * n == data.size()) && "Error sum");
	M = new double* [n];
	for (int i = 0; i < n; i++)
	{
		M[i] = new double[m];
		for (int j = 0; j < m; j++)
			M[i][j] = data[i * m + j];
	}
}

MyMatrix::MyMatrix(int n_, int m_)
{//ÊÎÍÑÒĞÓÊÒÎĞ ÌÀÒĞÈÖÛ ÇÀÏÎËÍÅÍÎÉ ÍÓËßÌÈ
	n = n_;
	m = m_;
	M = new double* [n];
	for (int i = 0; i < n; i++)
	{
		M[i] = new double[m];
		for (int j = 0; j < m; j++) M[i][j] = 0;
	}
}

MyMatrix::MyMatrix(int n_)
{//ÊÎÍÑÒĞÓÊÒÎĞ ÅÄÈÍÈ×ÍÎÉ ÌÀÒĞÈÖÛ
	n = n_;
	m = n_;
	M = new double* [n];
	for (int i = 0; i < n; i++)
	{
		M[i] = new double[n];
		for (int j = 0; j < n; j++)
			if (i != j) M[i][j] = 0;
			else M[i][j] = 1;
	}
}

MyMatrix::MyMatrix() : n(0), m(0), M(NULL)
{//ÊÎÍÑÒĞÓÊÒÎĞ ÁÅÇ ÏÀĞÀÌÅÒĞÀ
}

MyMatrix::MyMatrix(const MyMatrix& orig)
{//ÊÎÍÑÒĞÓÊÒÎĞ ÊÎÏÈĞÎÂÀÍÈß
	n = orig.n;
	m = orig.m;
	M = new double* [n];
	for (int i = 0; i < n; i++)
	{
		M[i] = new double[m];
		for (int j = 0; j < m; j++)
			M[i][j] = orig.M[i][j];
	}
}

MyMatrix::~MyMatrix()
{//ÄÅÑÒĞÓÊÒÎĞ
	for (int i = 0; i < n; i++) delete[] M[i];
	delete[] M;
}

MyMatrix MyMatrix::readMatrix(const std::string& file_name)
{//Ñ×ÈÒÛÂÀÅÍÈÅ ÌÀÒĞÈÖÛ
	std::ifstream file(file_name);
	assert(file.is_open() && "Îøèáêà îòêğûòèÿ ôàéëà");

	int rows, cols;
	file >> rows >> cols;

	std::vector<double> data;
	double element;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			file >> element;
			data.push_back(element);
		}
	}

	file.close();

	return MyMatrix(rows, cols, data);
}

MyMatrix MyMatrix::createRandMatrix(int n_, int m_)
{
	double minVal = 0.0, maxVal = 1.0;
	std::vector<double> randomVector(n_ * m_);
	for (int i = 0; i < randomVector.size(); ++i)
		randomVector[i] = static_cast<double>(rand()) / RAND_MAX * (maxVal - minVal) + minVal;

	return MyMatrix(n_, m_, randomVector);
}

std::pair<int, int> MyMatrix::size() const
{
	return { n, m };
}

double MyMatrix::inf_norma() const
{//ÁÅÑÊÎÍÅ×ÍÀß ÍÎĞÌÀ ÄËß ÌÀÒĞÈÖÛ
	double curr_sum = 0, desired_max = 0;
	for (int i = 0; i < n; i++)
	{
		curr_sum = 0;
		for (int j = 0; j < m; j++)
			curr_sum += std::abs(M[i][j]);
		if (curr_sum > desired_max) desired_max = curr_sum;
	}
	return desired_max;
}

MyMatrix MyMatrix::operator-() const
{//ÓÍÀĞÍÛÉ ÌÈÍÓÑ
	MyMatrix result(n, m);
	if (n > m)
	{
#pragma omp parallel for
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				result.M[i][j] = -M[i][j];
	}
	else
	{
#pragma omp parallel for
		for (int j = 0; j < m; j++)
			for (int i = 0; i < n; i++)
				result.M[i][j] = -M[i][j];

	}
	return result;
}

MyMatrix MyMatrix::operator-(const MyMatrix& that) const
{
	assert(((m == that.m) && (n == that.n)) && "Error sub");
	return *this + (-that);
}

MyMatrix MyMatrix::operator+(const MyMatrix& that) const
{//ÑËÎÆÅÍÈÅ ÌÀÒĞÈÖ
	assert(((m == that.m) && (n == that.n)) && "Error sum");
	MyMatrix result(n, m);
	if (n > m)
	{
#pragma omp parallel for
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				result.M[i][j] = M[i][j] + that.M[i][j];
	}
	else
	{
#pragma omp parallel for
		for (int j = 0; j < m; j++)
			for (int i = 0; i < n; i++)
				result.M[i][j] = M[i][j] + that.M[i][j];
	}
	return result;
}

MyMatrix MyMatrix::operator*(const MyMatrix& that) const
{//ÏĞÎÈÇÂÅÄÅÍÈÅ ÌÀÒĞÈÖ
	assert((m == that.n) && "Error mult");
	MyMatrix result(n, that.m);
	if (result.n > result.m)
	{
#pragma omp parallel for
		for (int i = 0; i < result.n; i++)
			for (int j = 0; j < result.m; j++)
				for (int t = 0; t < m; t++)
					result.M[i][j] += M[i][t] * that.M[t][j];
	}
	else
	{
#pragma omp parallel for
		for (int j = 0; j < result.m; j++)
			for (int i = 0; i < result.n; i++)
				for (int t = 0; t < m; t++)
					result.M[i][j] += M[i][t] * that.M[t][j];

	}
	return result;
}

MyMatrix MyMatrix::inverse() const
{//ÎÁĞÀÙÅÍÈÅ ÌÀÒĞÈÖÛ (äåëàë ïî "Êóğñó âû÷èñëèòåëüíûõ ìåòîäîâ" Ñ.Ï.Øàğûé, ñòğ. 156 - 165)
	assert((n == m) && "Error inverse (size)");
	//LU-ğàçëîæåíèå
	MyMatrix L(n), U = *this;
	for (int j = 0; j < n; j++)
	{
		//j-ûé ãëàâíûé ìèíîğ ğàâíÿåòñÿ íóëş => íå ñóùåñòâóåò LU-ğàçëîæåíèÿ
		assert((U.M[j][j] != 0) && "Error inverse (unexist)");

		for (int i = j + 1; i < n; i++)
		{//äîìíîæåíèå ñòğî÷êè j íà êîıôôèöèåíò L_{ij} è âû÷èòàíèå èç ñòğî÷êè i
			L.M[i][j] = U.M[i][j] / U.M[j][j];
			U.M[i][j] = 0;
#pragma omp parallel for
			for (int k = j + 1; k < n; k++)
				U.M[i][k] -= L.M[i][j] * U.M[j][k];
		}
	}
	//ğåøåíèå òğèóãîëüíûõ ÑËÀÓ
	MyMatrix Q(n);

	for (int j = 0; j < n; j++)
#pragma omp parallel for
		for (int i = j + 1; i < n; i++)
			for (int k = 0; k < n; k++)
				Q.M[i][k] -= L.M[i][j] * Q.M[j][k];

	for (int j = n - 1; j >= 0; j--)
	{
#pragma omp parallel for
		for (int i = 0; i < n; i++)
			Q.M[j][i] /= U.M[j][j];
#pragma omp parallel for
		for (int i = 0; i < j; i++)
			for (int k = 0; k < n; k++)
				Q.M[i][k] -= U.M[i][j] * Q.M[j][k];
	}
	return Q;
}

std::istream& operator>>(std::istream& is, MyMatrix& that)
{//ÂÂÎÄ
	int new_n, new_m;
	std::cout << "Enter the number of rows and columns: ";
	is >> new_n >> new_m;

	if (new_n != that.n || new_m != that.m)
	{//Ïåğåñîçäà¸ì ìàòğèöó åñëè ğàçìåğû îòëè÷àşòñÿ
		that.~MyMatrix();
		new (&that) MyMatrix(new_n, new_m);
	}

	std::cout << "Enter the elements of the Matrix:" << std::endl;
	for (int i = 0; i < that.n; i++)
		for (int j = 0; j < that.m; j++) is >> that.M[i][j];

	return is;
}

std::ostream& operator<<(std::ostream& os, const MyMatrix& that)
{//ÂÛÂÎÄ
	for (int i = 0; i < that.n; i++)
	{
		for (int j = 0; j < that.m; j++)
			os << that.M[i][j] << " ";
		os << "\n";
	}
	os << "\n";
	return os;
}
