#include "f1telem/Decoder.hpp"

#include <cstring>

using namespace F1Telem;

bool Decoder::ValidateLastPacket(int bytesRecieved) {
    bool result = bytesRead == bytesRecieved;
    ResetByteCount();
    return result;
}

void Decoder::ResetByteCount() { bytesRead = 0; }

uint8_t Decoder::DecodePacketHeader(char* buffer, PacketHeader* header) {
    std::memcpy(&header->m_packetFormat, buffer, sizeof(header->m_packetFormat));
    buffer += sizeof(header->m_packetFormat);
    bytesRead += sizeof(header->m_packetFormat);

    if (header->m_packetFormat >= 2019) {
        std::memcpy(&header->m_gameMajorVersion, buffer, sizeof(header->m_gameMajorVersion));
        buffer += sizeof(header->m_gameMajorVersion);
        bytesRead += sizeof(header->m_gameMajorVersion);
        std::memcpy(&header->m_gameMinorVersion, buffer, sizeof(header->m_gameMinorVersion));
        buffer += sizeof(header->m_gameMinorVersion);
        bytesRead += sizeof(header->m_gameMajorVersion);
    }

    std::memcpy(&header->m_packetVersion, buffer, sizeof(header->m_packetVersion));
    buffer += sizeof(header->m_packetVersion);
    bytesRead += sizeof(header->m_packetVersion);
    std::memcpy(&header->m_packetId, buffer, sizeof(header->m_packetId));
    buffer += sizeof(header->m_packetId);
    bytesRead += sizeof(header->m_packetId);
    std::memcpy(&header->m_sessionUID, buffer, sizeof(header->m_sessionUID));
    buffer += sizeof(header->m_sessionUID);
    bytesRead += sizeof(header->m_sessionUID);
    std::memcpy(&header->m_sessionTime, buffer, sizeof(header->m_sessionTime));
    buffer += sizeof(header->m_sessionTime);
    bytesRead += sizeof(header->m_sessionTime);
    std::memcpy(&header->m_frameIdentifier, buffer, sizeof(header->m_frameIdentifier));
    buffer += sizeof(header->m_frameIdentifier);
    bytesRead += sizeof(header->m_frameIdentifier);
    std::memcpy(&header->m_playerCarIndex, buffer, sizeof(header->m_playerCarIndex));
    buffer += sizeof(header->m_playerCarIndex);
    bytesRead += sizeof(header->m_playerCarIndex);

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&header->m_secondaryPlayerCarIndex, buffer, sizeof(header->m_secondaryPlayerCarIndex));
        buffer += sizeof(header->m_secondaryPlayerCarIndex);
        bytesRead += sizeof(header->m_secondaryPlayerCarIndex);
    }

    return header->m_packetId;
}

bool Decoder::DecodePacketMotionData(char* buffer, PacketHeader* header, PacketMotionData* packet) {
    if (!buffer || !header || !packet) {
        return false;
    }

    packet->m_header = header;

    // F12020 increased number of cars by 2, support this as default and set last 2 cars as null for earlier versions
    uint8_t carCount = F12020_CAR_COUNT;
    if (packet->m_header->m_packetFormat < 2020) {
        carCount = LEGACY_CAR_COUNT;
        packet->m_carMotionData[LEGACY_CAR_COUNT - 2] = nullptr;
        packet->m_carMotionData[LEGACY_CAR_COUNT - 1] = nullptr;
    }

    for (uint8_t i = 0; i < carCount; i++) {
        packet->m_carMotionData[i] = decodeCarMotionData(buffer);
    }

    for (auto& i : packet->m_suspensionPosition) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_suspensionVelocity) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_suspensionAcceleration) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_wheelSpeed) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_wheelSlip) {
        std::memcpy(&i, buffer, sizeof(i));
        buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    std::memcpy(&packet->m_localVelocityX, buffer, sizeof(packet->m_localVelocityX));
    buffer += sizeof(packet->m_localVelocityX);
    bytesRead += sizeof(packet->m_localVelocityX);
    std::memcpy(&packet->m_localVelocityY, buffer, sizeof(packet->m_localVelocityY));
    buffer += sizeof(packet->m_localVelocityY);
    bytesRead += sizeof(packet->m_localVelocityY);
    std::memcpy(&packet->m_localVelocityZ, buffer, sizeof(packet->m_localVelocityZ));
    buffer += sizeof(packet->m_localVelocityZ);
    bytesRead += sizeof(packet->m_localVelocityZ);

    std::memcpy(&packet->m_angularVelocityX, buffer, sizeof(packet->m_angularVelocityX));
    buffer += sizeof(packet->m_angularVelocityX);
    bytesRead += sizeof(packet->m_angularVelocityX);
    std::memcpy(&packet->m_angularVelocityY, buffer, sizeof(packet->m_angularVelocityY));
    buffer += sizeof(packet->m_angularVelocityY);
    bytesRead += sizeof(packet->m_angularVelocityY);
    std::memcpy(&packet->m_angularVelocityZ, buffer, sizeof(packet->m_angularVelocityZ));
    buffer += sizeof(packet->m_angularVelocityZ);
    bytesRead += sizeof(packet->m_angularVelocityZ);

    std::memcpy(&packet->m_angularAccelerationX, buffer, sizeof(packet->m_angularAccelerationX));
    buffer += sizeof(packet->m_angularAccelerationX);
    bytesRead += sizeof(packet->m_angularAccelerationX);
    std::memcpy(&packet->m_angularAccelerationY, buffer, sizeof(packet->m_angularAccelerationY));
    buffer += sizeof(packet->m_angularAccelerationY);
    bytesRead += sizeof(packet->m_angularAccelerationY);
    std::memcpy(&packet->m_angularAccelerationZ, buffer, sizeof(packet->m_angularAccelerationZ));
    buffer += sizeof(packet->m_angularAccelerationZ);
    bytesRead += sizeof(packet->m_angularAccelerationZ);

    std::memcpy(&packet->m_frontWheelsAngle, buffer, sizeof(packet->m_frontWheelsAngle));
    buffer += sizeof(packet->m_frontWheelsAngle);
    bytesRead += sizeof(packet->m_frontWheelsAngle);

    return true;
}

CarMotionData* Decoder::decodeCarMotionData(char* buffer) {
    auto data = new CarMotionData();

    std::memcpy(&data->m_worldPositionX, buffer, sizeof(data->m_worldPositionX));
    buffer += sizeof(data->m_worldPositionX);
    bytesRead += sizeof(data->m_worldPositionX);
    std::memcpy(&data->m_worldPositionY, buffer, sizeof(data->m_worldPositionY));
    buffer += sizeof(data->m_worldPositionY);
    bytesRead += sizeof(data->m_worldPositionY);
    std::memcpy(&data->m_worldPositionZ, buffer, sizeof(data->m_worldPositionZ));
    buffer += sizeof(data->m_worldPositionZ);
    bytesRead += sizeof(data->m_worldPositionZ);

    std::memcpy(&data->m_worldVelocityX, buffer, sizeof(data->m_worldVelocityX));
    buffer += sizeof(data->m_worldVelocityX);
    bytesRead += sizeof(data->m_worldVelocityX);
    std::memcpy(&data->m_worldVelocityY, buffer, sizeof(data->m_worldVelocityY));
    buffer += sizeof(data->m_worldVelocityY);
    bytesRead += sizeof(data->m_worldVelocityY);
    std::memcpy(&data->m_worldVelocityZ, buffer, sizeof(data->m_worldVelocityZ));
    buffer += sizeof(data->m_worldVelocityZ);
    bytesRead += sizeof(data->m_worldVelocityZ);

    std::memcpy(&data->m_worldForwardDirX, buffer, sizeof(data->m_worldForwardDirX));
    buffer += sizeof(data->m_worldForwardDirX);
    bytesRead += sizeof(data->m_worldForwardDirX);
    std::memcpy(&data->m_worldForwardDirY, buffer, sizeof(data->m_worldForwardDirY));
    buffer += sizeof(data->m_worldForwardDirY);
    bytesRead += sizeof(data->m_worldForwardDirY);
    std::memcpy(&data->m_worldForwardDirZ, buffer, sizeof(data->m_worldForwardDirZ));
    buffer += sizeof(data->m_worldForwardDirZ);
    bytesRead += sizeof(data->m_worldForwardDirZ);

    std::memcpy(&data->m_worldRightDirX, buffer, sizeof(data->m_worldRightDirX));
    buffer += sizeof(data->m_worldRightDirX);
    bytesRead += sizeof(data->m_worldRightDirX);
    std::memcpy(&data->m_worldRightDirY, buffer, sizeof(data->m_worldRightDirY));
    buffer += sizeof(data->m_worldRightDirY);
    bytesRead += sizeof(data->m_worldRightDirY);
    std::memcpy(&data->m_worldRightDirZ, buffer, sizeof(data->m_worldRightDirZ));
    buffer += sizeof(data->m_worldRightDirZ);
    bytesRead += sizeof(data->m_worldRightDirZ);

    std::memcpy(&data->m_gForceLateral, buffer, sizeof(data->m_gForceLateral));
    buffer += sizeof(data->m_gForceLateral);
    bytesRead += sizeof(data->m_gForceLateral);
    std::memcpy(&data->m_gForceLongitudinal, buffer, sizeof(data->m_gForceLongitudinal));
    buffer += sizeof(data->m_gForceLongitudinal);
    bytesRead += sizeof(data->m_gForceLongitudinal);
    std::memcpy(&data->m_gForceVertical, buffer, sizeof(data->m_gForceVertical));
    buffer += sizeof(data->m_gForceVertical);
    bytesRead += sizeof(data->m_gForceVertical);

    std::memcpy(&data->m_yaw, buffer, sizeof(data->m_yaw));
    buffer += sizeof(data->m_yaw);
    bytesRead += sizeof(data->m_yaw);
    std::memcpy(&data->m_pitch, buffer, sizeof(data->m_pitch));
    buffer += sizeof(data->m_pitch);
    bytesRead += sizeof(data->m_pitch);
    std::memcpy(&data->m_roll, buffer, sizeof(data->m_roll));
    buffer += sizeof(data->m_roll);
    bytesRead += sizeof(data->m_roll);

    return data;
}