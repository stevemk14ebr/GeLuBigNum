#include <chrono>
#include <thread>

#include "GenailLucas.h"

int main()
{	
	std::string mulLhs = "50";
	std::string mulRhs = "5";

	// multiply benchmark
	uint64_t totalTicks = 0;
	std::string mul = "";
	for (int i = 0; i < 1000; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		mul = GeLu::multiply(mulLhs, mulRhs);
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		totalTicks += std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
	}

	std::cout << "Multiply: " << mul << std::endl;
	std::cout << "Elapsed (ns): " << totalTicks / 1000 <<  std::endl;
	std::cout << std::endl;

	// addition benchmark
	totalTicks = 0;
	std::string sum = ""; // must be outside to prevent optimizer removing call
	for (int i = 0; i < 1000; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		sum = GeLu::sum("9999", "1");
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		totalTicks += std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
	}

	std::cout << "Sum: " << sum << std::endl;
	std::cout << "Elapsed (ns): " << totalTicks / 1000 << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(500));
    return 0;
}

