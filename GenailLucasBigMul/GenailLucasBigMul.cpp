#include <iostream>
#include <chrono>
#include <thread>

#include "BigNumber.h"
#include "GenailLucas.h"

int main()
{	
	std::string mulLhs = "50";
	std::string mulRhs = "5";

	uint64_t totalTicks = 0;
	for (int i = 0; i < 10; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		std::string mul = GeLuMul::multiply(mulLhs, mulRhs);
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		totalTicks += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	}

	std::cout << "Multiply: " << GeLuMul::multiply(mulLhs, mulRhs) << std::endl;;
	std::cout << "Elapsed (ns): " << totalTicks / 10 <<  std::endl;
	std::cout << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Sum: " << GeLuMul::sum("9999", "1") << std::endl;
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Elapsed (ns): " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(500));
    return 0;
}

