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
//
//

TEST_CASE("Multiply two digit small numbers", "MultiDigit multiply") {
	for (uint64_t i = 10; i < 100; i++)
	{
		BigNum first(std::to_string(i));
		for (uint64_t j = 10; j < 1000LL; j++)
		{
			BigNum second(std::to_string(j));
			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));

			BigNum result = GeLu::multiply(first, second);

			CHECK(result.str() == std::to_string(i * j));
		}
	}
}
//
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
	for (int i = 0; i < 10; i++) {
		BigNum single(std::to_string(i));
		REQUIRE(single.length() == 1);
		REQUIRE(single.str() == std::to_string(i));
	}
	
	BigNum num("1234");
	REQUIRE(num.bitStr() == "00010010 00110100 00000000");
	REQUIRE(num.length() == 4);
	REQUIRE(!num.isNegative());

	BigNum neg_num("1234", true);
	REQUIRE(neg_num.bitStr() == "00010010 00110100 00000010");
	REQUIRE(neg_num.length() == 4);
	REQUIRE(neg_num.isNegative());

	BigNum odd_size("12345");
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
				CHECK(num->at(i) == i + 1);
			}
			CHECK(num->str(false) == str);
		}
	}

	SECTION("Negative numbers") {
		BigNum negNum(std::to_string(-100));
		REQUIRE(negNum.isNegative());
		REQUIRE(negNum.bitStr() == "00010000 00000011");
		REQUIRE(negNum.str() == "-100");

		BigNum negNum2(std::to_string(100), true);
		REQUIRE(negNum2.isNegative());
		REQUIRE(negNum2.bitStr() == "00010000 00000011");
		REQUIRE(negNum2.str() == "-100");

		BigNum negNum3(std::to_string(-10));
		REQUIRE(negNum3.isNegative());
		REQUIRE(negNum3.bitStr() == "00010000 00000010");
		REQUIRE(negNum3.str() == "-10");

		BigNum negNum4(std::to_string(-1));
		REQUIRE(negNum4.isNegative());
		REQUIRE(negNum4.bitStr() == "00010011");
		REQUIRE(negNum4.str() == "-1");

		for (int i = -9; i < 0; i++) {
			BigNum negLoop(std::to_string(i));
			CHECK(negLoop.isNegative());
			CHECK(negLoop.length() == 1);
			CHECK(negLoop.str() == std::to_string(i));
		}	

		for (int i = -100; i < 0; i++) {
			BigNum negLoop(std::to_string(i));
			CHECK(negLoop.isNegative());
			CHECK(negLoop.str() == std::to_string(i));
		}

		for (long i = -100'000'000'000; i < -98'000'000'000; i++) {
			BigNum negLoop(std::to_string(i));
			CHECK(negLoop.isNegative());
			CHECK(negLoop.str() == std::to_string(i));
		}
	}
}

TEST_CASE("Bignum Addition", "BigNum operation") {
	SECTION("Small numbers") {
		for (int i = 1; i < 100; i++)
		{
			for (int j = 1; j < 100; j++)
			{
				BigNum num1(std::to_string(i));
				BigNum num2(std::to_string(j));

				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sum(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string(i + j));
			}
		}
	}

	SECTION("Sum large numbers") {
		for (uint64_t i = 100'000'000ULL; i < 100'000'100ULL; i++)
		{
			for (uint64_t j = 100'000'000ULL; j < 100'000'100ULL; j++)
			{
				BigNum num1(std::to_string(i));
				BigNum num2(std::to_string(j));
				
				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sum(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string(i + j));
			}
		}
	}

	SECTION("Sum negative with positive") {
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < i; j++)
			{
				BigNum num1(std::to_string(i));
				BigNum num2(std::to_string(-1*j));
			
				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sum(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string(i - j));
			}
		}
	}

	SECTION("Sum negative with negative") {
		for (int i = 1; i < 100; i++)
		{
			for (int j = 1; j < i; j++)
			{
				BigNum num1(std::to_string(-1 * i));
				BigNum num2(std::to_string(-1 * j));
				REQUIRE(num1.isNegative());
				REQUIRE(num2.isNegative());

				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sum(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string((-1*i) + (-1*j)));
			}
		}
	}
}

TEST_CASE("Bignum Subtraction", "BigNum operation") {
	SECTION("Small numbers, positive result") {
		for (int i = 1; i < 100; i++)
		{
			for (int j = 1; j < i; j++)
			{
				BigNum num1(std::to_string(i));
				BigNum num2(std::to_string(j));

				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sub(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string(i - j));
			}
		}
	}

	SECTION("Small numbers, negative results") {
		for (int i = 1; i < 100; i++)
		{
			for (int j = i + 1; j < 100; j++)
			{
				BigNum num1(std::to_string(i));
				BigNum num2(std::to_string(j));

				INFO("i=" + num1.str() + ", j=" + num2.str());
				BigNum result = BigNum::sub(num1, num2);
				std::string str = result.str();
				CHECK(str == std::to_string(i - j));
			}
		}
	}
}

TEST_CASE("Multiply two small numbers", "Small multiply") {
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			BigNum num1(std::to_string(i));

			INFO("i=" + num1.str() + ", j=" + std::to_string(j));
			
			BigNum result = GeLu::multiply_impl(j, num1);
			std::string str = result.str();
			str.erase(0, std::min(str.find_first_not_of('0'), str.size() - 1));

			CHECK(str == std::to_string(i * j));
		}
	}
}
