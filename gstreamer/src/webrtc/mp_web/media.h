#include <string.h>
#include "pipejoint.h"

// #define USE_H264 1
#define USE_VP8 1


enum AppState
{
    APP_STATE_UNKNOWN = 0,
    APP_STATE_ERROR = 1, /* generic error */
    SERVER_CONNECTING = 1000,
    SERVER_CONNECTION_ERROR,
    SERVER_CONNECTED, /* Ready to register */
    SERVER_REGISTERING = 2000,
    SERVER_REGISTRATION_ERROR,
    SERVER_REGISTERED, /* Ready to call a peer */
    SERVER_CLOSED,     /* server connection closed by us or the server */
    PEER_CONNECTING = 3000,
    PEER_CONNECTION_ERROR,
    PEER_CONNECTED,
    PEER_CALL_NEGOTIATING = 4000,
    PEER_CALL_STARTED,
    PEER_CALL_STOPPING,
    PEER_CALL_STOPPED,
    PEER_CALL_ERROR,
};
class WebRTC
{
 public:
    WebRTC(gchar *peer)
        : pipe(NULL)
        , webrtc(NULL)
        , ws_conn(NULL)
        , peer_id(NULL)
        , app_state(APP_STATE_UNKNOWN)
    {
        peer_id = new char[strlen(peer) + 1];
        memset(peer_id, 0, strlen(peer) + 1);
        strcpy(peer_id, peer);
    }
    ~WebRTC()
    {
        delete[] peer_id;
        if (pipe) {
            gst_element_set_state(GST_ELEMENT(pipe), GST_STATE_NULL);
            g_print("Pipeline stopped\n");
            gst_object_unref(pipe);
        }
        destroy_pipe_joint(&video_input_joint_);
        destroy_pipe_joint(&video_output_joint_);
        destroy_pipe_joint(&audio_input_joint_);
        destroy_pipe_joint(&audio_output_joint_);
    }

    GstElement *video_output_pipejoint() { return video_output_joint_.upstream_joint; }
    GstElement *video_input_pipejoint() { return video_input_joint_.downstream_joint; }
    GstElement *audio_output_pipejoint() { return audio_output_joint_.upstream_joint; }
    GstElement *audio_input_pipejoint() { return audio_input_joint_.downstream_joint; }

    GstElement *pipe;
    GstElement *webrtc;
    SoupWebsocketConnection *ws_conn;
    gchar *peer_id;
    AppState app_state;

    PipeJoint video_input_joint_;
    PipeJoint video_output_joint_;
    PipeJoint audio_input_joint_;
    PipeJoint audio_output_joint_;
};

WebRTC *create_ep(gchar *peer);
void terminate();