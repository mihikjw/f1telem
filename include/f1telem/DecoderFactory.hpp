#ifndef F1TELEM_DECODERFACTORY_HPP
#define F1TELEM_DECODERFACTORY_HPP

#include "f1telem/Decoder.hpp"
#include "f1telem/f12020/Decoder.hpp"

namespace F1Telem {

Decoder* DecoderFactory(Edition version) {
    switch (version) {
        case Edition::E_F1Legacy: {
            // old version
        }
        case Edition::E_F12018: {
            // f1 2018
        }
        case Edition::E_F12019: {
            // f1 2019
        }
        case Edition::E_F12020: {
            return new F12020::Decoder();
        }
        default: {
            throw std::runtime_error("invalid edition specified");
        }
    }
}

} // namespace F1Telem

#endif // F1TELEM_DECODERFACTORY_HPP