#ifndef F1TELEM_UDPREADER_HPP
#define F1TELEM_UDPREADER_HPP

#include "f1telem/Reader.hpp"

#ifdef __linux__
#include <netinet/in.h>
#elif _WIN32
#include <winsock2.h>
#endif

namespace F1Telem {

constexpr int UDP_BUFFER_SIZE = 1464;

class UDPReader : public Reader {
    public:
    UDPReader(const int port);
    ~UDPReader();
    char* CreateBuffer() final;
    bool Close() final;
    bool Open() final;
    int Read(char* buffer) final; // returns bytes read

    private:
    int sock;

#ifdef __linux__
    struct sockaddr_in serverAddr;
#elif _WIN32
    SOCKADDR_IN serverAddress;
#endif
};

} // namespace F1Telem

#endif // F1TELEM_UDPREADER_HPP