
class DataStack
{
    char* bytes;

    size_t length;
    size_t end;

    bool bytes_managed;

public:
    explicit DataStack(const char* bytes, size_t length);
    explicit DataStack(size_t length);

    virtual ~DataStack();

    template<typename Type>
    inline void Skip()
    {
        length += sizeof(Type);
    }

    inline void Skip(size_t size)
    {
        length += size;
    }

    inline char * GetAt()
    {
        return &bytes[length];
    }

    bool Read(void* buffer, size_t length);
    void Write(const void* buffer, size_t length);

    template<typename Type>
    Type Read()
    {
        Type tempValue;
        if (!Read(&tempValue, sizeof(Type)))
        {

        }

        return tempValue;
    }

    template<typename T>
    void Write(T value)
    {
        Write(&value, sizeof(T));
    }

    inline const char* GetBuffer() { return bytes; }
    inline size_t GetLength() { return length; }
};
