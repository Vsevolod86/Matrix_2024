#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <string>
#include <cmath>
#include <cassert>
#include <filesystem>
#include <cstdlib>
#include <map>

#include "MyMatrix.h"
#include "MyVector.h"


namespace fs = std::filesystem;

static std::pair<double, double> run_test(MyMatrix& M, int number_of_threads) {
	omp_set_num_threads(number_of_threads);

	auto start = std::chrono::high_resolution_clock::now();
	auto M_1 = M.inverse();
	auto end = std::chrono::high_resolution_clock::now();

	double err = (MyMatrix(M.size().first) - M * M_1).inf_norma();
	double dt = (end - start).count();
	return { err, dt };
}

static void run_tests(std::vector<int> Ns, std::vector<int> num_threads, std::string path) {
	std::ofstream out(path);
	assert(out.is_open() && "Ошибка открытия файла");

	out << "N,number_of_threads,err_inv,dt_inv" << std::endl;
	std::cout << "N,number_of_threads,err_inv,dt_inv" << std::endl;
	for (auto& n : Ns)
	{
		auto M = MyMatrix::createRandMatrix(n, n);
		for (auto& i : num_threads)
		{
			auto p = run_test(M, i);
			double err_inv = p.first, dt_inv = p.second;
			std::cout << n << "," << i << "," << err_inv << "," << dt_inv << std::endl;
			out << n << "," << i << "," << err_inv << "," << dt_inv << std::endl;
		}
	}

	if (fs::path(path).is_absolute())
	{
		std::cout << "The solution is saved in: " << path << std::endl;
	}
	else
	{
		fs::path current_file_path = __FILE__;
		auto current_dir = current_file_path.parent_path().string();
		std::cout << "\nThe solution is saved in: " << current_dir << "\\" << path << "\n\n";
	}

	out.close();
}

int main(int argc, char* argv[], char* env[])
{
	// Значения по умолчанию
	std::map<std::string, int> myEnvArgs;
	myEnvArgs["NUM_THREADS="] = omp_get_max_threads();
	myEnvArgs["N_MIN="] = 64;
	myEnvArgs["NUM_MATRICES="] = 4;
	std::string path_output = "results.txt";

	// Обновляем значения если они были объявлены
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		for (const auto& pair : myEnvArgs) {
			if (arg.rfind(pair.first, 0) == 0)
				myEnvArgs[pair.first] = std::stoi(arg.substr(pair.first.size()));
		}
		if (arg.rfind("PATH_OUTPUT=", 0) == 0)
			path_output = arg.substr(12);
	}

	// Создаём параметры для тестов
	std::vector<int> Ns{  };
	for (int n = myEnvArgs["N_MIN="]; n < myEnvArgs["N_MIN="] * std::pow(2, myEnvArgs["NUM_MATRICES="] + 1); n *= 2)
		Ns.push_back(n);

	std::vector<int> num_threads{  };
	for (int t = 1; t < std::min(omp_get_max_threads(), myEnvArgs["NUM_THREADS="]) + 1; t++)
		num_threads.push_back(t);

	// Запускаем тесты
	run_tests(Ns, num_threads, path_output);

	return 0;
}