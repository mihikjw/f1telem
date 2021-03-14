#ifndef F1TELEM_TYPES_HPP
#define F1TELEM_TYPES_HPP

#include <array>
#include <cstdint>
#include <string>

#include "f1telem/Defines.hpp"

/*
    - specs:
        - https://forums.codemasters.com/topic/50942-f1-2020-udp-specification/
        - https://forums.codemasters.com/topic/44592-f1-2019-udp-specification/
        - https://forums.codemasters.com/topic/30601-f1-2018-udp-specification/
        - Legacy spec is not supported, no intention of adding legacy support
    - All values are little-endian
    - All data packed, no padding used
*/

namespace F1Telem {

// UDP_BUFFER_SIZE is the maximum required size of a buffer to recieve the largest packet
constexpr int UDP_BUFFER_SIZE = 1464;

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

/*
PacketEventData gives details of events that happen during the course of a session
    - Frequency: When the event occurs
    - Size: 35 bytes (F12020)
    - Version: 1
*/
union EventDataDetails {
    struct {
        uint8_t vehicleIdx; // vehicle index of car achieving fastest lap
        float lapTime;      // lap time in seconds
    } FastestLap;

    struct {
        uint8_t vehicleIdx; // vehicle index of car retiering
    } Retirement;

    struct {
        uint8_t vehicleIdx; // vehicle index of team mate
    } TeamMateInPits;

    struct {
        uint8_t vehicleIdx; // vehicle index of the race winner
    } RaceWinner;

    struct {
        uint8_t penaltyType;      // Penalty type
        uint8_t infringementType; // Infringement type
        uint8_t vehicleIdx;       // Vehicle index of the car the penalty is applied to
        uint8_t otherVehicleIdx;  // Vehicle index of the other car involved
        uint8_t time;             // Time gained, or time spent doing action in seconds
        uint8_t lapNum;           // Lap the penalty occurred on
        uint8_t placesGained;     // Number of places gained by this
    } Penalty;

    struct {
        uint8_t vehicleIdx; // Vehicle index of the vehicle triggering speed trap
        float speed;        // Top speed achieved in kilometres per hour
    } SpeedTrap;
};

enum EventCode {
    SessionStarted,
    SessionEnded,
    FastestLap,
    Retirement,
    DRSEnabled,
    DRSDisabled,
    TeamMateInPits,
    ChequeredFlag,
    RaceWinner,
    PenaltyIssued,
    SpeedTrapTriggered,
    Unknown
};

struct PacketEventData {
    PacketHeader* m_header;          // Header
    EventCode m_eventCode;           // library translation from m_eventStringCode
    EventDataDetails m_eventDetails; // Event details - should be interpreted differently for each type
};

/*
PacketParticipantsData is a list of participants in the race. If controlled by AI, this will be the drivers name. If MP, the names will be the SteamID on PC,
    or the LAN name if appropriate. On Xbox One, the names will always be the driver name, on PS4 the name will be the LAN name if playing a LAN game, otherwise
    it will be the driver name. The array is indexed by vehicle index.
    - Frequency: every 5 seconds
    - Size: 1213 bytes (F12020)
    - Version: 1
*/
struct ParticipantData {
    uint8_t m_aiControlled;  // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t m_driverId;      // Driver id
    uint8_t m_teamId;        // Team id
    uint8_t m_raceNumber;    // Race number of the car
    uint8_t m_nationality;   // Nationality of the driver
    char m_name[48];         // Name of participant in UTF-8 format – null terminated, Will be truncated with … (U+2026) if too long
    uint8_t m_yourTelemetry; // The player's UDP setting, 0 = restricted, 1 = public
};

struct PacketParticipantsData {
    PacketHeader* m_header;  // Header
    uint8_t m_numActiveCars; // Number of active cars in the data – should match number of cars on HUD
    std::array<ParticipantData, F12020_CAR_COUNT> m_participants;
};

/*
PacketCarSetupData details the car setups for each vehicle in the session. Note in MP games, other player cars will appear as blank, you can
    only view your own setup and AI cars
    - Frequency: 2 per second
    - Size: 1102 bytes (F12020)
    - Version: 1
*/
struct CarSetupData {
    uint8_t m_frontWing;             // Front wing aero
    uint8_t m_rearWing;              // Rear wing aero
    uint8_t m_onThrottle;            // Differential adjustment on throttle (percentage)
    uint8_t m_offThrottle;           // Differential adjustment off throttle (percentage)
    float m_frontCamber;             // Front camber angle (suspension geometry)
    float m_rearCamber;              // Rear camber angle (suspension geometry)
    float m_frontToe;                // Front toe angle (suspension geometry)
    float m_rearToe;                 // Rear toe angle (suspension geometry)
    uint8_t m_frontSuspension;       // Front suspension
    uint8_t m_rearSuspension;        // Rear suspension
    uint8_t m_frontAntiRollBar;      // Front anti-roll bar
    uint8_t m_rearAntiRollBar;       // Rear anti-roll bar
    uint8_t m_frontSuspensionHeight; // Front ride height
    uint8_t m_rearSuspensionHeight;  // Rear ride height
    uint8_t m_brakePressure;         // Brake pressure (percentage)
    uint8_t m_brakeBias;             // Brake bias (percentage)
    float m_rearLeftTyrePressure;    // Rear left tyre pressure (PSI)
    float m_rearRightTyrePressure;   // Rear right tyre pressure (PSI)
    float m_frontLeftTyrePressure;   // Front left tyre pressure (PSI)
    float m_frontRightTyrePressure;  // Front right tyre pressure (PSI)
    uint8_t m_ballast;               // Ballast
    float m_fuelLoad;                // Fuel load
};

struct PacketCarSetupData {
    PacketHeader* m_header; // Header
    std::array<CarSetupData, F12020_CAR_COUNT> m_carSetups;
};

/*
PacketCarTelemetryData details telemetry for all cars in the race. It details various values that would be recorded on the car
    such as speed, throttle application, DRS etc
    - Frequency: Rate as specified in menus
    - Size: 1307 bytes (F12020)
    - Version: 1

    the temps order is not specified in the standard, but earlier has specified: RL, RR, FL, FR
*/
struct CarTelemetryData {
    uint16_t m_speed;                                 // Speed of car in kilometres per hour
    float m_throttle;                                 // Amount of throttle applied (0.0 to 1.0)
    float m_steer;                                    // Steering (-1.0 (full lock left) to 1.0 (full lock right))
    float m_brake;                                    // Amount of brake applied (0.0 to 1.0)
    uint8_t m_clutch;                                 // Amount of clutch applied (0 to 100)
    int8_t m_gear;                                    // Gear selected (1-8, N=0, R=-1)
    uint16_t m_engineRPM;                             // Engine RPM
    uint8_t m_drs;                                    // 0 = off, 1 = on
    uint8_t m_revLightsPercent;                       // Rev lights indicator (percentage)
    std::array<uint16_t, 4> m_brakesTemperature;      // Brakes temperature (celsius)
    std::array<uint8_t, 4> m_tyresSurfaceTemperature; // Tyres surface temperature (celsius)
    std::array<uint8_t, 4> m_tyresInnerTemperature;   // Tyres inner temperature (celsius)
    uint16_t m_engineTemperature;                     // Engine temperature (celsius)
    std::array<float, 4> m_tyresPressure;             // Tyres pressure (PSI)
    std::array<uint8_t, 4> m_surfaceType;             // Driving surface, see appendices
};

struct PacketCarTelemetryData {
    PacketHeader* m_header; // Header
    std::array<CarTelemetryData, F12020_CAR_COUNT> m_carTelemetryData;
    uint32_t m_buttonStatus; // Bit flags specifying which buttons are being pressed currently
    uint8_t m_mfdPanelIndex; // Index of MFD panel open - 255 = MFD closed; Single player, race – 0 = Car setup, 1 = Pits, 2 = Damage, 3 =  Engine, 4 =
                             // Temperatures; May vary depending on game mode
    uint8_t m_mfdPanelIndexSecondaryPlayer; // See above
    int8_t m_suggestedGear;                 // Suggested gear for the player (1-8) 0 if no gear suggested
};

/*
PacketCarStatusData details car stauses for all the cars in the race, includes values such as damage readings on the car
    - Frequency: as specified in menus
    - Size: 1344 bytes (F12020)
    - Version: 1
*/
struct CarStatusData {
    uint8_t m_tractionControl;          // 0 (off) - 2 (high)
    uint8_t m_antiLockBrakes;           // 0 (off) - 1 (on)
    uint8_t m_fuelMix;                  // Fuel mix - 0 = lean, 1 = standard, 2 = rich, 3 = max
    uint8_t m_frontBrakeBias;           // Front brake bias (percentage)
    uint8_t m_pitLimiterStatus;         // Pit limiter status - 0 = off, 1 = on
    float m_fuelInTank;                 // Current fuel mass
    float m_fuelCapacity;               // Fuel capacity
    float m_fuelRemainingLaps;          // Fuel remaining in terms of laps (value on MFD)
    uint16_t m_maxRPM;                  // Cars max RPM, point of rev limiter
    uint16_t m_idleRPM;                 // Cars idle RPM
    uint8_t m_maxGears;                 // Maximum number of gears
    uint8_t m_drsAllowed;               // 0 = not allowed, 1 = allowed, -1 = unknown
    uint16_t m_drsActivationDistance;   // 0 = DRS not available, non-zero - DRS will be available in [X] metres
    std::array<uint8_t, 4> m_tyresWear; // Tyre wear percentage
    uint8_t m_actualTyreCompound; // F1 Modern - 16 = C5, 17 = C4, 18 = C3, 19 = C2, 20 = C1,7 = inter, 8 = wet,F1 Classic - 9 = dry, 10 = wet,F2 – 11 = super
                                  // soft, 12 = soft, 13 = medium, 14 = hard, 15 = wet
    uint8_t m_visualTyreCompound; // F1 visual (can be different from actual compound), 16 = soft, 17 = medium, 18 = hard, 7 = inter, 8 = wet, F1 Classic – same
                                  // as above, F2 – same as above
    uint8_t m_tyresAgeLaps;               // Age in laps of the current set of tyres
    std::array<uint8_t, 4> m_tyresDamage; // Tyre damage (percentage)
    uint8_t m_frontLeftWingDamage;        // Front left wing damage (percentage)
    uint8_t m_frontRightWingDamage;       // Front right wing damage (percentage)
    uint8_t m_rearWingDamage;             // Rear wing damage (percentage)
    uint8_t m_drsFault;                   // Indicator for DRS fault, 0 = OK, 1 = fault
    uint8_t m_engineDamage;               // Engine damage (percentage)
    uint8_t m_gearBoxDamage;              // Gear box damage (percentage)
    int8_t m_vehicleFiaFlags;             // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
    float m_ersStoreEnergy;               // ERS energy store in Joules
    uint8_t m_ersDeployMode;              // ERS deployment mode, 0 = none, 1 = medium, 2 = overtake, 3 = hotlap
    float m_ersHarvestedThisLapMGUK;      // ERS energy harvested this lap by MGU-K
    float m_ersHarvestedThisLapMGUH;      // ERS energy harvested this lap by MGU-H
    float m_ersDeployedThisLap;           // ERS energy deployed this lap
};

struct PacketCarStatusData {
    PacketHeader* m_header; // Header
    std::array<CarStatusData, F12020_CAR_COUNT> m_carStatusData;
};

/*
PacketFinalClassificationData contains details on the final classification at the end of the race, and the data will match
    with the post-race results screen. This is especially useful for multiplayer games where it is not always possible to
    send lap times on the final frame because of network delay.
    - Frequency: Once at the end of a race
    - Size: 830 bytes (F1 2020)
    - Version: 1
*/
struct FinalClassificationData {
    uint8_t m_position;      // Finishing position
    uint8_t m_numLaps;       // Number of laps completed
    uint8_t m_gridPosition;  // Grid position of the car
    uint8_t m_points;        // Number of points scored
    uint8_t m_numPitStops;   // Number of pit stops made
    uint8_t m_resultStatus;  // Result status - 0 = invalid, 1 = inactive, 2 = active, 3 = finished, 4 = disqualified, 5 = not classified, 6 = retired
    float m_bestLapTime;     // Best lap time of the session in seconds
    double m_totalRaceTime;  // Total race time in seconds without penalties
    uint8_t m_penaltiesTime; // Total penalties accumulated in seconds
    uint8_t m_numPenalties;  // Number of penalties applied to this driver
    uint8_t m_numTyreStints; // Number of tyres stints up to maximum
    std::array<uint8_t, 8> m_tyreStintsActual; // Actual tyres used by this driver
    std::array<uint8_t, 8> m_tyreStintsVisual; // Visual tyres used by this driver
};

struct PacketFinalClassificationData {
    PacketHeader* m_header;
    uint8_t m_numCars; // Number of cars in the final classification
    std::array<FinalClassificationData, F12020_CAR_COUNT> m_classificationData;
};

/*
PacketLobbyInfoData details the players currently in a multiplayer lobby. It details each players selected car,
    any AI involved in the game and also the ready status of each of the participants.
    - Frequency: Two every second when in the lobby
    - Size: 1169 bytes (F12020)
    - Version: 1
*/
struct LobbyInfoData {
    uint8_t m_aiControlled; // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t m_teamId;       // Team id - see appendix (255 if no team currently selected)
    uint8_t m_nationality;  // Nationality of the driver
    char m_name[48];        // Name of participant in UTF-8 format – null terminated, will be truncated with ... (U+2026) if too long
    uint8_t m_readyStatus;  // 0 = not ready, 1 = ready, 2 = spectating
};

struct PacketLobbyInfoData {
    PacketHeader* m_header;
    uint8_t m_numPlayers; // Number of players in the lobby data
    std::array<LobbyInfoData, F12020_CAR_COUNT> m_lobbyPlayers;
};

} // namespace F1Telem

#endif // F1TELEM_F12020_TYPES_HPP