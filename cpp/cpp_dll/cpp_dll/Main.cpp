#include <iostream>
#include <thread>
#include <chrono>

#include "../math_dll/math_dll.h"

int main()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));

	double res_add = MathLibrary::Functions::Add(1, 15);
	double res_mul = MathLibrary::Functions::Multiply(4, 8);

	double * ref_res_add = &res_add;
	MathLibrary::Functions::IncrementConstant();

	std::cout << "Press any key to continue . . ." << std::endl;

	std::getchar();

	return 0;
}