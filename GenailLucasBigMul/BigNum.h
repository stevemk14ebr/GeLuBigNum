#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <cassert>

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

**/
class BigNum {
public:
	BigNum() = default;
	BigNum(std::string str, bool isNeg = false);
	
	// zero indexed get char at idx. DOESN'T bounds check
	uint8_t at(const uint32_t idx) const;

	// get stringized number
	std::string str(bool prependSign = true) const;

	// get bit string including control nibble
	std::string bitStr() const;

	bool isNegative() const;

	// how many chars are held (ignores control nibble)
	uint16_t length() const;
	
	// operations
	static BigNum sum(const BigNum& addend, const BigNum& addend2);
	static BigNum sub(const BigNum& minuend, const BigNum& subtrahend);

	// manual insertion
	void set(const uint32_t idx, const uint8_t val);
	void insertCtrlNibble(bool isOdd, bool isNeg);

	// alloc 'charCount' number of chars and fill with '0' 
	void resize(const uint32_t charCount);
private:
	enum flags : uint8_t {
		IS_ODD = 1 << 0,
		IS_NEG = 1 << 1,
	};

	enum flagsIdx : uint8_t {
		IS_ODD_IDX = 0,
		IS_NEG_IDX = 1
	};

	std::vector<uint8_t> m_bits;
};

#define CEIL_DIV(dividend,  divisor) (dividend / divisor + (dividend % divisor != 0))
#define SET_BIT_TO(num, x, idx) (num ^= (-(!!((unsigned long)x)) ^ num) & (1UL << idx))
#define IS_BIT_SET(num, idx) ((bool)((num >> idx) & 1ULL))
#define IS_ODD(num) ((bool)(num % 2))
#define IS_EVEN(num) ((bool)(!IS_ODD(num)))

