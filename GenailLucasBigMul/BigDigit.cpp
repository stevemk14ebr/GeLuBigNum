#include "BigNum.h"

BigNum::BigNum(const std::string str, bool isNeg) {
	// create bytes and zero them
	m_bits.resize(ceilDiv<size_t>(str.length(), 2) + 1);

	// walk 4 bits at a time
	for (int i = 0; i < str.length() - 1; i+=2) {
		m_bits[i / 2] |= (str[i] - '0') << 4;
		m_bits[i / 2] |= str[i + 1] - '0';
	}

	if (str.length() % 2 != 0) {
		int lastDigit = str.length() - 1;
		m_bits[lastDigit / 2] = str[lastDigit] << 4;
	}
	m_bits.back() |= ((uint8_t)isNeg) << 4;
	m_bits.back() |= (uint8_t)str.length() % 2 != 0;
}

uint8_t BigNum::at(const uint32_t idx) const {
	bool isOdd = idx % 2 != 0;
	if (isOdd) {
		return m_bits[(idx - 1) / 2] & 0xF ;
	}
	return m_bits[idx / 2] >> 4 & 0xF;
}

bool BigNum::isNegative() const {
	return (m_bits.back() >> 4) & 0xF;
}

std::string BigNum::str() const {
	return "";
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

