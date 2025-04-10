#include <iostream>

template <typename T>
struct Counter {
    static int objects_created;
    static int objects_alive;

    Counter() {
        ++objects_created;
        ++objects_alive;
    }

    Counter(const Counter&) {
        ++objects_created;
        ++objects_alive;
    }

protected:
    ~Counter() {
        --objects_alive;
    }
};

template <typename T>
int Counter<T>::objects_created{0};

template <typename T>
int Counter<T>::objects_alive{0};

template <typename T, int limit>
struct Limiter : Counter<T> {
    Limiter() {
        if (Counter<T>::objects_created > limit) {
            throw std::runtime_error(
                "Amount of created objects exceeded limit");
        }
    }
};

template <int limit>
class Foo : Limiter<Foo<limit>, limit> {
public:
    Foo() {
        std::cout << "Foo" << std::endl;
    }
};

int main() {
    return 0;
}
