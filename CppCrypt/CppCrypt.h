#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CppCrypt.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QHash>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QLineEdit>

#include "Crypto.h"
#include "QAddFile.h"
#include "GOST_28147.h"

#include "CryptHandler.h"

class CppCrypt : public QMainWindow
{
    Q_OBJECT
public:
    CppCrypt(QWidget *parent = Q_NULLPTR);

public slots:
    void setFilePath(QString);
    void setPasswd(QString);

    void crpEncryptHandler();
    void crpDecryptHandler();

private:
    //void initHashAlgNames();

    Ui::CppCryptClass ui;

    QByteArray inputBuffer; 
    QByteArray* outputBuffer;
    QByteArray passwd;

    QString pathFile;

    QFile input;
    QFile output;

    GOST_28147* gost_28147;

    //QHBoxLayout* hLayout;
    QVBoxLayout* vLayout;

    Crypto* crypto;
    CryptHandler* handler;

    QAddFile* addFile;
    QLineEdit* lePasswd;

    QComboBox* cbAlgorithms;
    QLabel* lbPix;

    QPushButton* btnEncrypt;
    QPushButton* btnDecrypt;
};
