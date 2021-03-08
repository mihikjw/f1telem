#ifndef F1TELEM_DEFINES_HPP
#define F1TELEM_DEFINES_HPP

#include <cstdint>

namespace F1Telem {

// car counts are the number of supported cars, changed in F12020
constexpr uint8_t F12020_CAR_COUNT = 22;
constexpr uint8_t LEGACY_CAR_COUNT = 20;

// PacketID's used to identify the 'm_packetId' field in the PacketHeader
namespace PacketID {
constexpr uint8_t Motion = 0;
constexpr uint8_t Session = 1;
constexpr uint8_t LapData = 2;
constexpr uint8_t Event = 3;
constexpr uint8_t Participants = 4;
constexpr uint8_t CarSetups = 5;
constexpr uint8_t CarTelemetry = 6;
constexpr uint8_t CarStatus = 7;
constexpr uint8_t FinalClassification = 8;
constexpr uint8_t LobbyInfo = 9;
} // namespace PacketID

} // namespace F1Telem

#endif // F1TELEM_DEFINES_HPP