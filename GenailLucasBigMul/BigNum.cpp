#include "BigNum.h"

/**Attempts are made to avoid branching by using the mathematical
identify of 0*x = 0. This is used for or-ing where the boolean is cast
to its numeric representation and multiplied so that if the condition is
false the or turned into a no-op since 'byte | 0 = byte';

if(cond) 
   byte |= x		====>    byte |= cond*x  
**/
BigNum::BigNum(const std::string str, bool isNeg) {
	assert(str.length() > 0);

	bool hasNegSign = str[0] == '-';

	// N/2 chars per byte + 1 char nibble/control nibble
	m_bits.resize((str.length() - hasNegSign) / 2 + 1);
	
	// walk 1 byte per 2 chars, skip neg if exists
	for (int i = hasNegSign; i < (str.length() - 1); i+=2) {
		m_bits[i / 2] |= (str[i] - '0') << 4;
		m_bits[i / 2] |= str[i + 1] - '0';
	}
	
	// if odd num chars then write high nibble of odd byte last char of str
	m_bits.back() |= (str.back() - '0') * IS_ODD((str.length() - hasNegSign)) << 4;
	SET_BIT_TO(m_bits.back(), isNeg || hasNegSign, flagsIdx::IS_NEG_IDX);
	SET_BIT_TO(m_bits.back(), (uint8_t)(str.length() - hasNegSign) % 2 != 0, flagsIdx::IS_ODD_IDX);
}

void BigNum::resize(const uint32_t charCount) {
	m_bits.resize(charCount / 2 + 1);
}

uint8_t BigNum::at(const uint32_t idx) const {
	assert(idx < length() && idx >= 0);
	// array indexing get correct byte, shift right 4 if idx char is high nibble, mask nibble to byte width
	return m_bits[idx / 2] >> (4 * ((uint8_t)IS_EVEN(idx))) & 0xF;
}

void BigNum::set(const uint32_t idx, const uint8_t val) {
	m_bits[idx / 2] |= ((val & 0xF) * IS_EVEN(idx)) << 4;
	m_bits[idx / 2] |= (val & 0xF) * IS_ODD(idx);
}

uint16_t BigNum::length() const {
	// 1 byte is 2 chars minus control byte, plus a char if using last nibble for odd len char
	return std::max<uint32_t>((m_bits.size() - 1) * 2,0) + (m_bits.back() & flags::IS_ODD);
}

bool BigNum::isNegative() const {
	return m_bits.back() & flags::IS_NEG;
}

std::string BigNum::str() const {
	std::string str;
	str.resize(this->length());

	// walk 1 byte per 2 chars
	for (int i = 0; i < (str.length() - 1) && !str.empty(); i += 2) {
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

void BigNum::insertCtrlNibble(bool isOdd, bool isNeg) {
	SET_BIT_TO(m_bits.back(), isNeg, flagsIdx::IS_NEG_IDX);
	SET_BIT_TO(m_bits.back(), isOdd, flagsIdx::IS_ODD_IDX);
}

BigNum BigNum::sum(const BigNum& addend, const BigNum& addend2)
{
	bool dontComplement = !addend.isNegative() && !addend2.isNegative();

	// plus one for the carry
	int maxLen = std::max<uint32_t>(addend.length(), addend2.length()) + (1*dontComplement);
	int addendLen = addend.length();
	int addend2Len = addend2.length();

	BigNum result;
	result.resize(maxLen);

	// start at right side and walk left with an offset, filling with zeros if needed, add digits
	uint8_t carry = 0;
	for (int offset = 0; offset < maxLen; offset++)
	{
		uint8_t tmpAddend2 = offset >= addend2Len ? 0 : addend2.at(addend2Len - offset - 1);
		uint8_t tmpAddend = offset >= addendLen ? 0 : addend.at(addendLen - offset - 1);
		
		if (addend.isNegative())
			tmpAddend = 9 - tmpAddend;

		if (addend2.isNegative())
			tmpAddend2 = 9 - tmpAddend2;

		// if positive do the operation x+y=result, otherwise do 10's complement subtraction on whichever is negative
		uint16_t tmpResult = carry + tmpAddend + tmpAddend2;
		carry = tmpResult / 10;
	    uint8_t val = tmpResult % 10;

		result.set(maxLen - offset - 1, val);
	}

	// our encoding requires this, we just inserted manually so now we must insert this
	result.insertCtrlNibble(maxLen % 2, !dontComplement && carry == 0);

	// add carry digit for summing positives. Otherwise add 1 for 9's complement
	if (dontComplement)
		result.set(0, carry);
	else if(carry != 0)
		result = sum(result, BigNum("1"));

	return result;
}
