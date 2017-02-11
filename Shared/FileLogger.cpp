/** @file Shared/FileLogger.cpp
*  @brief Threadsafe utility file logging class.
*
*  C++11 file operations should be threadsafe but I am not sure due to non matching standard descriptions.
*
*  @author MyU (myudev0@gmail.com)
*  @todo Make more descriptive f.e. ::Open error output etc.
*/

#include "FileLogger.h"

IO::CFileLogger::CFileLogger(bool _threadsafe, bool _timestamp) : m_timestamp(_timestamp)
{
	m_mutex = nullptr;

	if (_threadsafe)
	{
		m_mutex = std::make_unique<std::mutex>();
	}
}

IO::CFileLogger::~CFileLogger()
{
	m_ofstream.close();
}

bool IO::CFileLogger::Open(SString path)
{
	if (m_mutex)	std::lock_guard<std::mutex> guard(*m_mutex); // plox no writes while we open

	m_ofstream.open(path.GetSTLString(), std::ios::trunc | std::ios::out);

	if (m_ofstream.is_open())
		return true;

	return false;
}

void IO::CFileLogger::Writeln(const char *szFormat, ...)
{
	if (m_mutex)	std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!

	va_list args;
	va_start(args, szFormat);
	const size_t len = static_cast<size_t>(vsnprintf(NULL, 0, szFormat, args));
	char *pszBuffer = new char[len + 1];
	vsnprintf(pszBuffer, len+1, szFormat, args);
	va_end(args);

	if (m_timestamp) WriteTimestamp();
	m_ofstream << pszBuffer << "\n";
	m_ofstream.flush(); // fuuu
	delete[] pszBuffer;
}

void IO::CFileLogger::Writeln(SString & str)
{
	if (m_mutex)	std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!
	if (m_timestamp) WriteTimestamp();
	m_ofstream << str.GetSTLString() << "\n";

}

void IO::CFileLogger::Writeln(char * szString)
{
	if (m_mutex)	std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!
	if (m_timestamp) WriteTimestamp();
	m_ofstream << szString << "\n";
}

void IO::CFileLogger::WriteTimestamp()
{
	// !! we rely on mutex locking before this call!
	std::time_t t = std::time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &t);
	m_ofstream << std::put_time(&timeinfo, "%c") << " | ";
}
