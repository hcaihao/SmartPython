#pragma once

#include <QWidget>
#include "ui_inputbox.h"

class InputBox : public QDialog
{
	Q_OBJECT

public:
	InputBox(QString title, QWidget *parent = Q_NULLPTR);
	~InputBox();

	QString GetInput();
private:
	Ui::InputBox ui;
	QString m_input;
signals:
	void inputed(QString input);
private slots:
	void on_inputPushButton_clicked();
};
