#include <cstdio>
#include <memory>
#include <stdexcept>

#include "f1telem/Decoder.hpp"
#include "f1telem/Types.hpp"
#include "f1telem/UDPReader.hpp"

constexpr int SUCCESS_EXIT = 0;
constexpr int ERR_EXIT = 1;

constexpr int PORT = 20777;

int main() {
    auto reader = F1Telem::UDPReader(PORT);

    char* buffer = reader.CreateBuffer();
    char* bufferStart = buffer;
    if (!buffer || !bufferStart) {
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
    F1Telem::PacketSessionData sessionPacket;
    F1Telem::PacketLapData lapDataPacket;
    F1Telem::PacketEventData eventDataPacket;
    F1Telem::PacketParticipantsData participantsDataPacket;
    F1Telem::PacketCarSetupData carSetupDataPacket;
    F1Telem::PacketCarTelemetryData carTelemetryDataPacket;
    F1Telem::PacketCarStatusData carStatusDataPacket;
    F1Telem::PacketFinalClassificationData finalClassificationDataPacket;
    F1Telem::PacketLobbyInfoData packetLobbyInfoData;

    while ((bytes = reader.Read(buffer)) > 0) {
        if (!buffer) {
            std::printf("Buffer Failed Read\n");
            continue;
        }

        packetID = decoder.DecodePacketHeader(&buffer, &header);
        if (packetID == 255) {
            std::printf("Unable To Decode Packet Header\n");
            continue;
        }

        std::printf("Packet ID: %d\n", header.m_packetId);

        switch (packetID) {
            case F1Telem::MOTION: {
                decoder.DecodePacketMotionData(&buffer, &header, &motionPacket);
                break;
            }
            case F1Telem::SESSION: {
                decoder.DecodePacketSessionData(&buffer, &header, &sessionPacket);
                break;
            }
            case F1Telem::LAP_DATA: {
                decoder.DecodePacketLapData(&buffer, &header, &lapDataPacket);
                break;
            }
            case F1Telem::EVENT: {
                decoder.DecodePacketEventData(&buffer, &header, &eventDataPacket);
                break;
            }
            case F1Telem::PARTICIPANTS: {
                decoder.DecodePacketParticipantsData(&buffer, &header, &participantsDataPacket);
                break;
            }
            case F1Telem::CAR_SETUPS: {
                decoder.DecodePacketCarSetupData(&buffer, &header, &carSetupDataPacket);
                break;
            }
            case F1Telem::CAR_TELEMETRY: {
                decoder.DecodePacketCarTelemetryData(&buffer, &header, &carTelemetryDataPacket);
                break;
            }
            case F1Telem::CAR_STATUS: {
                decoder.DecodePacketCarStatusData(&buffer, &header, &carStatusDataPacket);
                break;
            }
            case F1Telem::FINAL_CLASSIFICATION: {
                decoder.DecodePacketFinalClassificationData(&buffer, &header, &finalClassificationDataPacket);
                break;
            }
            case F1Telem::LOBBY_INFO: {
                decoder.DecodeLobbyInfoData(&buffer, &header, &packetLobbyInfoData);
                break;
            }
            default: {
                decoder.ResetByteCount();
                continue;
            }
        }

        if (!decoder.ValidateLastPacket(bytes)) {
            std::printf("Packet Failed Validation\n");
        }

        buffer = bufferStart;
    }

    std::printf("Shutdown\n");
    return SUCCESS_EXIT;
}