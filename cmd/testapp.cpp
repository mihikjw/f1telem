#include <cstdio>
#include <memory>
#include <stdexcept>

#include "f1telem/Decoder.hpp"
#include "f1telem/Types.hpp"
#include "f1telem/UDPReader.hpp"

/*
Quick and dirty test application for recieving and decoding UDP data
*/

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
            case F1Telem::PacketID::Motion: {
                decoder.DecodePacketMotionData(&buffer, &header, &motionPacket);
                break;
            }
            case F1Telem::PacketID::Session: {
                decoder.DecodePacketSessionData(&buffer, &header, &sessionPacket);
                break;
            }
            case F1Telem::PacketID::LapData: {
                decoder.DecodePacketLapData(&buffer, &header, &lapDataPacket);
                break;
            }
            case F1Telem::PacketID::Event: {
                decoder.DecodePacketEventData(&buffer, &header, &eventDataPacket);
                break;
            }
            case F1Telem::PacketID::Participants: {
                decoder.DecodePacketParticipantsData(&buffer, &header, &participantsDataPacket);
                break;
            }
            case F1Telem::PacketID::CarSetups: {
                decoder.DecodePacketCarSetupData(&buffer, &header, &carSetupDataPacket);
                break;
            }
            case F1Telem::PacketID::CarTelemetry: {
                decoder.DecodePacketCarTelemetryData(&buffer, &header, &carTelemetryDataPacket);
                break;
            }
            case F1Telem::PacketID::CarStatus: {
                decoder.DecodePacketCarStatusData(&buffer, &header, &carStatusDataPacket);
                break;
            }
            case F1Telem::PacketID::FinalClassification: {
                decoder.DecodePacketFinalClassificationData(&buffer, &header, &finalClassificationDataPacket);
                break;
            }
            case F1Telem::PacketID::LobbyInfo: {
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
    delete buffer;
    return SUCCESS_EXIT;
}