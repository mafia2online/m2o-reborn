#pragma once
#include <string>
#include <stdarg.h>

class SuperString
{
public:
	SuperString() : m_str() {}
	SuperString(const char *szBuffer) : m_str(szBuffer) {}
	SuperString(std::string& str) : m_str(str) {}
	SuperString(SuperString& str) : m_str(str.GetSTLString()) {}

public:
	inline void Set(SuperString& string) { m_str.assign(string.GetCStr()); }
	inline void Set(const char* szString) { m_str.assign(szString); }
	inline void Set(std::string& string) { m_str = string; }

	void Format(const char *szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		const size_t len = static_cast<size_t>(vsnprintf(NULL, 0, szFormat, args));
		char *pszBuffer = new char[len + 1];
		vsprintf_s(pszBuffer, len+1, szFormat, args);
		va_end(args);
		Set(pszBuffer);
		delete[] pszBuffer;
	}

	inline std::string& GetSTLString() { return m_str; }
	inline const char* GetCStr() { return m_str.c_str(); }

public:
	//operator const char*() { return GetCStr(); }

	SuperString& operator+=(const char *szString) { m_str.append(szString); return *this; }
	SuperString& operator+=(SuperString strString) { m_str.append(strString.GetSTLString()); return *this; }
	
	SuperString operator+(const char *szString) { SuperString _str(m_str); _str += szString; return _str; }
	SuperString operator+(SuperString &str) { SuperString _str(m_str); _str += str.GetSTLString(); return _str; }

	SuperString& operator=(std::string &str) { m_str = str; return *this; }
	SuperString& operator=(SuperString &str) { m_str.assign(str.GetSTLString()); return *this; }


	bool operator==(std::string &str) { return m_str == str; }
	bool operator==(SuperString &sstr) { return m_str == sstr.GetSTLString(); }
	bool operator==(const char *szStr) { return strcmp(m_str.c_str(), szStr) == 0; }

private:
	std::string m_str;
};

using SString = SuperString;