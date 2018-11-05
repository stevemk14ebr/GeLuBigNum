#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>

/**Represents a variable length integer as a continguous array of 4 bit binary integers per each digit. 

Idk if this is a known algo but i'll describe what i've done. Each digits of the string
is represented as 4 bits wich gives each digit the possible values 0-15. This means we can fit up
to 2 digits of any stringized number (hex or dec) within a single byte. Since there is two digits
per byte we need to handle odd length stringized numbers by appending another byte. This byte will hold
the overflow nibble for odd length and the other nibble of the end byte is a bitfield. With this we can then expect that the memory footprint of this
encoding size in bytes is given by: num_bytes = truncate(string.length / 2) + 1 . We can also index into each digit in it's packed
form efficiently by walking every 4 bits: bit_idx = string_idx*4. Get number of chars w/o branching in packed form: char_cnt = byte_cnt - control_byte + (isOdd * 1).

// 2*n chars + 1 if odd len + control byte
-----------------------------------------------------------------
|		byte	|		byte	|        odd	byte			|
-----------------------------------------------------------------
|		|		|		|		|		|			|			|
|char	|char	|char	|char	|  char	|isNeg		|isOddLen	|
|		|		|		|		|		|			|			|
-----------------------------------------------------------------

Addendum:

So apparently this is basically BCD with some control fields. I've updated the encoding to do the control bits out-of-band in a second structure. 
This makes the logic a bit easier to follow with less shifts. It also removes some of the tricky +/- 1 issues that come up, and spoofing the 
sign for subtraction becomes possible as well as efficient append with only a vector resize (usually move?).
**/
class BigNum {
public:
	struct ControlFields {
		bool isNegative;
		bool isOdd;
	};

	BigNum() = default;
	BigNum(std::string str, bool isNeg = false);
	
	// get stringized number
	std::string str(bool prependSign = true) const;

	// get bit string including control nibble
	std::string bitStr() const;
	
	bool isNegative() const;

	// operations
	static inline BigNum sum(const BigNum& addend, const BigNum& addend2) {
			// pass unaltered control fields through
			return sum(addend, addend.getControl(), addend2, addend2.getControl());
	}

	static inline BigNum sub(const BigNum& minuend, const BigNum& subtrahend) {
		ControlFields spoofed;
		spoofed.isOdd = subtrahend.m_control.isOdd;
		spoofed.isNegative = !subtrahend.m_control.isNegative;
		return sum(minuend, minuend.getControl(), subtrahend, spoofed);
	}

	// zero indexed get char at idx. DOESN'T bounds check
	uint8_t at(const uint32_t idx) const;

	// how many chars are held
	uint32_t length() const;

	// manual insertion
	void set(const uint32_t idx, const uint8_t val);

	// alloc 'charCount' number of chars and fill with '0' 
	void resize(const uint32_t charCount);

	// append val, count # of times, NOTE: 'oddness' control field may change
	void append(const uint32_t count, const uint8_t val);

	// get/set control
	ControlFields& getControl();

	// get control
	ControlFields getControl() const;
private:
	// internal overload to avoid copies when spoofing sign
	static inline BigNum sum(const BigNum& addend, const ControlFields& cf1, const BigNum& addend2, const ControlFields& cf2);

	// keep control fields at end for safe future proof extension
	std::vector<uint8_t> m_bits;
	ControlFields m_control;
};

#define CEIL_DIV(dividend,  divisor) (dividend / divisor + (dividend % divisor != 0))
#define SET_BIT_TO(num, x, idx) (num ^= (-(!!((unsigned long)x)) ^ num) & (1UL << idx))
#define IS_BIT_SET(num, idx) ((bool)((num >> idx) & 1ULL))
#define IS_ODD(num) ((bool)(num % 2))
#define IS_EVEN(num) ((bool)(!IS_ODD(num)))
#define FAST_CEIL_DIV(x, y) (1 + ((x - 1) / y)) // (std::ceil( x / (double)y )
#include <limits.h>     /* CHAR_BIT */

#define BIT_MASK(__TYPE__, __ONE_COUNT__) \
    ((__TYPE__) (-((__ONE_COUNT__) != 0))) \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

void divmod10(uint32_t in, uint32_t &div, uint32_t &mod);

inline BigNum BigNum::sum(const BigNum& addend, const ControlFields& cf1, const BigNum& addend2, const ControlFields& cf2) {
	/* Be sure to operate on control fields and not control signals of BigNums here. This overload exists so cf's can be spoofed w/o copies*/

	bool bothNegative = cf1.isNegative && cf2.isNegative;
	bool bothPositive = !cf1.isNegative && !cf2.isNegative;
	bool dontComplement = bothPositive | bothNegative;

	// plus one for the carry which is (possibly) generated if magnitude increases (both pos or neg)
	int addendLen = addend.length();
	int addend2Len = addend2.length();
	int maxLen = std::max<uint32_t>(addendLen, addend2Len) + dontComplement;

	BigNum result;
	result.resize(maxLen);

	// start at right side and walk left with an offset, filling with zeros if needed, add digits. 
	uint8_t carry = 0;
	for (int offset = 0; offset < (maxLen - dontComplement); offset++)
	{
		uint8_t tmpAddend2 = offset >= addend2Len ? 0 : addend2.at(addend2Len - offset - 1);
		uint8_t tmpAddend = offset >= addendLen ? 0 : addend.at(addendLen - offset - 1);

		// 9's complement if negative
		if (cf1.isNegative && !dontComplement)
			tmpAddend = 9 - tmpAddend;

		// 9's complement if negative
		if (cf2.isNegative && !dontComplement)
			tmpAddend2 = 9 - tmpAddend2;

		// if positive do the operation x+y+c=result
		uint16_t tmpResult = carry + tmpAddend + tmpAddend2;

		// profiled as hot spot, do some bit magic
		uint32_t div, mod;
		divmod10(tmpResult, div, mod);

		carry = (uint8_t)div;
		uint8_t val = (uint8_t)mod;

		result.set(maxLen - offset - 1, val);
	}

	// our encoding requires this, we just inserted manually so now we must insert this
	result.m_control.isOdd = maxLen % 2;
	result.m_control.isNegative = (!bothPositive && carry == 0) || bothNegative;

	// add carry digit when summing positives.
	if (dontComplement) {
		result.set(0, carry);
	}
	else if (carry != 0) {
		// Otherwise add carry if exists for 9's complement
		result = sum(result, BigNum("1"));
	}
	else {
		// Otherwise if no carry then take complement and make negative
		for (int i = 0; i < result.length(); i++) {
			result.set(i, 9 - result.at(i));
		}

		result.m_control.isOdd = result.m_control.isOdd;
		result.m_control.isNegative = true;
	}
	return std::move(result);
}

