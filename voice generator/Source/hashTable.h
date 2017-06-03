#pragma once

#include <string>
#include "HashFunction.h"

// Bucket Struct
template<typename dataType>
class Bucket
{
public:
    Bucket()
    {
        name.clear();
        data = 0;
    }
    ~Bucket()
    {
        name.clear();
        data = 0;
    }

    // overloads
    Bucket& operator = (Bucket& otherBucket)
    {
        name = otherBucket.name;
        data = otherBucket.data;
        return *this;
    }

    // variables
    std::string name;
    dataType data;
};

// Hash Table class
template<typename dataType>
class HashTable
{
public:
    // default constructor
    HashTable(unsigned int size) : hashFunction(HashFunction::default), m_size(size), m_buckets(new Bucket<dataType>[size]) {}
    
    // copy constructor
    HashTable(HashTable& otherHashTable, unsigned int size)
    {
        hashFunction = otherHashTable.hashFunction;
        m_size = size;
        m_buckets = new Bucket<dataType>[m_size];
        for (unsigned int i = 0; i < otherHashTable.m_size; i++)
        {
            // ignore empty buckets
            if (!(otherHashTable.m_buckets[i].name.empty()))
            {
                std::string key = otherHashTable.m_buckets[i].name;
                getBucketByKey(key) = otherHashTable.getBucketByKey(key);
            }
        }
    }

    // destructor
    ~HashTable() { clear(); }

    // returns the address of a buckets data
    dataType& operator [] (const std::string& key)
    {
        // resize if load factor becomes too large
        if (getLoadFactor() > 0.7f)
        {
            resize();
        }
        // hash key
        unsigned int hashedKey = hashFunction((key.c_str())) % m_size;
        while (true)
        {
            // if the hashed key matches the bucket name or the bucket is empty
            if (m_buckets[hashedKey].name.empty())
            {
                m_buckets[hashedKey].name = key;
                return(m_buckets[hashedKey]).data;
            }
            else if (m_buckets[hashedKey].name == key)
            {
                // set the buckets name to key and return it
                return m_buckets[hashedKey].data;
            }
            // otherwise look at the next bucket
            else
            {
                if (hashedKey < m_size - 1)
                {
                    hashedKey++;
                }
                else
                {
                    hashedKey = 0;
                }
            }
        }
    }

    // returns the current loadFactor of the HashTable
    float getLoadFactor()
    {
        float usedBuckets = 0;
        for (unsigned int i = 0; i < m_size; i++)
        {
            // ignore empty buckets
            if (!m_buckets[i].name.empty())
            {
                usedBuckets++;
            }
        }

        return usedBuckets / m_size;
    }

    // resizes the HashTable by creating a larger HashTable and transfering to it
    void resize()
    {
        HashTable newHashTable(*this, m_size * 2);
        *this = newHashTable;
    }

    // cleans up memory
    void clear()
    {
        delete[] m_buckets;
    }

    // removes an element
    bool remove(std::string& bucketName)
    {
        Bucket<dataType>* foundBucket = &getBucketByKey(bucketName);
        if (foundBucket->name == bucketName)
        {
            foundBucket->name.clear();
            foundBucket->data = 0;
            return true;
        }
        return false;
    }

    // = overload
    HashTable& operator = (HashTable& otherHashTable)
    {
        clear();
        m_size = otherHashTable.m_size;
        m_buckets = new Bucket<dataType>[m_size];
        for (unsigned int i = 0; i < m_size; i++)
        {
            std::string key = otherHashTable.m_buckets[i].name;
            getBucketByKey(key) = otherHashTable.getBucketByKey(key);
        }
        return *this;
    }

protected:
    // returns the address of a bucket specified by a key
    Bucket<dataType>& getBucketByKey(std::string& key)
    {
        // hash key
        unsigned long hashedKey = hashFunction((key.c_str())) % m_size;

        // iterate through buckets until the right one is returned
        while (true)
        {
            if (m_buckets[hashedKey].name == key || m_buckets[hashedKey].name.empty())
            {
                return m_buckets[hashedKey];
            }
            else
            {
                if (hashedKey < m_size - 1)
                {
                    hashedKey++;
                }
                else
                {
                    hashedKey = 0;
                }
            }
        }
    }
    HashFunction::HashFunc hashFunction;
    Bucket<dataType>* m_buckets;
    unsigned int m_size;
};