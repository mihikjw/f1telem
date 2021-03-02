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

void Decoder::incrementBuffer(char** buffer, size_t size) {
    *buffer += size;
    bytesRead += size;
}

uint8_t Decoder::DecodePacketHeader(char** buffer, PacketHeader* header) {
    if (!buffer || !(*buffer) || !header) {
        return 255;
    }

    std::memcpy(&header->m_packetFormat, *buffer, sizeof(header->m_packetFormat));
    incrementBuffer(buffer, sizeof(header->m_packetFormat));

    if (header->m_packetFormat >= 2019) {
        std::memcpy(&header->m_gameMajorVersion, *buffer, sizeof(header->m_gameMajorVersion));
        incrementBuffer(buffer, sizeof(header->m_gameMajorVersion));
        std::memcpy(&header->m_gameMinorVersion, *buffer, sizeof(header->m_gameMinorVersion));
        incrementBuffer(buffer, sizeof(header->m_gameMinorVersion));
    }

    std::memcpy(&header->m_packetVersion, *buffer, sizeof(header->m_packetVersion));
    incrementBuffer(buffer, sizeof(header->m_packetVersion));
    std::memcpy(&header->m_packetId, *buffer, sizeof(header->m_packetId));
    incrementBuffer(buffer, sizeof(header->m_packetId));
    std::memcpy(&header->m_sessionUID, *buffer, sizeof(header->m_sessionUID));
    incrementBuffer(buffer, sizeof(header->m_sessionUID));
    std::memcpy(&header->m_sessionTime, *buffer, sizeof(header->m_sessionTime));
    incrementBuffer(buffer, sizeof(header->m_sessionTime));
    std::memcpy(&header->m_frameIdentifier, *buffer, sizeof(header->m_frameIdentifier));
    incrementBuffer(buffer, sizeof(header->m_frameIdentifier));
    std::memcpy(&header->m_playerCarIndex, *buffer, sizeof(header->m_playerCarIndex));
    incrementBuffer(buffer, sizeof(header->m_playerCarIndex));

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&header->m_secondaryPlayerCarIndex, *buffer, sizeof(header->m_secondaryPlayerCarIndex));
        incrementBuffer(buffer, sizeof(header->m_secondaryPlayerCarIndex));
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
        incrementBuffer(buffer, sizeof(i));
    }

    for (auto& i : packet->m_suspensionVelocity) {
        std::memcpy(&i, *buffer, sizeof(i));
        incrementBuffer(buffer, sizeof(i));
    }

    for (auto& i : packet->m_suspensionAcceleration) {
        std::memcpy(&i, *buffer, sizeof(i));
        incrementBuffer(buffer, sizeof(i));
    }

    for (auto& i : packet->m_wheelSpeed) {
        std::memcpy(&i, *buffer, sizeof(i));
        incrementBuffer(buffer, sizeof(i));
    }

    for (auto& i : packet->m_wheelSlip) {
        std::memcpy(&i, *buffer, sizeof(i));
        incrementBuffer(buffer, sizeof(i));
    }

    std::memcpy(&packet->m_localVelocityX, *buffer, sizeof(packet->m_localVelocityX));
    incrementBuffer(buffer, sizeof(packet->m_localVelocityX));
    std::memcpy(&packet->m_localVelocityY, *buffer, sizeof(packet->m_localVelocityY));
    incrementBuffer(buffer, sizeof(packet->m_localVelocityY));
    std::memcpy(&packet->m_localVelocityZ, *buffer, sizeof(packet->m_localVelocityZ));
    incrementBuffer(buffer, sizeof(packet->m_localVelocityZ));

    std::memcpy(&packet->m_angularVelocityX, *buffer, sizeof(packet->m_angularVelocityX));
    incrementBuffer(buffer, sizeof(packet->m_angularVelocityX));
    std::memcpy(&packet->m_angularVelocityY, *buffer, sizeof(packet->m_angularVelocityY));
    incrementBuffer(buffer, sizeof(packet->m_angularVelocityY));
    std::memcpy(&packet->m_angularVelocityZ, *buffer, sizeof(packet->m_angularVelocityZ));
    incrementBuffer(buffer, sizeof(packet->m_angularVelocityZ));

    std::memcpy(&packet->m_angularAccelerationX, *buffer, sizeof(packet->m_angularAccelerationX));
    incrementBuffer(buffer, sizeof(packet->m_angularAccelerationX));
    std::memcpy(&packet->m_angularAccelerationY, *buffer, sizeof(packet->m_angularAccelerationY));
    incrementBuffer(buffer, sizeof(packet->m_angularAccelerationY));
    std::memcpy(&packet->m_angularAccelerationZ, *buffer, sizeof(packet->m_angularAccelerationZ));
    incrementBuffer(buffer, sizeof(packet->m_angularAccelerationZ));

    std::memcpy(&packet->m_frontWheelsAngle, *buffer, sizeof(packet->m_frontWheelsAngle));
    incrementBuffer(buffer, sizeof(packet->m_frontWheelsAngle));

    return true;
}

void Decoder::decodeCarMotionData(char** buffer, CarMotionData* data) {
    std::memcpy(&data->m_worldPositionX, *buffer, sizeof(data->m_worldPositionX));
    incrementBuffer(buffer, sizeof(data->m_worldPositionX));
    std::memcpy(&data->m_worldPositionY, *buffer, sizeof(data->m_worldPositionY));
    incrementBuffer(buffer, sizeof(data->m_worldPositionY));
    std::memcpy(&data->m_worldPositionZ, *buffer, sizeof(data->m_worldPositionZ));
    incrementBuffer(buffer, sizeof(data->m_worldPositionZ));

    std::memcpy(&data->m_worldVelocityX, *buffer, sizeof(data->m_worldVelocityX));
    incrementBuffer(buffer, sizeof(data->m_worldVelocityX));
    std::memcpy(&data->m_worldVelocityY, *buffer, sizeof(data->m_worldVelocityY));
    incrementBuffer(buffer, sizeof(data->m_worldVelocityY));
    std::memcpy(&data->m_worldVelocityZ, *buffer, sizeof(data->m_worldVelocityZ));
    incrementBuffer(buffer, sizeof(data->m_worldVelocityZ));

    std::memcpy(&data->m_worldForwardDirX, *buffer, sizeof(data->m_worldForwardDirX));
    incrementBuffer(buffer, sizeof(data->m_worldForwardDirX));
    std::memcpy(&data->m_worldForwardDirY, *buffer, sizeof(data->m_worldForwardDirY));
    incrementBuffer(buffer, sizeof(data->m_worldForwardDirY));
    std::memcpy(&data->m_worldForwardDirZ, *buffer, sizeof(data->m_worldForwardDirZ));
    incrementBuffer(buffer, sizeof(data->m_worldForwardDirZ));

    std::memcpy(&data->m_worldRightDirX, *buffer, sizeof(data->m_worldRightDirX));
    incrementBuffer(buffer, sizeof(data->m_worldRightDirX));
    std::memcpy(&data->m_worldRightDirY, *buffer, sizeof(data->m_worldRightDirY));
    incrementBuffer(buffer, sizeof(data->m_worldRightDirY));
    std::memcpy(&data->m_worldRightDirZ, *buffer, sizeof(data->m_worldRightDirZ));
    incrementBuffer(buffer, sizeof(data->m_worldRightDirZ));

    std::memcpy(&data->m_gForceLateral, *buffer, sizeof(data->m_gForceLateral));
    incrementBuffer(buffer, sizeof(data->m_gForceLateral));
    std::memcpy(&data->m_gForceLongitudinal, *buffer, sizeof(data->m_gForceLongitudinal));
    incrementBuffer(buffer, sizeof(data->m_gForceLongitudinal));
    std::memcpy(&data->m_gForceVertical, *buffer, sizeof(data->m_gForceVertical));
    incrementBuffer(buffer, sizeof(data->m_gForceVertical));

    std::memcpy(&data->m_yaw, *buffer, sizeof(data->m_yaw));
    incrementBuffer(buffer, sizeof(data->m_yaw));
    std::memcpy(&data->m_pitch, *buffer, sizeof(data->m_pitch));
    incrementBuffer(buffer, sizeof(data->m_pitch));
    std::memcpy(&data->m_roll, *buffer, sizeof(data->m_roll));
    incrementBuffer(buffer, sizeof(data->m_roll));
}

bool Decoder::DecodePacketSessionData(char** buffer, PacketHeader* header, PacketSessionData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;

    std::memcpy(&packet->m_weather, *buffer, sizeof(packet->m_weather));
    incrementBuffer(buffer, sizeof(packet->m_weather));
    std::memcpy(&packet->m_trackTemperature, *buffer, sizeof(packet->m_trackTemperature));
    incrementBuffer(buffer, sizeof(packet->m_trackTemperature));
    std::memcpy(&packet->m_airTemperature, *buffer, sizeof(packet->m_airTemperature));
    incrementBuffer(buffer, sizeof(packet->m_airTemperature));

    std::memcpy(&packet->m_totalLaps, *buffer, sizeof(packet->m_totalLaps));
    incrementBuffer(buffer, sizeof(packet->m_totalLaps));
    std::memcpy(&packet->m_trackLength, *buffer, sizeof(packet->m_trackLength));
    incrementBuffer(buffer, sizeof(packet->m_trackLength));

    std::memcpy(&packet->m_sessionType, *buffer, sizeof(packet->m_sessionType));
    incrementBuffer(buffer, sizeof(packet->m_sessionType));
    std::memcpy(&packet->m_trackId, *buffer, sizeof(packet->m_trackId));
    incrementBuffer(buffer, sizeof(packet->m_trackId));
    std::memcpy(&packet->m_formula, *buffer, sizeof(packet->m_formula));
    incrementBuffer(buffer, sizeof(packet->m_formula));
    std::memcpy(&packet->m_sessionTimeLeft, *buffer, sizeof(packet->m_sessionTimeLeft));
    incrementBuffer(buffer, sizeof(packet->m_sessionTimeLeft));
    std::memcpy(&packet->m_sessionDuration, *buffer, sizeof(packet->m_sessionDuration));
    incrementBuffer(buffer, sizeof(packet->m_sessionDuration));

    std::memcpy(&packet->m_pitSpeedLimit, *buffer, sizeof(packet->m_pitSpeedLimit));
    incrementBuffer(buffer, sizeof(packet->m_pitSpeedLimit));

    std::memcpy(&packet->m_gamePaused, *buffer, sizeof(packet->m_gamePaused));
    incrementBuffer(buffer, sizeof(packet->m_gamePaused));
    std::memcpy(&packet->m_isSpectating, *buffer, sizeof(packet->m_isSpectating));
    incrementBuffer(buffer, sizeof(packet->m_isSpectating));
    std::memcpy(&packet->m_spectatorCarIndex, *buffer, sizeof(packet->m_spectatorCarIndex));
    incrementBuffer(buffer, sizeof(packet->m_spectatorCarIndex));
    std::memcpy(&packet->m_sliProNativeSupport, *buffer, sizeof(packet->m_sliProNativeSupport));
    incrementBuffer(buffer, sizeof(packet->m_sliProNativeSupport));

    std::memcpy(&packet->m_numMarshalZones, *buffer, sizeof(packet->m_numMarshalZones));
    incrementBuffer(buffer, sizeof(packet->m_numMarshalZones));

    for (uint8_t i = 0; i < MAX_MARSHAL_ZONES; i++) {
        decodeMarshalZoneData(buffer, &packet->m_marshalZones[i]);
    }

    std::memcpy(&packet->m_safetyCarStatus, *buffer, sizeof(packet->m_safetyCarStatus));
    incrementBuffer(buffer, sizeof(packet->m_safetyCarStatus));
    std::memcpy(&packet->m_networkGame, *buffer, sizeof(packet->m_networkGame));
    incrementBuffer(buffer, sizeof(packet->m_networkGame));

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&packet->m_numWeatherForecastSamples, *buffer, sizeof(packet->m_numWeatherForecastSamples));
        incrementBuffer(buffer, sizeof(packet->m_numWeatherForecastSamples));

        for (uint8_t i = 0; i < MAX_WEATHER_FORECAST_SAMPLES; i++) {
            decodeWeatherForecastSampleData(buffer, &packet->m_weatherForecastSamples[i]);
        }
    }

    return true;
}

void Decoder::decodeMarshalZoneData(char** buffer, MarshalZone* data) {
    std::memcpy(&data->m_zoneStart, *buffer, sizeof(data->m_zoneStart));
    incrementBuffer(buffer, sizeof(data->m_zoneStart));
    std::memcpy(&data->m_zoneFlag, *buffer, sizeof(data->m_zoneFlag));
    incrementBuffer(buffer, sizeof(data->m_zoneFlag));
}
void Decoder::decodeWeatherForecastSampleData(char** buffer, WeatherForecastSample* data) {
    std::memcpy(&data->m_sessionType, *buffer, sizeof(data->m_sessionType));
    incrementBuffer(buffer, sizeof(data->m_sessionType));
    std::memcpy(&data->m_timeOffset, *buffer, sizeof(data->m_timeOffset));
    incrementBuffer(buffer, sizeof(data->m_timeOffset));
    std::memcpy(&data->m_weather, *buffer, sizeof(data->m_weather));
    incrementBuffer(buffer, sizeof(data->m_weather));
    std::memcpy(&data->m_trackTemperature, *buffer, sizeof(data->m_trackTemperature));
    incrementBuffer(buffer, sizeof(data->m_trackTemperature));
    std::memcpy(&data->m_airTemperature, *buffer, sizeof(data->m_airTemperature));
    incrementBuffer(buffer, sizeof(data->m_airTemperature));
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
    incrementBuffer(buffer, sizeof(data->m_lastLapTime));
    std::memcpy(&data->m_currentLapTime, *buffer, sizeof(data->m_currentLapTime));
    incrementBuffer(buffer, sizeof(data->m_currentLapTime));

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_sector1TimeInMS, *buffer, sizeof(data->m_sector1TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_sector1TimeInMS));
        std::memcpy(&data->m_sector2TimeInMS, *buffer, sizeof(data->m_sector2TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_sector2TimeInMS));
    }

    std::memcpy(&data->m_bestLapTime, *buffer, sizeof(data->m_bestLapTime));
    incrementBuffer(buffer, sizeof(data->m_bestLapTime));

    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_bestLapNum, *buffer, sizeof(data->m_bestLapNum));
        incrementBuffer(buffer, sizeof(data->m_bestLapNum));
    }

    // recorded in seconds for <= f1 2019
    std::memcpy(&data->m_bestLapSector1TimeInMS, *buffer, sizeof(data->m_bestLapSector1TimeInMS));
    incrementBuffer(buffer, sizeof(data->m_bestLapSector1TimeInMS));
    std::memcpy(&data->m_bestLapSector2TimeInMS, *buffer, sizeof(data->m_bestLapSector2TimeInMS));
    incrementBuffer(buffer, sizeof(data->m_bestLapSector2TimeInMS));

    // f1 2020
    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_bestLapSector3TimeInMS, *buffer, sizeof(data->m_bestLapSector3TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_bestLapSector3TimeInMS));
        std::memcpy(&data->m_bestOverallSector1TimeInMS, *buffer, sizeof(data->m_bestOverallSector1TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector1TimeInMS));
        std::memcpy(&data->m_bestOverallSector1LapNum, *buffer, sizeof(data->m_bestOverallSector1LapNum));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector1LapNum));
        std::memcpy(&data->m_bestOverallSector2TimeInMS, *buffer, sizeof(data->m_bestOverallSector2TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector2TimeInMS));
        std::memcpy(&data->m_bestOverallSector2LapNum, *buffer, sizeof(data->m_bestOverallSector2LapNum));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector2LapNum));
        std::memcpy(&data->m_bestOverallSector3TimeInMS, *buffer, sizeof(data->m_bestOverallSector3TimeInMS));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector3TimeInMS));
        std::memcpy(&data->m_bestOverallSector3LapNum, *buffer, sizeof(data->m_bestOverallSector3LapNum));
        incrementBuffer(buffer, sizeof(data->m_bestOverallSector3LapNum));
    }

    std::memcpy(&data->m_lapDistance, *buffer, sizeof(data->m_lapDistance));
    incrementBuffer(buffer, sizeof(data->m_lapDistance));
    std::memcpy(&data->m_totalDistance, *buffer, sizeof(data->m_totalDistance));
    incrementBuffer(buffer, sizeof(data->m_totalDistance));
    std::memcpy(&data->m_safetyCarDelta, *buffer, sizeof(data->m_safetyCarDelta));
    incrementBuffer(buffer, sizeof(data->m_safetyCarDelta));

    std::memcpy(&data->m_carPosition, *buffer, sizeof(data->m_carPosition));
    incrementBuffer(buffer, sizeof(data->m_carPosition));
    std::memcpy(&data->m_currentLapNum, *buffer, sizeof(data->m_currentLapNum));
    incrementBuffer(buffer, sizeof(data->m_currentLapNum));
    std::memcpy(&data->m_pitStatus, *buffer, sizeof(data->m_pitStatus));
    incrementBuffer(buffer, sizeof(data->m_pitStatus));
    std::memcpy(&data->m_sector, *buffer, sizeof(data->m_sector));
    incrementBuffer(buffer, sizeof(data->m_sector));
    std::memcpy(&data->m_currentLapInvalid, *buffer, sizeof(data->m_currentLapInvalid));
    incrementBuffer(buffer, sizeof(data->m_currentLapInvalid));
    std::memcpy(&data->m_penalties, *buffer, sizeof(data->m_penalties));
    incrementBuffer(buffer, sizeof(data->m_penalties));
    std::memcpy(&data->m_gridPosition, *buffer, sizeof(data->m_gridPosition));
    incrementBuffer(buffer, sizeof(data->m_gridPosition));
    std::memcpy(&data->m_driverStatus, *buffer, sizeof(data->m_driverStatus));
    incrementBuffer(buffer, sizeof(data->m_driverStatus));
    std::memcpy(&data->m_resultStatus, *buffer, sizeof(data->m_resultStatus));
    incrementBuffer(buffer, sizeof(data->m_resultStatus));
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
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.SpeedTrap.vehicleIdx));
            std::memcpy(&packet->m_eventDetails.SpeedTrap.speed, *buffer, sizeof(packet->m_eventDetails.SpeedTrap.speed));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.SpeedTrap.speed));
            break;
        }
        case FastestLap: {
            std::memcpy(&packet->m_eventDetails.FastestLap.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.FastestLap.vehicleIdx));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.FastestLap.vehicleIdx));
            std::memcpy(&packet->m_eventDetails.FastestLap.lapTime, *buffer, sizeof(packet->m_eventDetails.FastestLap.lapTime));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.FastestLap.lapTime));
            break;
        }
        case RaceWinner: {
            std::memcpy(&packet->m_eventDetails.RaceWinner.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.RaceWinner.vehicleIdx));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.RaceWinner.vehicleIdx));
            break;
        }
        case Retirement: {
            std::memcpy(&packet->m_eventDetails.Retirement.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.Retirement.vehicleIdx));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Retirement.vehicleIdx));
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
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.TeamMateInPits.vehicleIdx));
            break;
        }
        case ChequeredFlag: {
            break; // no event data to read
        }
        case PenaltyIssued: {
            std::memcpy(&packet->m_eventDetails.Penalty.penaltyType, *buffer, sizeof(packet->m_eventDetails.Penalty.penaltyType));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.penaltyType));
            std::memcpy(&packet->m_eventDetails.Penalty.infringementType, *buffer, sizeof(packet->m_eventDetails.Penalty.infringementType));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.infringementType));
            std::memcpy(&packet->m_eventDetails.Penalty.vehicleIdx, *buffer, sizeof(packet->m_eventDetails.Penalty.vehicleIdx));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.vehicleIdx));
            std::memcpy(&packet->m_eventDetails.Penalty.otherVehicleIdx, *buffer, sizeof(packet->m_eventDetails.Penalty.otherVehicleIdx));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.otherVehicleIdx));
            std::memcpy(&packet->m_eventDetails.Penalty.time, *buffer, sizeof(packet->m_eventDetails.Penalty.time));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.time));
            std::memcpy(&packet->m_eventDetails.Penalty.lapNum, *buffer, sizeof(packet->m_eventDetails.Penalty.lapNum));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.lapNum));
            std::memcpy(&packet->m_eventDetails.Penalty.placesGained, *buffer, sizeof(packet->m_eventDetails.Penalty.placesGained));
            incrementBuffer(buffer, sizeof(packet->m_eventDetails.Penalty.placesGained));
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
            break;
        }
        case 2019: {
            bytesRead = 32;
            break;
        }
        case 2018: {
            bytesRead = 25;
            break;
        }
    }

    return true;
}

EventCode Decoder::readEventCode(char** buffer) {
    uint8_t eventStringCode[4];
    std::memcpy(eventStringCode, *buffer, sizeof(eventStringCode));
    incrementBuffer(buffer, sizeof(eventStringCode));

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
            break;
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
            break;
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
            break;
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

bool Decoder::DecodePacketParticipantsData(char** buffer, PacketHeader* header, PacketParticipantsData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;

    std::memcpy(&packet->m_numActiveCars, *buffer, sizeof(packet->m_numActiveCars));
    incrementBuffer(buffer, sizeof(packet->m_numActiveCars));

    uint8_t carCount = getCarCount(header);

    for (uint8_t i = 0; i < carCount; i++) {
        decodeParticipantData(buffer, header, &packet->m_participants[i]);
    }

    return true;
}

void Decoder::decodeParticipantData(char** buffer, PacketHeader* header, ParticipantData* data) {
    std::memcpy(&data->m_aiControlled, *buffer, sizeof(data->m_aiControlled));
    incrementBuffer(buffer, sizeof(data->m_aiControlled));
    std::memcpy(&data->m_driverId, *buffer, sizeof(data->m_driverId));
    incrementBuffer(buffer, sizeof(data->m_driverId));
    std::memcpy(&data->m_teamId, *buffer, sizeof(data->m_teamId));
    incrementBuffer(buffer, sizeof(data->m_teamId));
    std::memcpy(&data->m_raceNumber, *buffer, sizeof(data->m_raceNumber));
    incrementBuffer(buffer, sizeof(data->m_raceNumber));
    std::memcpy(&data->m_nationality, *buffer, sizeof(data->m_nationality));
    incrementBuffer(buffer, sizeof(data->m_nationality));
    std::memcpy(&data->m_name, *buffer, sizeof(data->m_name));
    incrementBuffer(buffer, sizeof(data->m_name));

    if (header->m_packetFormat >= 2019) {
        std::memcpy(&data->m_yourTelemetry, *buffer, sizeof(data->m_yourTelemetry));
        incrementBuffer(buffer, sizeof(data->m_yourTelemetry));
    }
}

bool Decoder::DecodePacketCarSetupData(char** buffer, PacketHeader* header, PacketCarSetupData* packet) {
    if (!buffer || !header || !*(buffer) || !packet) {
        return false;
    }

    packet->m_header = header;
    uint8_t carCount = getCarCount(header);

    for (uint8_t i = 0; i < carCount; i++) {
        decodeCarSetupData(buffer, header, &packet->m_carSetups[i]);
    }

    return true;
}

void Decoder::decodeCarSetupData(char** buffer, PacketHeader* header, CarSetupData* data) {
    std::memcpy(&data->m_frontWing, *buffer, sizeof(data->m_frontWing));
    incrementBuffer(buffer, sizeof(data->m_frontWing));
    std::memcpy(&data->m_rearWing, *buffer, sizeof(data->m_rearWing));
    incrementBuffer(buffer, sizeof(data->m_rearWing));

    std::memcpy(&data->m_onThrottle, *buffer, sizeof(data->m_onThrottle));
    incrementBuffer(buffer, sizeof(data->m_onThrottle));
    std::memcpy(&data->m_offThrottle, *buffer, sizeof(data->m_offThrottle));
    incrementBuffer(buffer, sizeof(data->m_offThrottle));

    std::memcpy(&data->m_frontCamber, *buffer, sizeof(data->m_frontCamber));
    incrementBuffer(buffer, sizeof(data->m_frontCamber));
    std::memcpy(&data->m_rearCamber, *buffer, sizeof(data->m_rearCamber));
    incrementBuffer(buffer, sizeof(data->m_rearCamber));

    std::memcpy(&data->m_frontToe, *buffer, sizeof(data->m_frontToe));
    incrementBuffer(buffer, sizeof(data->m_frontToe));
    std::memcpy(&data->m_rearToe, *buffer, sizeof(data->m_rearToe));
    incrementBuffer(buffer, sizeof(data->m_rearToe));

    std::memcpy(&data->m_frontSuspension, *buffer, sizeof(data->m_frontSuspension));
    incrementBuffer(buffer, sizeof(data->m_frontSuspension));
    std::memcpy(&data->m_rearSuspension, *buffer, sizeof(data->m_rearSuspension));
    incrementBuffer(buffer, sizeof(data->m_rearSuspension));

    std::memcpy(&data->m_frontAntiRollBar, *buffer, sizeof(data->m_frontAntiRollBar));
    incrementBuffer(buffer, sizeof(data->m_frontAntiRollBar));
    std::memcpy(&data->m_rearAntiRollBar, *buffer, sizeof(data->m_rearAntiRollBar));
    incrementBuffer(buffer, sizeof(data->m_rearAntiRollBar));

    std::memcpy(&data->m_frontSuspensionHeight, *buffer, sizeof(data->m_frontSuspensionHeight));
    incrementBuffer(buffer, sizeof(data->m_frontSuspensionHeight));
    std::memcpy(&data->m_rearSuspensionHeight, *buffer, sizeof(data->m_rearSuspensionHeight));
    incrementBuffer(buffer, sizeof(data->m_rearSuspensionHeight));

    std::memcpy(&data->m_brakePressure, *buffer, sizeof(data->m_brakePressure));
    incrementBuffer(buffer, sizeof(data->m_brakePressure));
    std::memcpy(&data->m_brakeBias, *buffer, sizeof(data->m_brakeBias));
    incrementBuffer(buffer, sizeof(data->m_brakeBias));

    // F1 2019 and earlier only considers "front" and "rear" pressure, not individual tyres
    std::memcpy(&data->m_rearLeftTyrePressure, *buffer, sizeof(data->m_rearLeftTyrePressure));
    incrementBuffer(buffer, sizeof(data->m_rearLeftTyrePressure));
    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_rearRightTyrePressure, *buffer, sizeof(data->m_rearRightTyrePressure));
        incrementBuffer(buffer, sizeof(data->m_rearRightTyrePressure));
    } else {
        data->m_rearRightTyrePressure = data->m_rearLeftTyrePressure;
    }
    std::memcpy(&data->m_frontLeftTyrePressure, *buffer, sizeof(data->m_frontLeftTyrePressure));
    incrementBuffer(buffer, sizeof(data->m_frontLeftTyrePressure));
    if (header->m_packetFormat >= 2020) {
        std::memcpy(&data->m_frontRightTyrePressure, *buffer, sizeof(data->m_frontRightTyrePressure));
        incrementBuffer(buffer, sizeof(data->m_frontRightTyrePressure));
    } else {
        data->m_frontRightTyrePressure = data->m_frontLeftTyrePressure;
    }

    std::memcpy(&data->m_ballast, *buffer, sizeof(data->m_ballast));
    incrementBuffer(buffer, sizeof(data->m_ballast));
    std::memcpy(&data->m_fuelLoad, *buffer, sizeof(data->m_fuelLoad));
    incrementBuffer(buffer, sizeof(data->m_fuelLoad));
}