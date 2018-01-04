# RFC6544 TCP Candidates with Interactive Connectivity Establishment (ICE)

https://tools.ietf.org/html/rfc6544

Interactive Connectivity Establishment (ICE) [RFC5245] defines a mechanism for NAT traversal for multimedia communication protocols based on the offer/answer model [RFC3264] of session negotiation. ICE works by providing a set of candidate transport addresses for each media stream, which are then validated with peer-to-peer connectivity checks based on Session Traversal Utilities for NAT (STUN) [RFC5389].  `However, ICE only defines procedures for UDP-based transport protocols.`

ICE主要用于NAT穿越，一般是基于udp的，但是也可以基于tcp，并且分为三种形式。


When agents perform address allocations to gather TCP-based candidates, three types of candidates can be obtained: `active candidates, passive candidates, and simultaneous-open (S-O)candidates`.  An **active candidate** is one for which the agent will attempt to open an outbound connection but will not receive incoming connection requests.  A passive candidate is one for which the agent will receive incoming connection attempts but not attempt a connection.  An S-O candidate is one for which the agent will attempt to open a connection simultaneously with its peer.