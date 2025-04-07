#ifndef IO_READER_H
#define IO_READER_H
#include <string>

#include "io_base.h"

class io_reader : virtual public io_base {
public:
    io_reader() : io_base(out) {}

protected:
    virtual io_reader& operator>>(int& out) = 0;
    virtual io_reader& operator>>(char& out) = 0;
    virtual io_reader& operator>>(double& out) = 0;
    virtual io_reader& operator>>(std::string& out) = 0;
};

#endif  // IO_READER_H
