#include <QtWidgets>

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <ostream>
#include <functional> 
#include <algorithm>
#include <vector>
#include <map>
#include <regex>
#include <sstream>


#pragma push_macro("slots")
#undef slots
#include <Python.h>
#include <frameobject.h>

//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python.hpp>
#pragma pop_macro("slots")

//using namespace boost::python;

#define PYTHON_DIR_NAME "Python34"
#define SCRIPT_FILE_NAME "script.py"
#define DATE_TIME_FORMAT "yyyy-MM-dd HH:mm:ss"
#define DATE_FORMAT "yyyy-MM-dd"


extern QString g_appDir;
extern QString g_appPath;


std::wstring UTF8_To_UTF16(const std::string &source);

std::string UTF16_To_UTF8(const std::wstring &source);

std::wstring GBK_To_UTF16(const std::string &source);

std::string UTF16_To_GBK(const std::wstring &source);

std::string GBK_To_UTF8(const std::string &source);

std::string UTF8_To_GBK(const std::string &source);

std::string ws2s(const std::wstring& ws);

std::wstring s2ws(const std::string& s);

std::vector<std::string> split(const std::string& str, const std::string& separator);

std::string& replace(std::string& str, const std::string& old_value, const std::string& new_value);

bool endswith(const std::string& str, const std::string& end);

bool startswith(const std::string& str, const std::string& start);