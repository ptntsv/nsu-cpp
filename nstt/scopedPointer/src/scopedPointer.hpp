#pragma once

#ifdef LOG
#include <iostream>
#endif

template <typename T>
class DeepCopyScopedPointer {
    T* pointer;

   public:
    bool empty() { return pointer == nullptr; }
    DeepCopyScopedPointer(T* raw) : pointer(raw) {}
    DeepCopyScopedPointer(const DeepCopyScopedPointer& other) {
#ifdef LOG
        std::cout << "LOG: DeepCopy copy ctr\n";
#endif
        pointer = new T(*other.pointer);  // what if T has no copy contructor?
    }
    DeepCopyScopedPointer(DeepCopyScopedPointer&& other) =
        delete;  // in this scenario move ctr is just a copy ctr

    DeepCopyScopedPointer& operator=(const DeepCopyScopedPointer& other) {
        this->~DeepCopyScopedPointer();
        pointer = new T(*other.pointer);
        return *this;
    }

    DeepCopyScopedPointer& operator=(DeepCopyScopedPointer&& other) = delete;
    ~DeepCopyScopedPointer() {
        delete pointer;
        pointer = nullptr;
    }

    T& operator*() { return *pointer; }
    const T& operator*() const { return *pointer; }

    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }
};

template <typename T>
class ScopedPointer {
    T* pointer = nullptr;

   public:
    bool empty() { return pointer == nullptr; }
    ScopedPointer(T* raw) : pointer(raw) {}
    ScopedPointer(const ScopedPointer& other) = delete;
    ScopedPointer(ScopedPointer&& other) { std::swap(pointer, other.pointer); }
    ScopedPointer& operator=(const ScopedPointer& other) = delete;
    ScopedPointer& operator=(ScopedPointer&& other) {
        this->~ScopedPointer();
        std::swap(pointer, other.pointer);
        return *this;
    }

    T& operator*() { return *pointer; }
    const T& operator*() const { return *pointer; }

    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }

    ~ScopedPointer() {
        delete pointer;
        pointer = nullptr;
    }
};
