# f1telem
F1 game real-time telemetry library, written in C++. An example application showing the use of the library is provided under `cmd/testapp.cpp`. The library supports both Windows & Linux (only tested on x86-64), and recieves telemetry data over UDP. All of the data structures are based on the Codemasters documented structs, with some minor changes from C to C++, for example using `std::array` rather than C arrays, and enums in some places, however the field names are the same. 

## Supported Games
- [F1 2020](https://forums.codemasters.com/topic/50942-f1-2020-udp-specification/)
- [F1 2019](https://forums.codemasters.com/topic/44592-f1-2019-udp-specification/)
- [F1 2018](https://forums.codemasters.com/topic/30601-f1-2018-udp-specification/)

## TO DO
- Documentation beyond this quick overview and the testapp!
- Unit tests (feel free to raise a PR ... :) )