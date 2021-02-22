#ifndef F1TELEM_F12020_TYPES_HPP
#define F1TELEM_F12020_TYPES_HPP

#include <cstdint>
#include <array>

/*
    - spec: https://forums.codemasters.com/topic/50942-f1-2020-udp-specification/
    - All values are little-endian
    - All data packed, no padding used
*/

namespace F1Telem {
namespace F12020 {

// CAR_COUNT is the number of supported cars in F1 2020
constexpr uint8_t CAR_COUNT = 22;

// PacketHeader is a header sent with all packets transmitted
struct PacketHeader {
    uint16_t m_packetFormat;           // 2020
    uint8_t m_gameMajorVersion;        // X.00
    uint8_t m_gameMinorVersion;        // 1.XX
    uint8_t m_packetVersion;           // version of packet type, all start from 1
    uint8_t m_packetId;                // identifier for packet type
    uint64_t m_sessionUID;             // uid for session
    float m_sessionTime;               // session timestamp
    uint32_t m_frameIdentifier;        // identifier for the frame the data was retrieved on
    uint8_t m_playerCarIndex;          // index of players car in array
    uint8_t m_secondaryPlayerCarIndex; // 255 if no split screen
};

// PacketID's used to identify the 'm_packetId' field in the PacketHeader
constexpr uint8_t MOTION = 0;
constexpr uint8_t SESSION = 1;
constexpr uint8_t LAP_DATA = 2;
constexpr uint8_t EVENT = 3;
constexpr uint8_t PARTICIPANTS = 4;
constexpr uint8_t CAR_SETUPS = 5;
constexpr uint8_t CAR_TELEMETRY = 6;
constexpr uint8_t CAR_STATUS = 7;
constexpr uint8_t FINAL_CLASSIFICATION = 8;
constexpr uint8_t LOBBY_INFO = 9;

/*
PacketMotionData gives physics data for all cars being driven. There is additional data for the car being driven.
    - For the normalised vectors below, to convert to float values divide by 32767.0f – 16-bit signed values are used to pack the data and on the assumption
        that direction values are always between -1.0f and 1.0f.
    - Frequency: Rate as specified in menus
    - Size: 1464 bytes
    - Version: 1
*/
struct CarMotionData {
    float m_worldPositionX;     // world space X position
    float m_worldPositionY;     // world space Y position
    float m_worldPositionZ;     // world space Z position
    float m_worldVelocityX;     // velocity in world space X
    float m_worldVelocityY;     // velocity in world space Y
    float m_worldVelocityZ;     // velocity in world space Z
    int16_t m_worldForwardDirX; // world space forward X direction (norm)
    int16_t m_worldForwardDirY; // world space forward Y direction (norm)
    int16_t m_worldForwardDirZ; // world space forward Z direction (norm)
    int16_t m_worldRightDirX;   // world space right X direction (norm)
    int16_t m_worldRightDirY;   // world space right Y direction (norm)
    int16_t m_worldRightDirZ;   // world space right Z direction (norm)
    float m_gForceLateral;      // lateral g-force component
    float m_gForceLongitudinal; // longitudinal g-force component
    float m_gForceVertical;     // vertical g-force component
    float m_yaw;                // yaw angle in radians
    float m_pitch;              // pitch angle in radians
    float m_roll;               // roll angle in radians
};

struct PacketMotionData {
    PacketHeader* m_header;                                // header
    std::array<CarMotionData*, CAR_COUNT> m_carMotionData; // data for all cars on track
    std::array<float, 4> m_suspensionPosition;             // all wheel arrays have the following order: RL, RR, FL, FR
    std::array<float, 4> m_suspensionVelocity;
    std::array<float, 4> m_suspensionAcceleration;
    std::array<float, 4> m_wheelSpeed; // speed of each wheel
    std::array<float, 4> m_wheelSlip;  // slip ratio for each wheel
    float m_localVelocityX;            // velocity in local space
    float m_localVelocityY;
    float m_localVelocityZ;
    float m_angularVelocityX; // angular velocity
    float m_angularVelocityY;
    float m_angularVelocityZ;
    float m_angularAccelerationX; // angular acceleration
    float m_angularAccelerationY;
    float m_angularAccelerationZ;
    float m_frontWheelsAngle; // current front wheels angle in radians
};

} // namespace F12020
} // namespace F1Telem

#endif // F1TELEM_F12020_TYPES_HPP