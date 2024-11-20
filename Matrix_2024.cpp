#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <cmath>
#include <map>

#include <omp.h>

#include "MyMatrix.h"
#include "MyVector.h"


namespace fs = std::filesystem;


static std::pair<double, double> run_test_inverse(const MyMatrix& M, int number_of_threads) {
	omp_set_num_threads(number_of_threads);

	auto start = std::chrono::high_resolution_clock::now();
	auto M_1 = M.inverse();
	auto end = std::chrono::high_resolution_clock::now();

	double err = (MyMatrix(M.size().first) - M * M_1).inf_norma();
	double dt = (end - start).count();
	return { err, dt };
}

static void run_tests_inverse(const std::vector<int> Ns, const std::vector<int> num_threads, std::ofstream& out) {
	// Начало работы тестов
	out << "N,number_of_threads,err_inv,time" << std::endl;
	std::cout << "N,number_of_threads,err_inv,time" << std::endl;
	for (auto& n : Ns)
	{
		auto M = MyMatrix::createRandMatrix(n, n);
		for (auto& i : num_threads)
		{
			auto p = run_test_inverse(M, i);
			double err_inv = p.first, time = p.second;
			std::cout << n << "," << i << "," << err_inv << "," << time << std::endl;
			out << n << "," << i << "," << err_inv << "," << time << std::endl;
		}
	}
}

static void run_tests_op2(const std::vector<int> Ns, const std::vector<int> num_threads, std::ofstream& out, MyMatrix(*op)(MyMatrix&, MyMatrix&)) {
	out << "N,number_of_threads,time" << std::endl;
	std::cout << "N,number_of_threads,time" << std::endl;

	for (auto& n : Ns)
	{
		auto M1 = MyMatrix::createRandMatrix(n, n);
		auto M2 = MyMatrix::createRandMatrix(n, n);
		for (auto& i : num_threads)
		{
			omp_set_num_threads(i);
			auto start = std::chrono::high_resolution_clock::now();
			auto M_1 = op(M1, M2);
			auto end = std::chrono::high_resolution_clock::now();

			std::cout << n << "," << i << "," << (end - start).count() << std::endl;
			out << n << "," << i << "," << (end - start).count() << std::endl;
		}
	}
}

int main(int argc, char* argv[], char* env[])
{
	// Значения по умолчанию
	std::map<std::string, int> myEnvIntArgs;
	myEnvIntArgs["NUM_THREADS="] = omp_get_max_threads();
	myEnvIntArgs["N_MIN="] = 100;
	myEnvIntArgs["NUM_MATRICES="] = 3;
	std::map<std::string, std::string> myEnvStrArgs;
	myEnvStrArgs["TEST_MODE="] = "SUM";
	myEnvStrArgs["PATH_OUTPUT="] = "results.txt";

	// Обновляем значения если они были объявлены
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		for (const auto& pair : myEnvIntArgs) {
			if (arg.rfind(pair.first, 0) == 0)
				myEnvIntArgs[pair.first] = std::stoi(arg.substr(pair.first.size()));
		}
		for (const auto& pair : myEnvStrArgs) {
			if (arg.rfind(pair.first, 0) == 0)
				myEnvStrArgs[pair.first] = arg.substr(pair.first.size());
		}
	}

	// Создаём параметры для тестов
	std::vector<int> Ns{  };
	for (int n = myEnvIntArgs["N_MIN="]; n < myEnvIntArgs["N_MIN="] * std::pow(2, myEnvIntArgs["NUM_MATRICES="] + 1); n *= 2)
		Ns.push_back(n);

	std::vector<int> num_threads{  };
	for (int t = 1; t < std::min(omp_get_max_threads(), myEnvIntArgs["NUM_THREADS="]) + 1; t++)
		num_threads.push_back(t);

	// Открытие выходного файла
	if (not fs::path(myEnvStrArgs["PATH_OUTPUT="]).is_absolute())
	{
		fs::path current_file_path = __FILE__;
		auto current_dir = current_file_path.parent_path().string();
		myEnvStrArgs["PATH_OUTPUT="] = current_dir + "\\" + myEnvStrArgs["PATH_OUTPUT="];
	}

	std::ofstream out(myEnvStrArgs["PATH_OUTPUT="]);
	assert(out.is_open() && "Ошибка открытия файла");
	// Запускаем тесты
	if (myEnvStrArgs["TEST_MODE="] == "SUM")
		run_tests_op2(Ns, num_threads, out, [](MyMatrix& a, MyMatrix& b) {return a + b; });
	else if (myEnvStrArgs["TEST_MODE="] == "PROD")
		run_tests_op2(Ns, num_threads, out, [](MyMatrix& a, MyMatrix& b) {return a * b; });
	else if (myEnvStrArgs["TEST_MODE="] == "INV")
		run_tests_inverse(Ns, num_threads, out);
	else
	{
		std::cout << "There is no such testing mode:" << myEnvStrArgs["TEST_MODE="] << std::endl;
		assert(false);
	}

	// Конец подсчёта тестов
	std::cout << "The solution is saved in: " << myEnvStrArgs["PATH_OUTPUT="] << std::endl;
	out.close();

	return 0;
}