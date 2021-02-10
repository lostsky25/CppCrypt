#include "CppCrypt.h"

CppCrypt::CppCrypt(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    //initHashAlgNames();
    
    QPixmap mLogo("m_logo.jpg");

    outputBuffer = new QByteArray();

    vLayout = new QVBoxLayout();
    addFile = new AddFile();

    cbAlgorithms = new QComboBox();

    btnEncrypt = new QPushButton(tr("Encrypt"));
    btnDecrypt = new QPushButton(tr("Decrypt"));

    lbPix = new QLabel();

    lePasswd = new QLineEdit();

    lbPix->setPixmap(mLogo);

    cbAlgorithms->addItem("GOST_28147");
    cbAlgorithms->addItem("GOST_281472");
    cbAlgorithms->setCurrentIndex(-1);

    algorithms.insert("GOST_28147", new GOST_28147());

    vLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

    connect(addFile, &AddFile::newFile, this, &CppCrypt::setFilePath);
    connect(lePasswd, &QLineEdit::textEdited, this, &CppCrypt::setPasswd);
    connect(cbAlgorithms, &QComboBox::currentTextChanged, this, &CppCrypt::setAlgoName);
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

CppCrypt::~CppCrypt() {
    qDeleteAll(algorithms);
    
    delete vLayout;
    delete addFile;
    delete cbAlgorithms;
    delete btnEncrypt;
    delete btnDecrypt;
    delete lbPix;
    delete lePasswd;
}

void CppCrypt::setAlgoName(QString currentAlgoName) {
    this->currentAlgoName = currentAlgoName;
}

void CppCrypt::setFilePath(QString pathFile) {
    this->pathFile = pathFile;
}

void CppCrypt::setPasswd(QString passwd) {
    this->passwd = passwd.toUtf8();
}

void CppCrypt::crpEncryptHandler() {
    AbstractBinary* ab = Q_NULLPTR;
    input.setFileName(QString(pathFile).replace("/", "\\"));
    output.setFileName(QString(pathFile).replace("/", "\\") + ".enc");

    //QDataStream inputStream(&input);

    if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
        QDataStream inputStream(&input);
        QDataStream outputStream(&output);

        inputBuffer = input.readAll();
        outputBuffer->resize(inputBuffer.size());
        
        if ((ab = algorithms.find(currentAlgoName).value()) != Q_NULLPTR) {
            if (ab->setPasswd(passwd)) {
                ab->algStart(
                    reinterpret_cast<uint8_t*>(outputBuffer->data()),
                    'E',
                    reinterpret_cast<uint8_t*>(inputBuffer.data()),
                    inputBuffer.size()
                );
            }
            else {
                QMessageBox::warning(
                    this,
                    tr("CppCrypt Warning"), 
                    tr("Invalid password"),
                    QMessageBox::Cancel
                    );
            }
        }

        output.write(*outputBuffer);
    }

    //outputBuffer->clear();
    output.close();
    input.close();
}

void CppCrypt::crpDecryptHandler() {
    AbstractBinary* ab = Q_NULLPTR;
    input.setFileName(QString(pathFile).replace("/", "\\"));
    output.setFileName(QString(pathFile).replace("/", "\\") + ".dec");

    if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
        QDataStream inputStream(&input);
        QDataStream outputStream(&output);

        inputBuffer = input.readAll();
        outputBuffer->resize(inputBuffer.size());

        if ((ab = algorithms.find(currentAlgoName).value()) != Q_NULLPTR) {
            if (ab->setPasswd(passwd)) {
                ab->algStart(
                    reinterpret_cast<uint8_t*>(outputBuffer->data()),
                    'D',
                    reinterpret_cast<uint8_t*>(inputBuffer.data()),
                    inputBuffer.size()
                );
            }
            else {
                QMessageBox::warning(
                    this,
                    tr("CppCrypt Warning"),
                    tr("Invalid password"),
                    QMessageBox::Cancel
                );
            }
        }

        output.write(*outputBuffer);
    }

    //outputBuffer->clear();
    output.close();
    input.close();
}

//void CppCrypt::initHashAlgNames() {
//    //hashAlgNames.insert("GOST_28147", crypto->gost_28147);
//}