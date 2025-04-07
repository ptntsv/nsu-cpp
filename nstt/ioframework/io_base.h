#ifndef IO_BASE_H
#define IO_BASE_H

enum OPENMODE { in = 1, out, app };
enum IOSTATE { goodbit, badbit, failbit, eofbit };

inline int operator|(OPENMODE m1, OPENMODE m2) {
    return static_cast<int>(m1) | static_cast<int>(m2);
}

class io_base {
public:
    io_base(){};
    io_base(const OPENMODE omode) : mode_(omode) {}
    int openmode() const { return mode_; }
    virtual ~io_base() = default;

protected:
    int mode_ = out;
    IOSTATE iostate_ = goodbit;
    bool is_open_ = false;
};

#endif
