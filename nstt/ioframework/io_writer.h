#ifndef IO_WRITER_H
#define IO_WRITER_H

#include <string>

#include "io_base.h"

class io_writer : virtual public io_base {
public:
    io_writer() : io_base(in) {}

protected:
    virtual io_writer& operator<<(int) = 0;
    virtual io_writer& operator<<(char) = 0;
    virtual io_writer& operator<<(double) = 0;
    virtual io_writer& operator<<(const std::string&) = 0;
};

#endif  // IO_WRITER_H
