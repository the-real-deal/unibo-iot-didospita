#pragma once

#include <stddef.h>

#include "pair.hpp"

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

    bool indexOutOfBounds(size_t index) { return index >= this->count; }

    T *get(size_t index)
    {
        if (this->indexOutOfBounds(index))
        {
            return nullptr;
        }
        return &this->data[index];
    }

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

    bool deleteAt(size_t index)
    {
        if (this->indexOutOfBounds(index))
        {
            return false;
        }
        for (size_t i = index; i < this->count - 1; i++)
        {
            this->data[i] = this->data[i + 1];
        }
        this->count--;
        return true;
    }

    template <typename Predicate>
    Pair<T*, size_t> find(Predicate predicate)
    {
        for (size_t i = 0; i < this->count; i++)
        {
            T* value = this->get(i);
            if (predicate(value))
            {
                return Pair<T*, size_t>(value, i);
            }
        }
        return Pair<T*, size_t>(nullptr, 0);
    }
};