#include "f1telem/Decoder.hpp"

#include <cstring>

using namespace F1Telem;

Decoder::Decoder() noexcept { bytesRead = 0; }

bool Decoder::ValidateLastPacket(int bytesRecieved) noexcept {
    bool result = bytesRead == bytesRecieved;
    ResetByteCount();
    return result;
}

void Decoder::ResetByteCount() noexcept { bytesRead = 0; }

uint8_t Decoder::DecodePacketHeader(char** buffer, PacketHeader* header) {
    if (!buffer || !(*buffer) || !header) {
        return 255;
    }

    std::memcpy(&header->m_packetFormat, *buffer, sizeof(header->m_packetFormat));
    *buffer += sizeof(header->m_packetFormat);
    bytesRead += sizeof(header->m_packetFormat);

    if (header->m_packetFormat >= 2019) {
        std::memcpy(&header->m_gameMajorVersion, *buffer, sizeof(header->m_gameMajorVersion));
        *buffer += sizeof(header->m_gameMajorVersion);
        bytesRead += sizeof(header->m_gameMajorVersion);
        std::memcpy(&header->m_gameMinorVersion, *buffer, sizeof(header->m_gameMinorVersion));
        *buffer += sizeof(header->m_gameMinorVersion);
        bytesRead += sizeof(header->m_gameMajorVersion);
    }

    std::memcpy(&header->m_packetVersion, *buffer, sizeof(header->m_packetVersion));
    *buffer += sizeof(header->m_packetVersion);
    bytesRead += sizeof(header->m_packetVersion);
    std::memcpy(&header->m_packetId, *buffer, sizeof(header->m_packetId));
    *buffer += sizeof(header->m_packetId);
    bytesRead += sizeof(header->m_packetId);
    std::memcpy(&header->m_sessionUID, *buffer, sizeof(header->m_sessionUID));
    *buffer += sizeof(header->m_sessionUID);
    bytesRead += sizeof(header->m_sessionUID);
    std::memcpy(&header->m_sessionTime, *buffer, sizeof(header->m_sessionTime));
    *buffer += sizeof(header->m_sessionTime);
    bytesRead += sizeof(header->m_sessionTime);
    std::memcpy(&header->m_frameIdentifier, *buffer, sizeof(header->m_frameIdentifier));
    *buffer += sizeof(header->m_frameIdentifier);
    bytesRead += sizeof(header->m_frameIdentifier);
    std::memcpy(&header->m_playerCarIndex, *buffer, sizeof(header->m_playerCarIndex));
    *buffer += sizeof(header->m_playerCarIndex);
    bytesRead += sizeof(header->m_playerCarIndex);

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&header->m_secondaryPlayerCarIndex, *buffer, sizeof(header->m_secondaryPlayerCarIndex));
        *buffer += sizeof(header->m_secondaryPlayerCarIndex);
        bytesRead += sizeof(header->m_secondaryPlayerCarIndex);
    }

    return header->m_packetId;
}

uint8_t Decoder::getCarCount(PacketHeader* header) noexcept {
    // F12020 increased number of cars by 2, support this as default and set last 2 cars as null for earlier versions
    uint8_t carCount = F12020_CAR_COUNT;
    if (header->m_packetFormat < 2020) {
        carCount = LEGACY_CAR_COUNT;
    }
    return carCount;
}

bool Decoder::DecodePacketMotionData(char** buffer, PacketHeader* header, PacketMotionData* packet) {
    if (!buffer || !*(buffer) || !header || !packet) {
        return false;
    }

    packet->m_header = header;
    uint8_t carCount = getCarCount(header);

    for (uint8_t i = 0; i < carCount; i++) {
        decodeCarMotionData(buffer, &packet->m_carMotionData[i]);
    }

    for (auto& i : packet->m_suspensionPosition) {
        std::memcpy(&i, *buffer, sizeof(i));
        *buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_suspensionVelocity) {
        std::memcpy(&i, *buffer, sizeof(i));
        *buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_suspensionAcceleration) {
        std::memcpy(&i, *buffer, sizeof(i));
        *buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_wheelSpeed) {
        std::memcpy(&i, *buffer, sizeof(i));
        *buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    for (auto& i : packet->m_wheelSlip) {
        std::memcpy(&i, *buffer, sizeof(i));
        *buffer += sizeof(i);
        bytesRead += sizeof(i);
    }

    std::memcpy(&packet->m_localVelocityX, *buffer, sizeof(packet->m_localVelocityX));
    *buffer += sizeof(packet->m_localVelocityX);
    bytesRead += sizeof(packet->m_localVelocityX);
    std::memcpy(&packet->m_localVelocityY, *buffer, sizeof(packet->m_localVelocityY));
    *buffer += sizeof(packet->m_localVelocityY);
    bytesRead += sizeof(packet->m_localVelocityY);
    std::memcpy(&packet->m_localVelocityZ, *buffer, sizeof(packet->m_localVelocityZ));
    *buffer += sizeof(packet->m_localVelocityZ);
    bytesRead += sizeof(packet->m_localVelocityZ);

    std::memcpy(&packet->m_angularVelocityX, *buffer, sizeof(packet->m_angularVelocityX));
    *buffer += sizeof(packet->m_angularVelocityX);
    bytesRead += sizeof(packet->m_angularVelocityX);
    std::memcpy(&packet->m_angularVelocityY, *buffer, sizeof(packet->m_angularVelocityY));
    *buffer += sizeof(packet->m_angularVelocityY);
    bytesRead += sizeof(packet->m_angularVelocityY);
    std::memcpy(&packet->m_angularVelocityZ, *buffer, sizeof(packet->m_angularVelocityZ));
    *buffer += sizeof(packet->m_angularVelocityZ);
    bytesRead += sizeof(packet->m_angularVelocityZ);

    std::memcpy(&packet->m_angularAccelerationX, *buffer, sizeof(packet->m_angularAccelerationX));
    *buffer += sizeof(packet->m_angularAccelerationX);
    bytesRead += sizeof(packet->m_angularAccelerationX);
    std::memcpy(&packet->m_angularAccelerationY, *buffer, sizeof(packet->m_angularAccelerationY));
    *buffer += sizeof(packet->m_angularAccelerationY);
    bytesRead += sizeof(packet->m_angularAccelerationY);
    std::memcpy(&packet->m_angularAccelerationZ, *buffer, sizeof(packet->m_angularAccelerationZ));
    *buffer += sizeof(packet->m_angularAccelerationZ);
    bytesRead += sizeof(packet->m_angularAccelerationZ);

    std::memcpy(&packet->m_frontWheelsAngle, *buffer, sizeof(packet->m_frontWheelsAngle));
    *buffer += sizeof(packet->m_frontWheelsAngle);
    bytesRead += sizeof(packet->m_frontWheelsAngle);

    return true;
}

void Decoder::decodeCarMotionData(char** buffer, CarMotionData* data) {
    std::memcpy(&data->m_worldPositionX, *buffer, sizeof(data->m_worldPositionX));
    *buffer += sizeof(data->m_worldPositionX);
    bytesRead += sizeof(data->m_worldPositionX);
    std::memcpy(&data->m_worldPositionY, *buffer, sizeof(data->m_worldPositionY));
    *buffer += sizeof(data->m_worldPositionY);
    bytesRead += sizeof(data->m_worldPositionY);
    std::memcpy(&data->m_worldPositionZ, *buffer, sizeof(data->m_worldPositionZ));
    *buffer += sizeof(data->m_worldPositionZ);
    bytesRead += sizeof(data->m_worldPositionZ);

    std::memcpy(&data->m_worldVelocityX, *buffer, sizeof(data->m_worldVelocityX));
    *buffer += sizeof(data->m_worldVelocityX);
    bytesRead += sizeof(data->m_worldVelocityX);
    std::memcpy(&data->m_worldVelocityY, *buffer, sizeof(data->m_worldVelocityY));
    *buffer += sizeof(data->m_worldVelocityY);
    bytesRead += sizeof(data->m_worldVelocityY);
    std::memcpy(&data->m_worldVelocityZ, *buffer, sizeof(data->m_worldVelocityZ));
    *buffer += sizeof(data->m_worldVelocityZ);
    bytesRead += sizeof(data->m_worldVelocityZ);

    std::memcpy(&data->m_worldForwardDirX, *buffer, sizeof(data->m_worldForwardDirX));
    *buffer += sizeof(data->m_worldForwardDirX);
    bytesRead += sizeof(data->m_worldForwardDirX);
    std::memcpy(&data->m_worldForwardDirY, *buffer, sizeof(data->m_worldForwardDirY));
    *buffer += sizeof(data->m_worldForwardDirY);
    bytesRead += sizeof(data->m_worldForwardDirY);
    std::memcpy(&data->m_worldForwardDirZ, *buffer, sizeof(data->m_worldForwardDirZ));
    *buffer += sizeof(data->m_worldForwardDirZ);
    bytesRead += sizeof(data->m_worldForwardDirZ);

    std::memcpy(&data->m_worldRightDirX, *buffer, sizeof(data->m_worldRightDirX));
    *buffer += sizeof(data->m_worldRightDirX);
    bytesRead += sizeof(data->m_worldRightDirX);
    std::memcpy(&data->m_worldRightDirY, *buffer, sizeof(data->m_worldRightDirY));
    *buffer += sizeof(data->m_worldRightDirY);
    bytesRead += sizeof(data->m_worldRightDirY);
    std::memcpy(&data->m_worldRightDirZ, *buffer, sizeof(data->m_worldRightDirZ));
    *buffer += sizeof(data->m_worldRightDirZ);
    bytesRead += sizeof(data->m_worldRightDirZ);

    std::memcpy(&data->m_gForceLateral, *buffer, sizeof(data->m_gForceLateral));
    *buffer += sizeof(data->m_gForceLateral);
    bytesRead += sizeof(data->m_gForceLateral);
    std::memcpy(&data->m_gForceLongitudinal, *buffer, sizeof(data->m_gForceLongitudinal));
    *buffer += sizeof(data->m_gForceLongitudinal);
    bytesRead += sizeof(data->m_gForceLongitudinal);
    std::memcpy(&data->m_gForceVertical, *buffer, sizeof(data->m_gForceVertical));
    *buffer += sizeof(data->m_gForceVertical);
    bytesRead += sizeof(data->m_gForceVertical);

    std::memcpy(&data->m_yaw, *buffer, sizeof(data->m_yaw));
    *buffer += sizeof(data->m_yaw);
    bytesRead += sizeof(data->m_yaw);
    std::memcpy(&data->m_pitch, *buffer, sizeof(data->m_pitch));
    *buffer += sizeof(data->m_pitch);
    bytesRead += sizeof(data->m_pitch);
    std::memcpy(&data->m_roll, *buffer, sizeof(data->m_roll));
    *buffer += sizeof(data->m_roll);
    bytesRead += sizeof(data->m_roll);
}

bool Decoder::DecodePacketSessionData(char** buffer, PacketHeader* header, PacketSessionData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;

    std::memcpy(&packet->m_weather, *buffer, sizeof(packet->m_weather));
    *buffer += sizeof(packet->m_weather);
    bytesRead += sizeof(packet->m_weather);
    std::memcpy(&packet->m_trackTemperature, *buffer, sizeof(packet->m_trackTemperature));
    *buffer += sizeof(packet->m_trackTemperature);
    bytesRead += sizeof(packet->m_trackTemperature);
    std::memcpy(&packet->m_airTemperature, *buffer, sizeof(packet->m_airTemperature));
    *buffer += sizeof(packet->m_airTemperature);
    bytesRead += sizeof(packet->m_airTemperature);

    std::memcpy(&packet->m_totalLaps, *buffer, sizeof(packet->m_totalLaps));
    *buffer += sizeof(packet->m_totalLaps);
    bytesRead += sizeof(packet->m_totalLaps);
    std::memcpy(&packet->m_trackLength, *buffer, sizeof(packet->m_trackLength));
    *buffer += sizeof(packet->m_trackLength);
    bytesRead += sizeof(packet->m_trackLength);

    std::memcpy(&packet->m_sessionType, *buffer, sizeof(packet->m_sessionType));
    *buffer += sizeof(packet->m_sessionType);
    bytesRead += sizeof(packet->m_sessionType);
    std::memcpy(&packet->m_trackId, *buffer, sizeof(packet->m_trackId));
    *buffer += sizeof(packet->m_trackId);
    bytesRead += sizeof(packet->m_trackId);
    std::memcpy(&packet->m_formula, *buffer, sizeof(packet->m_formula));
    *buffer += sizeof(packet->m_formula);
    bytesRead += sizeof(packet->m_formula);
    std::memcpy(&packet->m_sessionTimeLeft, *buffer, sizeof(packet->m_sessionTimeLeft));
    *buffer += sizeof(packet->m_sessionTimeLeft);
    bytesRead += sizeof(packet->m_sessionTimeLeft);
    std::memcpy(&packet->m_sessionDuration, *buffer, sizeof(packet->m_sessionDuration));
    *buffer += sizeof(packet->m_sessionDuration);
    bytesRead += sizeof(packet->m_sessionDuration);

    std::memcpy(&packet->m_pitSpeedLimit, *buffer, sizeof(packet->m_pitSpeedLimit));
    *buffer += sizeof(packet->m_pitSpeedLimit);
    bytesRead += sizeof(packet->m_pitSpeedLimit);

    std::memcpy(&packet->m_gamePaused, *buffer, sizeof(packet->m_gamePaused));
    *buffer += sizeof(packet->m_gamePaused);
    bytesRead += sizeof(packet->m_gamePaused);
    std::memcpy(&packet->m_isSpectating, *buffer, sizeof(packet->m_isSpectating));
    *buffer += sizeof(packet->m_isSpectating);
    bytesRead += sizeof(packet->m_isSpectating);
    std::memcpy(&packet->m_spectatorCarIndex, *buffer, sizeof(packet->m_spectatorCarIndex));
    *buffer += sizeof(packet->m_spectatorCarIndex);
    bytesRead += sizeof(packet->m_spectatorCarIndex);
    std::memcpy(&packet->m_sliProNativeSupport, *buffer, sizeof(packet->m_sliProNativeSupport));
    *buffer += sizeof(packet->m_sliProNativeSupport);
    bytesRead += sizeof(packet->m_sliProNativeSupport);

    std::memcpy(&packet->m_numMarshalZones, *buffer, sizeof(packet->m_numMarshalZones));
    *buffer += sizeof(packet->m_numMarshalZones);
    bytesRead += sizeof(packet->m_numMarshalZones);

    for (uint8_t i = 0; i < MAX_MARSHAL_ZONES; i++) {
        decodeMarshalZoneData(buffer, &packet->m_marshalZones[i]);
    }

    std::memcpy(&packet->m_safetyCarStatus, *buffer, sizeof(packet->m_safetyCarStatus));
    *buffer += sizeof(packet->m_safetyCarStatus);
    bytesRead += sizeof(packet->m_safetyCarStatus);
    std::memcpy(&packet->m_networkGame, *buffer, sizeof(packet->m_networkGame));
    *buffer += sizeof(packet->m_networkGame);
    bytesRead += sizeof(packet->m_networkGame);

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&packet->m_numWeatherForecastSamples, *buffer, sizeof(packet->m_numWeatherForecastSamples));
        *buffer += sizeof(packet->m_numWeatherForecastSamples);
        bytesRead += sizeof(packet->m_numWeatherForecastSamples);

        for (uint8_t i = 0; i < MAX_WEATHER_FORECAST_SAMPLES; i++) {
            decodeWeatherForecastSampleData(buffer, &packet->m_weatherForecastSamples[i]);
        }
    }

    return true;
}

void Decoder::decodeMarshalZoneData(char** buffer, MarshalZone* data) {
    std::memcpy(&data->m_zoneStart, *buffer, sizeof(data->m_zoneStart));
    *buffer += sizeof(data->m_zoneStart);
    bytesRead += sizeof(data->m_zoneStart);
    std::memcpy(&data->m_zoneFlag, *buffer, sizeof(data->m_zoneFlag));
    *buffer += sizeof(data->m_zoneFlag);
    bytesRead += sizeof(data->m_zoneFlag);
}
void Decoder::decodeWeatherForecastSampleData(char** buffer, WeatherForecastSample* data) {
    std::memcpy(&data->m_sessionType, *buffer, sizeof(data->m_sessionType));
    *buffer += sizeof(data->m_sessionType);
    bytesRead += sizeof(data->m_sessionType);
    std::memcpy(&data->m_timeOffset, *buffer, sizeof(data->m_timeOffset));
    *buffer += sizeof(data->m_timeOffset);
    bytesRead += sizeof(data->m_timeOffset);
    std::memcpy(&data->m_weather, *buffer, sizeof(data->m_weather));
    *buffer += sizeof(data->m_weather);
    bytesRead += sizeof(data->m_weather);
    std::memcpy(&data->m_trackTemperature, *buffer, sizeof(data->m_trackTemperature));
    *buffer += sizeof(data->m_trackTemperature);
    bytesRead += sizeof(data->m_trackTemperature);
    std::memcpy(&data->m_airTemperature, *buffer, sizeof(data->m_airTemperature));
    *buffer += sizeof(data->m_airTemperature);
    bytesRead += sizeof(data->m_airTemperature);
}

bool Decoder::DecodePacketLapData(char** buffer, PacketHeader* header, PacketLapData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;

    uint8_t carCount = getCarCount(header);

    for (uint8_t i = 0; i < carCount; i++) {
        decodeLapData(buffer, header, &packet->m_lapData[i]);
    }

    return true;
}

void Decoder::decodeLapData(char** buffer, PacketHeader* header, LapData* data) {
    std::memcpy(&data->m_lastLapTime, *buffer, sizeof(data->m_lastLapTime));
    *buffer += sizeof(data->m_lastLapTime);
    bytesRead += sizeof(data->m_lastLapTime);
    std::memcpy(&data->m_currentLapTime, *buffer, sizeof(data->m_currentLapTime));
    *buffer += sizeof(data->m_currentLapTime);
    bytesRead += sizeof(data->m_currentLapTime);

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_sector1TimeInMS, *buffer, sizeof(data->m_sector1TimeInMS));
        *buffer += sizeof(data->m_sector1TimeInMS);
        bytesRead += sizeof(data->m_sector1TimeInMS);
        std::memcpy(&data->m_sector2TimeInMS, *buffer, sizeof(data->m_sector2TimeInMS));
        *buffer += sizeof(data->m_sector2TimeInMS);
        bytesRead += sizeof(data->m_sector2TimeInMS);
    }

    std::memcpy(&data->m_bestLapTime, *buffer, sizeof(data->m_bestLapTime));
    *buffer += sizeof(data->m_bestLapTime);
    bytesRead += sizeof(data->m_bestLapTime);

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_bestLapNum, *buffer, sizeof(data->m_bestLapNum));
        *buffer += sizeof(data->m_bestLapNum);
        bytesRead += sizeof(data->m_bestLapNum);
    }

    // recorded in seconds for <= f1 2019
    std::memcpy(&data->m_bestLapSector1TimeInMS, *buffer, sizeof(data->m_bestLapSector1TimeInMS));
    *buffer += sizeof(data->m_bestLapSector1TimeInMS);
    bytesRead += sizeof(data->m_bestLapSector1TimeInMS);
    std::memcpy(&data->m_bestLapSector2TimeInMS, *buffer, sizeof(data->m_bestLapSector2TimeInMS));
    *buffer += sizeof(data->m_bestLapSector2TimeInMS);
    bytesRead += sizeof(data->m_bestLapSector2TimeInMS);

    // f1 2020
    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_bestLapSector3TimeInMS, *buffer, sizeof(data->m_bestLapSector3TimeInMS));
        *buffer += sizeof(data->m_bestLapSector3TimeInMS);
        bytesRead += sizeof(data->m_bestLapSector3TimeInMS);
        std::memcpy(&data->m_bestOverallSector1TimeInMS, *buffer, sizeof(data->m_bestOverallSector1TimeInMS));
        *buffer += sizeof(data->m_bestOverallSector1TimeInMS);
        bytesRead += sizeof(data->m_bestOverallSector1TimeInMS);
        std::memcpy(&data->m_bestOverallSector1LapNum, *buffer, sizeof(data->m_bestOverallSector1LapNum));
        *buffer += sizeof(data->m_bestOverallSector1LapNum);
        bytesRead += sizeof(data->m_bestOverallSector1LapNum);
        std::memcpy(&data->m_bestOverallSector2TimeInMS, *buffer, sizeof(data->m_bestOverallSector2TimeInMS));
        *buffer += sizeof(data->m_bestOverallSector2TimeInMS);
        bytesRead += sizeof(data->m_bestOverallSector2TimeInMS);
        std::memcpy(&data->m_bestOverallSector2LapNum, *buffer, sizeof(data->m_bestOverallSector2LapNum));
        *buffer += sizeof(data->m_bestOverallSector2LapNum);
        bytesRead += sizeof(data->m_bestOverallSector2LapNum);
        std::memcpy(&data->m_bestOverallSector3TimeInMS, *buffer, sizeof(data->m_bestOverallSector3TimeInMS));
        *buffer += sizeof(data->m_bestOverallSector3TimeInMS);
        bytesRead += sizeof(data->m_bestOverallSector3TimeInMS);
        std::memcpy(&data->m_bestOverallSector3LapNum, *buffer, sizeof(data->m_bestOverallSector3LapNum));
        *buffer += sizeof(data->m_bestOverallSector3LapNum);
        bytesRead += sizeof(data->m_bestOverallSector3LapNum);
    }

    std::memcpy(&data->m_lapDistance, *buffer, sizeof(data->m_lapDistance));
    *buffer += sizeof(data->m_lapDistance);
    bytesRead += sizeof(data->m_lapDistance);
    std::memcpy(&data->m_totalDistance, *buffer, sizeof(data->m_totalDistance));
    *buffer += sizeof(data->m_totalDistance);
    bytesRead += sizeof(data->m_totalDistance);
    std::memcpy(&data->m_safetyCarDelta, *buffer, sizeof(data->m_safetyCarDelta));
    *buffer += sizeof(data->m_safetyCarDelta);
    bytesRead += sizeof(data->m_safetyCarDelta);

    std::memcpy(&data->m_carPosition, *buffer, sizeof(data->m_carPosition));
    *buffer += sizeof(data->m_carPosition);
    bytesRead += sizeof(data->m_carPosition);
    std::memcpy(&data->m_currentLapNum, *buffer, sizeof(data->m_currentLapNum));
    *buffer += sizeof(data->m_currentLapNum);
    bytesRead += sizeof(data->m_currentLapNum);
    std::memcpy(&data->m_pitStatus, *buffer, sizeof(data->m_pitStatus));
    *buffer += sizeof(data->m_pitStatus);
    bytesRead += sizeof(data->m_pitStatus);
    std::memcpy(&data->m_sector, *buffer, sizeof(data->m_sector));
    *buffer += sizeof(data->m_sector);
    bytesRead += sizeof(data->m_sector);
    std::memcpy(&data->m_currentLapInvalid, *buffer, sizeof(data->m_currentLapInvalid));
    *buffer += sizeof(data->m_currentLapInvalid);
    bytesRead += sizeof(data->m_currentLapInvalid);
    std::memcpy(&data->m_penalties, *buffer, sizeof(data->m_penalties));
    *buffer += sizeof(data->m_penalties);
    bytesRead += sizeof(data->m_penalties);
    std::memcpy(&data->m_gridPosition, *buffer, sizeof(data->m_gridPosition));
    *buffer += sizeof(data->m_gridPosition);
    bytesRead += sizeof(data->m_gridPosition);
    std::memcpy(&data->m_driverStatus, *buffer, sizeof(data->m_driverStatus));
    *buffer += sizeof(data->m_driverStatus);
    bytesRead += sizeof(data->m_driverStatus);
    std::memcpy(&data->m_resultStatus, *buffer, sizeof(data->m_resultStatus));
    *buffer += sizeof(data->m_resultStatus);
    bytesRead += sizeof(data->m_resultStatus);
}

bool Decoder::DecodePacketEventData(char** buffer, PacketHeader* header, PacketEventData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;
    packet->m_eventCode = readEventCode(buffer);

    switch (packet->m_eventCode) {
        case SessionStarted: {
            break; // no event data to read
        }
        case SessionEnded: {
            break; // no event data to read
        }
        case SpeedTrapTriggered: {
            std::memcpy(&packet->m_eventDetails.SpeedTrap.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.SpeedTrap.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.SpeedTrap.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.SpeedTrap.vehicleIdx);
            std::memcpy(&packet->m_eventDetails.SpeedTrap.speed, *buffer, sizeof(packet->m_eventDetails.SpeedTrap.speed));
            *buffer += sizeof(packet->m_eventDetails.SpeedTrap.speed);
            bytesRead += sizeof(packet->m_eventDetails.SpeedTrap.speed);
            break;
        }
        case FastestLap: {
            std::memcpy(&packet->m_eventDetails.FastestLap.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.FastestLap.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.FastestLap.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.FastestLap.vehicleIdx);
            std::memcpy(&packet->m_eventDetails.FastestLap.lapTime, *buffer, sizeof(packet->m_eventDetails.FastestLap.lapTime));
            *buffer += sizeof(packet->m_eventDetails.FastestLap.lapTime);
            bytesRead += sizeof(packet->m_eventDetails.FastestLap.lapTime);
            break;
        }
        case RaceWinner: {
            std::memcpy(&packet->m_eventDetails.RaceWinner.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.RaceWinner.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.RaceWinner.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.RaceWinner.vehicleIdx);
            break;
        }
        case Retirement: {
            std::memcpy(&packet->m_eventDetails.Retirement.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.Retirement.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.Retirement.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.Retirement.vehicleIdx);
            break;
        }
        case DRSEnabled: {
            break; // no event data to read
        }
        case DRSDisabled: {
            break; // no event data to read
        }
        case TeamMateInPits: {
            std::memcpy(&packet->m_eventDetails.TeamMateInPits.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.TeamMateInPits.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.TeamMateInPits.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.TeamMateInPits.vehicleIdx);
            break;
        }
        case ChequeredFlag: {
            break; // no event data to read
        }
        case PenaltyIssued: {
            std::memcpy(&packet->m_eventDetails.Penalty.penaltyType, *buffer, sizeof(packet->m_eventDetails.Penalty.penaltyType));
            *buffer += sizeof(packet->m_eventDetails.Penalty.penaltyType);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.penaltyType);
            std::memcpy(&packet->m_eventDetails.Penalty.infringementType, *buffer, sizeof(packet->m_eventDetails.Penalty.infringementType));
            *buffer += sizeof(packet->m_eventDetails.Penalty.infringementType);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.infringementType);
            std::memcpy(&packet->m_eventDetails.Penalty.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.Penalty.vehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.Penalty.vehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.vehicleIdx);
            std::memcpy(&packet->m_eventDetails.Penalty.otherVehicleIdx, *buffer, sizeof(packet->m_eventDetails.Penalty.otherVehicleIdx));
            *buffer += sizeof(packet->m_eventDetails.Penalty.otherVehicleIdx);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.otherVehicleIdx);
            std::memcpy(&packet->m_eventDetails.Penalty.time, *buffer, sizeof(packet->m_eventDetails.Penalty.time));
            *buffer += sizeof(packet->m_eventDetails.Penalty.time);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.time);
            std::memcpy(&packet->m_eventDetails.Penalty.lapNum, *buffer, sizeof(packet->m_eventDetails.Penalty.lapNum));
            *buffer += sizeof(packet->m_eventDetails.Penalty.lapNum);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.lapNum);
            std::memcpy(&packet->m_eventDetails.Penalty.placesGained, *buffer, sizeof(packet->m_eventDetails.Penalty.placesGained));
            *buffer += sizeof(packet->m_eventDetails.Penalty.placesGained);
            bytesRead += sizeof(packet->m_eventDetails.Penalty.placesGained);
            break;
        }
        default: {
            // raise exception?
            break;
        }
    }

    /* manually set data read as this will be different depending on the event,
        just have to take the chance of an invalid packet until I can think of
        a better way of handling this */
    switch (packet->m_header->m_packetFormat) {
        case 2020: {
            bytesRead = 35;
        }
        case 2019: {
            bytesRead = 32;
        }
        case 2018: {
            bytesRead = 25;
        }
    }

    return true;
}

EventCode Decoder::readEventCode(char** buffer) {
    uint8_t eventStringCode[4];
    std::memcpy(&eventStringCode[0], *buffer, sizeof(uint8_t));
    *buffer += sizeof(eventStringCode);
    bytesRead += sizeof(eventStringCode);
    std::memcpy(&eventStringCode[1], *buffer, sizeof(uint8_t));
    *buffer += sizeof(eventStringCode);
    bytesRead += sizeof(eventStringCode);
    std::memcpy(&eventStringCode[2], *buffer, sizeof(uint8_t));
    *buffer += sizeof(eventStringCode);
    bytesRead += sizeof(eventStringCode);
    std::memcpy(&eventStringCode[3], *buffer, sizeof(uint8_t));
    *buffer += sizeof(eventStringCode);
    bytesRead += sizeof(eventStringCode);

    switch (eventStringCode[0]) {
        case 0x53: {
            switch (eventStringCode[1]) {
                case 0x53: {
                    return SessionStarted; // SSTA
                }
                case 0x45: {
                    return SessionEnded; // SEND
                }
                case 0x50: {
                    return SpeedTrapTriggered; // SPTP
                }
            }
        }
        case 0x46: {
            return FastestLap; // FTLP
        }
        case 0x52: {
            switch (eventStringCode[1]) {
                case 0x43: {
                    return RaceWinner; // RCWN
                }
                case 0x54: {
                    return Retirement; // RTMT
                }
            }
        }
        case 0x44: {
            switch (eventStringCode[3]) {
                case 0x45: {
                    return DRSEnabled; // DRSE
                }
                case 0x44: {
                    return DRSDisabled; // DRSD
                }
            }
        }
        case 0x54: {
            return TeamMateInPits; // TMPT
        }
        case 0x43: {
            return ChequeredFlag; // CHQF
        }
        case 0x50: {
            return PenaltyIssued; // PENA
        }
    }

    return Unknown;
}