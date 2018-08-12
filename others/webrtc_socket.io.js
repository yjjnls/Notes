const socket = require('socket.io-client');
const uuid = require('uuid');

class WebRTC {
    constructor(signal_bridge, room_id, endpoint_name, app, id = uuid.v4()) {

        this.signal_bridge = signal_bridge;
        this.room_id = room_id;
        this.endpoint_name = endpoint_name;
        this.app = app;
        this.id = id;
        this.connection = null;
        this.closed = false;
        this.io = null;
        this.socketMessageEvent = 'video-conference-demo';

        this.join_msg1 = {
            "remoteUserId": this.room_id,
            "message": {
                "newParticipationRequest": true,
                "isOneWay": false,
                "isDataOnly": false,
                "localPeerSdpConstraints": {
                    "OfferToReceiveAudio": true,
                    "OfferToReceiveVideo": true
                },
                "remotePeerSdpConstraints": {
                    "OfferToReceiveAudio": true,
                    "OfferToReceiveVideo": true
                }
            },
            "sender": this.endpoint_name,
            "password": false
        };
        this.join_msg2 = {
            "remoteUserId": this.room_id,
            "message": {
                "readyForOffer": true,
                "userPreferences": {
                    "extra": {},
                    "localPeerSdpConstraints": {
                        "OfferToReceiveAudio": true,
                        "OfferToReceiveVideo": true
                    },
                    "remotePeerSdpConstraints": {
                        "OfferToReceiveAudio": true,
                        "OfferToReceiveVideo": true
                    },
                    "isOneWay": true,
                    "isDataOnly": false,
                    "dontGetRemoteStream": false,
                    "dontAttachLocalStream": false,
                    "connectionDescription": {
                        "remoteUserId": "2",
                        "message": {
                            "newParticipationRequest": true,
                            "isOneWay": false,
                            "isDataOnly": false,
                            "localPeerSdpConstraints": {
                                "OfferToReceiveAudio": true,
                                "OfferToReceiveVideo": true
                            },
                            "remotePeerSdpConstraints": {
                                "OfferToReceiveAudio": true,
                                "OfferToReceiveVideo": true
                            }
                        },
                        "sender": this.endpoint_name,
                        "password": false
                    },
                    "streamsToShare": {
                    }
                }
            },
            "sender": this.endpoint_name
        };
        this.join_msg2.message.userPreferences.streamsToShare[this.id] = {
            "isAudio": true,
            "isVideo": true,
            "isScreen": false
        };

        // this.app.on('webrtc', (data, meta) => {
        //     let obj = JSON.parse(meta);
        //     if (obj.type === 'sdp') {
        //         if (this.io) {
        //             this.io.emit('sdp', JSON.parse(data));
        //         }

        //     }
        //     if (obj.type === 'ice') {
        //         if (this.io) {
        //             this.io.emit('sdp', JSON.parse(data));
        //         }
        //     }
        // });
    }

    handleSdp(data) {
        this.app.call('remote_sdp', {
            name: this.endpoint_name,
            type: data.type,
            sdp: data.sdp
        })
            .catch(err => {
                throw err;
            });
    }

    handleCandidate(data) {
        this.app.call('remote_candidate', {
            name: this.endpoint_name,
            candidate: data.candidate,
            sdpMLineIndex: data.sdpMLineIndex
        })
            .catch(err => {
                throw err;
            });
    }

    connect() {
        this.io = socket(`${this.signal_bridge}?userid=${this.id}&sessionid=${this.room_id}&msgEvent=${this.socketMessageEvent}&socketCustomEvent=RTCMultiConnection-Custom-Message&autoCloseEntireSession=false&maxParticipantsAllowed=1000`);

        let io = this.io;
        let self = this;
        io.on('connect', (socket) => {
            io.emit('extra-data-updated', {});
            
            io.emit(this.socketMessageEvent, this.join_msg1);

            io.emit(this.socketMessageEvent, this.join_msg2);

            io.on(this.socketMessageEvent, (data) => {
                console.log('~~~~~~~~~~~ received msg: ');
                console.log(data)
            });
        });
        io.on('disconnect', (reason) => {
            console.log('webrtc socket.io disconnect: ' + reason);
            this.close();
        });
        io.on('leave', (reason) => {
            console.log(reason);
            this.close();
        });
        io.on('reconnect_failed', () => {
            this.close();
            throw Error('reconnect_failed');
        });
        io.on('connect_timeout', () => {
            io.open();
        });
        io.on('connect_error', (error) => {
            this.close();
            throw error;
        });
    }

    close() {
        if (this.io) {
            this.io.close();
            this.io = null;
        }
        if (!this.closed) {
            this.closed = true;
            // this.app.emit('webrtc-peer-closed', this.endpoint_name);
        }

    }
}

module.exports = {
    WebRTC
};


let webrtc = new WebRTC('http://localhost:9001/', 2, 'nodejs_socket.io', null);
webrtc.connect();