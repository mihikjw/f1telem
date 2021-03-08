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

// TeamID's are used to identify team id where referenced in any struct - these are the F12020 values
namespace TeamID {
constexpr uint8_t Mercedes = 0;
constexpr uint8_t Ferrari = 1;
constexpr uint8_t RedBullRacing = 2;
constexpr uint8_t Williams = 3;
constexpr uint8_t RacingPoint = 4;
constexpr uint8_t Renault = 5;
constexpr uint8_t AlphaTauri = 6;
constexpr uint8_t Haas = 7;
constexpr uint8_t McLaren = 8;
constexpr uint8_t AlfaRomeo = 9;
constexpr uint8_t McLaren1988 = 10;
constexpr uint8_t McLaren1991 = 11;
constexpr uint8_t Williams1992 = 12;
constexpr uint8_t Ferrari1995 = 13;
constexpr uint8_t Williams1996 = 14;
constexpr uint8_t McLaren1998 = 15;
constexpr uint8_t Ferrari2002 = 16;
constexpr uint8_t Ferrari2004 = 17;
constexpr uint8_t Renault2006 = 18;
constexpr uint8_t Ferrari2007 = 19;
constexpr uint8_t McLaren2008 = 20;
constexpr uint8_t RedBull2010 = 21;
constexpr uint8_t Ferrari1976 = 22;
constexpr uint8_t ARTGrandPrix = 23;
constexpr uint8_t CamposVexatecRacing = 24;
constexpr uint8_t Carlin = 25;
constexpr uint8_t CharouzRacingSystem = 26;
constexpr uint8_t DAMS = 27;
constexpr uint8_t RussianTime = 28;
constexpr uint8_t MPMotorsport = 29;
constexpr uint8_t Pertamina = 30;
constexpr uint8_t McLaren1990 = 31;
constexpr uint8_t Trident = 32;
constexpr uint8_t BWTArden = 33;
constexpr uint8_t McLaren1976 = 34;
constexpr uint8_t Lotus1972 = 35;
constexpr uint8_t Ferrari1979 = 36;
constexpr uint8_t McLaren1982 = 37;
constexpr uint8_t Williams2003 = 38;
constexpr uint8_t Brawn2009 = 39;
constexpr uint8_t Lotus1978 = 40;
constexpr uint8_t F1GenericCar = 41;
constexpr uint8_t ARTGP19 = 42;
constexpr uint8_t Campos19 = 43;
constexpr uint8_t Carlin19 = 44;
constexpr uint8_t SauberJuniorCharouz19 = 45;
constexpr uint8_t Dams19 = 46;
constexpr uint8_t UniVirtousi19 = 47;
constexpr uint8_t MPMotorsport19 = 48;
constexpr uint8_t Prema19 = 49;
constexpr uint8_t Trident19 = 50;
constexpr uint8_t Arden19 = 51;
constexpr uint8_t Benetton1994 = 52;
constexpr uint8_t Benetton1995 = 53;
constexpr uint8_t Ferrari2000 = 54;
constexpr uint8_t Jordan1991 = 55;
} // namespace TeamID

} // namespace F1Telem

#endif // F1TELEM_DEFINES_HPP