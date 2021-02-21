#ifndef F1TELEM_F12020_TYPES_HPP
#define F1TELEM_F12020_TYPES_HPP

#include <cstdint>

namespace F1Telem {

namespace F12020 {

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

enum PacketID {
    Motion = 0,
    Session,
    LapData,
    Event,
    Participants,
    CarSetups,
    CarTelemetry,
    CarStatus,
    FinalClassification,
    LobbyInfo,
};

/*
CarMotionData & PacketMotionData gives physics data for all cars being driven. There is additional data for the car being driven.
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
    PacketHeader m_header;             // header
    CarMotionData m_carMotionData[22]; // data for all cars on track
    // player-only data
    float m_suspensionPosition[4]; // all wheel arrays have the following order: RL, RR, FL, FR
    float m_suspensionVelocity[4];
    float m_suspensionAcceleration[4];
    float m_wheelSpeed[4];  // speed of each wheel
    float m_wheelSlip[4];   // slip ration for each wheel
    float m_localVelocityX; // velocity in local space
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