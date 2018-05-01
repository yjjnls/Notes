



Network Working Group                                      S. Nandakumar
Internet-Draft                                               C. Jennings
Intended status: Informational                                     Cisco
Expires: February 5, 2016                                August 04, 2015


                           SDP for the WebRTC
                     draft-nandakumar-rtcweb-sdp-08

Abstract

   The Web Real-Time Communication [WebRTC] working group is charged to
   provide protocol support for direct interactive rich communication
   using audio, video and data between two peers' web browsers.  With in
   the WebRTC framework, Session Description protocol (SDP) [RFC4566] is
   used for negotiating session capabilities between the peers.  Such a
   negotiation happens based on the SDP Offer/Answer exchange mechanism
   described in [RFC3264].

   This document provides an informational reference in describing the
   role of SDP and the Offer/Answer exchange mechanism for the most
   common WebRTC use-cases.

   This SDP examples provided in this document is still a work in
   progress, but it aims to align closest to the evolving standards
   work.

Status of This Memo

   This Internet-Draft is submitted in full conformance with the
   provisions of BCP 78 and BCP 79.

   Internet-Drafts are working documents of the Internet Engineering
   Task Force (IETF).  Note that other groups may also distribute
   working documents as Internet-Drafts.  The list of current Internet-
   Drafts is at http://datatracker.ietf.org/drafts/current/.

   Internet-Drafts are draft documents valid for a maximum of six months
   and may be updated, replaced, or obsoleted by other documents at any
   time.  It is inappropriate to use Internet-Drafts as reference
   material or to cite them other than as "work in progress."

   This Internet-Draft will expire on February 5, 2016.








Nandakumar & Jennings   Expires February 5, 2016                [Page 1]

Internet-Draft                 SDP4WebRTC                    August 2015


Copyright Notice

   Copyright (c) 2015 IETF Trust and the persons identified as the
   document authors.  All rights reserved.

   This document is subject to BCP 78 and the IETF Trust's Legal
   Provisions Relating to IETF Documents
   (http://trustee.ietf.org/license-info) in effect on the date of
   publication of this document.  Please review these documents
   carefully, as they describe your rights and restrictions with respect
   to this document.  Code Components extracted from this document must
   include Simplified BSD License text as described in Section 4.e of
   the Trust Legal Provisions and are provided without warranty as
   described in the Simplified BSD License.

Table of Contents

   1.  Introduction  . . . . . . . . . . . . . . . . . . . . . . . .   3
   2.  Terminology . . . . . . . . . . . . . . . . . . . . . . . . .   3
   3.  SDP and the WebRTC  . . . . . . . . . . . . . . . . . . . . .   3
   4.  Offer/Answer and the WebRTC . . . . . . . . . . . . . . . . .   5
   5.  WebRTC Session Description Examples . . . . . . . . . . . . .   6
     5.1.  Some Conventions  . . . . . . . . . . . . . . . . . . . .   7
     5.2.  Basic Examples  . . . . . . . . . . . . . . . . . . . . .   8
       5.2.1.  Audio Only Session  . . . . . . . . . . . . . . . . .   8
       5.2.2.  Audio/Video Session . . . . . . . . . . . . . . . . .  11
       5.2.3.  Data Only Session . . . . . . . . . . . . . . . . . .  16
       5.2.4.  Audio Call On Hold  . . . . . . . . . . . . . . . . .  19
       5.2.5.  Audio with DTMF Session . . . . . . . . . . . . . . .  23
       5.2.6.  One Way Audio/Video Session - Document Camera . . . .  27
       5.2.7.  Audio, Video Session with BUNDLE Support Unknown  . .  31
       5.2.8.  Audio, Video and Data Session . . . . . . . . . . . .  38
       5.2.9.  Audio, Video Session with BUNDLE Unsupported  . . . .  43
       5.2.10. Audio, Video BUNDLED, but Data (Not BUNDLED)  . . . .  47
       5.2.11. Audio Only, Add Video to BUNDLE . . . . . . . . . . .  52
     5.3.  MultiResolution, RTX, FEC Examples  . . . . . . . . . . .  59
       5.3.1.  Sendonly Simulcast Session with 2 cameras and 2
               encodings     per camera  . . . . . . . . . . . . . .  59
       5.3.2.  Successful SVC Video Session  . . . . . . . . . . . .  65
       5.3.3.  Successful Simulcast Video Session with
               Retransmission  . . . . . . . . . . . . . . . . . . .  70
       5.3.4.  Successful 1-way Simulcast Sessio with 2 resolutions
               and         RTX - One resolution rejected . . . . . .  74
       5.3.5.  Simulcast Video Session with Forward Error Correction  79
     5.4.  Others  . . . . . . . . . . . . . . . . . . . . . . . . .  83
       5.4.1.  Audio Session - Voice Activity Detection  . . . . . .  83
       5.4.2.  Audio Conference - Voice Activity Detection . . . . .  86
       5.4.3.  Successful legacy Interop Fallaback with bundle-only   90



Nandakumar & Jennings   Expires February 5, 2016                [Page 2]

Internet-Draft                 SDP4WebRTC                    August 2015


       5.4.4.  Legacy Interop with RTP/AVP profile . . . . . . . . .  94
   6.  IANA Considerations . . . . . . . . . . . . . . . . . . . . .  99
   7.  Acknowledgments . . . . . . . . . . . . . . . . . . . . . . .  99
   8.  Change Log  . . . . . . . . . . . . . . . . . . . . . . . . .  99
   9.  Informative References  . . . . . . . . . . . . . . . . . . . 100
   Authors' Addresses  . . . . . . . . . . . . . . . . . . . . . . . 104

1.  Introduction

   Javascript Session Exchange Protocol(JSEP) [I-D.ietf-rtcweb-jsep]
   specifies a generic protocol needed to generate [RFC3264] Offers and
   Answers negotiated between the WebRTC peers for setting up, updating
   and tearing down a WebRTC session.  For this purpose, SDP is used to
   construct [RFC3264] Offers/Answers for describing (media and non-
   media) streams as appropriate for the recipients of the session
   description to participate in the session.

   The remainder of this document is organized as follows: Sections 3
   and 4 provides an overview of SDP and the Offer/Answer exchange
   mechanism.  Section 5 provides sample SDP generated for the most
   common WebRTC use-cases.

2.  Terminology

   The key words "MUST", "MUST NOT", "REQUIRED", "SHOULD", "SHOULD NOT",
   "RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be
   interpreted as described in [RFC2119].

3.  SDP and the WebRTC

   The purpose of this section is to provide a general overview of SDP
   and its components.  For a more in-depth understanding, the readers
   are advised to refer to [RFC4566].

   The Session Description Protocol (SDP) [RFC4566] describes multimedia
   sessions, which can contain audio, video, whiteboard, fax, modem, and
   other streams.  SDP provides a general purpose, standard
   representation to describe various aspects of multimedia session such
   as media capabilities, transport addresses and related metadata in a
   transport agnostic manner, for the purposes of session announcement,
   session invitation and parameter negotiation.

   As of today SDP is widely used in the context of Session Initiation
   Protocol [RFC3261], Real-time Transport Protocol [RFC3550] and Real-
   time Streaming Protocol applications [RFC2326].

   Below figure introduces high-level breakup of SDP into components
   that semantically describe a multimedia session, in our case, a



Nandakumar & Jennings   Expires February 5, 2016                [Page 3]

Internet-Draft                 SDP4WebRTC                    August 2015


   WebRTC session [WebRTC].  It by no means captures everything about
   SDP and hence, should be used for informational purposes only.

                                                 +---------------------+
                                                 |        v=           |
                                                 +---------------------+
                 +---------------------+         +---------------------+
         ====    |   Session Metadata  |  =====  |        o=           |
         |       +---------------------+         +----------------------
         |                                       +---------------------+
         |                                       |        t=           |
         |                                       +---------------------+
         |
         |
         |                                       +---------------------+
         |                                       |        c=           |
         |                                       +---------------------+
         |       +---------------------+
         ====    | Network Description |   =====
         |       +---------------------+
         |                                       +---------------------+
         |                                       |    a=candidate      |
         |                                       +---------------------+
         |
         |
         |                                       +---------------------+
         |                                       |        m=           |
         |                                       +---------------------+
         |        +---------------------+        +---------------------+
         ====     | Stream Description  |  ===== |      a=rtpmap       |
         |        +---------------------+        +----------------------
         |                                       +---------------------+
         |                                       |      a=fmtp         |
         |                                       +---------------------+
         |                                       +---------------------+
         |                                       |      a=sendrecv..   |
         |                                       +---------------------+
 +---------------+
 |    SEMANTIC   |
 | COMPONENTS OF |
 |     SDP       |
 +---------------+
         |                                       +---------------------+
         |                                       |      a=crypto       |
         |                                       +---------------------+
         |         +---------------------+       +---------------------+
         ====      |Security Descriptions|  =====|      a=ice-frag     |
         |         +---------------------+       +----------------------



Nandakumar & Jennings   Expires February 5, 2016                [Page 4]

Internet-Draft                 SDP4WebRTC                    August 2015


         |                                       +---------------------+
         |                                       |      a=ice-pwd      |
         |                                       +---------------------+
         |                                       +---------------------+
         |                                       |     a=fingerprint   |
         |                                       +---------------------+
         |
         |
         |
         |                                       +---------------------+
         |                                       |      a=rtcp-fb      |
         |                                       +---------------------+
         |         +---------------------+       +---------------------+
         ====      |   Qos,Grouping      |       |                     |
                   |   Descriptions      |  =====|       a=group       |
                   +---------------------+       +----------------------
                                                 +---------------------+
                                                 |       a=rtcpmux     |
                                                 +---------------------+





                   Figure 1: Semantic Components of SDP

   [WebRTC] proposes JavaScript application to fully specify and control
   the signaling plane of a multimedia session as described in the JSEP
   specification [I-D.ietf-rtcweb-jsep].  JSEP provides mechanisms to
   create session characterization and media definition information to
   conduct the session based on SDP exchanges.

   In this context, SDP serves two purposes:

   1.  Provide grammatical structure syntactically.

   2.  Semantically convey partipant's intention and capabilities
       required to successfully negotiate a session.

4.  Offer/Answer and the WebRTC

   This section introduces SDP Offer/Answer Exchange mechanism mandated
   by WebRTC for negotiating session capabilities while setting up,
   updating and tearing down a WebRTC session.  This section is
   intentionally brief in nature and interested readers are recommended
   to refer [RFC3264] for specific details on the protocol operation.





Nandakumar & Jennings   Expires February 5, 2016                [Page 5]

Internet-Draft                 SDP4WebRTC                    August 2015


   The Offer/Answer [RFC3264] model specifies rule for the bilateral
   exchange of Session Description Protocol (SDP) messages for creation
   of multimedia streams.  It defines protocol with involved
   participants exchanging desired session characteristics from each
   others perspective constructed as SDP to negotiate the session
   between them.

   In the most basic form,the protocol operation begins by one of the
   participants sending an initial SDP Offer describing its intent to
   start a multimedia communication session.  The participant receiving
   the offer MAY generate an SDP Answer accepting the offer or it MAY
   reject the offer.  If the session is accepted the Offer/Answer model
   guarantees a common view of the multimedia session between the
   participants.

   At any time, either participant MAY generate a new SDP offer that
   updates the session in progress.

   With in the context of WebRTC, the Offer/Answer model defines the
   state-machinery for WebRTC peers to negotiate session descriptions
   between them during the initial setup stages as well as for eventual
   session updates.  Javascript Session Establishment Protocol
   specification [I-D.ietf-rtcweb-jsep] for WebRTC provides the
   mechanism for generating [RFC3264] SDP Offers and Answers in order
   for both sides of the session to agree upon details such as list of
   media formats to be sent/received, bandwidth information, crypto
   parameters, transport parameters, for example.

5.  WebRTC Session Description Examples

   A typical web based real-time multimedia communication session can be
   characterized as below:

   o  It has zero or more Audio only, Video only or Audio/Video RTP
      Sessions,

   o  MAY contain zero or more non-media data sessions,

   o  All the sessions are secured with DTLS-SRTP,

   o  Supports NAT traversal using ICE mechanism,

   o  Provides RTCP based feedback mechanisms,

   o  Sessions can be over IPv4-only, IPv6-only, dual-stack based
      clients.





Nandakumar & Jennings   Expires February 5, 2016                [Page 6]

Internet-Draft                 SDP4WebRTC                    August 2015


5.1.  Some Conventions

   The examples given in this document follow the conventions listed
   below:

   o  In all the examples, Alice and Bob are assumed to be the WebRTC
      peers.

   o  [I-D.ietf-mmusic-sdp-bundle-negotiation] support for multiplexing
      several media streams over a single underlying transport is
      assumed by default unless explicitly specified otherwise.

   o  Call-flow diagrams that accompany the use-cases capture only the
      prominent aspects of the system behavior and intentionally is not
      detailed to improve readability.

   o  Eventhough the call-flow diagrams shows SDP being exchanged
      between the parties, it doesn't represent the only way an WebRTC
      setup is expected to work.  Other approaches may involve WebRTC
      applications to exchange the media setup information via non-SDP
      mechanisms as long as they confirm to the [I-D.ietf-rtcweb-jsep]
      API specification.

   o  The SDP examples deviate from actual on-the-wire SDP notation in
      several ways.  This is done to facilitate readability and to
      conform to the restrictions imposed by the RFC formatting rules.

      *  Any SDP line that is indented (compared to the initial line in
         the SDP block) is a continuation of the preceding line.  The
         line break and indent are to be interpreted as a single space
         character.

      *  Empty lines in any SDP example are inserted to make functional
         divisions in the SDP clearer, and are not actually part of the
         SDP syntax.

      *  Excepting the above two conventions, line endings are to be
         interpreted as <CR><LF> pairs (that is, an ASCII 13 followed by
         an ASCII 10).

   o  Against each SDP line, pointers to the appropriate RFCs are
      provided for further informational reference.  Also an attempt has
      been made to provide explanatory notes to enable better
      understanding of the SDP usage, wherever appropriate.

   o  Following SDP details are common across all the use-cases defined
      in this document unless mentioned otherwise.




Nandakumar & Jennings   Expires February 5, 2016                [Page 7]

Internet-Draft                 SDP4WebRTC                    August 2015


      *  DTLS fingerprint for SRTP (a=fingerprint)

      *  RTP/RTCP Multiplexing (a=rtcp-mux)

      *  RTCP Feedback support (a=rtcp-fb)

      *  Host and server-reflexive candidate lines (a=candidate)

      *  SRTP Setup framework parameters (a=setup)

      *  RTCP attribute (a=rtcp)

      *  RTP header extension indicating audio-levels from client to the
         mixer

      For more details, readers are recommended to refer to
      [I-D.ietf-rtcweb-jsep] specification.

   o  The term "Session" is used rather loosely in this document to
      refer to either a "Communication Session" or a "RTP Session" or a
      "RTP Stream" depending on the context.

   o  Payload type 109 is usually used for OPUS, 0 for PCMU, 8 for PCMA,
      99 for H.264 and 120 for VP8 in most of the examples to maintain
      uniformity.

   o  In the actual use the values that represent SSRCs, ICE candidate
      foundations, WebRTC Mediastream and MediaStreamTrack Ids shall be
      much larger and random than the ones shown in the examples.

      [OPEN ISSUE-1]: SDP Examples for Data Channel, Simulcast, SVC are
      still being discussed and doesn't represent the final solution.

5.2.  Basic Examples

5.2.1.  Audio Only Session

   This common scenario shows SDP for secure two-way audio session with
   Alice offering Opus, PCMU, PCMA and Bob accepting all the offered
   audio codecs.











Nandakumar & Jennings   Expires February 5, 2016                [Page 8]

Internet-Draft                 SDP4WebRTC                    August 2015


           2-Way Audio Only Session

   Alice                                Bob
   |                                     |
   |                                     |
   |    Offer(Audio:Opus,PCMU,PCMA)      |
   |------------------------------------>|
   |                                     |
   |                                     |
   |   Answer(Audio:Opus,PCMU,PCMA)      |
   |<------------------------------------|
   |                                     |
   |                                     |
   |Two-way Opus Audio (preferred-codec) |
   |.....................................|
   |                                     |
   |                                     |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing                |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus Codec 48khz, 2 channels   |
   | a=ptime:60                       | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus packetization of 60ms     |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:8 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464] Alice supports RTP   |



Nandakumar & Jennings   Expires February 5, 2016                [Page 9]

Internet-Draft                 SDP4WebRTC                    August 2015


   | hdrext:ssrc-audio-level          | header extension to indicate   |
   |                                  | audio levels                   |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv audio                     |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | for SRTP                       |
   | : 1f:66:79:a8:07                 |                                |
   | a=ice-ufrag:074c6550             | [RFC5245] - ICE user fragment  |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - ICE password       |
   | 474af08a068                      |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245] - RTP Host Candidate |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245] - RTCP Host          |
   | 192.168.1.4 54609 typ host       | Candidate                      |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245] - RTP Server         |
   | 24.23.204.141 64678 typ srflx    | Reflexive ICE Candidate        |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245] - RTCP Server        |
   | 24.23.204.141 64678 typ srflx    | Reflexive Candidate            |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:12345                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506] - Alice intends to   |
   |                                  | use reduced size RTCP for this |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 1: 5.2.1 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 10]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   |                                  | Opus Codec                     |
   | a=ptime:60                       | [I-D.ietf-payload-rtp-opus]    |
   |                                  | Packetization of 60ms          |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:8 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464] Bob supports audio   |
   | hdrext:ssrc-audio-level          | level RTP header extension as  |
   |                                  | well                           |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | recv audio                     |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing on port 49203  |
   | a=fingerprint:sha-1 c9:c7:70:9d: | [RFC5245] - DTLS Fingerprint   |
   | 1f:66:79:a8:07:99:41:49:83:4a:   | for SRTP                       |
   | 97:0e:1f:ef:6d:f7                |                                |
   | a=ice-ufrag:05067423             | [RFC5245] - ICE user fragment  |
   | a=ice-pwd:1747d1ee3474a28a397a4c | [RFC5245] - ICE password       |
   | 3f3af08a068                      | parameter                      |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245] - RTP/RTCP Host ICE  |
   | 192.168.1.7 49203 typ host       | Candidate                      |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245] - RTP/RTCP Server    |
   | 98.248.92.77 60654 typ srflx     | Reflexive ICE Candidate        |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:54321                     | [RFC5576]                      |
   | cname:NWs1ao1HmN4Xa5/yvY7        |                                |
   | a=rtcp-rsize                     | [RFC5506] - Bob intends to use |
   |                                  | reduced size RTCP for this     |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 2: 5.2.1 SDP Answer

5.2.2.  Audio/Video Session

   Alice and Bob establish a two-way audio and video session with Opus
   as the audio codec and H.264 as the video codec.





Nandakumar & Jennings   Expires February 5, 2016               [Page 11]

Internet-Draft                 SDP4WebRTC                    August 2015


            2-Way Audio,Video Session

   Alice                                       Bob
   |                                            |
   |                                            |
   |Offer(Audio:Opus,PCMU,PCMA Video:H.264,VP8) |
   |------------------------------------------->|
   |                                            |
   |                                            |
   |      Answer(Audio:Opus,Video:H.264)        |
   |<-------------------------------------------|
   |                                            |
   |                                            |
   |     Two-way Opus Audio, H.264 Video        |
   |............................................|
   |                                            |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing                |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus Codec 48khz, 2 channels   |
   | a=ptime:60                       | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus packetization of 60ms     |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:8 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 12]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv audio                     |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245] - ICE user fragment  |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - ICE password       |
   | 474af08a068                      | parameter                      |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | for SRTP                       |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245] - RTP Host Candidate |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245] - RTCP Host          |
   | 192.168.1.4 54609 typ host       | Candidate                      |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245] - RTP Server         |
   | 24.23.204.141 64678 typ srflx    | Reflexive ICE Candidate        |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245] - RTCP Server        |
   | 24.23.204.141 64678 typ srflx    | Reflexive Candidate.           |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:12345                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506] - Alice intends to   |
   |                                  | use reduced size RTCP for this |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 99 120                           |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:video                      | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing                |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:99 H264/90000           | [RFC3984] - H.264 Video Codec  |
   | a=fmtp:99 profile-level-         | [RFC3984]                      |
   | id=4d0028;packetization-mode=1   |                                |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8] - VP8   |
   |                                  | video codec                    |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv video                     |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |



Nandakumar & Jennings   Expires February 5, 2016               [Page 13]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ice-ufrag:074c6550             | [RFC5245] - ICE user fragment  |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - ICE password       |
   | 474af08a068                      | parameter                      |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | for SRTP                       |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245] - RTP Host ICE       |
   | 192.168.1.4 54609 typ host       | Candidate                      |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245] - RTCP Host          |
   | 192.168.1.4 54609 typ host       | Candidate                      |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245] - RTP Server         |
   | 24.23.204.141 64678 typ srflx    | Reflexive ICE Candidate        |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245] - RTCP Server        |
   | 24.23.204.141 64678 typ srflx    | Reflexive Candidate            |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:99 nack                | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=rtcp-fb:99 nack pli            | [RFC5104] - Indicates support  |
   |                                  | for Picture loss Indication    |
   |                                  | and NACK                       |
   | a=rtcp-fb:99 ccm fir             | [RFC5104] - Full Intra Frame   |
   |                                  | Request-Codec Control Message  |
   |                                  | support                        |
   | a=rtcp-fb:120 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=rtcp-fb:120 nack pli           | [RFC5104] - Indicates support  |
   |                                  | for Picture loss Indication    |
   |                                  | and NACK                       |
   | a=rtcp-fb:120 ccm fir            | [RFC5104] - Full Intra Frame   |
   |                                  | Request-Codec Control Message  |
   |                                  | support                        |
   | a=ssrc:1366781083                | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506] - Alice intends to   |
   |                                  | use reduced size RTCP for this |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 3: 5.2.2 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |



Nandakumar & Jennings   Expires February 5, 2016               [Page 14]

Internet-Draft                 SDP4WebRTC                    August 2015


   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Bob accepts only Opus Codec    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:60                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | recv audio                     |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=ice-ufrag:c300d85b             | [RFC5245] -  ICE username frag |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] - ICE password       |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | for SRTP                       |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 3618095783   | [RFC5245] - RTP/RTCP Host ICE  |
   | 192.168.1.7 49203 typ host       | Candidate                      |
   | a=candidate:1 1 UDP 565689203    | [RFC5245] - RTP/RTCP Server    |
   | 98.248.92.77 60065 typ srflx     | Reflexive ICE Candidate        |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:1366788312                | [RFC5576]                      |
   | cname:1f0fcgEocUG/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506] - Bob intends to use |
   |                                  | reduced size RTCP for this     |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 99                               |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:video                      | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtpmap:99 H264/90000           | [RFC3984] - Bob accepts H.264  |



Nandakumar & Jennings   Expires February 5, 2016               [Page 15]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | Video Codec.                   |
   | a=fmtp:99 profile-level-         | [RFC3984]                      |
   | id=4d0028;packetization-mode=1   |                                |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing                |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | recv video                     |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=ice-ufrag:c300d85b             | [RFC5245] -  ICE username frag |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] - ICE password       |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | for SRTP                       |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245] - Host ICE Candidate |
   | 192.168.1.7 49203 typ host       | for Opus Stream                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245] - Server Reflexive   |
   | 98.248.92.77 60065 typ srflx     | ICE Candidate for the above    |
   | raddr 192.168.1.7 rport 49203    | host candidate                 |
   | [RFC5245] - Server Reflexive     | a=rtcp-fb:99 nack              |
   | Candidate for the Second Host    |                                |
   | Candidate                        |                                |
   | [RFC5104] - Indicates support    | a=rtcp-fb:99 nack pli          |
   | for NACK based RTCP feedback     |                                |
   | [RFC5104] - Indicates support    | a=rtcp-fb:99 ccm fir           |
   | for Picture loss Indication and  |                                |
   | NACK                             |                                |
   | [RFC5104] - Full Intra Frame     | a=ssrc:3229706345              |
   | Request- Codec Control Message   | cname:Q/NWs1ao1HmN4Xa5         |
   | support                          |                                |
   | [RFC5576]                        | a=rtcp-rsize                   |
   | [RFC5506] - Bob intends to use   | a=ice-options:trickle          |
   | reduced size RTCP for this       |                                |
   | session                          |                                |
   | [I-D.ietf-mmusic-trickle-ice]    |
   +----------------------------------+--------------------------------+

                         Table 4: 5.2.2 SDP Answer

5.2.3.  Data Only Session

   This scenario illustrates SDP negotiated to setup a data-only session
   based on SCTP Data Channel, thus enabling use-cases such as file-
   transfer for example.






Nandakumar & Jennings   Expires February 5, 2016               [Page 16]

Internet-Draft                 SDP4WebRTC                    August 2015


         2-Way DataChannel Session

   Alice                            Bob
   |                                 |
   |                                 |
   |                                 |
   |      Offer(DataChannel)         |
   |-------------------------------->|
   |                                 |
   |                                 |
   |      Answer(DataChannel)        |
   |<--------------------------------|
   |                                 |
   |                                 |
   | Two-way SCTP based DataChannel  |
   |.................................|
   |                                 |
   |                                 |

































Nandakumar & Jennings   Expires February 5, 2016               [Page 17]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=group:BUNDLE data              | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-ufrag:074c6550             | [RFC5245] - Session Level ICE  |
   |                                  | parameter                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - Session Level ICE  |
   | 474af08a068                      | parameter                      |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - Session DTLS       |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | Fingerprint for SRTP           |
   | 9d:1f:66:79:a8:07                |                                |
   | m=application 56966 DTLS/SCTP    | [I-D.ietf-rtcweb-data-channel] |
   | 5000                             |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel                      |                                |
   | streams=16;label="channel 1";    |                                |
   | subprotocol="chat";              |                                |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv non-media data            |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 56966 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 24.23.204.141 56966 typ srflx    |                                |
   | raddr 192.168.1.7 rport 56966    |                                |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 5: 5.2.3 SDP Offer













Nandakumar & Jennings   Expires February 5, 2016               [Page 18]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=group:BUNDLE data              | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=application 55700 DTLS/SCTP    | [I-D.ietf-mmusic-sctp-sdp]     |
   | 5000                             |                                |
   | c=IN IP4 98.248.92.771           | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel:5000                 |                                |
   | streams=1;label="channel 1"      |                                |
   | ;subprotocol="chat";             |                                |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | recv non-media data            |
   | a=ice-ufrag:c300d85b             | [RFC5245] - Session Level ICE  |
   |                                  | username frag                  |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] - Session Level ICE  |
   | 7efbabd9a2                       | password                       |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - Session DTLS       |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | Fingerprint for SRTP           |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 55700 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 55700 typ srflx     |                                |
   | raddr 192.168.1.7 rport 55700    |                                |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 6: 5.2.3 SDP Answer

5.2.4.  Audio Call On Hold

   Alice calls Bob, but when Bob answers he places Alice on hold by
   setting the SDP direction attribute to a=sendonly in the Answer.








Nandakumar & Jennings   Expires February 5, 2016               [Page 19]

Internet-Draft                 SDP4WebRTC                    August 2015


              Audio On Hold

   Alice                            Bob
   |                                 |
   |                                 |
   |      Offer(Audio:Opus)          |
   |-------------------------------->|
   |                                 |
   |                                 |
   |  Answer(Audio:Opus,a=sendonly)  |
   |<--------------------------------|
   |                                 |
   |                                 |
   |      One-way Opus Audio         |
   |.................................|
   |                                 |
   |

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing                |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus Codec 48khz, 2 channels   |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus packetization of 20ms     |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv audio                     |



Nandakumar & Jennings   Expires February 5, 2016               [Page 20]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245] -  ICE user fragment |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] -  ICE password      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | for SRTP                       |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:3229706345                | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 7: 5.2.4 SDP Offer
























Nandakumar & Jennings   Expires February 5, 2016               [Page 21]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Bob accepts Opus Codec         |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendonly                       | [RFC3264] - Bob puts call On   |
   |                                  | Hold                           |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing                |
   | a=ice-ufrag:c300d85b             | [RFC5245] - ICE username frag  |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] - ICE password       |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | for SRTP                       |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:1366781083                | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 8: 5.2.4 SDP Answer




Nandakumar & Jennings   Expires February 5, 2016               [Page 22]

Internet-Draft                 SDP4WebRTC                    August 2015


5.2.5.  Audio with DTMF Session

   In this example, Alice wishes to establish two separate audio
   streams, one for normal audio and the other for telephone-events.
   Alice offers first audio stream with three codecs and the other with
   [RFC2833] tones (for DTMF).  Bob accepts both the audio streams by
   choosing Opus as the audio codec and telephone-event for the other
   stream.


               Audio Session with DTMF

   Alice                                              Bob
   |                                                   |
   |                                                   |
   |                                                   |
   |  Offer(Audio:Opus,PCMU,PCMA Audio:telephone-event)|
   |-------------------------------------------------->|
   |                                                   |
   |                                                   |
   |    Answer(Audio:Opus, Audio:telephone-event)      |
   |<--------------------------------------------------|
   |                                                   |
   |                                                   |
   |   Opus audio stream and telephone-event stream    |
   |...................................................|
   |                                                   |

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio dtmf        | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 23]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus Codec 48khz, 2 channels   |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Opus packetization of 20ms     |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:8 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv audio                     |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245] -  ICE user fragment |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - ICE password       |
   | 474af08a068                      | parameter                      |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | for SRTP                       |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:3229706345                | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 126                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:dtmf                       | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:126 telephone-          | [RFC2833]                      |
   | event/8000                       |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 24]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=sendonly                       | [RFC3264] - Alice can send     |
   |                                  | DTMF Events                    |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245] -  ICE user fragment |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245] - ICE password       |
   | 474af08a068                      | parameter                      |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] - DTLS Fingerprint   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | for SRTP                       |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104] - Indicates NACK     |
   |                                  | RTCP feedback support          |
   | a=ssrc:9032206345                | [RFC5576]                      |
   | cname:L/N9lk1ao1HmN4Xa5          |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 9: 5.2.5 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio dtmf        | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |



Nandakumar & Jennings   Expires February 5, 2016               [Page 25]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Bob accepts Opus Codec         |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | receive Opus audio             |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing on port 49203  |
   | a=ice-ufrag:c300d85b             | [RFC5245] -  ICE username frag |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] -  ICE password      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] -  Fingerprint for   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   | SRTP                           |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:0634322975                | [RFC5576]                      |
   | cname:Q/o1HmN4XNWs1aa5           |                                |
   | a=rtcp-rsize                     | [RFC5506] - Alice intends to   |
   |                                  | use reduced size RTCP for this |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 126                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:dtmf                       | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtpmap:126 telephone-          | [RFC2833]                      |
   | event/8000                       |                                |
   | a=recvonly                       | [RFC3264] - Alice can receive  |
   |                                  | DTMF events                    |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761] - Alice can perform  |
   |                                  | RTP/RTCP Muxing on port 54690  |
   | a=ice-ufrag:c300d85b             | [RFC5245] -  ICE username frag |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245] -  ICE password      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245] -  Fingerprint for   |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d | SRTP                           |



Nandakumar & Jennings   Expires February 5, 2016               [Page 26]

Internet-Draft                 SDP4WebRTC                    August 2015


   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:6345903220                | [RFC5576]                      |
   | cname:L/k1aN9lo1HmN4Xa5          |                                |
   | a=rtcp-rsize                     | [RFC5506] - Alice intends to   |
   |                                  | use reduced size RTCP for this |
   |                                  | session                        |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                        Table 10: 5.2.5 SDP Answer

5.2.6.  One Way Audio/Video Session - Document Camera

   In this scenario Alice and Bob engage in a 1 way audio and video
   session with Bob receiving Alice's audio and her presentation slides
   as video stream.



      One Way Audio & Video Session - Document Camera


   Alice                                                 Bob
   |                                                      |
   |                                                      |
   |                                                      |
   |   Alice Offers sendonly audio and video streams.     |
   |  The video stream corresponds to her presentation    |
   |                                                      |
   |           Offer(Audio:Opus, Video: VP8)              |
   |----------------------------------------------------->|
   |                                                      |
   |                                                      |
   |            (Audio:Opus, Video: VP8)                  |
   |<-----------------------------------------------------|
   |                                                      |
   |                                                      |
   |          One-way Opus Audio, VP8 Video               |
   |......................................................|
   |   Bob can hear Alice and see her presentation slides.|
   |                                                      |
   |                                                      |




Nandakumar & Jennings   Expires February 5, 2016               [Page 27]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendonly                       | [RFC3264] - Send only audio    |
   |                                  | stream                         |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 24.23.204.141 54609 typ host     |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5104]                      |
   | 24.23.204.141 54609 typ host     |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:6345903220                | [RFC5576]                      |
   | cname:L/k1aN9lo1HmN4Xa5          |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:video                      | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 28]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp:54609 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=content:slides                 | [RFC4796] -Alice's             |
   |                                  | presentation video stream      |
   | a=sendonly                       | [RFC3264] - Send only video    |
   |                                  | stream                         |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 24.23.204.141 54609 typ host     |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5104]                      |
   | 24.23.204.141 54609 typ host     |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:3429951804                | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 11: 5.2.6 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 16833 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 29]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=recvonly                       | [RFC3264] - Receive only audio |
   |                                  | stream                         |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d |                                |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 98.248.92.77 49203 typ host      |                                |
   | a=ssrc:9513429804                | [RFC5576]                      |
   | cname:Q/o1HmNWs1aN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:video                      | [RFC5888]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=content:slides                 | [RFC4796]                      |
   | a=recvonly                       | [RFC3264] - Receive Only       |
   |                                  | Alice's presentation stream    |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 98.248.92.77 49203 typ host      |                                |
   | a=ssrc:1366781083                | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 30]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                        Table 12: 5.2.6 SDP Answer

5.2.7.  Audio, Video Session with BUNDLE Support Unknown

   In this example, since Alice is unsure of the Bob's support of the
   BUNDLE framework, following 3 step procedures are performed in order
   to negotiate and setup a BUNDLE Address for the session

   o  An SDP Offer, in which the Alice assigns unique addresses to each
      "m=" line in the BUNDLE group, and requests the Answerer to select
      the Offerer's BUNDLE address.

   o  An SDP Answer, in which the Bob indicates its support for BUNDLE,
      and assigns its own BUNDLE address for the BUNDLED m= lines.

   o  A subsequent SDP Offer from Alice, which is used to perform BUNDLE
      Address Synchronization (BAS).

   Once the Offer/Answer exchange completes, both Alice and Bob each end
   up using single RTP Session for both the Media Streams.




























Nandakumar & Jennings   Expires February 5, 2016               [Page 31]

Internet-Draft                 SDP4WebRTC                    August 2015


    Two-Way Secure Audio,Video with BUNDLE support unknown

   Alice                                                 Bob
   |                                                      |
   |                                                      |
   |   Alice offers BUNDLE support with unique address    |
   |        for the audio and video m-line                |
   |                                                      |
   |                                                      |
   |          Offer(Audio:Opus Video:VP8)                 |
   |----------------------------------------------------->|
   |                                                      |Bob
   |                                                      |supports
   |                                                      |BUNDLE
   |          Answer(Audio:Opus Video:VP8)                |
   |<-----------------------------------------------------|
   |                                                      |
   |                                                      |Bob uses
   |                                                      |identical
   |                                                      |addresses
   |                                                      |
   |  Updated Offer for Bundle Address Synchronization.   |
   |----------------------------------------------------->|
   |                                                      |
   |                                                      |
   |       Answer (Bob accepts the updated offer)         |
   |<-----------------------------------------------------|
   |                                                      |
   |                                                      |
   |    2 Way Call with Audio and Video Multiplexed       |
   |......................................................|
   |                                                      |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 32]

Internet-Draft                 SDP4WebRTC                    August 2015


   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888] Audio m=line part of |
   |                                  | BUNDLE group with a unique     |
   |                                  | port number                    |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 62537 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:video                      | [RFC5888] Video m=line part of |
   |                                  | the Bundle group with a unique |
   |                                  | port number                    |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp-mux                       | [RFC5761]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 33]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp:62537 IN IP4              | [RFC3605] - Port for RTCP data |
   | 24.23.204.141                    |                                |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ice-ufrag:6550074c             | [RFC5245]                      |
   | a=ice-pwd:74af08a068a28a397a4c3f | [RFC5245]                      |
   | 31747d1ee34                      |                                |
   | a=fingerprint:sha-1 1f:ef:6d:f7: | [RFC5245]                      |
   | c9:c7:70:9d:1f:66:99:41:49:83:   |                                |
   | 4a:97:0e79:a8:07                 |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 62537 typ host       |                                |
   | a=candidate:0 2 2122194687       | [RFC5245]                      |
   | 192.168.1.4 62537 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 62537 typ srflx    |                                |
   | raddr 192.168.1.4 rport 62537    |                                |
   | a=candidate:1 2 UDP 1685987071   | [RFC5245]                      |
   | 24.23.204.141 62537 typ srflx    |                                |
   | raddr 192.168.1.4 rport 62537    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                    Table 13: 5.2.7 SDP Offer w/BUNDLE

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Bob supports BUNDLE |
   |                                  | semantics.                     |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 34]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=mid:audio                      | [RFC5888] Audio m=line part of |
   |                                  | the BUNDLE group               |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:Q/1HmN4Xa5NWs1ao           |                                |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:video                      | [RFC5888] Video m=line part of |
   |                                  | the BUNDLE group with the port |
   |                                  | from audio line repeated       |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145] - Bob carries out    |
   |                                  | DTLS Handshake in parallel     |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:44444                     | [RFC5576]                      |
   | cname:Q/2AqlmN4Xa5NWs            |                                |
   | a=ice-ufrag:85bc300d             | [RFC5245]                      |
   | a=ice-pwd:bd2de4e9991c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 41:49:83:4a: | [RFC5245]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 35]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 99:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                    Table 14: 5.2.7 SDP Answer w/BUNDLE

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=mid:audio                      | [RFC5888]  - Port number       |
   |                                  | finalized as Bundle Address.   |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 36]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                     Table 15: 5.2.7 SDP Offer for BAS








Nandakumar & Jennings   Expires February 5, 2016               [Page 37]

Internet-Draft                 SDP4WebRTC                    August 2015


5.2.8.  Audio, Video and Data Session

   This example shows SDP for negotiating a session with Audio, Video
   and data streams between Alice and Bob with BUNDLE support known.



       Audio,Video,Data with BUNDLE support known

   Alice                                       Bob
   |                                            |
   |                                            |
   |Alice indicates BUNDLE support with         |
   |identical address across all the m=lines    |
   |                                            |
   |                                            |
   |     Offer(Audio:Opus Video:VP8 Data)       |
   |------------------------------------------->|
   |                                            |Bob does
   |                                            |the same
   |    Answer(Audio:Opus,Video:VP8 Data)       |
   |<-------------------------------------------|
   |                                            |
   |                                            |
   |                                            |
   |    Two-way Audio,Video, Data multiplexed   |
   |............................................|
   |                                            |
   |                                            |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video data  | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 38]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 24.23.204.141                    |                                |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 39]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:Q/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=application 54609 DTLS/SCTP    | [I-D.ietf-rtcweb-data-channel] |
   | 5000                             |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel                      |                                |
   | streams=1;label="channel 1";     |                                |
   | subprotocol="chat";              |                                |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   +----------------------------------+--------------------------------+

                         Table 16: 5.2.8 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 40]

Internet-Draft                 SDP4WebRTC                    August 2015


   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video data  | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   |                                  | Bob's trickle support support  |
   |                                  | is indicated at the session    |
   |                                  | level                          |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:L/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 41]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:44444                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=application 49203 DTLS/SCTP    | [I-D.ietf-mmusic-sctp-sdp]     |
   | 5000                             |                                |
   | c=IN IP4 98.248.92.771           | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel                      |                                |
   | streams=16;label="channel 1";    |                                |
   | subprotocol="chat";              |                                |
   | a=setup:active                   | [RFC4145]                      |
   | a=sendrecv                       | [RFC3264]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1685987071   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   +----------------------------------+--------------------------------+

                        Table 17: 5.2.8 SDP Answer







Nandakumar & Jennings   Expires February 5, 2016               [Page 42]

Internet-Draft                 SDP4WebRTC                    August 2015


5.2.9.  Audio, Video Session with BUNDLE Unsupported

   This use-case illustrates SDP Offer/Answer exchange where the far-end
   (Bob) either doesn't support media bundling or doesn't want to group
   m=lines over a single 5-tuple.

   On successful Offer/Answer exchange, Alice and Bob each end up using
   unique 5-tuple for audio and video media streams respectively.



      Two-Way Secure Audio,Video with BUNDLE Unsupported

   Alice                                                 Bob
   |                                                      |
   |                                                      |
   |     Alice offers BUNDLE support with unique address  |
   |           for the audio and video m-line             |
   |                                                      |
   |                                                      |
   |           Offer(Audio:Opus Video:VP8)                |
   |----------------------------------------------------->|
   |                                                      |Bob
   |                                                      |doesn't
   |                                                      |support
   |                                                      |BUNDLE
   |           Answer(Audio:Opus Video:VP8)               |
   |<-----------------------------------------------------|
   |                                                      |Bob uses
   |                                                      |unique
   |                                                      |addresses
   |                                                      |across the
   |                                                      |m=lines
   |                                                      |
   |2Way Call with Audio and Video on different 5-tuples  |
   |......................................................|
   |                                                      |
   |                                                      |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 43]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 55232 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=mid:audio                      | [RFC5888] Audio m=line part of |
   |                                  | BUNDLE group with a unique     |
   |                                  | port number                    |
   | a=rtcp:55232 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=sendrecv                       | [RFC3264]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d |                                |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 55232 typ host       |                                |
   | a=candidate:0 2 UDP 2122194687   | [RFC5245]                      |
   | 192.168.1.4 55232 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 55232 typ srflx    |                                |
   | raddr 192.168.1.4 rport 55232    |                                |
   | a=candidate:1 2 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 55232 typ srflx    |                                |
   | raddr 192.168.1.4 rport 55232    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 54332 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 44]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=mid:video                      | [RFC5888] Video m=line part of |
   |                                  | the BUNDLE group with a unique |
   |                                  | port number                    |
   | a=rtcp:54332 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145] - Alice can perform  |
   |                                  | DTLS before Answer arrives     |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:yvY7/EocUG1f0fcg           |                                |
   | a=ice-ufrag:7872093              | [RFC5245]                      |
   | a=ice-pwd:ee3474af08a068a28a397a | [RFC5245]                      |
   | 4c3f31747d1                      |                                |
   | a=fingerprint:sha-1 6d:f7:c9:c7: | [RFC5245]                      |
   | 70:9d:1f:66:79:a8:07:99:41:      |                                |
   | 49:83:4a:97:0e:1f:ef             |                                |
   | a=candidate:0 1 UDP  2122194687  | [RFC5245]                      |
   | 192.168.1.4 54332 typ host       |                                |
   | a=candidate:0 2 2122194687       | [RFC5245]                      |
   | 192.168.1.4 54332 typ host       |                                |
   | a=candidate:1 1 UDP  1685987071  | [RFC5245]                      |
   | 24.23.204.141 54332 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54332    |                                |
   | a=candidate:1 2 UDP 1685987071   | [RFC5245]                      |
   | 24.23.204.141 54332 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54332    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                    Table 18: 5.2.9 SDP Offer w/BUNDLE

   +---------------------------------------+---------------------------+
   | SDP Contents                          | RFC#/Notes                |
   +---------------------------------------+---------------------------+
   | v=0                                   | [RFC4566]                 |
   | o=-  16833 0 IN IP4 0.0.0.0           | [RFC4566]                 |
   | s=-                                   | [RFC4566]                 |
   | t=0 0                                 | [RFC4566]                 |
   | a=msid-semantic:WMS ma                | [I-D.ietf-mmusic-msid]    |
   | a=ice-options:trickle                 | [I-D.ietf-mmusic-trickle- |
   |                                       | ice]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 45]

Internet-Draft                 SDP4WebRTC                    August 2015


   | m=audio 53214 UDP/TLS/RTP/SAVPF 109   | [RFC4566]                 |
   | c=IN IP4 98.248.92.77                 | [RFC4566]                 |
   | a=msid:ma ta                          | Identifies RTCMediaStream |
   |                                       | ID (ma) and               |
   |                                       | RTCMediaStreamTrack ID    |
   |                                       | (ta)                      |
   | a=rtcp:60065 IN IP4 98.248.92.77      | [RFC3605]                 |
   | a=rtpmap:109 opus/48000/2             | [I-D.ietf-payload-rtp-opu |
   |                                       | s]                        |
   | a=extmap:1 urn:ietf:params:rtp-hdrext | [RFC6464]                 |
   | :ssrc-audio-level                     |                           |
   | a=ptime:20                            | [I-D.ietf-payload-rtp-opu |
   |                                       | s]                        |
   | a=setup:active                        | [RFC4145] - Bob carries   |
   |                                       | out DTLS Handshake in     |
   |                                       | parallel                  |
   | a=sendrecv                            | [RFC3264]                 |
   | a=rtcp-fb:109 nack                    | [RFC5104]                 |
   | a=ice-ufrag:c300d85b                  | [RFC5245]                 |
   | a=ice-                                | [RFC5245]                 |
   | pwd:de4e99bd291c325921d5d47efbabd9a2  |                           |
   | a=fingerprint:sha-1 99:41:49:83:4a:97 | [RFC5245]                 |
   | :0e:1f:ef:6d:f7:c9:c7:70:             |                           |
   | 9d:1f:66:79:a8:07                     |                           |
   | a=candidate:0 1 UDP 2122194687        | [RFC5245]                 |
   | 192.168.1.7 53214 typ host            |                           |
   | a=candidate:1 1 UDP 1685987071        | [RFC5245]                 |
   | 98.248.92.77 53214 typ srflx raddr    |                           |
   | 192.168.1.7 rport 53214               |                           |
   | a=candidate:0 2 UDP 2122194687        | [RFC5245]                 |
   | 192.168.1.7 60065 typ host            |                           |
   | a=candidate:1 2 UDP 1685987071        | [RFC5245]                 |
   | 98.248.92.77 60065 typ srflx raddr    |                           |
   | 192.168.1.7 rport 60065               |                           |
   | a=rtcp-rsize                          | [RFC5506]                 |
   | m=video 58679 UDP/TLS/RTP/SAVPF 120   | [RFC4566]                 |
   | c=IN IP4 98.248.92.77                 | [RFC4566]                 |
   | a=msid:ma tb                          | Identifies RTCMediaStream |
   |                                       | ID (ma) and               |
   |                                       | RTCMediaStreamTrack ID    |
   |                                       | (tb)                      |
   | a=rtcp:56507 IN IP4 98.248.92.77      | [RFC3605]                 |
   | a=rtpmap:120 VP8/90000                | [I-D.ietf-payload-vp8]    |
   | a=setup:active                        | [RFC4145] - Bob carries   |
   |                                       | out DTLS Handshake in     |
   |                                       | parallel                  |
   | a=sendrecv                            | [RFC3264]                 |
   | a=ice-ufrag:85bC300                   | [RFC5245]                 |



Nandakumar & Jennings   Expires February 5, 2016               [Page 46]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ice-                                | [RFC5245]                 |
   | pwd:325921d5d47efbabd9a2de4e99bd291c  |                           |
   | a=fingerprint:sha-1 9d:1f:66:79:a8:07 | [RFC5245]                 |
   | :99:41:49:83:4a:97:0e:1f:             |                           |
   | ef:6d:f7:c9:c7:70                     |                           |
   | a=candidate:0 1 UDP 2122194687        | [RFC5245]                 |
   | 192.168.1.7 58679 typ host            |                           |
   | a=candidate:1 1 UDP 1685987071        | [RFC5245]                 |
   | 98.248.92.77 58679 typ srflx raddr    |                           |
   | 192.168.1.7 rport 58679               |                           |
   | a=candidate:0 1 UDP 2122194687        | [RFC5245]                 |
   | 192.168.1.7 56507 typ host            |                           |
   | a=candidate:1 1 UDP 1685987071        | [RFC5245]                 |
   | 98.248.92.77 56507 typ srflx raddr    |                           |
   | 192.168.1.7 rport 58679               |                           |
   | a=rtcp-fb:120 nack                    | [RFC5104]                 |
   | a=rtcp-fb:120 nack pli                | [RFC5104]                 |
   | a=rtcp-fb:120 ccm fir                 | [RFC5104]                 |
   | a=rtcp-rsize                          | [RFC5506]                 |
   +---------------------------------------+---------------------------+

                 Table 19: 5.2.9 SDP Answer without BUNDLE

5.2.10.  Audio, Video BUNDLED, but Data (Not BUNDLED)

   This example show-cases SDP for negotiating a session with Audio,
   Video and data streams between Alice and Bob with data stream not
   being part of the BUNDLE group.  This is shown by assigning unique
   port for data media sections.






















Nandakumar & Jennings   Expires February 5, 2016               [Page 47]

Internet-Draft                 SDP4WebRTC                    August 2015


          Audio, Video, with Data (Not in BUNDLE)

   Alice                                                 Bob
   |                                                      |
   |                                                      |
   |Alice wants to multiplex audio, video but not data    |
   |                                                      |
   |                                                      |
   |  Offer(Audio:Opus Video:VP8, Data(not in BUNDLE))    |
   |----------------------------------------------------->|
   |                                                      |
   |                                                      |
   |       Answer(Audio:Opus Video:VP8, Data)             |
   |<-----------------------------------------------------|
   |                                                      |
   |                                                      |
   |2 Way Call with Audio, Video Multiplexed except data  |
   |......................................................|
   |                                                      |



   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice wants to      |
   |                                  | BUNDLE only audio and video    |
   |                                  | media.                         |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |



Nandakumar & Jennings   Expires February 5, 2016               [Page 48]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:54609 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:Q/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=application 10000 DTLS/SCTP    | [I-D.ietf-rtcweb-data-channel] |
   | 5000                             |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 49]

Internet-Draft                 SDP4WebRTC                    August 2015


   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel                      |                                |
   | streams=16;label="channel 1";    |                                |
   | subprotocol="chat";              |                                |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=ice-ufrag:89819013             | [RFC5245]                      |
   | a=ice-pwd:1747d1ee3474af08a068a2 | [RFC5245]                      |
   | 8a397a4c3f3                      |                                |
   | a=fingerprint:sha-1 0e:1f:ef:6d: | [RFC5245]                      |
   | f7:c9:c7:70:99:41:49:83:4a:97:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 10000 typ host       |                                |
   +----------------------------------+--------------------------------+

                        Table 20: 5.2.10 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 50]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:L/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 98.248.92.771           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:44444                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=application 20000 DTLS/SCTP    | [I-D.ietf-mmusic-sctp-sdp]     |
   | 5000                             |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:data                       | [RFC5888]                      |
   | a=sctpmap:5000 webrtc-           | [I-D.ietf-mmusic-sctp-sdp]     |
   | DataChannel                      |                                |
   | streams=1;label="channel 1";     |                                |
   | subprotocol="chat";              |                                |
   | a=setup:active                   | [RFC4145]                      |
   | a=sendrecv                       | [RFC3264]                      |
   | a=ice-ufrag:991Ca2a5e            | [RFC5245]                      |
   | a=ice-pwd:921d5d47efbabd9a2de4e9 | [RFC5245]                      |
   | 9bd291c325                       |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 51]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=fingerprint:sha-1 6d:f7:c9:c7: | [RFC5245]                      |
   | 70:9d:1f:66:79:a8:07:99:41:49:   |                                |
   | 83:4a:97:0e:1f:ef                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 20000 typ host       |                                |
   +----------------------------------+--------------------------------+

                        Table 21: 5.2.10 SDP Answer

5.2.11.  Audio Only, Add Video to BUNDLE

   This example involves 2 Offer/Answer exchanges.  First one setting up
   Audio-only session followed by an updated Offer/Answer exchange to
   add video stream to the ongoing session.  Also the newly added video
   stream is BUNDLED with the audio stream.



            Audio Only , Add Video and BUNDLE

   Alice                                                 Bob
   |                                                      |
   |                                                      |
   |        Alice indicates support for BUNDLE            |
   |                                                      |
   |                Offer(Audio:Opus)                     |
   |----------------------------------------------------->|
   |                                                      |Bob
   |                                                      |supports
   |                                                      |BUNDLE
   |                Answer(Audio:Opus)                    |
   |<-----------------------------------------------------|
   |                                                      |Alice adds
   |                                                      |video stream
   |        Updated Offer(Audio:Opus, Video:VP8)          |to BUNDLE
   |----------------------------------------------------->|
   |                                                      |
   |                                                      |Bob accepts
   |        Updated Answer(Audio:Opus, Video:VP8)         |
   |<-----------------------------------------------------|
   |                                                      |
   |   2Way Call with Audio and Video Multiplexed         |
   |......................................................|
   |                                                      |
   |                                                      |


   +----------------------------------+--------------------------------+



Nandakumar & Jennings   Expires February 5, 2016               [Page 52]

Internet-Draft                 SDP4WebRTC                    August 2015


   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice wants to      |
   |                                  | BUNDLE only audio and video    |
   |                                  | media.                         |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 53]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+

                        Table 22: 5.2.11 SDP Offer
















































Nandakumar & Jennings   Expires February 5, 2016               [Page 54]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:L/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                        Table 23: 5.2.10 SDP Answer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |



Nandakumar & Jennings   Expires February 5, 2016               [Page 55]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | v=1                              | Version number incremented     |
   |                                  | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice wants to      |
   |                                  | BUNDLE only audio and video    |
   |                                  | media.                         |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d |                                |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 56]

Internet-Draft                 SDP4WebRTC                    August 2015


   | m=video 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:22222                     | [RFC5576]                      |
   | cname:Q/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                    Table 24: 5.2.11 SDP Updated Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=1                              | [RFC4566] Version number       |
   |                                  | incremented                    |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 57]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio video       | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:L/aoNWs11HmN4Xa5           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 120                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:video                      | [RFC5888]                      |
   | a=rtpmap:120 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 58]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=rtcp-fb:120 nack               | [RFC5104]                      |
   | a=rtcp-fb:120 nack pli           | [RFC5104]                      |
   | a=rtcp-fb:120 ccm fir            | [RFC5104]                      |
   | a=ssrc:44444                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                    Table 25: 5.2.11 SDP Updated Answer

5.3.  MultiResolution, RTX, FEC Examples

   This section deals with scenarios related to multi-source, multi-
   stream negotiation such as layered coding, simulcast, along with
   techniques that deal with providing robustness against transmission
   errors such as FEC and RTX.  Also to note, mechanisms such as FEC and
   RTX could be envisioned in the above basic scenarios as well.

5.3.1.  Sendonly Simulcast Session with 2 cameras and 2 encodings per
        camera

   The SDP below shows Offer/Answer exchange with one audio and two
   video sources.  Each of the video source can be sent at two different
   resolutions.

   One video source corresponds to VP8 encoding, while the other
   corresponds to H.264 encoding.

   bundle-only framework is used along with BUNDLE grouping framework to
   enable multiplexing of all the 5 streams (1 audio stream + 4 video
   streams) over a single RTP Session.











Nandakumar & Jennings   Expires February 5, 2016               [Page 59]

Internet-Draft                 SDP4WebRTC                    August 2015


          1 Way Successful Simulcast w/BUNDLE

   Alice                                            Bob
   |                                                 |
   |                                                 |
   |     Alice offers 2 sendonly video sources       |
   |     with 2 simulcast encodings per source       |
   |           and bundle-only for video             |
   |                                                 |
   |                                                 |
   |   Offer(Audio:Opus,Video1:VP8,Video2:H.264)     |
   |------------------------------------------------>|
   |                                                 |
   |                                                 |
   |    Answer(Audio:Opus Video1:VP8,Video2:H.264)   |
   |<------------------------------------------------|
   |                                                 |
   |One-Way 1 Opus, 2 H.264 and 2 VP8 video streams, |
   | all multiplexed                                 |
   |.................................................|
   |                                                 |
   |                                                 |



   +-------------------------------------+-----------------------------+
   | SDP Contents                        | RFC#/Notes                  |
   +-------------------------------------+-----------------------------+
   | v=0                                 | [RFC4566]                   |
   | o=- 20519 0 IN IP4 0.0.0.0          | [RFC4566]                   |
   | s=-                                 | [RFC4566]                   |
   | t=0 0                               | [RFC4566]                   |
   | a=msid-semantic:WMS ma              | [I-D.ietf-mmusic-msid]      |
   | a=group:BUNDLE m0 m1 m2             | [I-D.ietf-mmusic-sdp-bundle |
   |                                     | -negotiation] Alice         |
   |                                     | supports grouping of        |
   |                                     | m=lines under BUNDLE        |
   |                                     | semantics                   |
   | a=ice-options:trickle               | [I-D.ietf-mmusic-trickle-ic |
   |                                     | e]                          |
   | m=audio 54609 UDP/TLS/RTP/SAVPF 109 | [RFC4566]                   |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=msid:ma ta                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (ta) |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m0                            | [RFC5888]                   |
   | a=rtpmap:109 opus/48000/2           | [I-D.ietf-payload-rtp-opus] |



Nandakumar & Jennings   Expires February 5, 2016               [Page 60]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=extmap:1 urn:ietf:params:rtp-     | [RFC6464]                   |
   | hdrext:ssrc-audio-level             |                             |
   | a=ptime:20                          | [I-D.ietf-payload-rtp-opus] |
   | a=sendonly                          | [RFC3264]                   |
   | a=setup:actpass                     | [RFC4145]                   |
   | a=rtcp-mux                          | [RFC5761]                   |
   | a=rtcp-fb:109 nack                  | [RFC5104]                   |
   | a=ssrc:11111 C90alEocUG1f0fcg       | [RFC5576]                   |
   | a=ice-ufrag:074c6550                | [RFC5245]                   |
   | a=ice-pwd:a28a397a4c3f31747d1ee3474 | [RFC5245]                   |
   | af08a068                            |                             |
   | a=fingerprint:sha-1 99:41:49:83:4a: | [RFC5245]                   |
   | 97:0e:1f:ef:6d:f7:c9:c7:70:         |                             |
   | 9d:1f:66:79:a8:07                   |                             |
   | a=candidate:0 1 UDP  2113667327     | [RFC5245]                   |
   | 192.168.1.4 54609 typ host          |                             |
   | a=candidate:1 1 UDP  694302207      | [RFC5245]                   |
   | 24.23.204.141 54609 typ srflx raddr |                             |
   | 192.168.1.4 rport 54609             |                             |
   | a=candidate:0 2 UDP 2113667326      | [RFC5245]                   |
   | 192.168.1.4 64678 typ host          |                             |
   | a=candidate:1 2 UDP  1694302206     | [RFC5245]                   |
   | 24.23.204.141 64678 typ srflx raddr |                             |
   | 192.168.1.4 rport 64678             |                             |
   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 0 UDP/TLS/RTP/SAVPF 98 100  | bundle-only video line with |
   |                                     | port number set to zero     |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=msid:ma tb                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (tb) |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m1                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=rtpmap:98 VP8/90000               | [I-D.ietf-payload-vp8]      |
   | a=rtpmap:100 VP8/90000              | [I-D.ietf-payload-vp8]      |
   | a=imageattr:98 [x=1280,y=720]       | [RFC6236]Camera-1,Encoding- |
   |                                     | 1 Resolution                |
   | a=fmtp:98 max-fr=30                 | [RFC4566]                   |
   | a=imageattr:100 [x=640,y=480]       | [RFC6236]                   |
   |                                     | Camera-1,Encoding-2         |
   |                                     | Resolution                  |
   | a=fmtp:100 max-fr=15                | [RFC4566]                   |
   | a=simulcast: send 98;100            | [I-D.ietf-mmusic-sdp-simulc |
   |                                     | ast] Alice can send 2       |
   |                                     | resolutions                 |
   | a=ssrc:12345                        | [RFC5576] [RFC7022]         |
   | cname:axzo1278npDlAzM73             | Camera-1,Encoding-1 SSRC    |



Nandakumar & Jennings   Expires February 5, 2016               [Page 61]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                     | with Session CNAME          |
   | a=ssrc:45678                        | [RFC5576] [RFC7022]         |
   | cname:axzo1278npDlAzM73             | Camera-1,Encoding-2 SSRC    |
   |                                     | with Session CNAME          |
   | a=sendonly                          | [RFC3264] - Send only video |
   |                                     | stream                      |
   | a=rtcp-mux                          | [RFC5761]                   |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=rtcp-fb:98 nack                   | [RFC5104]                   |
   | a=rtcp-fb:98 nack pli               | [RFC5104]                   |
   | a=rtcp-fb:98 ccm fir                | [RFC5104]                   |
   | a=rtcp-fb:100 nack                  | [RFC5104]                   |
   | a=rtcp-fb:100 nack pli              | [RFC5104]                   |
   | a=rtcp-fb:100 ccm fir               | [RFC5104]                   |
   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 0 UDP/TLS/RTP/SAVPF 101 102 | bundle-only video line with |
   |                                     | port number set to zero     |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=msid:ma tc                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (tc) |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m2                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=rtpmap:101 H264/90000             | [RFC3984]                   |
   | a=rtpmap:102 H264/90000             | [RFC3984]                   |
   | a=fmtp:101 profile-level-id=4d0028  | [RFC3984]Camera-2,Encoding- |
   | ;packetization-mode=1;max-fr=30     | 1 Resolution                |
   | a=fmtp:102 profile-level-id=4d0028  | [RFC3984]Camera-2,Encoding- |
   | ;packetization-mode=1;max-fr=15     | 2 Resolution                |
   | a=simulcast: send 101;102           | [I-D.ietf-mmusic-sdp-simulc |
   |                                     | ast]                        |
   | a=ssrc:67890                        | [RFC5576] [RFC7022]         |
   | cname:axzo1278npDlAzM73             | Camera-2,Encoding-1 SSRC    |
   |                                     | with Session CNAME          |
   | a=ssrc:56789                        | [RFC5576] [RFC7022]         |
   | cname:axzo1278npDlAzM73             | Camera-2,Encoding-2 SSRC    |
   |                                     | with Session CNAME          |
   | a=sendonly                          | [RFC3264] - Send only video |
   |                                     | stream                      |
   | a=rtcp-mux                          | [RFC5761]                   |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=rtcp-fb:101 nack                  | [RFC5104]                   |
   | a=rtcp-fb:101 nack pli              | [RFC5104]                   |
   | a=rtcp-fb:101  ccm fir              | [RFC5104]                   |
   | a=rtcp-fb:102 nack                  | [RFC5104]                   |
   | a=rtcp-fb:102 nack pli              | [RFC5104]                   |
   | a=rtcp-fb:102 ccm fir               | [RFC5104]                   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 62]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp-rsize                        | [RFC5506]                   |
   +-------------------------------------+-----------------------------+

                         Table 26: 5.3.1 SDP Offer

   +-------------------------------------+-----------------------------+
   | SDP Contents                        | RFC#/Notes                  |
   +-------------------------------------+-----------------------------+
   | v=0                                 | [RFC4566]                   |
   | o=- 20519 0 IN IP4 0.0.0.0          | [RFC4566]                   |
   | s=-                                 | [RFC4566]                   |
   | t=0 0                               | [RFC4566]                   |
   | a=msid-semantic:WMS ma              | [I-D.ietf-mmusic-msid]      |
   | a=group:BUNDLE m0 m1 m2             | [I-D.ietf-mmusic-sdp-bundle |
   |                                     | -negotiation] Alice         |
   |                                     | supports grouping of        |
   |                                     | m=lines under BUNDLE        |
   |                                     | semantics                   |
   | a=ice-options:trickle               | [I-D.ietf-mmusic-trickle-ic |
   |                                     | e]                          |
   | m=audio 49203 UDP/TLS/RTP/SAVPF 109 | [RFC4566]                   |
   | c=IN IP4 98.248.92.77               | [RFC4566]                   |
   | a=rtcp:60065 IN IP4 98.248.92.77    | [RFC3605]                   |
   | a=mid:m0                            | [RFC5888]                   |
   | a=msid:ma ta                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (ta) |
   | a=rtpmap:109 opus/48000/2           | [I-D.ietf-payload-rtp-opus] |
   | a=extmap:1 urn:ietf:params:rtp-     | [RFC6464]                   |
   | hdrext:ssrc-audio-level             |                             |
   | a=rtcp-fb:109 nack                  | [RFC5104]                   |
   | a=ptime:20                          | [I-D.ietf-payload-rtp-opus] |
   | a=recvonly                          | [RFC3264]                   |
   | a=setup:active                      | [RFC4145]                   |
   | a=rtcp-mux                          | [RFC5761]                   |
   | a=ssrc:22222                        | [RFC5576]                   |
   | cname:y8/C90alEocUG1f0fcg           |                             |
   | a=ice-ufrag:c300d85b                | [RFC5245]                   |
   | a=ice-pwd:de4e99bd291c325921d5d47ef | [RFC5245]                   |
   | babd9a2                             |                             |
   | a=fingerprint:sha-1 99:41:49:83:4a: | [RFC5245]                   |
   | 97:0e:1f:ef:6d:f7:c9:c7:70:         |                             |
   | 9d:1f:66:79:a8:07                   |                             |
   | a=candidate:0 2 UDP  2113667327     | [RFC5245]                   |
   | 192.168.1.7 49203 typ host          |                             |
   | a=candidate:1 2 UDP  694302207      | [RFC5245]                   |
   | 98.248.92.77 49203 typ srflx raddr  |                             |
   | 192.168.1.4 rport 49203             |                             |



Nandakumar & Jennings   Expires February 5, 2016               [Page 63]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 49203 UDP/TLS/RTP/SAVPF 98  | BUNDLE accepted with port   |
   | 100                                 | repeated from the audio     |
   |                                     | port                        |
   | c=IN IP4 98.248.92.77               | [RFC4566]                   |
   | a=msid:ma tb                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (tb) |
   | a=rtcp:60065 IN IP4 98.248.92.77    | [RFC3605]                   |
   | a=mid:m1                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=rtpmap:98 VP8/90000               | [I-D.ietf-payload-vp8]      |
   | a=rtpmap:100 VP8/90000              | [I-D.ietf-payload-vp8]      |
   | a=imageattr:98 [x=1280,y=720]       | [RFC6236]Camera-1,Encoding- |
   |                                     | 1 Resolution                |
   | a=fmtp:98 max-fr=30                 | [RFC4566]                   |
   | a=imageattr:100 [x=640,y=480]       | [RFC6236]                   |
   |                                     | Camera-1,Encoding-2         |
   |                                     | Resolution                  |
   | a=fmtp:100 max-fr=15                | [RFC4566]                   |
   | a=recvonly                          | [RFC3264] - receive only    |
   |                                     | video stream                |
   | a=simulcast: recv 98;100            | [I-D.ietf-mmusic-sdp-simulc |
   |                                     | ast]                        |
   | a=ssrc:54321                        | [RFC5576]                   |
   | cname:y8/C90alEocUG1f0fcg           |                             |
   | a=ice-ufrag:c300d85b                | [RFC5245]                   |
   | a=ice-pwd:de4e99bd291c325921d5d47ef | [RFC5245]                   |
   | babd9a2                             |                             |
   | a=fingerprint:sha-1 99:41:49:83:4a: | [RFC5245]                   |
   | 97:0e:1f:ef:6d:f7:c9:c7:70:9d:      |                             |
   | 1f:66:79:a8:07                      |                             |
   | a=candidate:0 2 UDP 2113667326      | [RFC5245]                   |
   | 192.168.1.7 60065 typ host          |                             |
   | a=candidate:1 2 UDP  1694302206     | [RFC5245]                   |
   | 98.248.92.77 60065 typ srflx raddr  |                             |
   | 192.168.1.4 rport 60065             |                             |
   | a=setup:active                      | [RFC4145]                   |
   | a=rtcp-mux                          | [RFC5576]                   |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 54609 UDP/TLS/RTP/SAVPF 101 | BUNDLE accepted with port   |
   | 102                                 | repeated from the audio     |
   |                                     | port                        |
   | c=IN IP4 98.248.92.77               | [RFC4566]                   |
   | a=rtcp:56503 IN IP4 98.248.92.77    | [RFC3605]                   |
   | a=msid:ma tc                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |



Nandakumar & Jennings   Expires February 5, 2016               [Page 64]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                     | RTCMediaStreamTrack ID (tc) |
   | a=mid:m2                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=rtpmap:101 H264/90000             | [RFC3984]                   |
   | a=rtpmap:102 H264/90000             | [RFC3984]                   |
   | a=recvonly                          | [RFC3264]                   |
   | a=fmtp:101 profile-level-id=4d0028  | [RFC3984]                   |
   | ;packetization-mode=1;max-fr=30     |                             |
   | a=fmtp:102 profile-level-id=4d0028  | [RFC3984]                   |
   | ;packetization-mode=1;max-fr=15     |                             |
   | a=simulcast: recv 101;102           | [I-D.ietf-mmusic-sdp-simulc |
   |                                     | ast] Bob accepts to         |
   |                                     | receieve the offered        |
   |                                     | simulcast streams           |
   | a=ssrc:90876                        | [RFC5576]                   |
   | cname:axzo1278npDlAzM73             |                             |
   | a=ice-ufrag:ufrag:c300d85b          | [RFC5245]                   |
   | a=ice-pwd:de4e99bd291c325921d5d47ef | [RFC5245]                   |
   | babd9a2                             |                             |
   | a=fingerprint:sha-1 99:41:49:83:4a: | [RFC5245]                   |
   | 97:0e:1f:ef:6d:f7:c9:c7:70:         |                             |
   | 9d:1f:66:79:a8:07                   |                             |
   | a=candidate:0 2 UDP 2113667326      | [RFC5245]                   |
   | 192.168.1.7 60065 typ host          |                             |
   | a=candidate:1 2 UDP  1694302206     | [RFC5245]                   |
   | 98.248.92.77 60065 typ srflx raddr  |                             |
   | 192.168.1.7 rport 60065             |                             |
   | a=setup:active                      | [RFC4145]                   |
   | a=rtcp-mux                          | [RFC5576]                   |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=rtcp-rsize                        | [RFC5506]                   |
   +-------------------------------------+-----------------------------+

                         Table 27: 5.3.1 SDP Anwer

5.3.2.  Successful SVC Video Session

   This section shows an SDP Offer/Answer for a session with an audio
   and a single video source.  The video source is encoded as layered
   coding at 3 different resolutions based on [RFC5583].  The video
   m=line shows 3 streams with last stream (payload 100) dependent on
   streams with payload 96 and 97 for decoding.









Nandakumar & Jennings   Expires February 5, 2016               [Page 65]

Internet-Draft                 SDP4WebRTC                    August 2015


          SVC Session - 3 Layers w/BUNDLE

  Alice                                            Bob
  |                                                 |
  |                                                 |
  |      Alice offers 3 sendonly video streams      |
  |      as 3 layers of SVC and bundle-only         |
  |               for video streams.                |
  |                                                 |
  |            Offer(Video:H.264 SVC)               |
  |------------------------------------------------>|
  |                                                 |
  |                                                 |Bob accepts Alice's
  |                                                 |offered Codec
  |                                                 |operation points
  |                                                 |
  |             Answer(Video:H.264)                 |
  |<------------------------------------------------|
  |                                                 |
  |One-Way  H.264 SVC video streams                 |
  |.................................................|
  |                                                 |
  |                                                 |



   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m0                         | [RFC5888] Audio m=line part of |



Nandakumar & Jennings   Expires February 5, 2016               [Page 66]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | BUNDLE group with a unique     |
   |                                  | port number                    |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=sendonly                       | [RFC3264]                      |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=ssrc:67890                     | [RFC5576]                      |
   | cname:axzo1278npDlAzM73          |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 0 UDP/TLS/RTP/SAVPF 96   | bundle-only video line with    |
   | 97 100                           | port number set to zero        |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tc)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m1                         | [RFC5888] Audio m=line part of |
   |                                  | BUNDLE group                   |
   | a=msid:ma tb                     |                                |
   | a=rtpmap:96 H264/90000           | [RFC3984]                      |
   | a=fmtp:96 profile-level-         | [RFC3984]H.264 Layer 1         |
   | id=4d0028; packetization-mode=1  |                                |
   | ;max-fr=30;max-fs=8040           |                                |
   | a=rtpmap:97 H264/90000           | [RFC3984]                      |
   | a=fmtp:97 profile-level-         | [RFC3984] H.264 Layer 2        |
   | id=4d0028;packetization-mode=1;  |                                |
   | max-fr=15;max-fs=1200            |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 67]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtpmap:100 H264-SVC/90000      | [RFC3984]                      |
   | a=fmtp:100 profile-level-        | [RFC3984]                      |
   | id=4d0028;packetization-mode=1;  |                                |
   | max-fr=30;max-fs=8040            |                                |
   | a=depend:100 lay m1:96,97;       | [RFC5583]Layer 3 dependent on  |
   |                                  | layers 1 and 2                 |
   | a=sendonly                       | [RFC3264] - Send only video    |
   |                                  | stream                         |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=ssrc:1732846380                | [RFC5576]                      |
   | cname:axzo1278npDlAzM73          |                                |
   | a=ssrc:1732846381                | [RFC5576]                      |
   | cname:axzo1278npDlAzM73          |                                |
   | a=ssrc:1732846382                | [RFC5576]                      |
   | cname:axzo1278npDlAzM73          |                                |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                    Table 28: 5.3.2 SDP Offer with SVC

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m0                         | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 68]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 60065 typ srflx     |                                |
   | raddr 192.168.1.5 rport 60065    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 54609 UDP/TLS/RTP/SAVPF  | BUNDLE accepted Bundle address |
   | 96 100                           | same as audio m=line.          |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:56503 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m1                         | [RFC5888] Video m=line part of |
   |                                  | BUNDLE group                   |
   | a=rtpmap:96 H264/90000           | [RFC3984]                      |
   | a=fmtp:96 profile-level-         | [RFC3984]H.264 Layer 1         |
   | id=4d0028;packetization-mode=1;  |                                |
   | max-fr=30;max-fs=8040            |                                |
   | a=rtpmap:100 H264-SVC/90000      | [RFC3984]                      |
   | a=fmtp:100 profile-level-        | [RFC3984]                      |
   | id=4d0028;packetization-mode=1;  |                                |
   | max-fr=30;max-fs=8040            |                                |
   | a=depend:100 lay m1:96;          | [RFC5583] Bob chooses 2 Codec  |
   |                                  | Operation points               |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.5 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.142 64678 typ srflx    |                                |
   | raddr 192.168.1.5 rport 64678    |                                |
   | a=recvonly                       | [RFC3264] - Receive only video |
   |                                  | stream                         |
   | a=setup:active                   | [RFC4145]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 69]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=ssrc:4638117328                | [RFC5576]                      |
   | cname:axzo1278npDlAzM73          |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                    Table 29: 5.3.2 SDP Answer with SVC

5.3.3.  Successful Simulcast Video Session with Retransmission

   This section shows an SDP Offer/Answer exchange for a simulcast
   scenario with 2 two resolutions and has [RFC4588] style re-
   transmission flows.



         Simulcast Streams with Retransmission

   Alice                                                    Bob
   |                                                         |
   |                                                         |
   |Alice offers single audio and simulcasted video streams  |
   |                                                         |
   |                                                         |
   |    Offer(Audio:Opus Video:VP8 with 2 resolutions)       |
   |    & RTX stream                                         |
   |-------------------------------------------------------->|
   |                                                         |
   |                                                         |
   |          Answer (Bob accepts Alice's offer)             |
   |<--------------------------------------------------------|
   |                                                         |
   |                                                         |
   |One-Way 1 Opus, 2 VP8 and RTX video streams,all muxed    |
   |.........................................................|
   |                                                         |
   |                                                         |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 70]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m0                         | [RFC5888] Audio m=line part of |
   |                                  | BUNDLE group with a unique     |
   |                                  | port number                    |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=sendonly                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 0 UDP/TLS/RTP/SAVPF 98   | bundle-only video line with    |
   | 100 101 103                      | port number set to zero        |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 71]

Internet-Draft                 SDP4WebRTC                    August 2015


   |                                  | ID (tb)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m1                         | [RFC5888]                      |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:100 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:103 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:100 max-fr=15;max-fs=1200 | [RFC4566]                      |
   | a=fmtp:101 apt=98;rtx-time=3000  | [RFC4588]                      |
   | a=fmtp:103 apt=100;rtx-time=3000 | [RFC4588]                      |
   | a=simulast: send 98;100          | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ]                              |
   | a=ssrc-group:FID 12345 34567     | [RFC5888]                      |
   | a=ssrc-group:FID 78990 90887     | [RFC5888]                      |
   | a=ssrc:12345                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:78990                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:34567                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:90887                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=sendonly                       | [RFC3264]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                Table 30: 5.3.3 SDP Offer w/Simulcast, RTX

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |



Nandakumar & Jennings   Expires February 5, 2016               [Page 72]

Internet-Draft                 SDP4WebRTC                    August 2015


   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m0                         | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:L/HmN4Xa5NWs1ao1           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 64678 typ srflx     |                                |
   | raddr 192.168.1.7 rport 60065    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | BUNDLE accepted with Bundle    |
   | 98 100 101 103                   | address identical to audio     |
   |                                  | m-line                         |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m1                         | [RFC5888] Video m=line part of |
   |                                  | BUNDLE group                   |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:100 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:103 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:100 max-fr=15;max-fs=1200 | [RFC4566]                      |
   | a=fmtp:101 apt=98;rtx-time=3000  | [RFC4588]                      |
   | a=fmtp:103 apt=100;rtx-time=3000 | [RFC4588]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 73]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.772 60065 typ srflx    |                                |
   | raddr 192.168.1.7 rport 60065    |                                |
   | a=simulcast: recv 98;100         | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ]                              |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                Table 31: 5.3.3 SDP Answer w/Simulcast, RTX

5.3.4.  Successful 1-way Simulcast Sessio with 2 resolutions and RTX -
        One resolution rejected

   This section shows an SDP Offer/Answer exchange for a simulcast
   scenario with 2 two resolutions.

   It also showcases when Bob rejects one of the Simulcast Video Stream
   which results in the rejection of the associated repair stream
   implicitly.

















Nandakumar & Jennings   Expires February 5, 2016               [Page 74]

Internet-Draft                 SDP4WebRTC                    August 2015


     Simulcast Streams with Retransmission Rejected

Alice                                                    Bob
|                                                         |
|                                                         |
|Alice offers single audio and simulcasted video streams  |
| with bundle-only for video                              |
|                                                         |
|                                                         |
|Offer(Audio:Opus Video:VP8 with 2 resolutions,RTX Stream)|
|-------------------------------------------------------->|
|                                                         |
|                                                         |Bob accepts 1
|                                                         |simulcast,rtx
|                                                         |rejects the
|                                                         |other
|   Answer(Audio:Opus Video:VP8 with 1 res & RTX Stream)  |
|<--------------------------------------------------------|
|                                                         |
|                                                         |
|1-way audio,video session and its associated RTX stream, |
| all multiplexed                                         |
|.........................................................|
|                                                         |
|                                                         |



   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 75]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 24.23.204.141                    |                                |
   | a=mid:m0                         | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=sendonly                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:LP/NWs1ao1HmN4Xa5          |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 0 UDP/TLS/RTP/SAVPF 98   | bundle-only video line with    |
   | 100 101 103                      | port number set to zero        |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m1                         | [RFC5888]                      |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:100 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:103 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:100 max-fr=15;max-fs=1200 | [RFC4566]                      |
   | a=fmtp:101 apt=98;rtx-time=3000  | [RFC4588]                      |
   | a=fmtp:103 apt=100;rtx-time=3000 | [RFC4588]                      |
   | a=simulcast: send 98;100         | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ]                              |



Nandakumar & Jennings   Expires February 5, 2016               [Page 76]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ssrc-group:FID 12345 34567     | [RFC5888]                      |
   | a=ssrc-group:FID 78990 90887     | [RFC5888]                      |
   | a=ssrc:12345                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:78990                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:34567                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:90887                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=sendonly                       | [RFC3264]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                Table 32: 5.3.4 SDP Offer w/Simulcast, RTX

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:49203 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m0                         | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 77]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 60065 typ srflx     |                                |
   | raddr 192.168.1.7 rport 60065    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | BUNDLE accepted with Bundle    |
   | 98 101                           | address identical to audio     |
   |                                  | m-line                         |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m1                         | [RFC5888]                      |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:101 apt=98;rtx-time=3000  | [RFC4588]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 60065 typ srflx     |                                |
   | raddr 192.168.1.5 rport 60065    |                                |
   | a=simulcast: recv 98             | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ] Bob accepts only one         |
   |                                  | simulcast resolution           |
   | a=ssrc:54321                     | [RFC5576]                      |
   | cname:NWs1ao1HmN4Xa5             |                                |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+



Nandakumar & Jennings   Expires February 5, 2016               [Page 78]

Internet-Draft                 SDP4WebRTC                    August 2015


                  Table 33: 5.3.4 SDP Answer no Simulcast

5.3.5.  Simulcast Video Session with Forward Error Correction

   This section shows an SDP Offer/Answer exchange for Simulcast video
   stream at two resolutions and and has [RFC5956] style FEC flows.

   On completion of the Offer/Answer exchange mechanism we end up one
   audio stream, 2 simulcast video streams and 2 associated FEC streams
   are sent over a single 5-tuple.


      Simulcast Streams with Forward Error Correction

Alice                                                            Bob
|                                                               |
|                                                               |
|                                                               |
|Alice offers single audio and simulcasted video streams        |
|with bundle-only                                               |
|                                                               |
|                                                               |
|Offer(Audio:Opus Video:VP8 with 2 resolutions with FEC Streams)|
|-------------------------------------------------------------->|
|                                                               |
|                                                               |Bob
|                                                               |accepts
|                                                               |Alice's
|                                                               |offer
|Answer(Audio:Opus Video:VP8 with 2 resolutions w/FEC Streams)  |
|<--------------------------------------------------------------|
|                                                               |
|One-Way Audio,Video session with 4 video streams(Simulcast     |
| and FEC) all multiplexed                                      |
|...............................................................|
|                                                               |
|                                                               |
|                                                               |


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 79]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation] Alice supports      |
   |                                  | grouping of m=lines under      |
   |                                  | BUNDLE semantics               |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=mid:m0                         | [RFC5888]                      |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=sendonly                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 0 UDP/TLS/RTP/SAVPF 98   | bundle-only video line with    |
   | 100 101 103                      | port number set to zero        |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 80]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 24.23.204.141                    |                                |
   | a=mid:m1                         | [RFC5888] Video m=line part of |
   |                                  | BUNDLE group                   |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:100 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 1d-interleaved-     | [RFC5956]                      |
   | parityfec/90000                  |                                |
   | a=rtpmap:103 1d-interleaved-     | [RFC5956]                      |
   | parityfec/90000                  |                                |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:100 max-fr=15;max-fs=1200 | [RFC4566]                      |
   | a=fmtp:101 L=5; D=10; repair-    | [RFC5956]                      |
   | window=200000                    |                                |
   | a=fmtp:103 L=5; D=10; repair-    | [RFC5956]                      |
   | window=200000                    |                                |
   | a=simulcast: send 98;100         | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ]                              |
   | a=depend:98 fec m1:101           | TBD                            |
   | a=depend:100 fec m1:103          | TBD                            |
   | a=ssrc-group:FEC-FR 12345 34567  | [RFC5888]                      |
   | a=ssrc-group:FEC-FR 78990 90887  | [RFC5888]                      |
   | a=ssrc:12345                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:78990                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:34567                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=ssrc:90887                     | [RFC5576]                      |
   | cname:Q/NWs1ao1HmN4Xa5           |                                |
   | a=sendonly                       | [RFC3264]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                         Table 34: 5.3.5 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20519 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS m0           | [I-D.ietf-mmusic-msid]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 81]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=group:BUNDLE m0 m1             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109                              |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m0                         | [RFC5888] Audio m=line part of |
   |                                  | BUNDLE group with a unique     |
   |                                  | port number                    |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtcp-fb:109 nack               | [RFC5104]                      |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ssrc:33333                     | [RFC5576]                      |
   | cname:Y9/cZke09JAtpl98           |                                |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:9d |                                |
   | : 1f:66:79:a8:07                 |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 60065 typ srflx     |                                |
   | raddr 192.168.1.7 rport 60065    |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   | m=video 49203 UDP/TLS/RTP/SAVPF  | BUNDLE accepted with Bundle    |
   | 98 100 101 103                   | Address identical to audio     |
   |                                  | m=line.                        |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=msid:ma tb                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (tb)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=mid:m1                         | [RFC5888] Video m=line part of |
   |                                  | BUNDLE group                   |
   | a=rtpmap:98 VP8/90000            | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:100 VP8/90000           | [I-D.ietf-payload-vp8]         |
   | a=rtpmap:101 1d-interleaved-     | [RFC5956]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 82]

Internet-Draft                 SDP4WebRTC                    August 2015


   | parityfec/90000                  |                                |
   | a=rtpmap:103 1d-interleaved-     | [RFC5956]                      |
   | parityfec/90000                  |                                |
   | a=fmtp:98 max-fr=30;max-fs=8040  | [RFC4566]                      |
   | a=fmtp:100 max-fr=15;max-fs=1200 | [RFC4566]                      |
   | a=fmtp:101 L=5; D=10; repair-    | [RFC5956]                      |
   | window=200000                    |                                |
   | a=fmtp:103 L=5; D=10; repair-    | [RFC5956]                      |
   | window=200000                    |                                |
   | a=simulcast: recv 98;100         | [I-D.ietf-mmusic-sdp-simulcast |
   |                                  | ]                              |
   | a=depend:98 fec m1:101           | TBD                            |
   | a=depend:100 fec m1:103          | TBD                            |
   | a=recvonly                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=bundle-only                    | [UNIFIED-PLAN]                 |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.7 60065 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 98.248.92.77 60065 typ srflx     |                                |
   | raddr 192.168.1.7 rport 60065    |                                |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=rtcp-fb:* nack pli             | [RFC5104]                      |
   | a=rtcp-fb:* ccm fir              | [RFC5104]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                        Table 35: 5.3.5 SDP Answer

5.4.  Others

   The examples in the section provide SDP for a variety of scenarios
   related to RTP Header extension, Legacy Interop scenarios and more.

5.4.1.  Audio Session - Voice Activity Detection

   This example shows Alice indicating the support of the RTP header
   extension to include the audio-level of the audio sample carried in
   the RTP packet.





Nandakumar & Jennings   Expires February 5, 2016               [Page 83]

Internet-Draft                 SDP4WebRTC                    August 2015


              2-Way Audio with VAD

    Alice                                    Bob
    |                                         |
    |                                         |
    |Alice indicates support for including    |
    |audio level in RTP header                |
    |                                         |
    |     Offer(Audio:Opus,PCMU,PCMA)         |
    |---------------------------------------->|
    |                                         |
    |                                         |
    |     Answer(Audio:Opus,PCMU,PCMA)        |
    |<----------------------------------------|
    |                                         |
    |                                         |Bob accepts and
    |                                         |indicates his
    |                                         |support as well
    |                                         |
    |     Two way Opus Audio                  |
    |.........................................|
    |                                         |
    |Per packet audio-level is included in the|
    |RTP header                               |
    |                                         |



   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566]                      |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |



Nandakumar & Jennings   Expires February 5, 2016               [Page 84]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtpmap:0 PCMU/8000             | [RFC3551]                      |
   | a=rtpmap:0 PCMA/8000             | [RFC3551]                      |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:QCL/1HmN4Xa5CClapa         |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                         Table 36: 5.4.1 SDP Offer

   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 98                         |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 85]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus] -  |
   |                                  | Bob accepts only Opus Codec    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:0 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=sendrecv                       | [RFC3264] - Bob can send and   |
   |                                  | recv audio                     |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761] - Bob can perform    |
   |                                  | RTP/RTCP Muxing on port 49203  |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:1732846380                | [RFC5576]                      |
   | cname:EocUG1f0fcg/yvY7           |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                        Table 37: 5.4.1 SDP Answer

5.4.2.  Audio Conference - Voice Activity Detection

   This example shows SDP for RTP header extension that allows RTP-level
   mixers in audio conferences to deliver information about the audio
   level of individual participants.










Nandakumar & Jennings   Expires February 5, 2016               [Page 86]

Internet-Draft                 SDP4WebRTC                    August 2015


        Audio Conference with VAD Support

   Alice                                    Mixer
   |                                         |
   |Alice indicates her interest to audio    |
   |levels for the contributing sources      |
   |                                         |
   |Offer(Audio:Opus,PCMU,PCMA)              |
   |---------------------------------------->|
   |                                         |
   |                                         |
   |Answer(Audio:Opus,PCMU,PCMA)             |
   |<----------------------------------------|
   |                                         |
   |                                         |Mixer indicates
   |                                         |it can provide
   |                                         |audio-levels
   |Two way Opus Audio                       |
   |.........................................|
   |                                         |
   |Audio-levels per CSRCS is included in the|
   |RTP header                               |
   |                                         |



   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=- 20518 0 IN IP4 0.0.0.0       | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 54609 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 8                          |                                |
   | c=IN IP4 24.23.204.141           | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:64678 IN IP4              | [RFC3605]                      |
   | 24.23.204.141                    |                                |
   | a=extmap:1/recvonly              | [RFC6465]                      |



Nandakumar & Jennings   Expires February 5, 2016               [Page 87]

Internet-Draft                 SDP4WebRTC                    August 2015


   | urn:ietf:params:rtp-hdrext:csrc- |                                |
   | audio-level                      |                                |
   | a=extmap:1 urn:ietf:params:rtp-  | [RFC6464]                      |
   | hdrext:ssrc-audio-level          |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:0 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=sendrecv                       | [RFC3264] - Alice can send and |
   |                                  | recv audio                     |
   | a=setup:actpass                  | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:074c6550             | [RFC5245]                      |
   | a=ice-pwd:a28a397a4c3f31747d1ee3 | [RFC5245]                      |
   | 474af08a068                      |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:70:   |                                |
   | 9d:1f:66:79:a8:07                |                                |
   | a=candidate:0 1 UDP  2113667327  | [RFC5245]                      |
   | 192.168.1.4 54609 typ host       |                                |
   | a=candidate:1 1 UDP  694302207   | [RFC5245]                      |
   | 24.23.204.141 54609 typ srflx    |                                |
   | raddr 192.168.1.4 rport 54609    |                                |
   | a=candidate:0 2 UDP 2113667326   | [RFC5245]                      |
   | 192.168.1.4 64678 typ host       |                                |
   | a=candidate:1 2 UDP  1694302206  | [RFC5245]                      |
   | 24.23.204.141 64678 typ srflx    |                                |
   | raddr 192.168.1.4 rport 64678    |                                |
   | a=ssrc:11111                     | [RFC5576]                      |
   | cname:QCL/1HmN4Xa5CClapa         |                                |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                         Table 38: 5.4.2 SDP Offer
















Nandakumar & Jennings   Expires February 5, 2016               [Page 88]

Internet-Draft                 SDP4WebRTC                    August 2015


   +----------------------------------+--------------------------------+
   | SDP Contents                     | RFC#/Notes                     |
   +----------------------------------+--------------------------------+
   | v=0                              | [RFC4566]                      |
   | o=-  16833 0 IN IP4 0.0.0.0      | [RFC4566] - Session Origin     |
   |                                  | Information                    |
   | s=-                              | [RFC4566]                      |
   | t=0 0                            | [RFC4566]                      |
   | a=msid-semantic:WMS ma           | [I-D.ietf-mmusic-msid]         |
   | a=group:BUNDLE audio             | [I-D.ietf-mmusic-sdp-bundle-ne |
   |                                  | gotiation]                     |
   | a=ice-options:trickle            | [I-D.ietf-mmusic-trickle-ice]  |
   | m=audio 49203 UDP/TLS/RTP/SAVPF  | [RFC4566]                      |
   | 109 0 98                         |                                |
   | c=IN IP4 98.248.92.77            | [RFC4566]                      |
   | a=mid:audio                      | [RFC5888]                      |
   | a=msid:ma ta                     | Identifies RTCMediaStream ID   |
   |                                  | (ma) and RTCMediaStreamTrack   |
   |                                  | ID (ta)                        |
   | a=rtcp:60065 IN IP4 98.248.92.77 | [RFC3605]                      |
   | a=extmap:1/sendonly              | [RFC6465]                      |
   | urn:ietf:params:rtp-hdrext:csrc- |                                |
   | audio-level                      |                                |
   | a=rtpmap:109 opus/48000/2        | [I-D.ietf-payload-rtp-opus]    |
   | a=ptime:20                       | [I-D.ietf-payload-rtp-opus]    |
   | a=rtpmap:0 PCMU/8000             | [RFC3551] PCMU Audio Codec     |
   | a=rtpmap:0 PCMA/8000             | [RFC3551] PCMA Audio Codec     |
   | a=rtcp-fb:* nack                 | [RFC5104]                      |
   | a=sendrecv                       | [RFC3264]                      |
   | a=setup:active                   | [RFC4145]                      |
   | a=rtcp-mux                       | [RFC5761]                      |
   | a=ice-ufrag:c300d85b             | [RFC5245]                      |
   | a=ice-pwd:de4e99bd291c325921d5d4 | [RFC5245]                      |
   | 7efbabd9a2                       |                                |
   | a=fingerprint:sha-1 99:41:49:83: | [RFC5245]                      |
   | 4a:97:0e:1f:ef:6d:f7:c9:c7:      |                                |
   | 70:9d:1f:66:79:a8:07             |                                |
   | a=candidate:0 1 UDP 2113667327   | [RFC5245]                      |
   | 192.168.1.7 49203 typ host       |                                |
   | a=candidate:1 1 UDP 1694302207   | [RFC5245]                      |
   | 98.248.92.77 49203 typ srflx     |                                |
   | raddr 192.168.1.7 rport 49203    |                                |
   | a=ssrc:2222 cname:HmN4Xa5CC/lapa | [RFC5576]                      |
   | a=rtcp-rsize                     | [RFC5506]                      |
   +----------------------------------+--------------------------------+

                        Table 39: 5.4.2 SDP Answer




Nandakumar & Jennings   Expires February 5, 2016               [Page 89]

Internet-Draft                 SDP4WebRTC                    August 2015


5.4.3.  Successful legacy Interop Fallaback with bundle-only

   In the scenario described below, Alice is a multi-stream capable
   WebRTC endpoint while Bob is a legacy VOIP end-point.  The SDP Offer/
   Answer exchange demonstrates successful session setup with fallback
   to audio only stream negotiated via bundle-only framework between the
   end-points.  Specifically,

   o  Offer from Alice describes 2 cameras via 2 video m=lines with both
      marked as bundle-only.

   o  Since Bob doesnot recognize either the BUNDLE mechanism or the
      bundle-only attribute, he accepts only the audio stream from
      Alice.


         Successful 2-Way WebRTC <-> VOIP Interop

Alice                                                       Bob
|                                                           |
|                                                           |
|       Alice is a multistream capable WebRTC end-point     |
|          & Bob is behind a legacy VOIP system             |
|                                                           |
|Offer(Audio:Opus Video:2 VP8,2 H2.64 Streams) with         |
|          bundle-only                                      |
|---------------------------------------------------------->|
|     Alice marks both the video streams as bundle-only     |
|                                                           |
|                                                           |
|                  Answer(Audio:Opus)                       |
|<----------------------------------------------------------|
|                                                           |Bob
|                                                           |accepts
|                                                           |audio
|                                                           |stream,
|                                                           |since he
|                                                           |doesn't
|                                                           |recognize
|                                                           |bundle-only
|                                                           |
|                   Two way Opus Audio                      |
|...........................................................|
|                                                           |
|                                                           |


   +-------------------------------------+-----------------------------+



Nandakumar & Jennings   Expires February 5, 2016               [Page 90]

Internet-Draft                 SDP4WebRTC                    August 2015


   | SDP Contents                        | RFC#/Notes                  |
   +-------------------------------------+-----------------------------+
   | v=0                                 | [RFC4566]                   |
   | o=- 20519 0 IN IP4 0.0.0.0          | [RFC4566]                   |
   | s=-                                 | [RFC4566]                   |
   | t=0 0                               | [RFC4566]                   |
   | a=msid-semantic:WMS ma              | [I-D.ietf-mmusic-msid]      |
   | a=group:BUNDLE m0 m1 m2             | [I-D.ietf-mmusic-sdp-bundle |
   |                                     | -negotiation] Alice         |
   |                                     | supports grouping of        |
   |                                     | m=lines under BUNDLE        |
   |                                     | semantics                   |
   | a=ice-options:trickle               | [I-D.ietf-mmusic-trickle-ic |
   |                                     | e]                          |
   | m=audio 54609 UDP/TLS/RTP/SAVPF 109 | [RFC4566]                   |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m0                            | [RFC5888] Audio m=line part |
   |                                     | of BUNDLE group with a      |
   |                                     | unique port number          |
   | a=msid:ma ta                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (ta) |
   | a=rtpmap:109 opus/48000/2           | [I-D.ietf-payload-rtp-opus] |
   | a=extmap:1 urn:ietf:params:rtp-     | [RFC6464]                   |
   | hdrext:ssrc-audio-level             |                             |
   | a=ptime:20                          | [I-D.ietf-payload-rtp-opus] |
   | a=rtcp-fb:109 nack                  | [RFC5104]                   |
   | a=sendrecv                          | [RFC3264]                   |
   | a=setup:actpass                     | [RFC4145]                   |
   | a=rtcp-mux                          | [RFC5761]                   |
   | a=ice-ufrag:074c6550                | [RFC5245]                   |
   | a=ice-pwd:a28a397a4c3f31747d1ee3474 | [RFC5245]                   |
   | af08a068                            |                             |
   | a=fingerprint:sha-1 99:41:49:83:4a: | [RFC5245]                   |
   | 97:0e:1f:ef:6d:f7:c9:c7:70:         |                             |
   | 9d:1f:66:79:a8:07                   |                             |
   | a=candidate:0 1 UDP  2113667327     | [RFC5245]                   |
   | 192.168.1.4 54609 typ host          |                             |
   | a=candidate:1 1 UDP  694302207      | [RFC5245]                   |
   | 24.23.204.141 54609 typ srflx raddr |                             |
   | 192.168.1.4 rport 54609             |                             |
   | a=candidate:0 2 UDP 2113667326      | [RFC5245]                   |
   | 192.168.1.4 64678 typ host          |                             |
   | a=candidate:1 2 UDP  1694302206     | [RFC5245]                   |
   | 24.23.204.141 64678 typ srflx raddr |                             |
   | 192.168.1.4 rport 64678             |                             |
   | a=ssrc:11111                        | [RFC5576]E                  |



Nandakumar & Jennings   Expires February 5, 2016               [Page 91]

Internet-Draft                 SDP4WebRTC                    August 2015


   | cname:axzo1278npDlAzM73             |                             |
   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 0 UDP/TLS/RTP/SAVPF 98 100  | bundle-only video line with |
   |                                     | port number set to zero     |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m1                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=msid:ma tb                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (tb) |
   | a=rtpmap:98 VP8/90000               | [I-D.ietf-payload-vp8]      |
   | a=imageattr:98 [x=1280,y=720]       | [RFC6236]                   |
   | a=fmtp:98 max-fr=30                 | [RFC4566]                   |
   | a=ssrc:12345                        | [RFC5576]                   |
   | cname:axzo1278npDlAzM73             |                             |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=sendrecv                          | [RFC3264]                   |
   | a=rtcp-rsize                        | [RFC5506]                   |
   | m=video 0 UDP/TLS/RTP/SAVPF 101 103 | bundle-only video line with |
   |                                     | port number set to zero     |
   | c=IN IP4 24.23.204.141              | [RFC4566]                   |
   | a=rtcp:64678 IN IP4 24.23.204.141   | [RFC3605]                   |
   | a=mid:m2                            | [RFC5888] Video m=line part |
   |                                     | of BUNDLE group             |
   | a=msid:ma tc                        | Identifies RTCMediaStream   |
   |                                     | ID (ma) and                 |
   |                                     | RTCMediaStreamTrack ID (tc) |
   | a=rtpmap:101 H264/90000             | [RFC3984]                   |
   | a=rtpmap:103 H264/90000             | [RFC3984]                   |
   | a=fmtp:101 profile-level-id=4d0028  | [RFC3984]Camera-2,Encoding- |
   | ;packetization-mode=1;max-fr=30     | 1 Resolution                |
   | a=ssrc:67890                        | [RFC5576]                   |
   | cname:axzo1278npDlAzM73             |                             |
   | a=bundle-only                       | [UNIFIED-PLAN]              |
   | a=sendrecv                          | [RFC3264]                   |
   | a=rtcp-rsize                        | [RFC5506]                   |
   +-------------------------------------+-----------------------------+

                 Table 40: 5.4.3 SDP Simulcast bundle-only

   +------------------------------------------+------------------------+
   | SDP Contents                             | RFC#/Notes             |
   +------------------------------------------+------------------------+
   | v=0                                      | [RFC4566]              |
   | o=- 20519 0 IN IP4 0.0.0.0               | [RFC4566]              |
   | s=-                                      | [RFC4566]              |
   | t=0 0                                    | [RFC4566]              |



Nandakumar & Jennings   Expires February 5, 2016               [Page 92]

Internet-Draft                 SDP4WebRTC                    August 2015


   | m=audio 49203 UDP/TLS/RTP/SAVPF 109      | [RFC4566]              |
   | c=IN IP4 24.23.204.141                   | [RFC4566]              |
   | a=rtcp:60065 IN IP4 24.23.204.141        | [RFC3605]              |
   | a=rtpmap:109 opus/48000/2                | [I-D.ietf-payload-rtp- |
   |                                          | opus]                  |
   | a=extmap:1 urn:ietf:params:rtp-hdrext    | [RFC6464]              |
   | :ssrc-audio-level                        |                        |
   | a=ptime:20                               | [I-D.ietf-payload-rtp- |
   |                                          | opus]                  |
   | a=rtcp-fb:109 nack                       | [RFC5104]              |
   | a=sendrecv                               | [RFC3264]              |
   | a=setup:active                           | [RFC4145]              |
   | a=ice-ufrag:ufrag:c300d85b               | [RFC5245]              |
   | a=ice-                                   | [RFC5245]              |
   | pwd:de4e99bd291c325921d5d47efbabd9a2     |                        |
   | a=fingerprint:sha-1                      | [RFC5245]              |
   | 99:41:49:83:4a:97:0e:1f:ef:6d:f7:c9:c7:  |                        |
   | 70:9d:1f:66:79:a8:07                     |                        |
   | a=candidate:0 1 UDP  2113667327          | [RFC5245]              |
   | 192.168.1.7 49203 typ host               |                        |
   | a=candidate:1 1 UDP  694302207           | [RFC5245]              |
   | 98.248.92.77 49203 typ srflx raddr       |                        |
   | 192.168.1.7 rport 49203                  |                        |
   | a=candidate:0 2 UDP 2113667326           | [RFC5245]              |
   | 192.168.1.7 60065 typ host               |                        |
   | a=candidate:1 2 UDP  1694302206          | [RFC5245]              |
   | 98.248.92.77 60065 typ srflx raddr       |                        |
   | 192.168.1.7 rport 60065                  |                        |
   | a=rtcp-rsize                             | [RFC5506]              |
   | m=video 0 UDP/TLS/RTP/SAVPF 98 100       | Bob doesn't recognize  |
   |                                          | bundle-only and hence  |
   |                                          | rejects the video      |
   |                                          | stream                 |
   | c=IN IP4 98.248.92.77                    | [RFC4566]              |
   | a=rtpmap:98 VP8/90000                    | [I-D.ietf-payload-vp8] |
   | a=rtpmap:100 VP8/90000                   | [I-D.ietf-payload-vp8] |
   | a=imageattr:98 [x=1280,y=720]            | [RFC6236]              |
   | a=fmtp:98 max-fr=30                      | [RFC4566]              |
   | m=video 0 UDP/TLS/RTP/SAVPF 98 100       | Bob doesn't recognize  |
   |                                          | bundle-only and hence  |
   |                                          | rejects the video      |
   |                                          | stream                 |
   | c=IN IP4 98.248.92.77                    | [RFC4566]              |
   | a=rtpmap:101 H264/90000                  | [RFC3984]              |
   | a=fmtp:101 profile-level-id=4d0028       | [RFC3984]Camera-2,Enco |
   | ;packetization-mode=1;max-fr=30          | ding-1 Resolution      |
   +------------------------------------------+------------------------+




Nandakumar & Jennings   Expires February 5, 2016               [Page 93]

Internet-Draft                 SDP4WebRTC                    August 2015


                         Table 41: 5.4.3 SDP Anwer

5.4.4.  Legacy Interop with RTP/AVP profile

   In this section, we attempt to provide session descriptions
   showcasing inter-operability between a WebRTC end-point and a Legacy
   VOIP end-point.  The ideas included in here are not fully baked into
   the standards and might be controversial in nature.  The hope here is
   to demonstrate a plausible SDP composition to enchance seamless
   inter-operability between the aforementioned communication systems.

   In the scenario desribed below, Alice is a legacy end-point which
   sends [RFC3264] Offer with two sets of media descriptions per media
   type.

      One set that correponds to [WebRTC] compliant UDP/TLS/RTP/SAVPF
      based audio and video descriptions.

      Another set with RTP/AVP based audio and video descriptions for
      the legacy Interop purposes.

      Also to note, Alice includes session level DTLS information and
      media level RTCP feedback information as applicable to both the
      sets of media descriptions

   On the other hand, Bob being a WebRTC end-point, recognizes accepts
   the media descriptions with RTP/AVP profile.  The security and
   feedback requirements for the session are either handled by a
   intermediate gateway or with some combination of Alice's capabilities
   and the intermediate gateway.





















Nandakumar & Jennings   Expires February 5, 2016               [Page 94]

Internet-Draft                 SDP4WebRTC                    August 2015


           Successful 2-Way WebRTC <-> VOIP Interop

Alice                                                          Bob
|                                                               |
|                                                               |
|  Alice is a legacy VOIP End-point & Bob is a WebRTC End-Point |
|                                                               |
|                                                               |
|                                                               |
|              Offer(Audio:Opus Video:H.264)                    |
|-------------------------------------------------------------->|
|                                                               |
|                                                               |
|Alice includes 2 copies of media descriptions                  |
|1. WebRTC compliant media description (UDP/TLS/RTP/SAVPF)      |
|2. Legacy compliant media description (RTP/AVP)                |
|                                                               |
|                                                               |
|             Answer(Audio:Opus, Video:H.264)                   |
|<--------------------------------------------------------------|
|                                                               |Bob
|                                                               |accepts
|                                                               |"legacy
|                                                               |compliant"
|                                                               |m=line
|                                                               |
|                                                               |
|             Two way Opus Audio, H.264 Video                   |
|...............................................................|
|       Session also suports RTP/RTCP Mux, RTCP Feedback        |
|                                                               |


   +-----------------------------------------------+-------------------+
   | SDP Contents                                  | RFC#/Notes        |
   +-----------------------------------------------+-------------------+
   | v=0                                           | [RFC4566]         |
   | o=- 20518 0 IN IP4 0.0.0.0                    | [RFC4566]         |
   | s=-                                           | [RFC4566]         |
   | t=0 0                                         | [RFC4566]         |
   | a=ice-ufrag:074c6550                          | [RFC5245]         |
   | a=ice-pwd:a28a397a4c3f31747d1ee3474af08a068   | [RFC5245]         |
   | a=fingerprint:sha-1 99:41:49:83:4a:97:0e:1f:e | [RFC5245]         |
   | f:6d:f7:c9:c7:70:9d:1f:66:79:a8:07            |                   |
   | a=rtcp-rsize                                  | [RFC5506]         |
   | m=audio 54609 UDP/TLS/RTP/SAVPF 109           | [RFC4566]         |
   | c=IN IP4 24.23.204.141                        | [RFC4566]         |
   | a=rtpmap:109 opus/48000                       |                   |



Nandakumar & Jennings   Expires February 5, 2016               [Page 95]

Internet-Draft                 SDP4WebRTC                    August 2015


   | a=ptime:20                                    |                   |
   | a=sendrecv                                    | [RFC3264]         |
   | a=rtcp-mux                                    | [RFC5761]         |
   | a=candidate:0 1 UDP  2113667327 192.168.1.4   | [RFC5245]         |
   | 54609 typ host                                |                   |
   | a=candidate:1 1 UDP  694302207 24.23.204.141  | [RFC5245]         |
   | 54609 typ srflx raddr 192.168.1.4 rport 54609 |                   |
   | a=candidate:0 2 UDP 2113667326 192.168.1.4    | [RFC5245]         |
   | 64678 typ host                                |                   |
   | a=candidate:1 2 UDP  1694302206 24.23.204.141 | [RFC5245]         |
   | 64678 typ srflx raddr 192.168.1.4 rport 64678 |                   |
   | a=rtcp-fb:109 nack                            | [RFC5104]         |
   | m=video  62537 UDP/TLS/RTP/SAVPF 120          | [RFC4566]         |
   | c=IN IP4 24.23.204.141                        | [RFC4566]         |
   | a=rtpmap:120 VP8/90000                        | [I-D.ietf-payload |
   |                                               | -vp8]             |
   | a=sendrecv                                    | [RFC3264]         |
   | a=rtcp-mux                                    | [RFC5761]         |
   | a=candidate:0 1 UDP  2113667327 192.168.1.4   | [RFC5245]         |
   | 62537 typ host                                |                   |
   | a=candidate:1 1 UDP  1694302207 24.23.204.141 | [RFC5245]         |
   | 62537 typ srflx raddr 192.168.1.4 rport 62537 |                   |
   | a=candidate:0 2 2113667326 192.168.1.4 54721  | [RFC5245]         |
   | typ host                                      |                   |
   | a=candidate:1 2 UDP 1694302206 24.23.204.141  | [RFC5245]         |
   | 54721 typ srflx raddr 192.168.1.4 rport 54721 |                   |
   | a=rtcp-fb:120 nack pli                        | [RFC5104]         |
   | a=rtcp-fb:120 ccm fir                         | [RFC5104]         |
   | ---------------                               | These set of      |
   |                                               | media             |
   |                                               | descriptions are  |
   |                                               | for Legacy Inter- |
   |                                               | op purposes       |
   | m=audio 54732 RTP/AVP 109                     | [RFC4566]Alice    |
   |                                               | includes RTP/AVP  |
   |                                               | audio stream      |
   |                                               | description       |
   | c=IN IP4 24.23.204.141                        | [RFC4566]         |
   | a=fingerprint:sha-1 99:41:49:83:4a:97:0e:1f:7 | [RFC5245]         |
   | f:7d:f9:c9:c7:70:9d:1f:66:79:a8:07            |                   |
   | a=rtpmap:109 opus/48000                       |                   |
   | a=ptime:20                                    |                   |
   | a=sendrecv                                    | [RFC3264]         |
   | a=rtcp-mux                                    | [RFC5761]Alice    |
   |                                               | still includes    |
   |                                               | RTP/RTCP Mux      |
   |                                               | support           |
   | a=candidate:0 1 UDP  2113667327 192.168.1.4   | [RFC5245]         |



Nandakumar & Jennings   Expires February 5, 2016               [Page 96]

Internet-Draft                 SDP4WebRTC                    August 2015


   | 54732 typ host                                |                   |
   | a=candidate:1 1 UDP  694302207 24.23.204.141  | [RFC5245]         |
   | 54732 typ srflx raddr 192.168.1.4 rport 54732 |                   |
   | a=candidate:0 2 UDP 2113667326 192.168.1.4    | [RFC5245]         |
   | 64678 typ host                                |                   |
   | a=candidate:1 2 UDP  1694302206 24.23.204.141 | [RFC5245]         |
   | 64678 typ srflx raddr 192.168.1.4 rport 64678 |                   |
   | a=rtcp-fb:109 nack                            | [RFC5104]She adds |
   |                                               | her intent for    |
   |                                               | NACK RTCP         |
   |                                               | feedback support  |
   | m=video  62445 RTP/AVP 120                    | [RFC4566]Alice    |
   |                                               | includes RTP/AVP  |
   |                                               | video stream      |
   |                                               | description       |
   | c=IN IP4 24.23.204.141                        | [RFC4566]         |
   | a=fingerprint:sha-1 99:41:49:83:4a:97:0e:1f:e | [RFC5245]         |
   | f:7d:f7:c9:c7:70:9d:1f:66:79:a8:07            |                   |
   | a=rtpmap:120 VP8/90000                        | [I-D.ietf-payload |
   |                                               | -vp8]             |
   | a=sendrecv                                    | [RFC3264]         |
   | a=rtcp-mux                                    | [RFC5761]Alice    |
   |                                               | intends to        |
   |                                               | perform RTP/RTCP  |
   |                                               | Mux               |
   | a=candidate:0 1 UDP  2113667327 192.168.1.4   | [RFC5245]         |
   | 62445 typ host                                |                   |
   | a=candidate:1 1 UDP  1694302207 24.23.204.141 | [RFC5245]         |
   | 62537 typ srflx raddr 192.168.1.4 rport 62445 |                   |
   | a=candidate:0 2 2113667326 192.168.1.4 54721  | [RFC5245]         |
   | typ host                                      |                   |
   | a=candidate:1 2 UDP 1694302206 24.23.204.141  | [RFC5245]         |
   | 54721 typ srflx raddr 192.168.1.4 rport 54721 |                   |
   | a=rtcp-fb:120 nack pli                        | [RFC5104] Alice   |
   |                                               | indicates support |
   |                                               | for Picture loss  |
   |                                               | Indication and    |
   |                                               | NACK RTCP         |
   |                                               | feedback          |
   | a=rtcp-fb:120 ccm fir                         | [RFC5104]         |
   +-----------------------------------------------+-------------------+

                         Table 42: 5.4.5 SDP Offer








Nandakumar & Jennings   Expires February 5, 2016               [Page 97]

Internet-Draft                 SDP4WebRTC                    August 2015


   +-----------------------------------------------+-------------------+
   | SDP Contents                                  | RFC#/Notes        |
   +-----------------------------------------------+-------------------+
   | v=0                                           | [RFC4566]         |
   | o=-  16833 0 IN IP4 0.0.0.0                   | [RFC4566]         |
   | s=-                                           | [RFC4566]         |
   | t=0 0                                         | [RFC4566]         |
   | a=ice-ufrag:c300d85b                          | [RFC5245]         |
   | a=ice-pwd:de4e99bd291c325921d5d47efbabd9a2    | [RFC5245]         |
   | a=fingerprint:sha-1 99:41:49:83:4a:97:0e:1f:e | [RFC5245]         |
   | f:6d:f7:c9:c7:70:9d:1f:66:79:a8:07            |                   |
   | m=audio 49203 RTP/AVP 109                     | [RFC4566] Bob     |
   |                                               | accepts RTP/AVP   |
   |                                               | based audio       |
   |                                               | stream            |
   | c=IN IP4 98.248.92.77                         | [RFC4566]         |
   | a=rtpmap:109 opus/48000                       |                   |
   | a=ptime:20                                    |                   |
   | a=sendrecv                                    | [RFC3264]         |
   | a=candidate:0 1 UDP 2113667327 192.168.1.7    | [RFC5245]         |
   | 49203 typ host                                |                   |
   | a=candidate:1 1 UDP 1694302207 98.248.92.77   | [RFC5245]         |
   | 49203 typ srflx raddr 192.168.1.7 rport 49203 |                   |
   | a=candidate:0 2 UDP 2113667326 192.168.1.7    | [RFC5245]         |
   | 60065 typ host                                |                   |
   | a=candidate:1 2 UDP 1694302206 98.248.92.77   | [RFC5245]         |
   | 60065 typ srflx raddr 192.168.1.7 rport 60065 |                   |
   | m=video  63130 RTP/SAVP 120                   | [RFC4566] Bob     |
   |                                               | accepts RTP/AVP   |
   |                                               | based video stram |
   | c=IN IP4 98.248.92.771                        | [RFC4566]         |
   | a=rtpmap:120 VP8/90000                        | [I-D.ietf-payload |
   |                                               | -vp8]             |
   | a=sendrecv                                    | [RFC3264]         |
   | a=candidate:0 1 UDP 2113667327 192.168.1.7    | [RFC5245]         |
   | 63130 typ host                                |                   |
   | a=candidate:1 1 UDP 1694302207 98.248.92.77   | [RFC5245]         |
   | 63130 typ srflx raddr 192.168.1.7 rport 63130 |                   |
   | a=candidate:0 2 UDP 2113667326 192.168.1.7    | [RFC5245]         |
   | 56607 typ host                                |                   |
   | a=candidate:1 2 UDP 1694302206 98.248.92.77   | [RFC5245]         |
   | 56607 typ srflx raddr 192.168.1.7 rport 56607 |                   |
   +-----------------------------------------------+-------------------+

                        Table 43: 5.4.5 SDP Answer






Nandakumar & Jennings   Expires February 5, 2016               [Page 98]

Internet-Draft                 SDP4WebRTC                    August 2015


6.  IANA Considerations

   This document requires no actions from IANA.

7.  Acknowledgments

   We would like to thanks Justin Uberti, Chris Flo for their detailed
   review and inputs.

8.  Change Log

   [RFC EDITOR NOTE: Please remove this section when publishing]

   Changes from draft-nandakumar-rtcweb-sdp-06 and draft-nandakumar-
   rtcweb-sdp-07

   o  Added clarification on Call-Flow diagram usage

   o  More cleanups

   Changes from draft-nandakumar-rtcweb-sdp-05

   o  Added Ascii chart for all the SDP Eaxamples

   o  Improved text and updated SDP Examples for Simulcast and FEC

   o  Fixed MediaStream ID Semantics SDP Errors

   Changes from draft-nandakumar-rtcweb-sdp-04

   o  Interim version of the draft to avert expiry

   o  Corrected placement of c= line as per RFC4566

   o  Updated simulcast SDP to reflect draft-westerlund-avtcore-rtp-
      simulcast-04

   Changes from draft-nandakumar-rtcweb-sdp-03

   o  Aligned more closely with JSEP version -05

   o  Added Conventions to help readability

   o  Add more examples to clarify BUNDLE use-cases

   Changes from draft-nandakumar-rtcweb-sdp-02

   o  Major refactoring was done to group the examples in to categories



Nandakumar & Jennings   Expires February 5, 2016               [Page 99]

Internet-Draft                 SDP4WebRTC                    August 2015


   o  SDP was updated through out to reflect JSEP-04 style of defining
      attributes per m=line than at the session level.

   o  Added 8 new examples.

   o  Updated references for Trickle, Unified Plan

   o  Add section to explain the syntax conventions followed in the
      examples.

   Changes from draft-nandakumar-rtcweb-sdp-01

   o  Updated references to OPUS RTP Payload Specification.

   o  Updated BUNDLE examples based on the latest draft-ietf-mmusic-sdp-
      bundle-negotiation.

   o  Added examples for multiple audio and video flows based on Unified
      Plan.

   o  Added new examples for RTX and FEC streams

   o  Updated Simulcast and SVC examples

   Changes from draft-nandakumar-rtcweb-sdp-00

   o  Fixed editorial comments on the mailing list.

   o  Updated Data-channel SDP information based on draft-ietf-mmusic-
      sctp-sdp.

   o  Updated BUNDLE examples based on draft-ietf-mmusic-sdp-bundle-
      negotiation.

   o  Added examples for few more BUNDLE variants

   o  Added new examples for Simulcast and SVC

9.  Informative References

   [RFC3264]  Rosenberg, J. and H. Schulzrinne, "An Offer/Answer Model
              with Session Description Protocol (SDP)", RFC 3264, June
              2002.

   [RFC4145]  Yon, D. and G. Camarillo, "TCP-Based Media Transport in
              the Session Description Protocol (SDP)", RFC 4145,
              September 2005.




Nandakumar & Jennings   Expires February 5, 2016              [Page 100]

Internet-Draft                 SDP4WebRTC                    August 2015


   [RFC4566]  Handley, M., Jacobson, V., and C. Perkins, "SDP: Session
              Description Protocol", RFC 4566, July 2006.

   [RFC2119]  Bradner, S., "Key words for use in RFCs to Indicate
              Requirement Levels", BCP 14, RFC 2119, March 1997.

   [RFC5245]  Rosenberg, J., "Interactive Connectivity Establishment
              (ICE): A Protocol for Network Address Translator (NAT)
              Traversal for Offer/Answer Protocols", RFC 5245, April
              2010.

   [RFC5506]  Johansson, I. and M. Westerlund, "Support for Reduced-Size
              Real-Time Transport Control Protocol (RTCP): Opportunities
              and Consequences", RFC 5506, April 2009.

   [RFC3551]  Schulzrinne, H. and S. Casner, "RTP Profile for Audio and
              Video Conferences with Minimal Control", STD 65, RFC 3551,
              July 2003.

   [RFC3952]  Duric, A. and S. Andersen, "Real-time Transport Protocol
              (RTP) Payload Format for internet Low Bit Rate Codec
              (iLBC) Speech", RFC 3952, December 2004.

   [RFC4796]  Hautakorpi, J. and G. Camarillo, "The Session Description
              Protocol (SDP) Content Attribute", RFC 4796, February
              2007.

   [RFC5761]  Perkins, C. and M. Westerlund, "Multiplexing RTP Data and
              Control Packets on a Single Port", RFC 5761, April 2010.

   [RFC3556]  Casner, S., "Session Description Protocol (SDP) Bandwidth
              Modifiers for RTP Control Protocol (RTCP) Bandwidth", RFC
              3556, July 2003.

   [RFC5104]  Wenger, S., Chandra, U., Westerlund, M., and B. Burman,
              "Codec Control Messages in the RTP Audio-Visual Profile
              with Feedback (AVPF)", RFC 5104, February 2008.

   [RFC4588]  Rey, J., Leon, D., Miyazaki, A., Varsa, V., and R.
              Hakenberg, "RTP Retransmission Payload Format", RFC 4588,
              July 2006.

   [RFC5956]  Begen, A., "Forward Error Correction Grouping Semantics in
              the Session Description Protocol", RFC 5956, September
              2010.

   [RFC5888]  Camarillo, G. and H. Schulzrinne, "The Session Description
              Protocol (SDP) Grouping Framework", RFC 5888, June 2010.



Nandakumar & Jennings   Expires February 5, 2016              [Page 101]

Internet-Draft                 SDP4WebRTC                    August 2015


   [RFC6236]  Johansson, I. and K. Jung, "Negotiation of Generic Image
              Attributes in the Session Description Protocol (SDP)", RFC
              6236, May 2011.

   [RFC3984]  Wenger, S., Hannuksela, M., Stockhammer, T., Westerlund,
              M., and D. Singer, "RTP Payload Format for H.264 Video",
              RFC 3984, February 2005.

   [RFC5583]  Schierl, T. and S. Wenger, "Signaling Media Decoding
              Dependency in the Session Description Protocol (SDP)", RFC
              5583, July 2009.

   [RFC5576]  Lennox, J., Ott, J., and T. Schierl, "Source-Specific
              Media Attributes in the Session Description Protocol
              (SDP)", RFC 5576, June 2009.

   [RFC3550]  Schulzrinne, H., Casner, S., Frederick, R., and V.
              Jacobson, "RTP: A Transport Protocol for Real-Time
              Applications", STD 64, RFC 3550, July 2003.

   [RFC3261]  Rosenberg, J., Schulzrinne, H., Camarillo, G., Johnston,
              A., Peterson, J., Sparks, R., Handley, M., and E.
              Schooler, "SIP: Session Initiation Protocol", RFC 3261,
              June 2002.

   [RFC2326]  Schulzrinne, H., Rao, A., and R. Lanphier, "Real Time
              Streaming Protocol (RTSP)", RFC 2326, April 1998.

   [RFC3605]  Huitema, C., "Real Time Control Protocol (RTCP) attribute
              in Session Description Protocol (SDP)", RFC 3605, October
              2003.

   [RFC2833]  Schulzrinne, H. and S. Petrack, "RTP Payload for DTMF
              Digits, Telephony Tones and Telephony Signals", RFC 2833,
              May 2000.

   [RFC6464]  Lennox, J., Ivov, E., and E. Marocco, "A Real-time
              Transport Protocol (RTP) Header Extension for Client-to-
              Mixer Audio Level Indication", RFC 6464, December 2011.

   [RFC6465]  Ivov, E., Marocco, E., and J. Lennox, "A Real-time
              Transport Protocol (RTP) Header Extension for Mixer-to-
              Client Audio Level Indication", RFC 6465, December 2011.

   [RFC7022]  Begen, A., Perkins, C., Wing, D., and E. Rescorla,
              "Guidelines for Choosing RTP Control Protocol (RTCP)
              Canonical Names (CNAMEs)", RFC 7022, September 2013.




Nandakumar & Jennings   Expires February 5, 2016              [Page 102]

Internet-Draft                 SDP4WebRTC                    August 2015


   [I-D.ietf-mmusic-sdp-bundle-negotiation]
              Holmberg, C., Alvestrand, H., and C. Jennings,
              "Negotiating Media Multiplexing Using the Session
              Description Protocol (SDP)", draft-ietf-mmusic-sdp-bundle-
              negotiation-12 (work in progress), October 2014.

   [I-D.ietf-mmusic-sdp-simulcast]
              Westerlund, M., Nandakumar, S., and M. Zanaty, "Using
              Simulcast in SDP and RTP Sessions", draft-ietf-mmusic-sdp-
              simulcast-00 (work in progress), January 2015.

   [I-D.ietf-payload-rtp-opus]
              Spittka, J., Vos, K., and J. Valin, "RTP Payload Format
              for Opus Speech and Audio Codec", draft-ietf-payload-rtp-
              opus-07 (work in progress), January 2015.

   [I-D.ietf-payload-vp8]
              Westin, P., Lundin, H., Glover, M., Uberti, J., and F.
              Galligan, "RTP Payload Format for VP8 Video", draft-ietf-
              payload-vp8-13 (work in progress), October 2014.

   [I-D.ietf-rtcweb-jsep]
              Uberti, J., Jennings, C., and E. Rescorla, "Javascript
              Session Establishment Protocol", draft-ietf-rtcweb-jsep-08
              (work in progress), October 2014.

   [I-D.ietf-mmusic-trickle-ice]
              Ivov, E., Rescorla, E., and J. Uberti, "Trickle ICE:
              Incremental Provisioning of Candidates for the Interactive
              Connectivity Establishment (ICE) Protocol", draft-ietf-
              mmusic-trickle-ice-02 (work in progress), January 2015.

   [I-D.ietf-mmusic-msid]
              Alvestrand, H., "WebRTC MediaStream Identification in the
              Session Description Protocol", draft-ietf-mmusic-msid-07
              (work in progress), October 2014.

   [I-D.ietf-mmusic-sctp-sdp]
              Holmberg, C., Loreto, S., and G. Camarillo, "Stream
              Control Transmission Protocol (SCTP)-Based Media Transport
              in the Session Description Protocol (SDP)", draft-ietf-
              mmusic-sctp-sdp-12 (work in progress), January 2015.

   [I-D.ietf-rtcweb-data-channel]
              Jesup, R., Loreto, S., and M. Tuexen, "WebRTC Data
              Channels", draft-ietf-rtcweb-data-channel-13 (work in
              progress), January 2015.




Nandakumar & Jennings   Expires February 5, 2016              [Page 103]

Internet-Draft                 SDP4WebRTC                    August 2015


   [WebRTC]   W3C, "WebRTC 1.0: Real-time Communication Between
              Browsers",
              <http://dev.w3.org/2011/webrtc/editor/webrtc.html> , .

   [UNIFIED-PLAN]
              Roach, A., Uberti, J., and M. Thomson, "A Unified Plan for
              Using SDP with Large Numbers of Media Flows", draft-roach-
              mmusic-unified-plan (work in progress), July 2013.

Authors' Addresses

   Suhas Nandakumar
   Cisco
   170 West Tasman Drive
   San Jose, CA  95134
   USA

   Email: snandaku@cisco.com


   Cullen Jennings
   Cisco
   170 West Tasman Drive
   San Jose, CA  95134
   USA

   Phone: +1 408 421-9990
   Email: fluffy@cisco.com























Nandakumar & Jennings   Expires February 5, 2016              [Page 104]