#pragma once

#include <QtWidgets/QWidget>
#include "ui_smartpython.h"

#include <QtConcurrent>

class SmartPython : public QWidget
{
	Q_OBJECT

public:
	SmartPython(QWidget *parent = Q_NULLPTR);
	~SmartPython();
private:
	Ui::SmartPythonClass ui;

	void InitControl();
	void InitPython();
	void UnInitPython();

private slots:
	void on_saveScriptPushButton_clicked();
	void on_runScriptPushButton_clicked();
	void on_stopScriptPushButton_clicked();
	void on_compileScriptPushButton_clicked();
};
