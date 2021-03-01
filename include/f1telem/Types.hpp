#ifndef F1TELEM_TYPES_HPP
#define F1TELEM_TYPES_HPP

#include <array>
#include <cstdint>

/*
    - specs:
        - https://forums.codemasters.com/topic/50942-f1-2020-udp-specification/
    - All values are little-endian
    - All data packed, no padding used
*/

namespace F1Telem {

// PacketHeader is a header sent with all packets transmitted - not all fields will be filled, check packetFormat
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

// car counts are the number of supported cars, changed in F12020
constexpr uint8_t F12020_CAR_COUNT = 22;
constexpr uint8_t LEGACY_CAR_COUNT = 20;

/*
PacketMotionData gives physics data for all cars being driven. There is additional data for the car being driven.
    - For the normalised vectors below, to convert to float values divide by 32767.0f – 16-bit signed values are used to pack the data and on the assumption
        that direction values are always between -1.0f and 1.0f.
    - Frequency: Rate as specified in menus
    - Size: 1464 bytes (F12020)
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
    PacketHeader* m_header;                                      // header
    std::array<CarMotionData, F12020_CAR_COUNT> m_carMotionData; // data for all cars on track
    std::array<float, 4> m_suspensionPosition;                   // all wheel arrays have the following order: RL, RR, FL, FR
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

/*
PacketSessionData includes details about the current session in progress
    - Frequency: 2 per-second
    - Size: 251 bytes (F12020)
    - Version: 1
*/
struct MarshalZone {
    float m_zoneStart; // fraction (0..1) of way through the lap the marshal zone starts
    int8_t m_zoneFlag; // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
};

struct WeatherForecastSample {
    uint8_t m_sessionType; // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ, 10 = R, 11 = R2, 12 = Time Trial
    uint8_t m_timeOffset;  // time in minutes the forecast is for
    uint8_t m_weather;     // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t m_trackTemperature; // track temp in celcius
    int8_t m_airTemperature;   // air temp in celcius
};

constexpr uint8_t MAX_MARSHAL_ZONES = 21;
constexpr uint8_t MAX_WEATHER_FORECAST_SAMPLES = 20;

struct PacketSessionData {
    PacketHeader* m_header;    // header
    uint8_t m_weather;         // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t m_trackTemperature; // track temp in celcius
    int8_t m_airTemperature;   // air temp in celcius
    uint8_t m_totalLaps;       // total no. of laps in this race
    uint16_t m_trackLength;    // track length in meters
    uint8_t m_sessionType; // 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P, 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ, 10 = R, 11 = R2, 12 = Time Trial
    int8_t m_trackId;      // -1 for unknown, 0-21 for tracks, see tracks constants
    uint8_t m_formula;     // 0 = F1 Modern, 1 = F1 Classic, 2 = F2, 3 = F1 Generic NOTE: before F1 2018 (m_era): 0 = modern, 1 = classic
    uint16_t m_sessionTimeLeft;                                                               // time left in session in seconds
    uint16_t m_sessionDuration;                                                               // session duration in seconds
    uint8_t m_pitSpeedLimit;                                                                  // pit speed limit in kph
    uint8_t m_gamePaused;                                                                     // whether the game is paused
    uint8_t m_isSpectating;                                                                   // whether the player is spectating
    uint8_t m_spectatorCarIndex;                                                              // index of the car being spectated
    uint8_t m_sliProNativeSupport;                                                            // SLI pro support, 0 = inactive, 1 = active
    uint8_t m_numMarshalZones;                                                                // number of marshal zones to follow
    std::array<MarshalZone, MAX_MARSHAL_ZONES> m_marshalZones;                                // list of marshal zones
    uint8_t m_safetyCarStatus;                                                                // 0 = no safety car, 1 = full safety car, 2 = VSC
    uint8_t m_networkGame;                                                                    // 0 = offline, 1 = online
    uint8_t m_numWeatherForecastSamples;                                                      // number of weather samples to follow
    std::array<WeatherForecastSample, MAX_WEATHER_FORECAST_SAMPLES> m_weatherForecastSamples; // array of weather forecase samples
};

/*
PacketLapData gives details of all the cars in the session
    - Frequency: Rate as specified in menus
    - Size: 1190 bytes (F12020)
    - Version: 1
*/
struct LapData {
    float m_lastLapTime;                   // last lap time in seconds
    float m_currentLapTime;                // current time around the lap in seconds
    uint16_t m_sector1TimeInMS;            // sector 1 time in milliseconds
    uint16_t m_sector2TimeInMS;            // sector 2 time in milliseconds
    float m_bestLapTime;                   // best lap time of the session in seconds
    uint8_t m_bestLapNum;                  // lap number best time achieved on
    uint16_t m_bestLapSector1TimeInMS;     // Sector 1 time of best lap in the session in milliseconds
    uint16_t m_bestLapSector2TimeInMS;     // Sector 2 time of best lap in the session in milliseconds
    uint16_t m_bestLapSector3TimeInMS;     // Sector 3 time of best lap in the session in milliseconds
    uint16_t m_bestOverallSector1TimeInMS; // Best overall sector 1 time of the session in milliseconds
    uint8_t m_bestOverallSector1LapNum;    // Lap number best overall sector 1 time achieved on
    uint16_t m_bestOverallSector2TimeInMS; // Best overall sector 2 time of the session in milliseconds
    uint8_t m_bestOverallSector2LapNum;    // Lap number best overall sector 2 time achieved on
    uint16_t m_bestOverallSector3TimeInMS; // Best overall sector 3 time of the session in milliseconds
    uint8_t m_bestOverallSector3LapNum;    // Lap number best overall sector 3 time achieved on
    float m_lapDistance;                   // Distance vehicle is around current lap in metres – could be negative if line hasn’t been crossed yet
    float m_totalDistance;                 // Total distance travelled in session in metres – could be negative if line hasn’t been crossed yet
    float m_safetyCarDelta;                // Delta in seconds for safety car
    uint8_t m_carPosition;                 // Car race position
    uint8_t m_currentLapNum;               // Current lap number
    uint8_t m_pitStatus;                   // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t m_sector;                      // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t m_currentLapInvalid;           // Current lap invalid - 0 = valid, 1 = invalid
    uint8_t m_penalties;                   // Accumulated time penalties in seconds to be added
    uint8_t m_gridPosition;                // Grid position the vehicle started the race in
    uint8_t m_driverStatus;                // Status of driver - 0 = in garage, 1 = flying lap, 2 = in lap, 3 = out lap, 4 = on track
    uint8_t m_resultStatus; // Result status - 0 = invalid, 1 = inactive, 2 = active, 3 = finished, 4 = disqualified, 5 = not classified, 6 = retired
};

struct PacketLapData {
    PacketHeader* m_header;                          // header
    std::array<LapData, F12020_CAR_COUNT> m_lapData; // lap data for all cars on track
};

} // namespace F1Telem

#endif // F1TELEM_F12020_TYPES_HPP