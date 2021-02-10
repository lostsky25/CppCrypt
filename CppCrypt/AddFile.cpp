#include "AddFile.h"

AddFile::AddFile(QWidget* parent) : QWidget(parent) {
    hLayout = new QHBoxLayout();

    btnAddFile = new QPushButton(tr("..."));
    lePathToFile = new QLineEdit();

    hLayout->setMargin(0);

    btnAddFile->setMaximumWidth(40);

    lePathToFile->setMinimumWidth(300);
    lePathToFile->setReadOnly(true);

    connect(btnAddFile, &QPushButton::clicked, this, &AddFile::openFileDialog);
    connect(this, &AddFile::newFile, lePathToFile, &QLineEdit::setText);

    hLayout->addWidget(lePathToFile);
    hLayout->addWidget(btnAddFile);

    setLayout(hLayout);
}

AddFile::~AddFile() {
    delete hLayout;
    delete btnAddFile;
    delete lePathToFile;
}

void AddFile::openFileDialog() {
    pathToFile = QFileDialog::getOpenFileName(this, "...", "C:\\");
    emit newFile(pathToFile);
}