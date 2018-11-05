#pragma once
#include <array>
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <cassert>

#include "BigNum.h"

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
	
	BigNum summation(const std::vector<BigNum>& terms)
	{
		assert(terms.size() > 0);
		return std::accumulate(std::next(terms.begin()), terms.end(),
			terms[0], // start with first element
			[](const BigNum& lhs, const BigNum& rhs) {
				return BigNum::sum(lhs, rhs);
		});
	}

	// N digit by 1 digit (0-9)
	BigNum multiply_impl(const uint8_t multiplier, const BigNum& multiplicand)
	{
		assert(multiplier < 10);

		// result is at most one addition character for a carry
		BigNum result;
		result.resize(multiplicand.length() + 1);

		/*Our result will always use the odd digit cell. This cell exists for all encoding
		with more than one char. A multiply is a minimum 1 digit by 1 digit. The max value
		for the min length is 9*9 = 81 which means we always need at min two chars to handle 
		the carry. Meaning we always need two chars min and thus always have the odd byte cell
		and so must fill it first since this goes right to left.*/
		result.getControl().isNegative = multiplicand.isNegative();
		result.getControl().isOdd = false;

		uint8_t regionIdx = Ruler::getRegionStart(multiplier);
		
		// special case, not in lut
		if (multiplier == 0) {
			Dbg(std::cout << (int)multiplier << " * " << multiplicand.str() << " = 0" << std::endl);
			return BigNum("0");
		}

		uint32_t maxLen = multiplicand.length();
		int offset = 0;
		for (; offset < maxLen; offset++) {
			const uint8_t rulerIdx = multiplicand.at(multiplicand.length() - offset - 1) + 1;
			const Ruler& ruler = Lut::rulers.at(rulerIdx);

			// insert digits right to left
			result.set(maxLen - offset, ruler.rhs.at(regionIdx));

			Dbg(std::cout << (int)multiplier << " push " << (int)ruler.rhs.at(regionIdx) << " = " << result.str() << std::endl);

			// follow the triangle to the next region on sibling ruler
			regionIdx = ruler.getCarry(regionIdx);
		}

		uint8_t carry = Lut::rulers.at(0).rhs.at(regionIdx);
		result.getControl().isOdd = (maxLen + 1) % 2;

		result.set(0, carry);
		
		Dbg(std::cout << "push carry " << (int)carry << std::endl);
		return result;
	}

	// N digit by N digit
	BigNum multiply(const BigNum& multiplier, const BigNum& multiplicand)
	{
		/*Terms:						    				 [addend] [addend]
																||		||
		Ex: 45 * 23 = (4*23)*10^1 + (5*23)*10^0 = 92'0' + 115 = 920 + 115 = 1035
		*/
		BigNum sum("0");

		// each digit of multiplier times entire multicand (1 by N), then sum those results
		const uint32_t multiplierLength = multiplier.length();
		for (uint32_t place = 0 ; place < multiplierLength; place++)
		{
			BigNum gelu = multiply_impl(multiplier.at(place), multiplicand);
			Dbg(std::cout << gelu.str() << std::endl);
	
			gelu.append(multiplierLength - (place + 1), 0);
			sum = BigNum::sum(sum, gelu);
		}
		return sum;
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
