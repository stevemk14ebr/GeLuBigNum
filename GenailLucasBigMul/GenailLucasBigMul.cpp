#include <chrono>
#include <thread>
#include <vector>

//#define SHOW_DBG
#include "GenailLucas.h"
#include "BigNum.h"

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
//
//TEST_CASE("Multiply two small numbers", "Small multiply") {
//	for (int i = 1; i < 100; i++)
//	{
//		for (int j = 1; j < 100; j++)
//		{
//			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
//			CHECK(GeLu::multiply(std::to_string(i), std::to_string(j)) == std::to_string(i * j));
//		}
//	}
//}
//
//TEST_CASE("Sum two small numbers", "Small addition") {
//	for (int i = 1; i < 100; i++)
//	{
//		for (int j = 1; j < 100; j++)
//		{
//			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
//			CHECK(GeLu::sum(std::to_string(i), std::to_string(j)) == std::to_string(i + j));
//		}
//	}
//}
//
//TEST_CASE("Multiply two large numbers", "Large multiply") {
//	for (uint64_t i = 100'000'000ULL; i < 100'000'100ULL; i++)
//	{
//		for (uint64_t j = 100'000'000ULL; j < 100'000'100ULL; j++)
//		{
//			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
//			CHECK(GeLu::multiply(std::to_string(i), std::to_string(j)) == std::to_string(i * j));
//		}
//	}
//}
//
//TEST_CASE("Sum two large numbers", "Large addition") {
//	for (uint64_t i = 100'000'000ULL; i < 100'000'100ULL; i++)
//	{
//		for (uint64_t j = 100'000'000ULL; j < 100'000'100ULL; j++)
//		{
//			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
//			CHECK(GeLu::sum(std::to_string(i), std::to_string(j)) == std::to_string(i + j));
//		}
//	}
//}
//
//TEST_CASE("Factorial small", "Small factorial") {
//	std::string product = "1";
//	for (int i = 1; i <= 50; i++)
//		product = GeLu::multiply(product, std::to_string(i));
//	REQUIRE(product == "30414093201713378043612608166064768844377641568960512000000000000");	
//}
//
//TEST_CASE("Factorial large", "Large factorial") {
//	std::string product = "1";
//	for (int i = 1; i <= 120; i++)
//		product = GeLu::multiply(product, std::to_string(i));
//	std::cout << product << std::endl;
//	REQUIRE(product == "6689502913449127057588118054090372586752746333138029810295671352301"
//		"6335572449629893668741652719849813081576378932140905525344085894081218598984811143"
//		"89650005964960521256960000000000000000000000000000");
//}

TEST_CASE("BigNum encoding", "BigNum Encode") {
	BigNum num("1234");
	REQUIRE(num.bitStr() == "00010010 00110100 00000000");
	REQUIRE(num.length() == 4);
	REQUIRE(!num.isNegative());

	BigNum neg_num("1234", true);
	REQUIRE(neg_num.bitStr() == "00010010 00110100 00000010");
	REQUIRE(neg_num.length() == 4);
	REQUIRE(neg_num.isNegative());

	BigNum odd_size("12345");
	std::cout << odd_size.bitStr() << std::endl;
	REQUIRE(odd_size.bitStr() == "00010010 00110100 01010001");
	REQUIRE(odd_size.length() == 5);
	REQUIRE(!odd_size.isNegative());

	BigNum odd_size_neg("12345", true);
	REQUIRE(odd_size_neg.bitStr() == "00010010 00110100 01010011");
	REQUIRE(odd_size_neg.length() == 5);
	REQUIRE(odd_size_neg.isNegative());

	BigNum upperDigits("0123456789");
	REQUIRE(upperDigits.bitStr() == "00000001 00100011 01000101 01100111 10001001 00000000");
	REQUIRE(upperDigits.length() == 10);

	SECTION("Extraction") {
		for (int i = 0; i < upperDigits.length(); i++) {
			REQUIRE(upperDigits.at(i) == i);
		}
		
		std::vector<BigNum*> nums{ &num, &neg_num, &odd_size, &odd_size_neg };
		for (BigNum* num : nums) {
			std::string str;
			for (int i = 0; i < num->length(); i++) {
				str += std::to_string(i + 1);
				REQUIRE(num->at(i) == i + 1);
			}
			REQUIRE(num->str() == str);
		}
	}
}
