#pragma once
#include <QString>
#include <cstdint>
#include <cctype>

#include "CryptHandler.h"

#define BUFF_SIZE 256

class AbstractBinary : public CryptHandler
{
public:
	AbstractBinary();

	QString getName();

	//void setBuffer(QByteArray&);
	void setPasswd(QByteArray);
	//void setLength(size_t);
	
	//virtual void handleRequestDecrypt() = 0;
	//virtual void handleRequestEncrypt() = 0;


protected:
	

	QString name;

	QByteArray key256;
	//uint8_t encrypted[BUFF_SIZE] = { 0 }, decrypted[BUFF_SIZE] = { 0 };// , buffer[BUFF_SIZE] = { 0 };
	uint8_t ch = 0;

	virtual void split_256bits_to_32bits(uint8_t*, uint32_t*);
	virtual void split_64bits_to_32bits(uint64_t, uint32_t*);
	virtual void split_64bits_to_8bits(uint64_t, uint8_t*);
	virtual void split_32bits_to_8bits(uint32_t, uint8_t*);

	virtual uint64_t join_8bits_to_64bits(uint8_t*);
	virtual uint64_t join_32bits_to_64bits(uint32_t*);
	virtual uint32_t join_4bits_to_32bits(uint8_t*);
};

