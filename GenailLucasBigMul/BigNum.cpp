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
	m_bits.resize(std::ceil((str.length() - hasNegSign) / 2.0));
	
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
	m_bits.resize(std::ceil(charCount / 2.0));
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

uint16_t BigNum::length() const {
	// 1 byte is 2 chars
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
 	str.erase(0, std::min(str.find_first_not_of('0'), str.size() - 1));

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

BigNum BigNum::sum(const BigNum& addend, const BigNum& addend2)
{
	// pass unaltered control fields through
	return sum(addend, addend.getControl(), addend2, addend2.getControl());
}

BigNum BigNum::sum(const BigNum& addend, const ControlFields& cf1, const BigNum& addend2, const ControlFields& cf2) {
	/* Be sure to operate on control fields and not control signals of BigNums here. This overload exists so cf's can be spoofed w/o copies*/

	bool bothNegative = cf1.isNegative && cf2.isNegative;
	bool bothPositive = !cf1.isNegative && !cf2.isNegative;
	bool dontComplement = bothPositive || bothNegative;

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
		carry = tmpResult / 10;
		uint8_t val = tmpResult % 10;

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
	return result;
}

bool BigNum::isNegative() const {
	return m_control.isNegative;
}

BigNum BigNum::sub(const BigNum& minuend, const BigNum& subtrahend) {
	ControlFields spoofed;
	spoofed.isOdd = subtrahend.m_control.isOdd;
	spoofed.isNegative = !subtrahend.m_control.isNegative;
	return sum(minuend, minuend.getControl(), subtrahend, spoofed);
}
