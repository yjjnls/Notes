# RFC6544 TCP Candidates with Interactive Connectivity Establishment (ICE)

https://tools.ietf.org/html/rfc6544

Interactive Connectivity Establishment (ICE) [RFC5245] defines a mechanism for NAT traversal for multimedia communication protocols based on the offer/answer model [RFC3264] of session negotiation. ICE works by providing a set of candidate transport addresses for each media stream, which are then validated with peer-to-peer connectivity checks based on Session Traversal Utilities for NAT (STUN) [RFC5389].  `However, ICE only defines procedures for UDP-based transport protocols.`

ICE主要用于NAT穿越，一般是基于udp的，但是也可以基于tcp，并且分为三种形式。


When agents perform address allocations to gather TCP-based candidates, three types of candidates can be obtained: `active candidates, passive candidates, and simultaneous-open (S-O)candidates`.  An **active candidate** is one for which the agent will attempt to open an outbound connection but will not receive incoming connection requests.  A passive candidate is one for which the agent will receive incoming connection attempts but not attempt a connection.  An S-O candidate is one for which the agent will attempt to open a connection simultaneously with its peer.


   The default candidate is chosen primarily based on the likelihood of
   it working with a non-ICE peer.  When media streams supporting mixed
   modes (both TCP and UDP) are used with ICE, it is RECOMMENDED that,
   for real-time streams (such as RTP), the default candidates be UDP-
   based.  However, the default SHOULD NOT be a simultaneous-open
   candidate.

   If a media stream is inherently TCP-based, it is RECOMMENDED for an
   offering full agent to select an active candidate as the default
   candidate and use [RFC4145] "setup" attribute value "active".  This
   increases the chances for a successful NAT traversal even without ICE
   support if the agent is behind a NAT and the peer is not.  For the
   same reason, for a lite agent, it is RECOMMENDED to use a passive
   candidate and "setup" attribute value "passive" in the offer


---
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_TCP_ACTIVE

   owr设定 rtp 59000 rtcp59001
   只会有一个rtp/udp 59000

   owr不设定端口
   两个udp 63109 63110

   owr设定 rtp/rtcp端口范围 60000-60100
   两个udp 端口 60060 60994，正好是设置两次NICE_CANDIDATE_TRANSPORT_UDP的值

   会进行如下设置
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_TCP_ACTIVE
   NICE_CANDIDATE_TRANSPORT_UDP
   udp端口都在rtp设置范围内

----
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_TCP_ACTIVE
   NICE_CANDIDATE_TRANSPORT_TCP_PASSIVE

   分别建立一个udp和tcp对应于NICE_CANDIDATE_TRANSPORT_UDP和NICE_CANDIDATE_TRANSPORT_TCP_PASSIVE
   范围都是rtp的端口范围

-----
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_TCP_PASSIVE
会按照如下形式进行传递
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_UDP
   NICE_CANDIDATE_TRANSPORT_TCP_PASSIVE
分别建立两个udp和一个tcp，范围都是rtp的端口范围

-----
NICE_CANDIDATE_TRANSPORT_UDP
会建立3个udp端口（在rtp端口范围）