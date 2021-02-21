#include "f1telem/f12020/Decoder.hpp"

using namespace F1Telem;

Packet* F12020::Decoder::DecodePacket(char* buffer) {
    if (!buffer) {
        return nullptr;
    }

    // not yet implemented
    return nullptr;
}