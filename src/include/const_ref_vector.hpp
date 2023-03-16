#pragma once
#include <vector>

typedef unsigned int size_t;

template<typename T>
class ConstRefVector
{
    std::vector<std::vector<T>> vv;
    const size_t m_chunk_size;
    size_t m_current_chunk;

    ConstRefVector(size_t chunk_size)
        : m_chunk_size{chunk_size}
    {
        vv.push_back(std::vector<T>());
        m_current_chunk = 0;
        vv[0].reserve(m_chunk_size);
    }


    T* emplace_back(T x)
    {
        if(vv[m_current_chunk].size() < m_chunk_size - 1)
        {
            return vv[m_current_chunk].emplace_back(x);
        }
        else
        {
            vv.push_back(std::vector<T>());
            vv[++m_current_chunk].reserve(m_chunk_size);
            return vv[m_current_chunk].emplace_back(x);
        }
    }
};