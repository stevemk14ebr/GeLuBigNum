#pragma once
#include <array>
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <cassert>

namespace GeLu {
	typedef std::array<int8_t, 45> GeLuLut;


#if defined(_DEBUG) && defined(SHOW_DBG)
#define Dbg(x) x
#else
#define Dbg(x)
#endif

	class Ruler
	{
	public:
		Ruler(const GeLuLut& rhs, const GeLuLut& triangles);

		/*
		The start of each region gives the right digit of a multiply, this
		returns the index into the m_RhsLut
		*/
		static uint8_t getRegionStart(const uint8_t region);

		/*
		Get the left digit of a multiply (carry), this expects the index of the
		rhs number you want to get the carry of. It returns an index like this:
		$numeric_number = (ruler_index - 1).rhs[getCarry(rhsIdx)]
		*/
		uint8_t getCarry(const int8_t rhsIdx) const;
	
		GeLuLut rhs;
		GeLuLut triangles;
	};

	namespace Lut {
		std::array<Ruler, 11> rulers = {
			Ruler({0, 0,1, 0,1,2, 0,1,2,3, 0,1,2,3,4, 0,1,2,3,4,5, 0,1,2,3,4,5,6, 0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,8},{0}), //<- index ruler. The second rows are the delta between the column start and the triangle tip
			Ruler({0, 0,1, 0,1,2, 0,1,2,3, 0,1,2,3,4, 0,1,2,3,4,5, 0,1,2,3,4,5,6, 0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,8},{0,  0,1,  0,1,2,     0,1,2,3,     0,1,2,3,4,      0,1,2,3,4,5,       0,1,2,3,4,5,6,        0,1,2,3,4,5,6,7,         0,1,2,3,4,5,6,7,8}), // 0
			Ruler({1, 2,3, 3,4,5, 4,5,6,7, 5,6,7,8,9, 6,7,8,9,0,1, 7,8,9,0,1,2,3, 8,9,0,1,2,3,4,5, 9,0,1,2,3,4,5,6,7},{0,  0,1,  0,1,2,     0,1,2,3,     0,1,2,3,4,      0,1,2,3,3,4,       0,1,2,2,3,4,5,        0,1,1,2,3,4,5,6,         0,0,1,2,3,4,5,6,7}), // 1
			Ruler({2 ,4,5, 6,7,8, 8,9,0,1, 0,1,2,3,4, 2,3,4,5,6,7, 4,5,6,7,8,9,0, 6,7,8,9,0,1,2,3, 8,9,0,1,2,3,4,5,6},{0,  0,1,  0,1,2,     0,1,1,2,    -1,0,1,2,3,     -1,0,1,2,3,4,      -1,0,1,2,3,4,4,       -1,0,1,2,2,3,4,5,        -1,0,0,1,2,3,4,5,6}), // 2
			Ruler({3, 6,7, 9,0,1, 2,3,4,5, 5,6,7,8,9, 8,9,0,1,2,3, 1,2,3,4,5,6,7, 4,5,6,7,8,9,0,1, 7,8,9,0,1,2,3,4,5},{0,  0,1,  0,0,1,    -1,0,1,2,    -1,0,1,2,3,     -1,0,0,1,2,3,      -2,-1,0,1,2,3,4,      -2,-1,0,1,2,3,3,4,       -2,-1,0,0,1,2,3,4,5}), // 3
			Ruler({4, 8,9, 2,3,4, 6,7,8,9, 0,1,2,3,4, 4,5,6,7,8,9, 8,9,0,1,2,3,4, 2,3,4,5,6,7,8,9, 6,7,8,9,0,1,2,3,4},{0,  0,1, -1,0,1,    -1,0,1,2,    -2,-1,0,1,2,    -2,-1,0,1,2,3,     -2,-1,-1,0,1,2,3,     -3,-2,-1,0,1,2,3,4,      -3,-2,-1,0,0,1,2,3,4}), // 4
			Ruler({5, 0,1, 5,6,7, 0,1,2,3, 5,6,7,8,9, 0,1,2,3,4,5, 5,6,7,8,9,0,1, 0,1,2,3,4,5,6,7, 5,6,7,8,9,0,1,2,3},{0, -1,0, -1,0,1,    -2,-1,0,1,   -2,-1,0,1,2,    -3,-2,-1,0,1,2,    -3,-2,-1,0,1,1,2,     -4,-3,-2,-1,0,1,2,3,     -4,-3,-2,-1,0,0,1,2,3}), // 5
			Ruler({6, 2,3, 8,9,0, 4,5,6,7, 0,1,2,3,4, 6,7,8,9,0,1, 2,3,4,5,6,7,8, 8,9,0,1,2,3,4,5, 4,5,6,7,8,9,0,1,2},{0, -1,0, -1,0,0,    -2,-1,0,1,   -3,-2,-1,0,1,   -3,-2,-1,0,0,1,    -4,-3,-2,-1,0,1,2,    -4,-3,-3,-2,-1,0,1,2,    -5,-4,-3,-2,-1,0,0,1,2}), // 6
			Ruler({7, 4,5, 1,2,3, 8,9,0,1, 5,6,7,8,9, 2,3,4,5,6,7, 9,0,1,2,3,4,5, 6,7,8,9,0,1,2,3, 3,4,5,6,7,8,9,0,1},{0, -1,0, -2,-1,0,   -2,-1,-1,0,  -3,-2,-1,0,1,   -4,-3,-2,-1,0,1,   -4,-4,-3,-2,-1,0,1,   -5,-4,-3,-2,-2,-1,0,1,   -6,-5,-4,-3,-2,-1,0,0,1}), // 7
			Ruler({8, 6,7, 4,5,6, 2,3,4,5, 0,1,2,3,4, 8,9,0,1,2,3, 6,7,8,9,0,1,2, 4,5,6,7,8,9,0,1, 2,3,4,5,6,7,8,9,0},{0, -1,0, -2,-1,0,   -3,-2,-1,0,  -4,-3,-2,-1,0,  -4,-3,-3,-2,-1,0,  -5,-4,-3,-2,-2,-1,0,  -6,-5,-4,-3,-2,-1,-1,0,  -7,-6,-5,-4,-3,-2,-1,0,0}), // 8
			Ruler({9, 8,9, 7,8,9, 6,7,8,9, 5,6,7,8,9, 4,5,6,7,8,9, 3,4,5,6,7,8,9, 2,3,4,5,6,7,8,9, 1,2,3,4,5,6,7,8,9},{0, -1,0, -2,-1,0,   -3,-2,-1,0,  -4,-3,-2,-1,0,  -5,-4,-3,-2,-1,0,  -6,-5,-4,-3,-2,-1,0,  -7,-6,-5,-4,-3,-2,-1,0,  -8,-7,-6,-5,-4,-3,-2,-1,0}) // 9
		};
	}
	
	std::string sum(const std::string& addend, const std::string& addend2)
	{
		std::string result;
		result.reserve(addend.length() + 1);

		const std::string* const smaller = addend2.length() < addend.length() ? &addend2 : &addend;
		const std::string* const larger = addend2.length() < addend.length() ? &addend : &addend2;
		
		// loop both numbers in reverse, add digit-wise, carry through
		uint8_t carry = 0;
		for (int idx1 = larger->length() - 1, idx2 = smaller->length() - 1; idx1 >= 0; idx1--, idx2--)
		{
			int tmpAddend2 = idx2 >= smaller->length() ? '0' : smaller->at(idx2);
			int tmpResult = larger->at(idx1) + tmpAddend2 + carry - '0' - '0'; // single '0' subtraction gives correct char representation of result
			carry = tmpResult / 10; // minus '0' to get us to algebraic result
			result.insert(0, std::string(1, tmpResult % 10 + '0'));
		}
		result.insert(0, std::string(carry > 0, carry + '0'));
		return result;
	}

	std::string summation(const std::vector<std::string>& terms)
	{
		assert(terms.size() > 0);
		return std::accumulate(std::next(terms.begin()), terms.end(),
			terms[0], // start with first element
			[](std::string lhs, std::string rhs) {
			return sum(lhs, rhs);
		});
	}

	// N digit by 1 digit
	std::string multiply_impl(const char multiplier, const std::string& multiplicand)
	{
		// result is at most one addition character for a carry
		std::string result = "";
		result.reserve(multiplicand.length() + 1);

		uint8_t regionIdx = Ruler::getRegionStart(multiplier - 0x30);
		
		// special case, not in lut
		if (multiplier - '0' == 0) {
			Dbg(std::cout << multiplier << " * " << multiplicand << " = 0" << std::endl);
			return "0";
		}

		// walk the lut right to left
		for (int i = multiplicand.length() - 1; i >= 0; i--)
		{
			const uint8_t rulerIdx = multiplicand.at(i) + 1 - '0';
			const Ruler& ruler = Lut::rulers.at(rulerIdx);

			// prepend the next digit
			result.insert(0, std::string(1, ruler.rhs.at(regionIdx) + '0'));

			Dbg(std::cout << multiplier << " push " << (char)(ruler.rhs.at(regionIdx) + '0') << " = " << result << std::endl);

			// follow the triangle to the next region on sibling ruler
			regionIdx = ruler.getCarry(regionIdx);
		}
		
		// prepend the carry from the index ruler if its not 0
		char carry = Lut::rulers.at(0).rhs.at(regionIdx);
		result.insert(0, std::string(carry > 0, carry + '0'));
		Dbg(std::cout << "prepend index" << (char)(carry + '0') << std::endl);
		return result;
	}

	// N digit by N digit
	std::string multiply(const std::string& multiplier, const std::string& multiplicand)
	{
		/*Terms:						    				 [addend] [addend]
																||		||
		Ex: 45 * 23 = (4*23)*10^1 + (5*23)*10^0 = 92'0' + 115 = 920 + 115 = 1035
		*/
		std::vector<std::string> addends;		
		addends.reserve(multiplier.length());

		// each digit of multiplier times entire multicand (1 by N), then sum those results
		const uint32_t multiplierLength = multiplier.length();
		for (uint32_t place = 0 ; place < multiplierLength; place++)
		{
			std::string gelu = multiply_impl(multiplier.at(place), multiplicand);
			
			gelu.append(multiplierLength - (place + 1), '0'); // * std::pow(10, place_weight)

			Dbg(std::cout << gelu << std::endl);

			addends.push_back(gelu);
		}
		Dbg(std::cout << std::endl);
		return summation(addends);
	}
	 
	Ruler::Ruler(const GeLuLut& rhs, const GeLuLut& triangles)
	{
		this->rhs = rhs;
		this->triangles = triangles;
	}

	uint8_t Ruler::getRegionStart(const uint8_t region)
	{
		// regions grow by nth triangle, eq: n(n+1)/2 with n=region-1;
		return (region*region - region) / 2;
	}

	uint8_t Ruler::getCarry(const int8_t rhsIdx) const
	{
		return rhsIdx - triangles.at(rhsIdx);
	}
} // end namespace GeLuMul
