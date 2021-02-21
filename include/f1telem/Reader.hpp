#ifndef F1TELEM_READER_HPP
#define F1TELEM_READER_HPP

namespace F1Telem {

class Reader {
    public:
    virtual ~Reader() = default;

    virtual char* CreateBuffer() = 0;
    virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual int Read(char* buffer) = 0;

    protected:
    Reader() { open = false; }

    bool open;
};

} // namespace F1Telem

#endif // F1TELEM_READER_HPP