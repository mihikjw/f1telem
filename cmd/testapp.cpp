#include <cstdio>
#include <memory>

#include "f1telem/Decoder.hpp"
#include "f1telem/Types.hpp"
#include "f1telem/UDPReader.hpp"

constexpr int SUCCESS_EXIT = 0;
constexpr int ERR_EXIT = 1;

constexpr int PORT = 20777;

int main() {
    auto reader = F1Telem::UDPReader(PORT);

    char* buffer = reader.CreateBuffer();
    if (!buffer) {
        std::printf("Failed To Create Buffer\n");
        return ERR_EXIT;
    }

    reader.Open();
    std::printf("UDP Server Listening On Port %d\n", PORT);

    int bytes;
    uint8_t packetID;

    F1Telem::Decoder decoder;
    F1Telem::PacketHeader header;
    F1Telem::PacketMotionData motionPacket;

    while ((bytes = reader.Read(buffer)) > 0) {
        if (!buffer) {
            std::printf("Buffer Failed Read\n");
            continue;
        }

        packetID = decoder.DecodePacketHeader(buffer, &header);
        std::printf("Packet ID: %d\n", header.m_packetId);

        switch (packetID) {
            case F1Telem::MOTION: {
                decoder.DecodePacketMotionData(buffer, &header, &motionPacket);
                break;
            }
            case F1Telem::SESSION: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::LAP_DATA: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::EVENT: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::PARTICIPANTS: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::CAR_SETUPS: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::CAR_TELEMETRY: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::CAR_STATUS: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::FINAL_CLASSIFICATION: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            case F1Telem::LOBBY_INFO: {
                std::printf("Packet Decode Not Implemented\n");
                decoder.ResetByteCount();
                continue;
            }
            default: {
                std::printf("Failed To Decode Packet\n");
                decoder.ResetByteCount();
                continue;
            }
        }

        if (!decoder.ValidateLastPacket(bytes)) {
            std::printf("Packet Failed Validation\n");
        }
    }

    std::printf("Shutdown\n");
}