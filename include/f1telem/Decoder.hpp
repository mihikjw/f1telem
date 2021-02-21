#ifndef F1TELEM_DECODER_HPP
#define F1TELEM_DECODER_HPP

#include <stdexcept>

#include "f1telem/Packet.hpp"

namespace F1Telem {

class Decoder {
    public:
    virtual ~Decoder() = default;

    virtual Packet* DecodePacket(char* buffer) = 0;

    protected:
    Decoder() = default;
};

enum Edition { E_F1Legacy, E_F12018, E_F12019, E_F12020 };

} // namespace F1Telem

#endif // F1TELEM_F12020_DECODER_HPP