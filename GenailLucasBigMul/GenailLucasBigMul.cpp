#include <chrono>
#include <thread>

//#define SHOW_DBG
#include "GenailLucas.h"

#define CATCH_CONFIG_MAIN
#include "Catch.hpp"

// Use this to print the indexes of the rulers in the tables
//
//	GeLu::Ruler ruler = GeLu::Lut::rulers.at(1); <- ruler '0'
//	GeLu::Ruler idx = GeLu::Lut::rulers.at(0); <- index ruler
//
//	// 9 regions
//	for (uint8_t region = 1; region <= 9; region++)
//	{
//		// each region's size is = to its region number
//		for (int regionCount = 0; regionCount < region; regionCount++) {
//
//			uint8_t rulerIdx = (int)ruler.getRegionStart(region) + regionCount;
//			uint8_t tipIdx = ruler.getCarry(rulerIdx);
//			std::cout << "Ruler Idx:" << (int)rulerIdx << " Tip Idx: " << (int)tipIdx << std::endl;
//		}	
//		std::cout << std::endl;
//	}

TEST_CASE("Multiply two small numbers", "Small multiply") {
	for (int i = 1; i < 100; i++)
	{
		for (int j = 1; j < 100; j++)
		{
			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
			CHECK(GeLu::multiply(std::to_string(i), std::to_string(j)) == std::to_string(i * j));
		}
	}
}

TEST_CASE("Sum two small numbers", "Small addition") {
	for (int i = 1; i < 100; i++)
	{
		for (int j = 1; j < 100; j++)
		{
			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
			CHECK(GeLu::sum(std::to_string(i), std::to_string(j)) == std::to_string(i + j));
		}
	}
}

TEST_CASE("Multiply two large numbers", "Large multiply") {
	for (uint64_t i = 100'000'000ULL; i < 100'000'100ULL; i++)
	{
		for (uint64_t j = 100'000'000ULL; j < 100'000'100ULL; j++)
		{
			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
			CHECK(GeLu::multiply(std::to_string(i), std::to_string(j)) == std::to_string(i * j));
		}
	}
}

TEST_CASE("Sum two large numbers", "Large addition") {
	for (uint64_t i = 100'000'000ULL; i < 100'000'100ULL; i++)
	{
		for (uint64_t j = 100'000'000ULL; j < 100'000'100ULL; j++)
		{
			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
			CHECK(GeLu::sum(std::to_string(i), std::to_string(j)) == std::to_string(i + j));
		}
	}
}
