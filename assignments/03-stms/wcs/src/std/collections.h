#pragma once

#include <stddef.h>

// Queue operations not implemented with head and tail pointer to reduce memory usage
template <typename T, size_t N>
class Array
{
private:
    T data[N];
    size_t count;

public:
    Array() : count(0) {}

    bool isEmpty() { return this->count == 0; }

    bool isFull() { return this->count == N; }

    size_t size() { return this->count; }

    size_t capacity() const { return N; }

    T get(size_t index)
    {
        if (index >= this->count)
        {
            return T();
        }
        return this->data[index];
    }

    T operator[](size_t index) { return this->get(index); }

    bool pushLast(T value)
    {
        if (this->isFull())
        {
            return false;
        }
        this->data[this->count] = value;
        this->count++;
        return true;
    };

    bool pushFirst(T value)
    {
        if (this->isFull())
        {
            return false;
        }
        for (size_t i = this->count - 1; i >= 0; i--)
        {
            this->data[i + 1] = this->data[i];
        }
        this->data[0] = value;
        this->count++;
        return true;
    }

    T popLast()
    {
        if (this->isEmpty())
        {
            return T();
        }
        this->count--;
        return this->data[this->count];
    }

    T popFirst()
    {
        if (this->isEmpty())
        {
            return T();
        }
        T value = this->data[0];
        for (size_t i = 1; i < this->count; i++)
        {
            this->data[i - 1] = this->data[i];
        }
        this->count--;
        return value;
    };
};