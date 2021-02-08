#include "Crypto.h"

Crypto::Crypto() {
	gost_28147 = new GOST_28147();
	hashAlgNames.insert("GOST_28147", gost_28147);
}

Crypto::~Crypto() {
}

AbstractBinary* Crypto::getAlgorithm(QString algName)
{
	return hashAlgNames.find(algName).value();
}