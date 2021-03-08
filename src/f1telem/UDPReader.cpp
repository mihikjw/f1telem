#include "f1telem/UDPReader.hpp"

#include <cstring>
#include <stdexcept>

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

using namespace F1Telem;

UDPReader::UDPReader(const int port) {
    open = false;

#ifdef __linux__
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
#elif _WIN32
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

UDPReader::~UDPReader() { Close(); }

bool UDPReader::Close() {
#ifdef _WIN32
    if (closesocket(sock) != 0) {
        return false;
    }

    if (WSACleanup() != 0) {
        return false;
    }
#endif

    open = false;
    return true;
}

char* UDPReader::CreateBuffer() { return new char[UDP_BUFFER_SIZE]; }

bool UDPReader::Open() {
#ifdef __linux__
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        return false;
    }

    if (bind(sock, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        return false;
    }
#elif _WIN32
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    if (bind(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return false;
    }
#endif

    open = true;
    return open;
}

int UDPReader::Read(char* buffer) {
    if (!open) {
        throw std::runtime_error("socket not open");
    }

    std::memset(buffer, 0, UDP_BUFFER_SIZE);
    return recvfrom(sock, buffer, UDP_BUFFER_SIZE, 0, nullptr, nullptr);
}