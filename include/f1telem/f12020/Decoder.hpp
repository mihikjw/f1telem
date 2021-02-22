#ifndef F1TELEM_F12020_DECODER_HPP
#define F1TELEM_F12020_DECODER_HPP

#include <cstdint>

#include "f1telem/Decoder.hpp"
#include "f1telem/f12020/Types.hpp"

namespace F1Telem {

namespace F12020 {

class Decoder : public F1Telem::Decoder {
    public:
    Decoder() = default;
    ~Decoder() = default;

    Packet* DecodePacket(char* buffer) final;

    private:
    uint8_t decodePacketHeader(char* buffer, PacketHeader* header);
    Packet* decodePacketMotionData(char* buffer, PacketHeader* header);
    CarMotionData* decodeCarMotionData(char* buffer);
};

} // namespace F12020

} // namespace F1Telem

#endif // F1TELEM_F12020_DECODER_HPP