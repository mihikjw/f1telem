# f1telem
F1 game real-time telemetry library, written in C++.

## Supported Games
### Current
- F1 2020

### Planned
- F1 2019
- F1 2018

## Capture

``` bash
sudo tcpdump -i eth0 udp port 20777 -w ./tmp/f12020-traffic.pcap
sudo tcpreplay --intf1=eth0 --stats=5 ./tmp/f12020-traffic.pcap 
```