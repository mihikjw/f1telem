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

bool Decoder::DecodePacketSessionData(char* buffer, PacketHeader* header, PacketSessionData* packet) {
    if (!buffer || !header || !packet) {
        return false;
    }

    packet->m_header = header;

    // -28 (\344) in buffer, 228 (\344) after memcpy? wtffffff?
    std::memcpy(&packet->m_weather, buffer, sizeof(packet->m_weather));
    buffer += sizeof(packet->m_weather);
    bytesRead += sizeof(packet->m_weather);
    std::memcpy(&packet->m_trackTemperature, buffer, sizeof(packet->m_trackTemperature));
    buffer += sizeof(packet->m_trackTemperature);
    bytesRead += sizeof(packet->m_trackTemperature);
    std::memcpy(&packet->m_airTemperature, buffer, sizeof(packet->m_airTemperature));
    buffer += sizeof(packet->m_airTemperature);
    bytesRead += sizeof(packet->m_airTemperature);

    std::memcpy(&packet->m_totalLaps, buffer, sizeof(packet->m_totalLaps));
    buffer += sizeof(packet->m_totalLaps);
    bytesRead += sizeof(packet->m_totalLaps);
    std::memcpy(&packet->m_trackLength, buffer, sizeof(packet->m_trackLength));
    buffer += sizeof(packet->m_trackLength);
    bytesRead += sizeof(packet->m_trackLength);

    std::memcpy(&packet->m_sessionType, buffer, sizeof(packet->m_sessionType));
    buffer += sizeof(packet->m_sessionType);
    bytesRead += sizeof(packet->m_sessionType);
    std::memcpy(&packet->m_trackId, buffer, sizeof(packet->m_trackId));
    buffer += sizeof(packet->m_trackId);
    bytesRead += sizeof(packet->m_trackId);
    std::memcpy(&packet->m_formula, buffer, sizeof(packet->m_formula));
    buffer += sizeof(packet->m_formula);
    bytesRead += sizeof(packet->m_formula);
    std::memcpy(&packet->m_sessionTimeLeft, buffer, sizeof(packet->m_sessionTimeLeft));
    buffer += sizeof(packet->m_sessionTimeLeft);
    bytesRead += sizeof(packet->m_sessionTimeLeft);
    std::memcpy(&packet->m_sessionDuration, buffer, sizeof(packet->m_sessionDuration));
    buffer += sizeof(packet->m_sessionDuration);
    bytesRead += sizeof(packet->m_sessionDuration);

    std::memcpy(&packet->m_pitSpeedLimit, buffer, sizeof(packet->m_pitSpeedLimit));
    buffer += sizeof(packet->m_pitSpeedLimit);
    bytesRead += sizeof(packet->m_pitSpeedLimit);

    std::memcpy(&packet->m_gamePaused, buffer, sizeof(packet->m_gamePaused));
    buffer += sizeof(packet->m_gamePaused);
    bytesRead += sizeof(packet->m_gamePaused);
    std::memcpy(&packet->m_isSpectating, buffer, sizeof(packet->m_isSpectating));
    buffer += sizeof(packet->m_isSpectating);
    bytesRead += sizeof(packet->m_isSpectating);
    std::memcpy(&packet->m_spectatorCarIndex, buffer, sizeof(packet->m_spectatorCarIndex));
    buffer += sizeof(packet->m_spectatorCarIndex);
    bytesRead += sizeof(packet->m_spectatorCarIndex);
    std::memcpy(&packet->m_sliProNativeSupport, buffer, sizeof(packet->m_sliProNativeSupport));
    buffer += sizeof(packet->m_sliProNativeSupport);
    bytesRead += sizeof(packet->m_sliProNativeSupport);

    std::memcpy(&packet->m_numMarshalZones, buffer, sizeof(packet->m_numMarshalZones));
    buffer += sizeof(packet->m_numMarshalZones);
    bytesRead += sizeof(packet->m_numMarshalZones);

    // might get weird data after this if all zones in array are always sent - requires testing
    for (uint8_t i = 0; i < packet->m_numMarshalZones; i++) {
        packet->m_marshalZones[i] = decodeMarshalZoneData(buffer);
    }

    std::memcpy(&packet->m_safetyCarStatus, buffer, sizeof(packet->m_safetyCarStatus));
    buffer += sizeof(packet->m_safetyCarStatus);
    bytesRead += sizeof(packet->m_safetyCarStatus);
    std::memcpy(&packet->m_networkGame, buffer, sizeof(packet->m_networkGame));
    buffer += sizeof(packet->m_networkGame);
    bytesRead += sizeof(packet->m_networkGame);
    std::memcpy(&packet->m_numWeatherForecastSamples, buffer, sizeof(packet->m_numWeatherForecastSamples));
    buffer += sizeof(packet->m_numWeatherForecastSamples);
    bytesRead += sizeof(packet->m_numWeatherForecastSamples);

    for (uint8_t i = 0; i < packet->m_numWeatherForecastSamples; i++) {
        packet->m_weatherForecastSamples[i] = decodeWeatherForecastSampleData(buffer);
    }

    return true;
}

MarshalZone* Decoder::decodeMarshalZoneData(char* buffer) {
    auto data = new MarshalZone();
    std::memcpy(&data->m_zoneStart, buffer, sizeof(data->m_zoneStart));
    buffer += sizeof(data->m_zoneStart);
    bytesRead += sizeof(data->m_zoneStart);
    std::memcpy(&data->m_zoneFlag, buffer, sizeof(data->m_zoneFlag));
    buffer += sizeof(data->m_zoneFlag);
    bytesRead += sizeof(data->m_zoneFlag);
    return data;
}

WeatherForecastSample* Decoder::decodeWeatherForecastSampleData(char* buffer) {
    auto data = new WeatherForecastSample();
    std::memcpy(&data->m_sessionType, buffer, sizeof(data->m_sessionType));
    buffer += sizeof(data->m_sessionType);
    bytesRead += sizeof(data->m_sessionType);
    std::memcpy(&data->m_timeOffset, buffer, sizeof(data->m_timeOffset));
    buffer += sizeof(data->m_timeOffset);
    bytesRead += sizeof(data->m_timeOffset);
    std::memcpy(&data->m_weather, buffer, sizeof(data->m_weather));
    buffer += sizeof(data->m_weather);
    bytesRead += sizeof(data->m_weather);
    std::memcpy(&data->m_trackTemperature, buffer, sizeof(data->m_trackTemperature));
    buffer += sizeof(data->m_trackTemperature);
    bytesRead += sizeof(data->m_trackTemperature);
    std::memcpy(&data->m_airTemperature, buffer, sizeof(data->m_airTemperature));
    buffer += sizeof(data->m_airTemperature);
    bytesRead += sizeof(data->m_airTemperature);
    return data;
}