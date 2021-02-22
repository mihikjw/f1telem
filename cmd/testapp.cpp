#include <cstdio>
#include <memory>

#include "f1telem/Decoder.hpp"
#include "f1telem/DecoderFactory.hpp"
#include "f1telem/UDPReader.hpp"

constexpr int SUCCESS_EXIT = 0;
constexpr int ERR_EXIT = 1;

int main() {
    auto reader = F1Telem::UDPReader(20777);

    char* buffer = reader.CreateBuffer();
    if (!buffer) {
        std::printf("Failed To Create Buffer\n");
        return ERR_EXIT;
    }

    auto decoder = std::unique_ptr<F1Telem::Decoder>(F1Telem::DecoderFactory(F1Telem::Edition::E_F12020));

    reader.Open();
    std::printf("UDP Server Listening On Port 20777\n");

    int bytes;
    F1Telem::Packet* packet;

    while ((bytes = reader.Read(buffer)) > 0) {
        packet = decoder->DecodePacket(buffer);
        std::printf("Packet Read: %d\n", bytes);
    }

    delete packet;
    std::printf("Shutdown\n");
}