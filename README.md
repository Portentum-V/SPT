# Simple Persistent Threat (SPT)

> Portentum | September 2020

This project will be an effort to build lightweight c programs that can provide file transfer utility and command line access to remote devices, along with a python server to control multiple clients.

### Framework
 - [ ] Encoding / Serilization (nanopb: https://github.com/nanopb/nanopb)
 - [ ] Compression (fastLZ: https://github.com/ariya/FastLZ)
 - [ ] Encryption (SSL?)
 - [X] Message format

### Windows Client
- [ ] Network communication
- [ ] cmd
- [ ] powershell
- [ ] File upload
- [ ] File download

### Python Server
- [ ] Network communication
- [ ] Session Handler
- [ ] Logging
- [ ] File download
- [ ] File upload

### Linux Client
- [ ] Network communication
- [ ] bash
- [ ] sh
- [ ] File upload
- [ ] File download

### Bonus
- [ ] File integrity
- [ ] Encryption
- [ ] UDP communications
- [ ] Error correction

## Resources
- https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzahg/rzahgcomm.htm
- https://students.mimuw.edu.pl/SO/Linux/Kod/include/linux/socket.h.html
- https://docs.microsoft.com/en-us/windows/win32/winsock/ipv6-enabled-client-code-2
- https://beej.us/guide/bgnet/html//index.html#windows
- https://tangentsoft.net/wskfaq/articles/bsd-compatibility.html
- https://en.wikipedia.org/wiki/CPUID
- https://github.com/llazzaro/chem-fingerprints/blob/master/src/cpuid.h
- https://docs.microsoft.com/en-us/windows/win32/iphlp/getting-started-with-ip-helper
- https://www.codeguru.com/cpp/i-n/network/networkinformation/article.php/c5451/Three-ways-to-get-your-MAC-address.htm
- https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_addresses_lh
- https://www.roguelynn.com/words/asyncio-initial-setup/
- https://blog.cloudflare.com/increasing-ipv6-mtu/

### OS defs
- https://web.archive.org/web/20191012035921/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
- https://sourceforge.net/p/predef/wiki/OperatingSystems/

### VS stuff...
- https://marketplace.visualstudio.com/items?itemName=MaciejGudanowicz.AddMultipleProjectsToSolution
- https://docs.microsoft.com/en-us/visualstudio/ide/solutions-and-projects-in-visual-studio?view=vs-2019
- https://elanderson.net/2016/09/add-git-ignore-to-existing-visual-studio-project/

### Windows
- https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms740504(v=vs.85)


## External Libraries
- https://github.com/nanopb/nanopb.git
- https://github.com/GerHobbelt/pthread-win32
- https://github.com/skeeto/ulid-c/blob/master/ulid.c
- https://github.com/rxi/log.c/tree/master

## Ideas
#### System Design Interview - Alex Xu
 - Message queue (p22): Input services create messages which are published to a queue. Other services read the queue and perform actions defined by the messages
 - Metrics (p23): understand the health of the system [CPU, Memory, I/O, etc]
 - Latency (p35): Disk is slow, compression is fast
 - Hearbeat (p224): Client sends a 'heartbeat' every Xs, if not seen for Ys assumed to be offline
 - File transfer (p294): Split (block) -> Compress -> Encrypt -> Send

#### Designing Data-Intensive Applications
 - Maintainability (p19): Operability (keep the system running), Simplicity (remove as much complexity as possible), Evolvability (changing/adapting should be easy)

## Asumptions 
 - MTU of 1280: All non-local messages will be 1166 bytes max
 - Minimum IPv6 MTU 1280 - Max TCP Header 60 - IPv6 Header 40 - Ethernet Header 14 = 1166