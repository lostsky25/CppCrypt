#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>

class QAddFile : public QWidget
{
	Q_OBJECT
public:
	QAddFile(QWidget* parent = Q_NULLPTR);

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

