#include "stdafx.h"
#include "inputbox.h"

InputBox::InputBox(QString title, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowTitle(title);

	//Qt::NonModal ：该窗口不是模态，不会阻塞其它界面接受输入信息；
	//Qt::WindowModal：该窗口是一个当以层次的模态窗口，会阻塞它的父窗口、祖父窗口和各个兄弟窗口接受输入信息；
	//Qt::ApplicationModal：该窗口是应用模态窗口，会阻塞所有窗口接受输入信息。

	//qDebug() << (0x1001 &~ 0x0001);
	//qDebug() << (0x1000 | 0x0001);
	setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint);
	//setAttribute(Qt::WA_DeleteOnClose, true);

	exec();
}

InputBox::~InputBox()
{
}

void InputBox::on_inputPushButton_clicked()
{
	m_input = ui.inputLineEdit->text().trimmed();
	emit inputed(m_input);
	
	close();
}

QString InputBox::GetInput()
{
	return m_input;
}