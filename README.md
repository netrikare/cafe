# cafe

This is a Linux kernel module that executes predefined system commands based on low level packet data. 

Currently data can only be send as ICMP packets.

Install module:
> make insmod

Send packet to the server:
> ./ping.sh

### TODO:
- encryption (public key?)
- send data as IP packets
