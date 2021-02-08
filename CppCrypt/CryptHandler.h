#pragma once
//Chain of Responsibility Pattern
#include <QString>

class CryptHandler
{
public:
	CryptHandler() = default;
	virtual ~CryptHandler();


	virtual void setHandler(CryptHandler*);
	virtual uint8_t* handleRequestDecrypt();
	virtual uint8_t* handleRequestEncrypt();

private:
	CryptHandler* successor;

};

