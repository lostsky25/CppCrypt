#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CppCrypt.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QHash>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QLineEdit>

#include "AddFile.h"
#include "GOST_28147.h"

class CppCrypt : public QMainWindow
{
    Q_OBJECT
public:
    CppCrypt(QWidget *parent = Q_NULLPTR);
    ~CppCrypt();

public slots:
    void setFilePath(QString);
    void setPasswd(QString);
    void setAlgoName(QString);

    void crpEncryptHandler();
    void crpDecryptHandler();

signals:
    void invalidPasswd();

private:
    //void initHashAlgNames();

    Ui::CppCryptClass ui;

    QByteArray inputBuffer;
    QByteArray* outputBuffer;
    QString passwd;

    QString pathFile;

    QFile input;
    QFile output;

    QHash<QString, AbstractBinary*> algorithms;
    QString currentAlgoName;

    //QHBoxLayout* hLayout;
    QVBoxLayout* vLayout;

    AddFile* addFile;
    QLineEdit* lePasswd;

    QComboBox* cbAlgorithms;
    QLabel* lbPix;

    QPushButton* btnEncrypt;
    QPushButton* btnDecrypt;
};
