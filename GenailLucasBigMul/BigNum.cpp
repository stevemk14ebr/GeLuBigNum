#include "BigNum.h"

/**Attempts are made to avoid branching by using the mathematical
identify of 0*x = 0. This is used for or-ing where the boolean is cast
to its numeric representation and multiplied so that if the condition is
false the or turned into a no-op since 'byte | 0 = byte';

if(cond) 
   byte |= x		====>    byte |= cond*x  

Subtraction is a special case of addition. This is implemented via
the 9's complement technique.
**/
BigNum::BigNum(const std::string str, bool isNeg) {
	assert(str.length() > 0);
	if (str.empty())
		return;

	bool hasNegSign = str[0] == '-';

	// N/2 chars per byte
	m_bits.resize(FAST_CEIL_DIV(str.length() - hasNegSign, 2));
	
	// walk 1 byte per 2 chars, skip neg if exists
	for (int i = hasNegSign; i < (str.length() - 1); i+=2) {
		m_bits[i / 2] |= (str[i] - '0') << 4;
		m_bits[i / 2] |= str[i + 1] - '0';
	}
	
	// if odd num chars then write high nibble of odd byte last char of str
	m_bits.back() |= (str.back() - '0') * IS_ODD((str.length() - hasNegSign)) << 4;

	m_control.isNegative = isNeg || hasNegSign;
	m_control.isOdd = (str.length() - hasNegSign) % 2 != 0;
}

void BigNum::resize(const uint32_t charCount) {
	m_bits.resize(FAST_CEIL_DIV(charCount, 2));
}

uint8_t BigNum::at(const uint32_t idx) const {
	assert(idx < length() && idx >= 0);
	// array indexing get correct byte, shift right 4 if idx char is high nibble, mask nibble to byte width
	return m_bits[idx / 2] >> (4 * ((uint8_t)IS_EVEN(idx))) & 0xF;
}

BigNum::ControlFields BigNum::getControl() const {
	return m_control;
}

BigNum::ControlFields& BigNum::getControl() {
	return m_control;
}

void BigNum::set(const uint32_t idx, const uint8_t val) {
	assert(idx < length() && idx >= 0);

	uint8_t masked = val & 0xF;
	uint8_t orig = m_bits[idx / 2];
	if (IS_EVEN(idx)) {
		m_bits[idx / 2] = (masked << 4) | (orig & 0xF);
	} else {
		m_bits[idx / 2] = (orig & 0xF0) | masked;
	}
}

uint32_t BigNum::length() const {
	// 1 byte is 2 chars
	/*Odd: [ value '1' | empty ] len = 2 - 1 = 1
	  Even: [ value '1' | value '0' ] len = 2 - 0 = 2
	*/
	return m_bits.size() * 2 - m_control.isOdd;
}

std::string BigNum::str(bool prependSign /*= true*/) const {
	std::string str;
	str.resize(this->length());

	// walk 1 byte per 2 chars
	for (int i = 0; i < (str.length() - 1); i += 2) {
		str[i] = (m_bits[i / 2] >> 4 & 0xF) + '0';
		str[i + 1] = (m_bits[i / 2] & 0xF) + '0';
	}

	str.back() |= (((m_bits.back() >> 4) & 0xF) + '0') * m_control.isOdd;
 	//str.erase(0, std::min(str.find_first_not_of('0'), str.size() - 1));

	if (prependSign && m_control.isNegative)
		return "-" + str;
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

void BigNum::append(const uint32_t count, const uint8_t val) {
	uint32_t oldLen = length();
	uint32_t newLen = oldLen + count;

	/* 4 bit nibbles of bytes get changes in xor pattern.
	Think of the last cell [ value '3' | empty] where that is an 'odd' cell.
	If you add another 'odd' cell [ value '1' | empty] the final cell is
	[ '3' | '1' ] making an 'even' cell*/
	bool isOdd = m_control.isOdd ^ (count % 2);
	resize(newLen);
	m_control.isOdd = isOdd;
	for (int i = oldLen; i < newLen; i++) {
		set(i, val);
	}
}

bool BigNum::isNegative() const {
	return m_control.isNegative;
}

void BigNum::erase(const uint32_t first, const uint32_t last) {
	const uint32_t rmCount = last - first;
	const uint32_t len = length();

	// overwrite hole with stuff past last
	int i = 0;
	for (; i < rmCount && (last + i) < len; i++) {
		set(first + i, at(last + i));
	}
	
	// pull tail down to after last
	int dest_of = 0;
	for (int j = last + i; j < len; j++) {
		set(last + dest_of, at(j));
		dest_of++;
	}

	resize(len - rmCount);
	m_control.isOdd = (len - rmCount) % 2;
}

void divmod10(uint32_t in, uint32_t &div, uint32_t &mod)
{
	uint32_t x = (in | 1) - (in >> 2); // div = in/10 <~~> div = (0.75*in) >> 3
	uint32_t q = (x >> 4) + x;
	x = q;
	q = (q >> 8) + x;
	q = (q >> 8) + x;
	q = (q >> 8) + x;
	q = (q >> 8) + x;

	div = (q >> 3);
	mod = in - (((div << 2) + div) << 1);
}