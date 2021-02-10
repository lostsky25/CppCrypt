#include "AbstractBinary.h"

AbstractBinary::AbstractBinary()
{
}

//AbstractBinary::~AbstractBinary() {
//	//free(buffer);
//}

//void AbstractBinary::setName(QString name) {
//	this->name = name;
//}
//
//QString AbstractBinary::getName() {
//	return name;
//}
//
bool AbstractBinary::setPasswd(QString& passwd) {
	if (!passwd.isEmpty()) {
		key256 = reinterpret_cast<uint8_t*>(passwd.data());
		return true;
	}
	else {
		return false;
	}
}

void AbstractBinary::split_256bits_to_32bits(uint8_t* block256b, uint32_t* blocks32b)
{
	uint8_t* p8 = block256b;
	// p32[0] = 00000000000000000000000000000000
	for (uint32_t* p32 = blocks32b; p32 < blocks32b + 8; ++p32) {
		// 00000000000000000000000000000000 << 8 | 10010010 = 00000000000000000000000010010010
		// 00000000000000000000000010010010 << 8 | 00011110 = 00000000000000001001001000011110
		// 00000000000000001001001000011110 << 8 | 11100011 = 00000000100100100001111011100011
		// 00000000100100100001111011100011 << 8 | 01010101 = 10010010000111101110001101010101
		for (uint8_t i = 0; i < 4; ++i) {
			*p32 = (*p32 << 8) | *(p8 + i);
		}
		p8 += 4;
	}
}

void AbstractBinary::split_64bits_to_32bits(uint64_t block64b, uint32_t* bloks32b)
{
	//                       block64b = 0000111001111011000001100010111001001001010011100100110101001001
	bloks32b++; //                    last index contain right part =>  01001001010011100100110101001001
	*bloks32b = (uint32_t)(block64b);
	bloks32b--;
	//first index contain left part =>  00001110011110110000011000101110
	*bloks32b = (uint32_t)((block64b) >> 32);
}

void AbstractBinary::split_64bits_to_8bits(uint64_t block64b, uint8_t* blocks8b) {
	for (size_t i = 0; i < 8; ++i) {
		// blocks8b[0] = 
		// = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> ((7 - 0) * 8)
		// = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> 56 =
		// = (uint8_t)0000000000000000000000000000000000000000000000000000000000001010 =
		// = 00001010
		blocks8b[i] = (uint8_t)(block64b >> ((7 - i) * 8));
	}
}

//void AbstractBinary::split_64bits_to_8bits(uint64_t block64b, uint8_t* blocks8b)
//{
//	for (uint8_t i = 0; i < 8; i++)
//	{
//		// [ Removing by explicit conversion ]
//		// [iter 0 >> 0]  [ 00001110|01111011|00000110|00101110|01001001|01001110|01001101 ] 01001001
//		// [iter 1 >> 8]  [ 00001110|01111011|00000110|00101110|01001001|01001110 ] 01001101
//		// [iter 2 >> 16] [ 00001110|01111011|00000110|00101110|01001001 ] 01001110
//		// etc.
//
//		blocks8b[7 - i] = (uint8_t)(block64b >> (i * 8));
//	}
//}

void AbstractBinary::split_32bits_to_8bits(uint32_t block32b, uint8_t* blocks8b)
{
	for (uint8_t i = 0; i < 4; ++i) {
		// blocks8b[0] = (uint8_t)10111101000101010100101110100010 >> (28 - (0 * 8)) =
		// = (uint8_t)10101010101010101010101010101010 >> 28 = 
		// = (uint8_t)00000000000000000000000010111101
		// = 10111101
		blocks8b[i] = (uint8_t)(block32b >> (24 - (i * 8)));
	}
}

uint64_t AbstractBinary::join_8bits_to_64bits(uint8_t* blocks8b)
{
	uint64_t block64b;
	// block64b = 0000000000000000000000000000000000000000000000000000000000000000
	for (uint8_t* p = blocks8b; p < blocks8b + 8; ++p) {
		// i = 0
		// (0000000000000000000000000000000000000000000000000000000000000000 << 8) | 11001100 = 
		// 0000000000000000000000000000000000000000000000000000000011001100
		// i = 1
		// (0000000000000000000000000000000000000000000000000000000011001100 << 8) | 11110011 = 
		// 0000000000000000000000000000000000000000000000001100110000000000 | 11110011 = 
		// 0000000000000000000000000000000000000000000000001100110011110011
		// ... i < 8 ...
		block64b = (block64b << 8) | *p;
	}
	return block64b;
}

uint64_t AbstractBinary::join_32bits_to_64bits(uint32_t* blocks32b)
{
	uint64_t block64b;
	// block64b = 10101010101010101010101010101010 = 
	// 0000000000000000000000000000000010101010101010101010101010101010
	block64b = blocks32b[1];
	// block64b = 
	// = (0000000000000000000000000000000010101010101010101010101010101010 << 32) | 11111111111111111111111111111111 = 
	// = 1010101010101010101010101010101000000000000000000000000000000000 | 11111111111111111111111111111111 = 
	// = 101010101010101010101010101010111111111111111111111111111111111
	block64b = (block64b << 32) | blocks32b[0];
	return block64b;
}

uint32_t AbstractBinary::join_4bits_to_32bits(uint8_t* blocks4b)
{
	uint32_t block32b;
	// block64b = 00000000000000000000000000000000
	for (uint8_t i = 0; i < 4; ++i) {
		// i = 0
		// (00000000000000000000000000000000 << 8) | 11001100 = 
		// 00000000000000000000000011001100
		// i = 1
		// (00000000000000000000000011001100 << 8) | 11110011 = 
		// 00000000000000001100110000000000 | 11110011 = 
		// 00000000000000001100110011110011
		// ... i < 4 ...
		block32b = (block32b << 8) | blocks4b[i];
	}
	return block32b;
}
