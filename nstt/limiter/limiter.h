#include <iostream>

template <typename T>
struct Counter {
    static int objects_created;

    Counter() { ++objects_created; }

    Counter(const Counter&) { ++objects_created; }
};

template <typename T>
int Counter<T>::objects_created{0};

template <typename T, int limit>
struct Limiter : Counter<T> {
    Limiter() {
        if (Counter<T>::objects_created > limit) {
            throw std::runtime_error(
                "Amount of created objects exceeded limit");
        }
    }
};
