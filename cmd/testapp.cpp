#include <cstdio>

#include "f1telem/UDPReader.hpp"

constexpr int SUCCESS_EXIT = 0;
constexpr int ERR_EXIT = 1;

int main() {
    auto reader = F1Telem::UDPReader(8080);

    char* buffer = reader.CreateBuffer();
    if (!buffer) {
        std::printf("Failed To Create Buffer\n");
        return ERR_EXIT;
    }

    reader.Open();
    std::printf("UDP Server Listening On Port 8080\n");
    int bytes;

    while ((bytes = reader.Read(buffer)) > 0) {
        std::printf("Packet Read: %d\n", bytes);
    }

    std::printf("Shutdown\n");
}