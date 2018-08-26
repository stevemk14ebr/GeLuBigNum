#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <stdint.h>

/**Represents a variable length integer as a continguous array of 4 bit binary integers per each digit. 

Idk if this is a known algo but i'll describe what i've done. Each digits of the string
is represented as 4 bits wich gives each digit the possible values 0-15. This means we can fit up
to 2 digits of any stringized number (hex or dec) within a single byte. Since there is two digits
per byte we need to handle odd length stringized numbers and append another byte at the very end of all the concatenated bytes
to specify whether to ignore that last 4 bits or not as well as the sign (sign is high nibble, ignore nibble is low). With this we can then expect that the memory footprint of this
encoding size in bytes is given by: num_bytes = ceil(string.length / 2) + 1 . We can also index into each digit in it's packed
form efficiently by walking every 4 bits: bit_idx = string_idx*4.**/
class BigNum {
public:
	BigNum(std::string str, bool isNeg = false);
	uint8_t at(const uint32_t idx) const;
	std::string str() const;
	std::string bitStr() const;
	bool isNegative() const;
private:
	template<typename T>
	T ceilDiv(const T dividend, const T divisor) {
		return dividend / divisor + (dividend % divisor != 0);
	}

	std::vector<uint8_t> m_bits;
};