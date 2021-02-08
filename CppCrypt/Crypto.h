#pragma once
#include <QObject>
#include <QByteArray>
#include <QHash>

#include "GOST_28147.h"

class Crypto : public QObject
{
	Q_OBJECT

public:
	Crypto();
	~Crypto();

	AbstractBinary* getAlgorithm(QString);

private:
	GOST_28147* gost_28147;
	QHash<QString, AbstractBinary*> hashAlgNames;

};

