namespace IO
{
    class CFileLogger
    {
    public:
        CFileLogger(bool _threadsafe, bool _timestamp = false);
        ~CFileLogger();
    public:
        bool Open(std::string path);
        void Close();
        void SetCallback(std::function<void(const char*)> func) { m_funcback = func; }

    public:
        void Writeln(const char *szFormat, ...);
        void Writeln(std::string& str);
        void Writeln(char *szString);

    private:
        void WriteTimestamp();

    private:
        bool m_timestamp;
        std::unique_ptr<std::mutex> m_mutex;
        std::function<void(const char*)> m_funcback;
        std::ofstream m_ofstream;
    };
};


IO::CFileLogger::CFileLogger(bool _threadsafe, bool _timestamp) : m_timestamp(_timestamp)
{
    m_mutex = nullptr;
    m_funcback = nullptr;

    if (_threadsafe)
    {
        m_mutex = std::make_unique<std::mutex>();
    }
}

IO::CFileLogger::~CFileLogger()
{
    m_ofstream.close();
}

bool IO::CFileLogger::Open(std::string path)
{
    if (m_mutex)    std::lock_guard<std::mutex> guard(*m_mutex); // plox no writes while we open

    m_ofstream.open(path, std::ios::trunc | std::ios::out);

    if (m_ofstream.is_open())
        return true;

    return false;
}

void IO::CFileLogger::Close()
{
    if (m_mutex)    std::lock_guard<std::mutex> guard(*m_mutex);
    m_ofstream.close();
}

void IO::CFileLogger::Writeln(const char *szFormat, ...)
{
    if (m_mutex)    std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!

    va_list args;
    va_start(args, szFormat);
    const size_t len = static_cast<size_t>(vsnprintf(NULL, 0, szFormat, args));
    char *pszBuffer = new char[len + 1];
    vsnprintf(pszBuffer, len+1, szFormat, args);
    va_end(args);

    if (m_funcback) m_funcback(pszBuffer);

    if (m_timestamp) WriteTimestamp();
    m_ofstream << pszBuffer << "\n";
    m_ofstream.flush(); // fuuu
    delete[] pszBuffer;
}

void IO::CFileLogger::Writeln(std::string & str)
{
    if (m_mutex)    std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!
    if (m_timestamp) WriteTimestamp();
    if (m_funcback) m_funcback(str.c_str());

    m_ofstream << str << "\n";

}

void IO::CFileLogger::Writeln(char * szString)
{
    if (m_mutex)    std::lock_guard<std::mutex> guard(*m_mutex); // if the mutex is available -> lock it!
    if (m_timestamp) WriteTimestamp();
    if (m_funcback) m_funcback(szString);

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
