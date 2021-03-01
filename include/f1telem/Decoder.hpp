#ifndef F1TELEM_DECODER_HPP
#define F1TELEM_DECODER_HPP

#include <cstdint>
#include <stdexcept>

#include "f1telem/Types.hpp"

namespace F1Telem {

class Decoder {
    public:
    Decoder() noexcept;
    ~Decoder() = default;

    uint8_t DecodePacketHeader(char** buffer, PacketHeader* header);
    bool DecodePacketMotionData(char** buffer, PacketHeader* header, PacketMotionData* packet);
    bool DecodePacketSessionData(char** buffer, PacketHeader* header, PacketSessionData* packet);
    bool DecodePacketLapData(char** buffer, PacketHeader* header, PacketLapData* packet);

    bool ValidateLastPacket(int bytesRecieved) noexcept;
    void ResetByteCount() noexcept;

    private:
    uint8_t getCarCount(PacketHeader* header) noexcept;
    void decodeCarMotionData(char** buffer, CarMotionData* data);
    void decodeMarshalZoneData(char** buffer, MarshalZone* data);
    void decodeWeatherForecastSampleData(char** buffer, WeatherForecastSample* data);
    void decodeLapData(char** buffer, LapData*);
    int bytesRead;
};

} // namespace F1Telem

#endif // F1TELEM_F12020_DECODER_HPP