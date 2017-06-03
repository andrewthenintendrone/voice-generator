#include "HashFunction.h"

namespace HashFunction
{
    //################################################//
    //  basic Hash                                    //
    //################################################//
    unsigned long basicHash(const char* data)
    {
        unsigned long hash = 0;
        int c = 0;

        while (c = *data++)
        {
            hash += c;
        }

        return hash;
    }

    //################################################//
    //  BKDR Hash                                     //
    //################################################//
    unsigned long BKDRHash(const char* data)
    {
        unsigned long hash = 0;
        int c = 0;

        while (c = *data++)
        {
            hash = (hash * 1313) + c;
        }

        return (hash & 0x7FFFFFFFL);
    }

    //################################################//
    //  ELF Hash                                      //
    //################################################//
    unsigned long ELFHash(const char* data)
    {
        unsigned long hash = 0, x = 0;
        int c = 0;

        while (c = *data++)
        {
            hash = (hash << 4) + c;
            if ((x = hash & 0xF0000000L))
            {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }
        return (hash & 0x7FFFFFFFL);
    }

    //################################################//
    //  djb2 Hash                                     //
    //################################################//
    unsigned long djb2Hash(const char* data)
    {
        unsigned long hash = 5381;
        int c = 0;

        while (c = *data++)
        {
            //  (x << 5) + x is the same as (x * 33)
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }

    //################################################//
    //  SDBM Hash                                     //
    //################################################//
    unsigned long SDBMHash(const char* data)
    {
        unsigned long hash = 0;
        int c = 0;

        while (c = *data++)
        {
            //  ((x << 6) + (x << 16) - x) is the same as (x * 65599)
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }
}