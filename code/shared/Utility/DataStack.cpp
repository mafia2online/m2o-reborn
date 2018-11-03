
#include <memory>
#include <string.h>
#include <utility/DataStack.h>

DataStack::DataStack(const char* bytes, size_t length)
    : bytes_managed(false), bytes(const_cast<char*>(bytes)), end(length), length(0)
{

}

DataStack::DataStack(size_t length)
    : end(length), bytes_managed(true), length(0)
{
    bytes = new char[length];
}

DataStack::~DataStack()
{
    if (bytes_managed)
    {
        delete[] bytes;
    }
}

bool DataStack::Read(void* buffer, size_t length)
{
    if (this->length > end) return false;

    memcpy(buffer, &bytes[this->length], length);
    this->length += length;

    return true;
}

void DataStack::Write(const void* buffer, size_t length)
{
    if (this->length > end) return;

    memcpy(&bytes[this->length], buffer, length);
    this->length += length;
}
