#include <iostream>

#include "vector.h"

int Vector::m_arrayCounter = 0;

Vector::Vector(int size, int value)
    : m_size(size)
    , m_capacity(size + DEFAULT_SIZE)
{

    if (size < 0)
        throw WrongIndex();

    m_arrayPtr = new int[ m_capacity ];

    std::fill_n(m_arrayPtr, m_size, value);

    m_arrayCounter++;
}

Vector::Vector(const Vector& vcp)
    : m_size(vcp.m_size)
    , m_capacity(vcp.m_capacity)
{
    m_arrayPtr = new int[ m_capacity ];
    ++m_arrayCounter;

    std::copy(vcp.m_arrayPtr, vcp.m_arrayPtr + vcp.m_size, m_arrayPtr);
}

Vector::~Vector()
{
    delete [] m_arrayPtr;
    --m_arrayCounter; 
}

Vector& Vector::operator= (const Vector& rhs)
{
    Vector temp(rhs);
    std::swap(m_size, temp.m_size);
    std::swap(m_arrayPtr, temp.m_arrayPtr);
    std::swap(m_capacity, temp.m_capacity);
 
    return *this; 
}

int& Vector::operator[] (int index)
{
    return m_arrayPtr[index];
}

const int& Vector::operator[] (int index) const
{
    return m_arrayPtr[index];
}

Vector Vector::operator+ (const Vector &rhs)
{
    if (m_size != rhs.m_size)
        throw ArrayNotEqual(); 

    Vector temp(m_size,0);
    for (int i = 0; i < m_size; ++i)
       temp.m_arrayPtr[ i ] = m_arrayPtr[ i ] + rhs.m_arrayPtr[ i ];
    return temp;

}

Vector Vector::operator* (const Vector &rhs)
{
    if (m_size == rhs.m_size)
    {
        Vector temp(m_size, 0);
        for (int i = 0; i < m_size; ++i)
           temp.m_arrayPtr[ i ] = m_arrayPtr[ i ] * rhs.m_arrayPtr[ i ];
        return temp;
    }
    throw ArrayNotEqual();
}

Vector operator+ (int value, const Vector& rhs)
{
    if (rhs.m_size != 0)
    {
        Vector temp(rhs.m_size, 0);
        for (int i = 0; i < rhs.m_size; ++i)
            temp.m_arrayPtr[i] = rhs.m_arrayPtr[ i ] + value;
        return temp;
    }
    throw Vector::UninitializedVector();
}

Vector operator* (int value,const Vector& rhs)
{
    if (rhs.m_size != 0)
    {
        Vector temp(rhs.m_size, 0);
        for (int i = 0; i < rhs.m_size; ++i)
            temp.m_arrayPtr[i] = rhs.m_arrayPtr[ i ] * value;
        return temp; 
    }
    throw Vector::UninitializedVector(); 
}

std::ostream& operator<< (std::ostream& os, const Vector& rhs)
{
    if(rhs.m_size != 0)
    {
        for (int i = 0; i < rhs.m_size; ++i)
            os << rhs.m_arrayPtr[ i ] << " ";
        return os;
    }
    else
        throw Vector::UninitializedVector();
}

std::istream& operator>> (std::istream& is, Vector& rhs)
{
    for (int i = 0; i < rhs.m_size; ++i)
        is >> rhs.m_arrayPtr[i];
    return is;
}

//returns quantity of elements of a vector
int Vector::size() const
{
    return m_size;
}

//returns vector size
int Vector::capacity() const
{
    return m_capacity;
}

void Vector::reverse()
{
    for (int i = 0; i < m_size / 2; ++i)
        std::swap(m_arrayPtr[ i ], m_arrayPtr[m_size - i - 1]);
}

void Vector::push_back(int value)
{
    if (m_size < m_capacity)
        m_arrayPtr[ m_size++ ] = value;
    else
    {
        m_capacity = m_size + DEFAULT_SIZE;
        int* arrayPtrTemp = new int[ m_capacity ];
        std::copy(m_arrayPtr, m_arrayPtr + m_size, arrayPtrTemp);
        delete[] m_arrayPtr;
        arrayPtrTemp[ m_size ] = value;
        m_size++;
        m_arrayPtr = arrayPtrTemp;
    }
}

const int* Vector::begin() const
{
    return m_arrayPtr;
}

const int* Vector::end() const
{
    return m_arrayPtr + m_size;
}

int& Vector::at(int index) const
{
    if (m_size == 0)
        throw ElementNotExists(); 

    if (index < 0 || index >= m_size)
        throw WrongIndex(); 

    return m_arrayPtr[index];
}

const int& Vector::at(int index)
{
    if (m_size == 0)
        throw ElementNotExists(); 

    if (index < 0 || index >= m_size)
        throw WrongIndex(); 

    return m_arrayPtr[index];
}

void Vector::null()
{      
    for (int i = 0; i < m_size; ++i)
        m_arrayPtr[i] = 0;
    /* m_size = 0; ???? */
}

bool Vector::empty() const
{  
    if (m_size == 0)
        return true;
    else 
        return false;
}

void Vector::insert(int element, int position)
{
    if (position < 0 || position > m_size)
    {
         throw WrongIndex();
    }

    if (m_size < m_capacity)
    {
        for (int i = m_size - 1; i > position - 1; --i)
            m_arrayPtr[i + 1] = m_arrayPtr[i];
        m_arrayPtr[position] = element;
        m_size++;
    }

    else
    {
        m_capacity = m_size + DEFAULT_SIZE;
        int* arrayPtrTemp = new int[ m_capacity ];
        std::copy(m_arrayPtr, m_arrayPtr + m_size, arrayPtrTemp);
        delete[] m_arrayPtr;

        for (int i = m_size - 1; i > position - 1; --i)
            arrayPtrTemp[i + 1] = arrayPtrTemp[i];
        arrayPtrTemp[ position ] = element;
        m_size++;

        m_arrayPtr = arrayPtrTemp;
    }
}

void Vector::insert(int* begin, int* end, int position)
{
    int seqLength = end - begin;
    
    if (position < 0 || position > m_size)
    {
         throw WrongIndex();
    }

    if (seqLength < m_capacity - m_size)
    {
        for (int i = m_size - 1; i > position - 1; --i)
            m_arrayPtr[i + seqLength] = m_arrayPtr[i];
        
        for (int i = position; i < position + seqLength; ++i, begin++)
            m_arrayPtr[i] = *begin;

        m_size += seqLength;
    }

    else
    {
        m_capacity = m_size + DEFAULT_SIZE + seqLength;
        int* arrayPtrTemp = new int[ m_capacity ];
        std::copy(m_arrayPtr, m_arrayPtr + m_size, arrayPtrTemp);
        delete[] m_arrayPtr;

        for (int i = m_size - 1; i > position - 1; --i)
            arrayPtrTemp[i + seqLength] = arrayPtrTemp[i];
        
        for (int i = position; i < position + seqLength; ++i, begin++)
            arrayPtrTemp[i] = *begin;

        m_size += seqLength;

        m_arrayPtr = arrayPtrTemp;
    }
}
        
void Vector::clear()
{
    delete [] m_arrayPtr;
    --m_arrayCounter;

    m_size = 0;
    m_capacity = DEFAULT_SIZE;
    m_arrayPtr = new int[ m_capacity ];

    m_arrayCounter++;
}

void Vector::erase(int position)
{
    for (int i = position; i < m_size; ++i)
        m_arrayPtr[i] = m_arrayPtr[i + 1];
    m_arrayPtr[m_size] = 0;
    m_size--;
}

void Vector::erase(int* begin, int* end, int position)
{
    int seqLength = end - begin;
    if(seqLength < m_size)
    {
        for (int i = position; i < m_size; ++i)
            m_arrayPtr[i] = m_arrayPtr[i + seqLength];

        m_capacity = m_size + DEFAULT_SIZE - seqLength;
        int* arrayPtrTemp = new int[ m_capacity ];
        std::copy(m_arrayPtr, m_arrayPtr + m_size - seqLength, arrayPtrTemp);
        delete[] m_arrayPtr;
        m_size -= seqLength;

        m_arrayPtr = arrayPtrTemp;
    }
    else
        throw ElementNotExists();
}
        

            
        





