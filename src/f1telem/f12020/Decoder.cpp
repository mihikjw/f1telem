#include "f1telem/f12020/Decoder.hpp"

#include <cstring>

using namespace F1Telem;

Packet* F12020::Decoder::DecodePacket(char* buffer) {
    if (!buffer) {
        return nullptr;
    }

    auto header = new PacketHeader();
    uint8_t packetID = decodePacketHeader(buffer, header);

    switch (packetID) {
        case F12020::MOTION: {
            return decodePacketMotionData(buffer, header);
        }
        case F12020::SESSION: {
            return nullptr;
        }
        case F12020::LAP_DATA: {
            return nullptr;
        }
        case F12020::EVENT: {
            return nullptr;
        }
        case F12020::PARTICIPANTS: {
            return nullptr;
        }
        case F12020::CAR_SETUPS: {
            return nullptr;
        }
        case F12020::CAR_TELEMETRY: {
            return nullptr;
        }
        case F12020::CAR_STATUS: {
            return nullptr;
        }
        case F12020::FINAL_CLASSIFICATION: {
            return nullptr;
        }
        case F12020::LOBBY_INFO: {
            return nullptr;
        }
        default: {
            return nullptr;
        }
    }
}

uint8_t F12020::Decoder::decodePacketHeader(char* buffer, PacketHeader* header) {
    std::memcpy(&header->m_packetFormat, buffer, sizeof(header->m_packetFormat));
    buffer += sizeof(header->m_packetFormat);
    std::memcpy(&header->m_gameMajorVersion, buffer, sizeof(header->m_gameMajorVersion));
    buffer += sizeof(header->m_gameMajorVersion);
    std::memcpy(&header->m_gameMinorVersion, buffer, sizeof(header->m_gameMinorVersion));
    buffer += sizeof(header->m_gameMinorVersion);
    std::memcpy(&header->m_packetVersion, buffer, sizeof(header->m_packetVersion));
    buffer += sizeof(header->m_packetVersion);
    std::memcpy(&header->m_packetId, buffer, sizeof(header->m_packetId));
    buffer += sizeof(header->m_packetId);
    std::memcpy(&header->m_sessionUID, buffer, sizeof(header->m_sessionUID));
    buffer += sizeof(header->m_sessionUID);
    std::memcpy(&header->m_sessionTime, buffer, sizeof(header->m_sessionTime));
    buffer += sizeof(header->m_sessionTime);
    std::memcpy(&header->m_frameIdentifier, buffer, sizeof(header->m_frameIdentifier));
    buffer += sizeof(header->m_frameIdentifier);
    std::memcpy(&header->m_playerCarIndex, buffer, sizeof(header->m_playerCarIndex));
    buffer += sizeof(header->m_playerCarIndex);
    std::memcpy(&header->m_secondaryPlayerCarIndex, buffer, sizeof(header->m_secondaryPlayerCarIndex));
    buffer += sizeof(header->m_secondaryPlayerCarIndex);
    return header->m_packetId;
}

Packet* F12020::Decoder::decodePacketMotionData(char* buffer, PacketHeader* header) {
    auto packet = new PacketMotionData();
    packet->m_header = header;

    for (auto& i : packet->m_carMotionData) {
        i = decodeCarMotionData(buffer);
    }

    for (auto& i : packet->m_suspensionPosition) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
    }

    for (auto& i : packet->m_suspensionVelocity) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
    }

    for (auto& i : packet->m_suspensionAcceleration) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
    }

    for (auto& i : packet->m_wheelSpeed) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
    }

    for (auto& i : packet->m_wheelSlip) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
    }

    std::memcpy(&packet->m_localVelocityX, buffer, sizeof(packet->m_localVelocityX));
    buffer += sizeof(packet->m_localVelocityX);
    std::memcpy(&packet->m_localVelocityY, buffer, sizeof(packet->m_localVelocityY));
    buffer += sizeof(packet->m_localVelocityY);
    std::memcpy(&packet->m_localVelocityZ, buffer, sizeof(packet->m_localVelocityZ));
    buffer += sizeof(packet->m_localVelocityZ);

    std::memcpy(&packet->m_angularVelocityX, buffer, sizeof(packet->m_angularVelocityX));
    buffer += sizeof(packet->m_angularVelocityX);
    std::memcpy(&packet->m_angularVelocityY, buffer, sizeof(packet->m_angularVelocityY));
    buffer += sizeof(packet->m_angularVelocityY);
    std::memcpy(&packet->m_angularVelocityZ, buffer, sizeof(packet->m_angularVelocityZ));
    buffer += sizeof(packet->m_angularVelocityZ);

    std::memcpy(&packet->m_angularAccelerationX, buffer, sizeof(packet->m_angularAccelerationX));
    buffer += sizeof(packet->m_angularAccelerationX);
    std::memcpy(&packet->m_angularAccelerationY, buffer, sizeof(packet->m_angularAccelerationY));
    buffer += sizeof(packet->m_angularAccelerationY);
    std::memcpy(&packet->m_angularAccelerationZ, buffer, sizeof(packet->m_angularAccelerationZ));
    buffer += sizeof(packet->m_angularAccelerationZ);

    std::memcpy(&packet->m_frontWheelsAngle, buffer, sizeof(packet->m_frontWheelsAngle));
    buffer += sizeof(packet->m_frontWheelsAngle);

    return nullptr;
}

F12020::CarMotionData* F12020::Decoder::decodeCarMotionData(char* buffer) {
    auto data = new CarMotionData();

    std::memcpy(&data->m_worldPositionX, buffer, sizeof(data->m_worldPositionX));
    buffer += sizeof(data->m_worldPositionX);
    std::memcpy(&data->m_worldPositionY, buffer, sizeof(data->m_worldPositionY));
    buffer += sizeof(data->m_worldPositionY);
    std::memcpy(&data->m_worldPositionZ, buffer, sizeof(data->m_worldPositionZ));
    buffer += sizeof(data->m_worldPositionZ);

    std::memcpy(&data->m_worldVelocityX, buffer, sizeof(data->m_worldVelocityX));
    buffer += sizeof(data->m_worldVelocityX);
    std::memcpy(&data->m_worldVelocityY, buffer, sizeof(data->m_worldVelocityY));
    buffer += sizeof(data->m_worldVelocityY);
    std::memcpy(&data->m_worldVelocityZ, buffer, sizeof(data->m_worldVelocityZ));
    buffer += sizeof(data->m_worldVelocityZ);

    std::memcpy(&data->m_worldForwardDirX, buffer, sizeof(data->m_worldForwardDirX));
    buffer += sizeof(data->m_worldForwardDirX);
    std::memcpy(&data->m_worldForwardDirY, buffer, sizeof(data->m_worldForwardDirY));
    buffer += sizeof(data->m_worldForwardDirY);
    std::memcpy(&data->m_worldForwardDirZ, buffer, sizeof(data->m_worldForwardDirZ));
    buffer += sizeof(data->m_worldForwardDirZ);

    std::memcpy(&data->m_worldRightDirX, buffer, sizeof(data->m_worldRightDirX));
    buffer += sizeof(data->m_worldRightDirX);
    std::memcpy(&data->m_worldRightDirY, buffer, sizeof(data->m_worldRightDirY));
    buffer += sizeof(data->m_worldRightDirY);
    std::memcpy(&data->m_worldRightDirZ, buffer, sizeof(data->m_worldRightDirZ));
    buffer += sizeof(data->m_worldRightDirZ);

    std::memcpy(&data->m_gForceLateral, buffer, sizeof(data->m_gForceLateral));
    buffer += sizeof(data->m_gForceLateral);
    std::memcpy(&data->m_gForceLongitudinal, buffer, sizeof(data->m_gForceLongitudinal));
    buffer += sizeof(data->m_gForceLongitudinal);
    std::memcpy(&data->m_gForceVertical, buffer, sizeof(data->m_gForceVertical));
    buffer += sizeof(data->m_gForceVertical);

    std::memcpy(&data->m_yaw, buffer, sizeof(data->m_yaw));
    buffer += sizeof(data->m_yaw);
    std::memcpy(&data->m_pitch, buffer, sizeof(data->m_pitch));
    buffer += sizeof(data->m_pitch);
    std::memcpy(&data->m_roll, buffer, sizeof(data->m_roll));
    buffer += sizeof(data->m_roll);

    return data;
}