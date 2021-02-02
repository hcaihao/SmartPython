#include "stdafx.h"
#include "smartpython.h"
#include <QtWidgets/QApplication>

#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "imm32.lib")
//#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "winspool.lib")
//#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "comdlg32.lib")
//#pragma comment(lib, "oleaut32.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "kernel32.lib")
//#pragma comment(lib, "shell32.lib")
//#pragma comment(lib, "uuid.lib")
//#pragma comment(lib, "ole32.lib")
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "advapi32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "version.lib")


QString g_appDir;
QString g_appPath;

//https://stackoverflow.com/questions/34075757/tracing-code-execution-in-embedded-python-interpreter/34076235
//https://docs.python.org/3.5/c-api/init.html#profiling-and-tracing
//http://mateusz.loskot.net/post/2011/12/01/python-sys-stdout-redirection-in-cpp/
//http://justinwatson.name/2017/03/21/Embedding_Python_in_C++.html
//https://blog.csdn.net/raby_gyl/article/details/70888387
//http://mateusz.loskot.net/post/2011/12/01/python-sys-stdout-redirection-in-cpp/
//http://www.xefan.com/archives/84082.html
//http://www.cnblogs.com/turtle-fly/p/3280519.html
//https://stackoverflow.com/questions/955962/how-to-buffer-stdout-in-memory-and-write-it-from-a-dedicated-thread/956269#956269
//https://stackoverflow.com/questions/7935975/asynchronously-redirect-stdout-stdin-from-embedded-python-to-c

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	g_appDir = QDir::toNativeSeparators(QApplication::applicationDirPath());
	g_appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());

	SmartPython w;
	w.show();
	return a.exec();
}
