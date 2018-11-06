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

//TEST_CASE("Multiply two digit small numbers", "MultiDigit multiply") {
//	for (uint64_t i = 10; i < 1000LL; i++)
//	{
//		BigNum first(std::to_string(i));
//		for (uint64_t j = 10; j < 1000LL; j++)
//		{
//			BigNum second(std::to_string(j));
//			INFO("i=" + std::to_string(i) + ", j=" + std::to_string(j));
//
//			BigNum result = GeLu::multiply(first, second);
//			CHECK(result.str() == std::to_string(i * j));
//		}
//	}
//}
//
//

TEST_CASE("Factorial small", "Small factorial") {
	BigNum product = BigNum("1");
	for (int i = 1; i <= 99; i++) {
		product = GeLu::multiply(std::move(product), std::to_string(i));
	}

	REQUIRE(product.str() == "93326215443944152681699238856266700490715968264381621468592963895217"
		"5999932299156089414639761565182862536979208272237582511852109168640000000"
		"000000000000000");
}

TEST_CASE("Factorial large", "Large factorial") {
	BigNum product = BigNum("1");
	for (int i = 1; i <= 999; i++) {
		product = GeLu::multiply(std::move(product), std::to_string(i));
	}

	std::cout << product.str() << std::endl;

	REQUIRE(product.str() == "402387260077093773543702433923003985719374864210714632543799910429"   
		"9385123986290205920442084869694048004799886101971960586316668729948085589"
		"0132382966994459099742450408707375991882362772718873251977950595099527612"
		"0874975462497043601418278094646496291056393887437886487337119181045825783"
		"6478499770124766328898359557354325131853239584630755574091142624174743493"
		"4755342864657661166779739666882029120737914385371958824980812686783837455"
		"9731746136085379534524221586593201928090878297308431392844403281231558611"
		"0369768013573042161687476096758713483120254785893207671691324484262361314"
		"1250878020800026168315102734182797770478463586817016436502415369139828126"
		"4810213092761244896359928705114964975419909342221566832572080821333186116"
		"8115536158365469840467089756029009505376164758477284218896796462449451607"
		"6535340819890138544248798495995331910172335555660213945039973628075013783"
		"7615307127761926849034352625200015888535147331611702103968175921510907788"
		"0193931781141945452572238655414610628921879602238389714760885062768629671"
		"4667469756291123408243920816015378088989396451826324367161676217916890977"
		"9911903754031274622289988005195444414282012187361745992642956581746628302"
		"9555702990243241531816172104658320367869061172601587835207515162842255402"
		"6517048330422614397428693306169089796848259012545832716822645806652676995"
		"8652682272807075781391858178889652208164348344825993266043367660176999612"
		"8318607883861502794659551311565520360939881806121385586003014356945272242"
		"0634463179746059468257310379008402443243846565724501440282188525247093519"
		"0620929023136493273497565513958720559654228749774011413346962715422845862"
		"3773875382304838656889764619273838149001407673104466402598994902222217659"
		"0433990188601856652648506179970235619389701786004081188972991831102117122"
		"9845901641921068884387121855646124960798722908519296819372388642614839657"
		"3822911231250241866493531439701374285319266498753372189406942814341185201"
		"5801412334482801505139969429015348307764456909907315243327828826986460278"
		"9864321139083506217095002597389863554277196742822248757586765752344220207"
		"5736305694988250879689281627538488633969099598262809561214509948717012445"
		"1646126037902930912088908694202851064018215439945715680594187274899809425"
		"4742173582401063677404595741785160829230135358081840096996372524230560855"
		"9037006242712434169090041536901059339838357779394109700277534720000000000"
		"0000000000000000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000000000000000"
		"00000000000000000");
}

TEST_CASE("BigNum encoding", "BigNum Encode") {
	for (int i = 0; i < 10; i++) {
		BigNum single(std::to_string(i));
		REQUIRE(single.length() == 1);
		REQUIRE(single.str() == std::to_string(i));
	}
	
	BigNum num("1234");
	REQUIRE(num.bitStr() == "00010010 00110100");
	REQUIRE(num.length() == 4);
	REQUIRE(!num.isNegative());

	BigNum neg_num("1234", true);
	REQUIRE(neg_num.bitStr() == "00010010 00110100");
	REQUIRE(neg_num.length() == 4);
	REQUIRE(neg_num.isNegative());

	BigNum odd_size("12345");
	REQUIRE(odd_size.bitStr() == "00010010 00110100 01010000");
	REQUIRE(odd_size.length() == 5);
	REQUIRE(!odd_size.isNegative());

	BigNum odd_size_neg("12345", true);
	REQUIRE(odd_size_neg.bitStr() == "00010010 00110100 01010000");
	REQUIRE(odd_size_neg.length() == 5);
	REQUIRE(odd_size_neg.isNegative());

	BigNum upperDigits("0123456789");
	REQUIRE(upperDigits.bitStr() == "00000001 00100011 01000101 01100111 10001001");
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
		REQUIRE(negNum.bitStr() == "00010000 00000000");
		REQUIRE(negNum.str() == "-100");

		BigNum negNum2(std::to_string(100), true);
		REQUIRE(negNum2.isNegative());
		REQUIRE(negNum2.bitStr() == "00010000 00000000");
		REQUIRE(negNum2.str() == "-100");

		BigNum negNum3(std::to_string(-10));
		REQUIRE(negNum3.isNegative());
		REQUIRE(negNum3.bitStr() == "00010000");
		REQUIRE(negNum3.str() == "-10");

		BigNum negNum4(std::to_string(-1));
		REQUIRE(negNum4.isNegative());
		REQUIRE(negNum4.bitStr() == "00010000");
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

			CHECK(str == std::to_string(i * j));
		}
	}

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			BigNum num1(std::to_string(i));
			BigNum num2(std::to_string(j));
			INFO("i=" + num1.str() + ", j=" + std::to_string(j));

			BigNum result = GeLu::multiply(num1, num2);
			std::string str = result.str();

			CHECK(str == std::to_string(i * j));
		}
	}
}

TEST_CASE("Bignum erase", "BigNum operation") {
	BigNum num("0123456789");
	SECTION("middle") {
		REQUIRE(num.str() == "0123456789");
		num.erase(2, 5);
		REQUIRE(num.str() == "0156789");
	}

	SECTION("begginning") {
		REQUIRE(num.str() == "0123456789");
		num.erase(0, 2);
		REQUIRE(num.str() == "23456789");
	}

	SECTION("end") {
		REQUIRE(num.str() == "0123456789");
		num.erase(8, 10);
		REQUIRE(num.str() == "01234567");
	}

	SECTION("more") {
		REQUIRE(num.str() == "0123456789");
		num.erase(6, 9);
		REQUIRE(num.str() == "0123459");
	}
}
