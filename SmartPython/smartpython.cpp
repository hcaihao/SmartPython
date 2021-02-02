#include "stdafx.h"
#include "smartpython.h"
#include "highlighter.h"
#include "inputbox.h"

#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/iostream.h>

//#define TYTI_PYLOGHOOK_USE_BOOST
#include "PyLogHook.h"

std::string g_script;
bool g_isAbort = false;

SmartPython* g_pSmartPython = NULL;
QPlainTextEdit* g_pLogPlainTextEdit = NULL;
QPlainTextEdit* g_pScriptPlainTextEdit = NULL;

void ShowLog(std::string text)
{
	//QMetaObject::invokeMethod(g_pLogPlainTextEdit, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, QStringLiteral("[%1] %2").arg(QDateTime::currentDateTime().toString(DATE_TIME_FORMAT)).arg(QString::fromStdString(text))));

	QMetaObject::invokeMethod(g_pSmartPython, [=]() {
		g_pLogPlainTextEdit->moveCursor(QTextCursor::End);
		g_pLogPlainTextEdit->insertPlainText(text.c_str());
		g_pLogPlainTextEdit->moveCursor(QTextCursor::End);
	}, Qt::BlockingQueuedConnection);

	//QTimer::singleShot(0, g_pSmartPython, [=] {
	//	g_pLogPlainTextEdit->moveCursor(QTextCursor::End);
	//	g_pLogPlainTextEdit->insertPlainText(text.c_str());
	//	g_pLogPlainTextEdit->moveCursor(QTextCursor::End);
	//});
}

void ShowLine(int line)
{
	QMetaObject::invokeMethod(g_pSmartPython, [=]() {
		QTextCursor tc = g_pScriptPlainTextEdit->textCursor();
		int position = g_pScriptPlainTextEdit->document()->findBlockByNumber(line - 1).position();
		tc.setPosition(position, QTextCursor::MoveAnchor);
		g_pScriptPlainTextEdit->setTextCursor(tc);


		QList<QTextEdit::ExtraSelection> extraSelections;
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(220, 220, 220);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = g_pScriptPlainTextEdit->textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);

		g_pScriptPlainTextEdit->setExtraSelections(extraSelections);
	}, Qt::BlockingQueuedConnection);


	//QTimer::singleShot(0, g_pSmartPython, [=] {
	//	QTextCursor tc = g_pScriptPlainTextEdit->textCursor();
	//	int position = g_pScriptPlainTextEdit->document()->findBlockByNumber(line - 1).position();
	//	tc.setPosition(position, QTextCursor::MoveAnchor);
	//	g_pScriptPlainTextEdit->setTextCursor(tc);


	//	QList<QTextEdit::ExtraSelection> extraSelections;
	//	QTextEdit::ExtraSelection selection;

	//	QColor lineColor = QColor(220, 220, 220);

	//	selection.format.setBackground(lineColor);
	//	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	//	selection.cursor = g_pScriptPlainTextEdit->textCursor();
	//	selection.cursor.clearSelection();
	//	extraSelections.append(selection);

	//	g_pScriptPlainTextEdit->setExtraSelections(extraSelections);
	//});
}

int MyTrace(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
	if (g_isAbort)
	{
		qDebug() << "User abort.";
		PyErr_SetString(PyExc_KeyboardInterrupt, "User abort.");
		g_isAbort = false;
		return 0;
	}

	if (what == PyTrace_LINE)
	{
		char const* fileName = _PyUnicode_AsString(frame->f_code->co_filename);
		char const* name = _PyUnicode_AsString(frame->f_code->co_name);

		if (strcmp(fileName, "<string>") == 0 && strcmp(name, "__new__") != 0)
		{
			int line = PyFrame_GetLineNumber(frame);
			ShowLine(line);

			qDebug() << "filename" << fileName << "name" << name << "line" << line << "frame" << frame << "f_back" << frame->f_back;
		}
	}

	return 0;
}

std::string MyInput()
{
	//// invoke on the main thread
	//QMetaObject::invokeMethod(qApp, [] { ... });

	//// invoke on an object's thread
	//QMetaObject::invokeMethod(obj, [] { ... });

	//// invoke on a particular thread
	//QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(thread), [] { ... });

	qDebug() << __FUNCTION__ << QThread::currentThreadId() << QThread::currentThread();

	//Qt 5.10 +
	QString result;
	QMetaObject::invokeMethod(g_pSmartPython, [&](){
		InputBox* inputBox = new InputBox(QStringLiteral("输入"));
		result = inputBox->GetInput();
		delete inputBox;
	}, Qt::BlockingQueuedConnection);

	if (result.isEmpty())
	{
		result = "\n";
	}

	return result.toStdString().c_str();
}

SmartPython::SmartPython(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitControl();

	InitPython();
}

SmartPython::~SmartPython()
{
	UnInitPython();
}

void SmartPython::InitControl()
{
	g_pSmartPython = this;
	g_pLogPlainTextEdit = ui.logPlainTextEdit;
	g_pScriptPlainTextEdit = ui.scriptPlainTextEdit;

	ui.logPlainTextEdit->setMaximumBlockCount(1000);

	ui.mainSplitter->setStretchFactor(0, 2);
	ui.mainSplitter->setStretchFactor(1, 1);

	QFont font;
	font.setFamily("Courier");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(12);
	ui.scriptPlainTextEdit->setFont(font);
	ui.scriptPlainTextEdit->setTabStopWidth(4 * fontMetrics().width(' '));
	ui.scriptPlainTextEdit->setAcceptDrops(false);
	Highlighter *highlighter = new Highlighter(ui.scriptPlainTextEdit->document());
	ui.scriptPlainTextEdit->setTabStopDistance(QFontMetricsF(ui.scriptPlainTextEdit->font()).width(" ") * 4);
	//ui.scriptPlainTextEdit->setTabStopWidth(ui.scriptPlainTextEdit->fontMetrics().width(" ") * 4);

	QString filePath = QStringLiteral("%1\\script.py").arg(g_appDir);
	std::ifstream file(filePath.toStdString());
	std::string script((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	ui.scriptPlainTextEdit->setPlainText(script.c_str());
	g_script = script;

	ui.stopScriptPushButton->setEnabled(false);
}

void SmartPython::InitPython()
{
	//单次输出
	//	std::string stdOutErr =
	//		"import sys\n\
	//class CatchOutErr:\n\
	//    def __init__(self):\n\
	//        self.value = ''\n\
	//    def write(self, txt):\n\
	//        self.value += txt\n\
	//catchOutErr = CatchOutErr()\n\
	//sys.stdout = catchOutErr\n\
	//sys.stderr = catchOutErr\n\
	//"; //this is python code to redirect stdouts/stderr
	//
	//	Py_Initialize();
	//	PyObject *pModule = PyImport_AddModule("__main__"); //create main module
	//	PyRun_SimpleString(stdOutErr.c_str()); //invoke code to redirect
	//	PyRun_SimpleString("print(1+1)"); //this is ok stdout
	//	PyRun_SimpleString("1+a"); //this creates an error
	//	PyObject *catcher = PyObject_GetAttrString(pModule, "catchOutErr"); //get our catchOutErr created above
	//	PyErr_Print(); //make python print any errors
	//	PyObject *output = PyObject_GetAttrString(catcher, "value"); //get the stdout and stderr from our catchOutErr object
	//	printf("Here's the output:\n %s", _PyUnicode_AsString(output)); //it's not in our C++ portion
	//	Py_Finalize();


	//AllocConsole();
	//SetConsoleTitle(L"Debug Console");
	//freopen("conin$", "r+t", stdin);
	//freopen("conout$", "w+t", stdout);
	//freopen("conout$", "w+t", stderr);


	QString pythonPath = QStringLiteral("%1\\%2").arg(g_appDir).arg(PYTHON_DIR_NAME);
	wchar_t path[MAX_PATH] = { 0 };
	wcscpy(path, pythonPath.toStdWString().c_str());

	Py_SetPythonHome(path);
	Py_Initialize();

	tyti::pylog::redirect_stdout(ShowLog);
	tyti::pylog::redirect_stderr(ShowLog);
	tyti::pylog::redirect_stdin(MyInput);

	PyEval_SetTrace(MyTrace, NULL);

	//PyObject *sys = PyImport_ImportModule("sys");
	//PyObject* io_stdout = PyFile_FromFile(stdout, "stdout", "a", nullptr);
	//PyObject_SetAttrString(sys, "stdout", io_stdout);
	//PyObject* io_stderr = PyFile_FromFile(stderr, "stderr", "a", nullptr);
	//PyObject_SetAttrString(sys, "stderr", io_stderr);
	//PyObject* io_stdin = PyFile_FromFile(stdin, "stdin", "r", nullptr);
	//PyObject_SetAttrString(sys, "stdin", io_stdin);

}

void SmartPython::UnInitPython()
{
	Py_Finalize();
}

void SmartPython::on_saveScriptPushButton_clicked()
{
	QString script = ui.scriptPlainTextEdit->toPlainText();
	g_script = script.toLocal8Bit().data();

	QString filePath = QStringLiteral("%1\\script.py").arg(g_appDir);

	//ios::in	为输入(读)而打开文件
	//ios::out	为输出(写)而打开文件
	//ios::ate	初始位置：文件尾
	//ios::app	所有输出附加在文件末尾
	//ios::trunc	如果文件已存在则先删除该文件
	//ios::binary	二进制方式
	//默认
	//ofstream	ios::out | ios::trunc
	//ifstream	ios::in
	//fstream	ios::in | ios::out
	std::ofstream file(filePath.toLocal8Bit().data());
	if (file.is_open())
	{
		file << script.toUtf8().data();

		//file.write(buffer, size);
		file.close();
	}

}

void SmartPython::on_runScriptPushButton_clicked()
{
	g_isAbort = false;

	ui.logPlainTextEdit->clear();
	on_saveScriptPushButton_clicked();

	qDebug() << __FUNCTION__ << QThread::currentThreadId() << QThread::currentThread();
	
	QtConcurrent::run([=]()
	{
		qDebug() << __FUNCTION__ << QThread::currentThreadId() << QThread::currentThread();

		QMetaObject::invokeMethod(ui.runScriptPushButton, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, false));
		QMetaObject::invokeMethod(ui.stopScriptPushButton, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
		PyRun_SimpleString(GBK_To_UTF8(g_script).c_str());
		QMetaObject::invokeMethod(ui.runScriptPushButton, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
		QMetaObject::invokeMethod(ui.stopScriptPushButton, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, false));
	});
}

void SmartPython::on_stopScriptPushButton_clicked()
{
	g_isAbort = true;
}

void SmartPython::on_compileScriptPushButton_clicked()
{
	QString scriptPath = QStringLiteral("%1\\%2").arg(g_appDir).arg(SCRIPT_FILE_NAME);
	QString pythonPath = QStringLiteral("%1\\%2\\python.exe").arg(g_appDir).arg(PYTHON_DIR_NAME);
	QString pyInstallerPath = QStringLiteral("%1\\%2\\Scripts\\pyinstaller-script.py").arg(g_appDir).arg(PYTHON_DIR_NAME);
	QString cmd = QStringLiteral("cd %1 && %2 %3 -F %4 && pause").arg(g_appDir).arg(pythonPath).arg(pyInstallerPath).arg(scriptPath);

	system(cmd.toLocal8Bit().data());
}