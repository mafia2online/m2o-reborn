#pragma once
#include <Common.h>
#include <string>

#include <m2/C_IE.h>
#include <stdarg.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace ue::sys::utils
{
    static C_String NULL_STRING;

    class C_String {
    public:
        int32_t refCounter = 0; // default is nullstr, no ref count

    private:
        int32_t stringLength = 0;
        int32_t wideStringLength = 0; // double of stringLength
        char* stringPtr = nullptr;


    public:
        ~C_String() {
            if (stringPtr) {
                delete stringPtr;
            }
        }; // not virtual

        C_String() = default;
        C_String(ue::sys::utils::C_String const& ref) {
            // TODO (don't break ref counting in ref)
            //0x183AF70
        }

        C_String(const char* str) {
            auto length = strlen(str);
            C_String(str, length);
        }

        C_String(char const* str, int32_t length) {
            stringLength = length + 1;
            wideStringLength = stringLength * 2;

            auto allocStrMem = C_IE::Alloc(stringLength);
            // Overwrite ptr value to new allocated mem
            *stringPtr = allocStrMem;
            memcpy((void*)stringPtr, str, stringLength);

            refCounter++;
        }

        C_String(char const* firstStr, char const* secondStr) {
            auto firstStrLength = strlen(firstStr);
            auto secondStrLength = strlen(secondStr);
            
            auto combinedStringLength = firstStrLength + 1 + secondStrLength;
            firstStrLength = combinedStringLength;
            wideStringLength = combinedStringLength * 2;

            auto allocStrMem = C_IE::Alloc(combinedStringLength);
            // Overwrite ptr value to new allocated mem
            *stringPtr = allocStrMem;
            memcpy((void*)stringPtr, firstStr, stringLength);
            memcpy((void*)(*(uintptr_t*)stringPtr + firstStrLength), secondStr, secondStrLength);

            refCounter++;
        }

        C_String* SetString(char const* str) { // 0x183AEC0
            // delete old ref and cleanup
            delete stringPtr;
            stringLength = 0;
            wideStringLength = 0;
            refCounter = 0;

            auto length = strlen(str);
            stringLength = length + 1;
            wideStringLength = stringLength * 2;

            auto allocStrMem = C_IE::Alloc(stringLength);
            // Overwrite ptr value to new allocated mem
            *stringPtr = allocStrMem;
            memcpy((void*)stringPtr, str, stringLength);

            refCounter++;
        }

        C_String* Empty() {
            if (refCounter == 1) 
                delete stringPtr;

            stringLength = 0;
            wideStringLength = 0;
            --refCounter;
        }

        char* FormatString(char const* format, ...) { 
            char szString[8129] = { 0 };
            va_list vaArgs;
            va_start(vaArgs, format);
            vsnprintf(szString, sizeof(szString), format, vaArgs);
            va_end(vaArgs);

            SetString(szString);
        }

        char* FormatStringVA(char const*, char*) {
            // TODO
            // 0x183AE10
            // what the heck is going on there
        }

        C_String* GetEmptyString() {
            ++NULL_STRING.refCounter;
            return &NULL_STRING;
        }

        bool IsEmpty() { // 0x181CBF0
            // shouldn't we be using strlen here?
            return refCounter == 0;
        }

        C_String* Delete() { // same as Empty function
            if (refCounter == 1)
                delete stringPtr;

            stringLength = 0;
            wideStringLength = 0;
            --refCounter;
        }

        C_String* GetCopy() {
            // TODO
            return nullptr;
        }

        int32_t Begin() {
            return ((uintptr_t)this);
        }

        int32_t End() {
            // My mem addr minus 1 byte for 0 termination plus string length
            return ((uintptr_t)this) - 1 + stringLength;
        }

        int32_t Length() {
            return stringLength;
        }

        C_String* SubString(uint32_t start, uint32_t end) {
            // TODO
            return nullptr;
        }

        C_String* SubString(const char *start, const char *end) {
            // TODO
            return nullptr;
        }

        C_String* ToLower() {
            stringPtr = strlwr(stringPtr);
            return this;
        }

        C_String* ToUpper() {
            stringPtr = strupr(stringPtr);
            return this;
        }

        C_String* Replace(const char*, const char*) {
            // TODO
            return nullptr;
        }

        C_String* TrimLeft(char) {
            // TODO
            return nullptr;
        }

        C_String* TrimRight(char) {
            // TODO
            return nullptr;
        }

        int32_t Find(char) {
            // TODO
            return 0;
        }

        int32_t FindLast(char) {
            // TODO
            return 0;
        }

        int32_t FindFromRight(const char*) {
            // TODO
            return 0;
        }

        C_String* Left(int32_t, size_t) {
            // TODO
            return nullptr;
        }
        C_String* Right(int32_t, size_t) {
            // TODO
            return nullptr;
        }

        int32_t ExpandDataExact(int32_t) {
            // TODO
            return 0;
        }

        C_String* operator=(const char*) {
            // TODO
            return nullptr;
        }

        C_String* operator=(C_String const&) {
            // TODO
            return nullptr;
        }

        C_String* operator+=(char) {
            // TODO
            return nullptr;
        }

        C_String* operator+=(const char*) {
            // TODO
            return nullptr;
        }
    };
}
