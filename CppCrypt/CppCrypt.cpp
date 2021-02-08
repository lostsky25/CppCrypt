#include "CppCrypt.h"

CppCrypt::CppCrypt(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    //initHashAlgNames();
    
    QPixmap mLogo("m_logo.jpg");

    crypto = new Crypto();
    handler = new CryptHandler();

    outputBuffer = new QByteArray();

    gost_28147 = new GOST_28147();

    vLayout = new QVBoxLayout();
    addFile = new QAddFile();

    cbAlgorithms = new QComboBox();

    btnEncrypt = new QPushButton(tr("Encrypt"));
    btnDecrypt = new QPushButton(tr("Decrypt"));

    lbPix = new QLabel();

    lePasswd = new QLineEdit();

    lbPix->setPixmap(mLogo);

    cbAlgorithms->addItem("GOST_28147");

    //for (QHash<QString, int>::iterator it = hashAlgNames.begin(); it != hashAlgNames.end(); it++)
      //  cbAlgorithms->addItem(it.key());

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    connect(addFile, &QAddFile::newFile, this, &CppCrypt::setFilePath);
    connect(lePasswd, &QLineEdit::textEdited, this, &CppCrypt::setPasswd);
    //connect(btnEncrypt, &QPushButton::clicked, );
    connect(btnEncrypt, &QPushButton::clicked, this, &CppCrypt::crpEncryptHandler);
    connect(btnDecrypt, &QPushButton::clicked, this, &CppCrypt::crpDecryptHandler);

    vLayout->addWidget(lbPix);
    vLayout->addWidget(addFile);
    vLayout->addWidget(lePasswd);
    vLayout->addWidget(cbAlgorithms);
    vLayout->addWidget(btnEncrypt);
    vLayout->addWidget(btnDecrypt);

    centralWidget()->setLayout(vLayout);
}

void CppCrypt::setFilePath(QString pathFile) {
    this->pathFile = pathFile;
}

void CppCrypt::setPasswd(QString passwd) {
    this->passwd = passwd.toUtf8();
}

void CppCrypt::crpEncryptHandler() {
    input.setFileName(QString(pathFile).replace("/", "\\"));
    output.setFileName(QString(pathFile).replace("/", "\\") + ".enc");

    //QDataStream inputStream(&input);

    if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
        QDataStream inputStream(&input);
        QDataStream outputStream(&output);

        inputBuffer = input.readAll();
        outputBuffer->resize(inputBuffer.size());

        //uint8_t* ch_temp = (uint8_t*)malloc(inputBuffer.size() * sizeof(uint8_t));
        //uint8_t ch_tempp[32] = "qwerty";
        //uint8_t* ch_tempp2 = (uint8_t*)malloc(inputBuffer.size() * sizeof(uint8_t));

        /*int i;
        for (i = 0; i < inputBuffer.size(); i++) {
            ch_temp[i] = inputBuffer[i];
        }*/
        
        //temp[i] = '\0';
        
        //for (int i = 0; i < 6; i++) {
        //    ch_tempp[i] = passwd[i];
        //}

        /*
        crypto->getAlgorithm("GOST_28147")->setPasswd(passwd.toUtf8());
        reinterpret_cast<GOST_28147*>(crypto->getAlgorithm("GOST_28147"))->setLength(temp.size());
        reinterpret_cast<GOST_28147*>(crypto->getAlgorithm("GOST_28147"))->setBuffer((uint8_t*)temp.data());
        handler->setHandler(crypto->getAlgorithm("GOST_28147"));
        uint8_t* ch_temp = handler->handleRequestEncrypt();
        */

        //QByteArray key256;
        ////uint8_t encrypted[5000] = { 0 }, decrypted[5000] = { 0 };
        //uint8_t ch = 0;

        gost_28147->algStart(
            reinterpret_cast<uint8_t*>(outputBuffer->data()), 
            'E', 
            reinterpret_cast<uint8_t*>(passwd.data()),
            reinterpret_cast<uint8_t*>(inputBuffer.data()), 
            inputBuffer.size()
        );

        //for (int i = 0; i < inputBuffer.size(); i++) {
        //    inputBuffer[i] = ch_tempp2[i];
        //}

        //outputStream << temp;
        output.write(*outputBuffer);
    }

    output.close();
    input.close();
}

void CppCrypt::crpDecryptHandler()
{
    input.setFileName(QString(pathFile).replace("/", "\\"));
    output.setFileName(QString(pathFile).replace("/", "\\") + ".dec");

    if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
        QDataStream inputStream(&input);
        QDataStream outputStream(&output);

        inputBuffer = input.readAll();
        outputBuffer->resize(inputBuffer.size());

        gost_28147->algStart(
            reinterpret_cast<uint8_t*>(outputBuffer->data()),
            'D',
            reinterpret_cast<uint8_t*>(passwd.data()),
            reinterpret_cast<uint8_t*>(inputBuffer.data()),
            inputBuffer.size()
        );

        output.write(*outputBuffer);
    }

    output.close();
    input.close();
}

//void CppCrypt::initHashAlgNames() {
//    //hashAlgNames.insert("GOST_28147", crypto->gost_28147);
//}