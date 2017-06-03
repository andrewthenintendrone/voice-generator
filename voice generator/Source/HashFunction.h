#pragma once

#include <functional>

namespace HashFunction
{
    typedef std::function<unsigned long(const char*)> HashFunc;

    // HashFunction definitions
    unsigned long basicHash(const char* data);
    unsigned long BKDRHash(const char* data);
    unsigned long ELFHash(const char* data);
    unsigned long djb2Hash(const char* data);
    unsigned long SDBMHash(const char* data);

    // default HashFunction
    static HashFunc default = ELFHash;
};
