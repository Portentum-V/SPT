# Simple Persistent Threat (SPT)

> Portentum | September 2020

This project will be an effort to build lightweight c programs that can provide file transfer utility and command line access to remote devices, along with a python server to control multiple clients.

### Framework
 - [ ] Encoding / Serilization (ProtoBufs?)
 - [ ] Message format

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

### VS stuff...
- https://marketplace.visualstudio.com/items?itemName=MaciejGudanowicz.AddMultipleProjectsToSolution
- https://docs.microsoft.com/en-us/visualstudio/ide/solutions-and-projects-in-visual-studio?view=vs-2019
- https://elanderson.net/2016/09/add-git-ignore-to-existing-visual-studio-project/

### Windows
- https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms740504(v=vs.85)

## Ideas
#### System Design Interview - Alex Xu
 - Message queue (p22): Input services create messages which are published to a queue. Other services read the queue and perform actions defined by the messages
 - Metrics (p23): understand the health of the system [CPU, Memory, I/O, etc]
 - Latency (p35): Disk is slow, compression is fast
 - Hearbeat (p224): Client sends a 'heartbeat' every Xs, if not seen for Ys assumed to be offline
 - File transfer (p294): Split (block) -> Compress -> Encrypt -> Send

#### Designing Data-Intensive Applications
 - Maintainability (p19): Operability (keep the system running), Simplicity (remove as much complexity as possible), Evolvability (changing/adapting should be easy)