#include "stdafx.h"

//https://blog.csdn.net/charlessimonyi/article/details/8722859

std::wstring UTF8_To_UTF16(const std::string &source)
{
	unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, NULL, NULL);
	if (len == 0)
		return std::wstring();
	wchar_t *buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, buffer, len);

	std::wstring dest(buffer);
	delete[] buffer;
	return dest;
}

std::string UTF16_To_UTF8(const std::wstring &source)
{
	unsigned long len = ::WideCharToMultiByte(CP_UTF8, NULL, source.c_str(), -1, NULL, NULL, NULL, NULL);
	if (len == 0)
		return std::string();
	char *buffer = new char[len];
	::WideCharToMultiByte(CP_UTF8, NULL, source.c_str(), -1, buffer, len, NULL, NULL);

	std::string dest(buffer);
	delete[] buffer;
	return dest;
}

std::wstring GBK_To_UTF16(const std::string &source)
{
	enum { GB2312 = 936 };

	unsigned long len = ::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, NULL, NULL);
	if (len == 0)
		return std::wstring();
	wchar_t *buffer = new wchar_t[len];
	::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, buffer, len);

	std::wstring dest(buffer);
	delete[] buffer;
	return dest;
}

std::string UTF16_To_GBK(const std::wstring &source)
{
	enum { GB2312 = 936 };

	unsigned long len = ::WideCharToMultiByte(GB2312, NULL, source.c_str(), -1, NULL, NULL, NULL, NULL);
	if (len == 0)
		return std::string();
	char *buffer = new char[len];
	::WideCharToMultiByte(GB2312, NULL, source.c_str(), -1, buffer, len, NULL, NULL);

	std::string dest(buffer);
	delete[] buffer;
	return dest;
}

std::string GBK_To_UTF8(const std::string &source)
{
	enum { GB2312 = 936 };

	unsigned long len = ::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, NULL, NULL);
	if (len == 0)
		return std::string();
	wchar_t *wide_char_buffer = new wchar_t[len];
	::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, wide_char_buffer, len);

	len = ::WideCharToMultiByte(CP_UTF8, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
	if (len == 0)
	{
		delete[] wide_char_buffer;
		return std::string();
	}
	char *multi_byte_buffer = new char[len];
	::WideCharToMultiByte(CP_UTF8, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);

	std::string dest(multi_byte_buffer);
	delete[] wide_char_buffer;
	delete[] multi_byte_buffer;
	return dest;
}

std::string UTF8_To_GBK(const std::string &source)
{
	enum { GB2312 = 936 };

	unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, NULL, NULL);
	if (len == 0)
		return std::string();
	wchar_t *wide_char_buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, wide_char_buffer, len);

	len = ::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
	if (len == 0)
	{
		delete[] wide_char_buffer;
		return std::string();
	}
	char *multi_byte_buffer = new char[len];
	::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);

	std::string dest(multi_byte_buffer);
	delete[] wide_char_buffer;
	delete[] multi_byte_buffer;
	return dest;
}

std::string ws2s(const std::wstring& ws)
{
	int nBufSize = WideCharToMultiByte(GetACP(), 0, ws.c_str(), -1, NULL, 0, 0, FALSE);
	char *szBuf = new char[nBufSize];
	WideCharToMultiByte(GetACP(), 0, ws.c_str(), -1, szBuf, nBufSize, 0, FALSE);
	std::string strRet(szBuf);
	delete[]szBuf;
	szBuf = NULL;
	return strRet;
}

std::wstring s2ws(const std::string& s)
{
	int nBufSize = MultiByteToWideChar(GetACP(), 0, s.c_str(), -1, NULL, 0);
	wchar_t *wsBuf = new wchar_t[nBufSize];
	MultiByteToWideChar(GetACP(), 0, s.c_str(), -1, wsBuf, nBufSize);
	std::wstring wstrRet(wsBuf);
	delete[]wsBuf;
	wsBuf = NULL;
	return wstrRet;
}

std::vector<std::string> split(const std::string& str, const std::string& separator)
{
	std::vector<std::string> strvec;
	std::string::size_type pos1, pos2;
	pos2 = str.find(separator);
	pos1 = 0;

	while (std::string::npos != pos2)
	{
		strvec.push_back(str.substr(pos1, pos2 - pos1));
		pos1 = pos2 + separator.length();
		pos2 = str.find(separator, pos1);
	}
	strvec.push_back(str.substr(pos1));
	return strvec;
}

std::string& replace(std::string& str, const std::string& old_value, const std::string& new_value)
{
	while (true)
	{
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}

bool endswith(const std::string& str, const std::string& end)
{
	int srclen = str.size();
	int endlen = end.size();
	if (srclen >= endlen)
	{
		std::string temp = str.substr(srclen - endlen, endlen);
		if (temp == end)
			return true;
	}

	return false;
}

bool startswith(const std::string& str, const std::string& start)
{
	int srclen = str.size();
	int startlen = start.size();
	if (srclen >= startlen)
	{
		std::string temp = str.substr(0, startlen);
		if (temp == start)
			return true;
	}

	return false;
}
