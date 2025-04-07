#ifndef IO_READER_WRITER_H
#define IO_READER_WRITER_H
#include "io_reader.h"
#include "io_writer.h"

class io_reader_writer : public io_reader, public io_writer {
public:
    io_reader_writer() { mode_ = in | out; }
};
#endif  // IO_READER_WRITER_H
