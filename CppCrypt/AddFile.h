#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>

class AddFile : public QWidget
{
	Q_OBJECT
public:
	AddFile(QWidget* parent = Q_NULLPTR);
	~AddFile();

public slots:
	void openFileDialog();

signals:
	void newFile(QString);

private:
	QString pathToFile;
	QHBoxLayout* hLayout;
	QLineEdit* lePathToFile;
	QPushButton* btnAddFile;
};

