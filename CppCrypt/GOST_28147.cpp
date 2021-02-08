#include "GOST_28147.h"

//GOST_28147::GOST_28147()
//{
//}
void GOST_28147::setBuffer(uint8_t* bf) {
	buffer = bf;
}

void GOST_28147::setLength(size_t length)
{
	poss = length;
}


uint8_t* GOST_28147::handleRequestDecrypt()
{
	return 0;
}

uint8_t* GOST_28147::handleRequestEncrypt()
{
	return 0;
}

uint8_t* GOST_28147::algStart(uint8_t* to, uint8_t mode, uint8_t* key256b, uint8_t* from, size_t length) {
	length = length % 8 == 0 ? length : length + (8 - (length % 8));
	//encrypted = (uint8_t*)malloc(length * sizeof(uint8_t));
	uint32_t N[2], keys32b[8];

	//Splitting the key into 32 bits
	split_256bits_to_32bits(key256b, keys32b);

	for (size_t i = 0; i < length; i += 8)
	{
		split_64bits_to_32bits(join_8bits_to_64bits(from + i), N);
		feistel_cipher(mode, N, keys32b);
		split_64bits_to_8bits(
			join_32bits_to_64bits(N),
			(to + i)
		);
	}
	//from = to;
	return from;
}

void GOST_28147::round_of_feistel_cipher(uint32_t* blocks32b, uint32_t* keys32b, uint8_t round)
{
	uint32_t result_of_iter, temp;

	//RES = (N1 + Ki) mod 2^32
	result_of_iter = (blocks32b[0] + keys32b[round % 8]) % UINT32_MAX;

	//RES = RES -> Sbox –абота с таблицей замен
	result_of_iter = substitution_table(result_of_iter, round % 8);

	//RES = RES << 11 циклический сдвиг влево
	result_of_iter = (uint32_t)LSHIFT_nBIT(result_of_iter, 11, 32);

	//ќбмен N1 и N2
	temp = blocks32b[0];
	blocks32b[0] = result_of_iter ^ blocks32b[1];
	blocks32b[1] = temp;
}

void GOST_28147::feistel_cipher(uint8_t mode, uint32_t* blocks32b, uint32_t* keys32b)
{
	switch (tolower(mode)) {
	case 'e': {
		for (uint8_t round = 0; round < 24; ++round)
		{
			round_of_feistel_cipher(blocks32b, keys32b, round);
		}
		for (uint8_t round = 31; round >= 24; --round)
		{
			round_of_feistel_cipher(blocks32b, keys32b, round);
		}
		break;
	}
	case 'd': {
		// K0, K1, K2, K3, K4, K5, K6, K7
		for (uint8_t round = 0; round < 8; ++round)
			round_of_feistel_cipher(blocks32b, keys32b, round);

		// K7, K6, K5, K4, K3, K2, K1, K0, K7, K6, K5, K4, K3, K2, K1, K0, K7, K6, K5, K4, K3, K2, K1, K0
		for (uint8_t round = 31; round >= 8; --round)
			round_of_feistel_cipher(blocks32b, keys32b, round);
		break;
	}
	}
}

uint32_t GOST_28147::substitution_table(uint32_t block32b, uint8_t sbox_row)
{
	//32-х битный блок должен разделитьс€ на блоки по 4-е бита

	uint8_t blocks4b[4];

	//–аздел€ем (проход€т через S блок)
	split_32bits_to_8bits(block32b, blocks4b);
	substitution_table_by_4bits(blocks4b, sbox_row);
	return join_4bits_to_32bits(blocks4b);
}

void GOST_28147::substitution_table_by_4bits(uint8_t* blocks4b, uint8_t sbox_row)
{
	uint8_t block4b_1, block4b_2;
	for (uint8_t i = 0; i < 4; ++i)
	{
		//sbox_row - это раунд
		//blocks4b[i] & 0x0F - ??????????????????????
		block4b_1 = Sbox[sbox_row][blocks4b[i] & 0x0F];

		block4b_2 = Sbox[sbox_row][blocks4b[i] >> 4];

		blocks4b[i] = block4b_2;

		blocks4b[i] = (blocks4b[i] << 4) | block4b_1;
	}
}