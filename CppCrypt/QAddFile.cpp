#include "QAddFile.h"

QAddFile::QAddFile(QWidget* parent) : QWidget(parent) {
    hLayout = new QHBoxLayout();

    btnAddFile = new QPushButton(tr("..."));
    lePathToFile = new QLineEdit();

    hLayout->setMargin(0);

    btnAddFile->setMaximumWidth(40);

    lePathToFile->setMinimumWidth(300);
    lePathToFile->setReadOnly(true);

    connect(btnAddFile, &QPushButton::clicked, this, &QAddFile::openFileDialog);
    connect(this, &QAddFile::newFile, lePathToFile, &QLineEdit::setText);

    hLayout->addWidget(lePathToFile);
    hLayout->addWidget(btnAddFile);

    setLayout(hLayout);
}

void QAddFile::openFileDialog() {
    pathToFile = QFileDialog::getOpenFileName(this, "...", "C:\\");
    emit newFile(pathToFile);
}