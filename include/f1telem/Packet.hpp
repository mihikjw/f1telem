#ifndef F1TELEM_PACKET_HPP
#define F1TELEM_PACKET_HPP

namespace F1Telem {

struct Packet {
    public:
    Packet() = default;
    virtual ~Packet() = default;
};

} // namespace F1Telem

#endif // F1TELEM_PACKET_HPPCAR_COUNT