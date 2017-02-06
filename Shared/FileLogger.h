#pragma once
#include "Common.h"
#include <mutex>
#include <memory>
#include <fstream>
#include <stdarg.h>
#include <ctime>
#include <iomanip>

namespace IO
{
	class CFileLogger
	{
	public:
		CFileLogger(bool _threadsafe, bool _timestamp = false);
		~CFileLogger();
	public:
		bool Open(SString path);

	public:
		void Writeln(const char *szFormat, ...);
		void Writeln(SString& str);
		void Writeln(char *szString);
		
	private:
		void WriteTimestamp();

	private:
		bool m_timestamp;
		std::unique_ptr<std::mutex> m_mutex;
		std::ofstream m_ofstream;
	};
};