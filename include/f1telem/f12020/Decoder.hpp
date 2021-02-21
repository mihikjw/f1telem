#ifndef F1TELEM_F12020_DECODER_HPP
#define F1TELEM_F12020_DECODER_HPP

#include "f1telem/Decoder.hpp"

namespace F1Telem {

namespace F12020 {

class Decoder : public F1Telem::Decoder {
    public:
    Decoder() = default;
    ~Decoder() = default;

    Packet* DecodePacket(char* buffer) final;
};

} // namespace F12020

} // namespace F1Telem

#endif // F1TELEM_F12020_DECODER_HPP