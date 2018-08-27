#include "BigNum.h"
#define CEIL_DIV(dividend,  divisor) (dividend / divisor + (dividend % divisor != 0))
#define SET_BIT_TO(num, x, idx) (num = num & ~(1ULL << idx) | (x << idx))
#define IS_BIT_SET(num, idx) ((bool)((num >> idx) & 1ULL))
#define IS_ODD(num) ((bool)(num % 2))
#define IS_EVEN(num) ((bool)(!IS_ODD(num)))

BigNum::BigNum(const std::string str, bool isNeg) {
	// create bytes and zero them
	m_bits.resize(str.length() / 2 + 1);

	// walk 1 byte per 2 chars
	for (int i = 0; i < str.length() - 1; i+=2) {
		m_bits[i / 2] |= (str[i] - '0') << 4;
		m_bits[i / 2] |= str[i + 1] - '0';
	}
	
	// if odd num chars then write high nibble of odd byte last char of str
	m_bits.back() |= (str.back() - '0') * IS_ODD(str.length()) << 4;
	SET_BIT_TO(m_bits.back(), isNeg, flagsIdx::IS_NEG_IDX);
	SET_BIT_TO(m_bits.back(), (uint8_t)str.length() % 2 != 0, flagsIdx::IS_ODD_IDX);
}

uint8_t BigNum::at(const uint32_t idx) const {
	// array indexing get correct byte, shift right 4 if idx char is high nibble, mask nibble to byte width
	return m_bits[idx / 2] >> (4 * ((uint8_t)IS_EVEN(idx))) & 0xF;
}

uint16_t BigNum::length() const {
	// 1 byte is 2 chars minus control byte, plus a char if using last nibble for odd len char
	return (m_bits.size() - 1) * 2 + (m_bits.back() & flags::IS_ODD);
}

bool BigNum::isNegative() const {
	return m_bits.back() & flags::IS_NEG;
}

std::string BigNum::str() const {
	std::string str;
	str.resize(this->length());

	// walk 1 byte per 2 chars
	for (int i = 0; i < str.length() - 1; i += 2) {
		str[i] = (m_bits[i / 2] >> 4 & 0xF) + '0';
		str[i + 1] = (m_bits[i / 2] & 0xF) + '0';
	}
	str.back() |= (((m_bits.back() >> 4) & 0xF) + '0') * IS_ODD(str.length());
	return str;
}

std::string BigNum::bitStr() const {
	bool append = false;
	std::stringstream ss;
	for (const uint8_t& byt : m_bits) {
		if (append) {
			ss << " ";
		}
		ss << std::bitset<8>(byt);
		append = true;
	}
	return ss.str();
}

