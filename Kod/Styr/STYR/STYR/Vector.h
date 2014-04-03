#ifndef VECTOR_H_
#define VECTOR_H_

#include <iosfwd>

class Vector
{
    friend std::ostream& operator<< (std::ostream&, const Vector&);
    friend std::istream& operator>> (std::istream&, Vector&);

    friend Vector operator+ (int, const Vector&);
    friend Vector operator* (int, const Vector&);

public:

    class WrongIndex : public std::exception
    {};
    
    class ElementNotExists : public std::exception
    {};
    
    class ArrayNotEqual : public std::exception
    {};

    class UninitializedVector : public std::exception
    {};

    explicit Vector(int = 0, int = DEFAULT_VALUE); /*size, element*/
    Vector(const Vector&);
    ~Vector();

    int size() const;
    int capacity() const;
    void reverse();
    void push_back(int);
    void null();
    bool empty() const;
    void clear();
    /* int* ??? */ void erase(int);
    void erase(int*, int*, int);
    void insert(int, int);
    void insert(int*, int*, int);

    Vector& operator= (const Vector&);
    Vector operator+ (const Vector&);
    Vector operator* (const Vector&); 
    
    int& at(int) const;
    const int& at(int);

    int& operator[] (int);
    const int& operator[] (int) const;
    
    const int* begin() const;
    const int* end() const;



private:
    static const int DEFAULT_SIZE = 10;
    static const int DEFAULT_VALUE = 0;
    static int m_arrayCounter;

    int m_capacity;
    int m_size;
    int* m_arrayPtr;
};

#endif /* VECTOR_H_ */