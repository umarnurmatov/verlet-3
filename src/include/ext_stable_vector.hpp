#pragma once
#include <vector>
#include <boost/container/stable_vector.hpp>


/// @brief Extended boost::container::stable_vector. References don't invalidate even when element is erased.
/// @tparam T element type
template<typename T>
class ext_stable_vector
{
    boost::container::stable_vector<T> m_data;
    std::vector<size_t> m_free_space;

public:
    ext_stable_vector()
    {
    }

    T* emplace_back(T x)
    {
        if(!m_free_space.empty())
        {
            size_t _i = *m_free_space.end();
            m_free_space.pop_back();
            m_data[_i] = x;
            return &m_data[_i];
        }
        else
            return &m_data.emplace_back(x);
    }

    void erase(size_t i)
    {
        m_free_space.push_back(i);
    }

    T &operator[](size_t i)
    {
        return m_data[i];
    }

    T &at(size_t i)
    {
        return m_data.at(i);
    }

    size_t size()
    {
        return m_data.size();
    }
};