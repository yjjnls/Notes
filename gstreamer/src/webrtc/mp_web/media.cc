#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/webrtc/webrtc.h>

/* For signalling */
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

#include <string.h>

#include "media.h"




static GMainLoop *loop;
// static const gchar *server_url = "wss://webrtc.nirbheek.in:8443";
static const gchar *server_url = "ws://localhost:8443";
static gboolean strict_ssl = FALSE;

// static GOptionEntry entries[] =
//     {
//         {"peer-id", 0, 0, G_OPTION_ARG_STRING, &peer_id, "String ID of the peer to connect to", "ID"},
//         {"server", 0, 0, G_OPTION_ARG_STRING, &server_url, "Signalling server to connect to", "URL"},
//         {NULL},
// };

static gboolean
cleanup_and_quit_loop(const gchar *msg, WebRTC *ep, enum AppState state)
{
    if (msg)
        g_printerr("%s\n", msg);
    if (state > 0)
        ep->app_state = state;

    if (ep->ws_conn) {
        if (soup_websocket_connection_get_state(ep->ws_conn) ==
            SOUP_WEBSOCKET_STATE_OPEN)
            /* This will call us again */
            soup_websocket_connection_close(ep->ws_conn, 1000, "");
        else
            g_object_unref(ep->ws_conn);
    }

    if (loop) {
        g_main_loop_quit(loop);
        loop = NULL;
    }

    /* To allow usage as a GSourceFunc */
    return G_SOURCE_REMOVE;
}

static gchar *
get_string_from_json_object(JsonObject *object)
{
    JsonNode *root;
    JsonGenerator *generator;
    gchar *text;

    /* Make it the root node */
    root = json_node_init_object(json_node_alloc(), object);
    generator = json_generator_new();
    json_generator_set_root(generator, root);
    text = json_generator_to_data(generator, NULL);

    /* Release everything */
    g_object_unref(generator);
    json_node_free(root);
    return text;
}

static void
handle_media_stream(GstPad *pad, GstElement *pipe, const char *convert_name, const char *sink_name)
{
    GstPad *qpad;
    GstElement *q, *conv, *resample, *sink;
    GstPadLinkReturn ret;

    g_print("Trying to handle stream with %s ! %s", convert_name, sink_name);

    q = gst_element_factory_make("queue", NULL);
    g_assert_nonnull(q);
    conv = gst_element_factory_make(convert_name, NULL);
    g_assert_nonnull(conv);
    sink = gst_element_factory_make(sink_name, NULL);
    g_assert_nonnull(sink);

    if (g_strcmp0(convert_name, "audioconvert") == 0) {
        /* Might also need to resample, so add it just in case.
     * Will be a no-op if it's not required. */
        resample = gst_element_factory_make("audioresample", NULL);
        g_assert_nonnull(resample);
        gst_bin_add_many(GST_BIN(pipe), q, conv, resample, sink, NULL);
        gst_element_sync_state_with_parent(q);
        gst_element_sync_state_with_parent(conv);
        gst_element_sync_state_with_parent(resample);
        gst_element_sync_state_with_parent(sink);
        gst_element_link_many(q, conv, resample, sink, NULL);
    } else {
        gst_bin_add_many(GST_BIN(pipe), q, conv, sink, NULL);
        gst_element_sync_state_with_parent(q);
        gst_element_sync_state_with_parent(conv);
        gst_element_sync_state_with_parent(sink);
        gst_element_link_many(q, conv, sink, NULL);
    }

    qpad = gst_element_get_static_pad(q, "sink");

    ret = gst_pad_link(pad, qpad);
    g_assert_cmphex(ret, ==, GST_PAD_LINK_OK);
}

static void
on_incoming_decodebin_stream(GstElement *decodebin, GstPad *pad, GstElement *pipe)
{
    GstCaps *caps;
    const gchar *name;

    if (!gst_pad_has_current_caps(pad)) {
        g_printerr("Pad '%s' has no caps, can't do anything, ignoring\n",
                   GST_PAD_NAME(pad));
        return;
    }

    caps = gst_pad_get_current_caps(pad);
    name = gst_structure_get_name(gst_caps_get_structure(caps, 0));

    if (g_str_has_prefix(name, "video")) {
        handle_media_stream(pad, pipe, "videoconvert", "autovideosink");
    } else if (g_str_has_prefix(name, "audio")) {
        handle_media_stream(pad, pipe, "audioconvert", "autoaudiosink");
    } else {
        g_printerr("Unknown pad %s, ignoring", GST_PAD_NAME(pad));
    }
}

GstPadProbeReturn on_monitor_data(GstPad *pad, GstPadProbeInfo *info, gpointer rtspclient)
{
    printf("-");
    return GST_PAD_PROBE_OK;
}
static void
on_incoming_stream(GstElement *webrtc_element, GstPad *new_pad, WebRTC *ep)
{
    WebRTC *webrtc = ep;

    GstElement *out = NULL;
    GstPad *sink = NULL;
    GstCaps *caps;
    GstStructure *s;
    const gchar *encoding_name;

    if (GST_PAD_DIRECTION(new_pad) != GST_PAD_SRC)
        return;

    caps = gst_pad_get_current_caps(new_pad);
    if (!caps)
        caps = gst_pad_query_caps(new_pad, NULL);
    s = gst_caps_get_structure(caps, 0);
    encoding_name = gst_structure_get_string(s, "encoding-name");
#ifdef USE_VP8
    if (g_strcmp0(encoding_name, "VP8") == 0) {
        out = gst_parse_bin_from_description(
            // "rtpvp8depay ! tee name=local_tee allow-not-linked=true ! queue ! vp8dec ! videoconvert ! ximagesink",
            "rtpvp8depay ! tee name=local_tee allow-not-linked=true",
            TRUE,
            NULL);
#elif USE_H264
    if (g_strcmp0(encoding_name, "H264") == 0) {
        out = gst_parse_bin_from_description(
            "rtph264depay ! tee name=local_tee allow-not-linked=true",
            TRUE,
            NULL);
#endif

        g_warn_if_fail(gst_bin_add(GST_BIN(out), webrtc->video_input_joint_.upstream_joint));
        GstElement *local_tee = gst_bin_get_by_name_recurse_up(GST_BIN(out), "local_tee");
        g_warn_if_fail(gst_element_link(local_tee, webrtc->video_input_joint_.upstream_joint));
        gst_object_unref(local_tee);
    } else if (g_strcmp0(encoding_name, "PCMA") == 0) {
        printf("~~~~~~~~~~~~~~~~~pcma~~~~~~~~~~~\n");
        out = gst_parse_bin_from_description(
            "rtppcmadepay ! tee name=local_audio_tee allow-not-linked=true ! queue ! alawdec ! audioconvert ! spectrascope shader=3 ! ximagesink sync=false",
            // "rtppcmadepay ! tee name=local_audio_tee allow-not-linked=true",
            TRUE,
            NULL);

        g_warn_if_fail(gst_bin_add(GST_BIN(out), webrtc->audio_input_joint_.upstream_joint));
        GstElement *local_tee = gst_bin_get_by_name_recurse_up(GST_BIN(out), "local_audio_tee");
        g_warn_if_fail(gst_element_link(local_tee, webrtc->audio_input_joint_.upstream_joint));

        // GstPad *pad = gst_element_get_static_pad(webrtc->audio_input_joint_.upstream_joint, "sink");
        // gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, on_monitor_data, NULL, NULL);
        // gst_object_unref(pad);

        gst_object_unref(local_tee);
    } else {
        g_critical("Unknown encoding name %s", encoding_name);
        g_assert_not_reached();
    }
    gst_bin_add(GST_BIN(webrtc->pipe), out);
    gst_element_sync_state_with_parent(out);
    sink = (GstPad *)out->sinkpads->data;

    gst_pad_link(new_pad, sink);

    gst_caps_unref(caps);
}

static void
send_ice_candidate_message(GstElement *webrtc G_GNUC_UNUSED, guint mlineindex, gchar *candidate, WebRTC *ep)
{
    gchar *text;
    JsonObject *ice, *msg;

    if (ep->app_state < PEER_CALL_NEGOTIATING) {
        cleanup_and_quit_loop("Can't send ICE, not in call", ep, APP_STATE_ERROR);
        return;
    }

    ice = json_object_new();
    json_object_set_string_member(ice, "candidate", candidate);
    json_object_set_int_member(ice, "sdpMLineIndex", mlineindex);
    msg = json_object_new();
    json_object_set_object_member(msg, "ice", ice);
    text = get_string_from_json_object(msg);
    json_object_unref(msg);
    g_print("==========>send ice candidate:\n%s\n", text);

    soup_websocket_connection_send_text(ep->ws_conn, text);
    g_free(text);
}

static void
send_sdp_offer(GstWebRTCSessionDescription *offer, WebRTC *ep)
{
    gchar *text;
    JsonObject *msg, *sdp;

    if (ep->app_state < PEER_CALL_NEGOTIATING) {
        cleanup_and_quit_loop("Can't send offer, not in call", ep, APP_STATE_ERROR);
        return;
    }

    text = gst_sdp_message_as_text(offer->sdp);

    sdp = json_object_new();
    json_object_set_string_member(sdp, "type", "offer");
    json_object_set_string_member(sdp, "sdp", text);
    g_free(text);

    msg = json_object_new();
    json_object_set_object_member(msg, "sdp", sdp);
    text = get_string_from_json_object(msg);
    json_object_unref(msg);

    g_print("==========>Sending offer:\n%s\n", text);
    soup_websocket_connection_send_text(ep->ws_conn, text);
    g_free(text);
}

/* Offer created by our pipeline, to be sent to the peer */
static void
on_offer_created(GstPromise *promise, gpointer data)
{
    WebRTC *ep = (WebRTC *)data;
    g_print("==========>on_offer_created\n");
    GstWebRTCSessionDescription *offer = NULL;
    const GstStructure *reply;

    g_assert_cmphex(ep->app_state, ==, PEER_CALL_NEGOTIATING);

    g_assert_cmphex(gst_promise_wait(promise), ==, GST_PROMISE_RESULT_REPLIED);
    reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &offer, NULL);
    gst_promise_unref(promise);
    gst_sdp_media_add_attribute((GstSDPMedia *)&g_array_index(offer->sdp->medias, GstSDPMedia, 0),
                                "fmtp",
                                "96 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e01f");
    gchar *desc;
    desc = gst_sdp_message_as_text(offer->sdp);
    g_print("==========>Created offer:\n%s\n", desc);
    g_free(desc);

    promise = gst_promise_new();
    g_signal_emit_by_name(ep->webrtc, "set-local-description", offer, promise);
    gst_promise_interrupt(promise);
    gst_promise_unref(promise);

    /* Send offer to peer */
    send_sdp_offer(offer, ep);
    gst_webrtc_session_description_free(offer);
}

static void
on_negotiation_needed(GstElement *element, WebRTC *ep)
{
    GstPromise *promise;

    ep->app_state = PEER_CALL_NEGOTIATING;
    promise = gst_promise_new_with_change_func(on_offer_created, ep, NULL);

    g_signal_emit_by_name(ep->webrtc, "create-offer", NULL, promise);
}

#define STUN_SERVER " stun-server=stun://stun.l.google.com:19302 "
#define RTP_CAPS_AUDIO "application/x-rtp,media=audio,encoding-name=PCMA,payload="
#ifdef USE_VP8
#define RTP_CAPS_VIDEO "application/x-rtp,media=video,encoding-name=VP8,payload="
#elif USE_H264
#define RTP_CAPS_VIDEO "application/x-rtp,media=video,encoding-name=H264,payload="
#endif
static gboolean
start_pipeline(WebRTC *ep)
{
    GstStateChangeReturn ret;
    GError *error = NULL;

    ep->pipe = gst_parse_launch(
        "webrtcbin name=sendrecv "
    // "videotestsrc pattern=ball ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay ! "
#ifdef USE_VP8
        "queue name=video_joint ! rtpvp8pay ! " RTP_CAPS_VIDEO
#elif USE_H264
        "queue name=video_joint ! rtph264pay config-interval=-1 ! " RTP_CAPS_VIDEO
#endif
        "96 ! sendrecv. "
        // "audiotestsrc wave=red-noise ! audioconvert ! audioresample ! queue ! alawenc ! rtppcmapay ! "
        "queue name=audio_joint ! rtppcmapay ! " RTP_CAPS_AUDIO "8 ! sendrecv. ",
        &error);

    if (error) {
        g_printerr("Failed to parse launch: %s\n", error->message);
        g_error_free(error);
        if (ep->pipe)
            g_clear_object(&ep->pipe);
        if (ep->webrtc)
            ep->webrtc = NULL;
        return FALSE;
    }

    ep->webrtc = gst_bin_get_by_name(GST_BIN(ep->pipe), "sendrecv");
    g_assert_nonnull(ep->webrtc);

    /* This is the gstwebrtc entry point where we create the offer and so on. It
   * will be called when the pipeline goes to PLAYING. */
    g_signal_connect(ep->webrtc, "on-negotiation-needed", G_CALLBACK(on_negotiation_needed), ep);

    g_signal_connect(ep->webrtc, "on-ice-candidate", G_CALLBACK(send_ice_candidate_message), ep);
    /* Incoming streams will be exposed via this signal */
    g_signal_connect(ep->webrtc, "pad-added", G_CALLBACK(on_incoming_stream), ep);
    /* Lifetime is the same as the pipeline itself */
    gst_object_unref(ep->webrtc);
    ////////////////////////////////////////////////////////////////
    static int session_count = 0;
    // video
    static std::string video_media_type = "video";
    std::string video_output_pipejoint_name = std::string("webrtc_video_output_joint_") +
                                              std::to_string(session_count);
    ep->video_output_joint_ = make_pipe_joint(video_media_type, video_output_pipejoint_name);
    g_warn_if_fail(gst_bin_add(GST_BIN(ep->pipe), ep->video_output_joint_.downstream_joint));
    GstElement *video_joint = gst_bin_get_by_name_recurse_up(GST_BIN(ep->pipe), "video_joint");
    g_warn_if_fail(gst_element_link(ep->video_output_joint_.downstream_joint, video_joint));

    std::string video_input_pipejoint_name = std::string("webrtc_video_input_joint_") +
                                             std::to_string(session_count);
    ep->video_input_joint_ = make_pipe_joint(video_media_type, video_input_pipejoint_name);
    // audio
    static std::string audio_media_type = "audio";
    std::string audio_output_pipejoint_name = std::string("webrtc_audio_output_joint_") +
                                              std::to_string(session_count);
    ep->audio_output_joint_ = make_pipe_joint(audio_media_type, audio_output_pipejoint_name);
    g_warn_if_fail(gst_bin_add(GST_BIN(ep->pipe), ep->audio_output_joint_.downstream_joint));
    GstElement *audio_joint = gst_bin_get_by_name_recurse_up(GST_BIN(ep->pipe), "audio_joint");
    g_warn_if_fail(gst_element_link(ep->audio_output_joint_.downstream_joint, audio_joint));

    std::string audio_input_pipejoint_name = std::string("webrtc_audio_input_joint_") +
                                             std::to_string(session_count);
    ep->audio_input_joint_ = make_pipe_joint(audio_media_type, audio_input_pipejoint_name);
    session_count++;
    ////////////////////////////////////////////////////////////////
    g_print("Starting pipeline\n");
    ret = gst_element_set_state(GST_ELEMENT(ep->pipe), GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
        goto err;

    return TRUE;

err:
    if (ep->pipe)
        g_clear_object(&ep->pipe);
    if (ep->webrtc)
        ep->webrtc = NULL;
    return FALSE;
}

static gboolean
setup_call(WebRTC *ep)
{
    gchar *msg;

    if (soup_websocket_connection_get_state(ep->ws_conn) !=
        SOUP_WEBSOCKET_STATE_OPEN)
        return FALSE;

    if (!ep->peer_id)
        return FALSE;

    g_print("Setting up signalling server call with %s\n", ep->peer_id);
    ep->app_state = PEER_CONNECTING;
    msg = g_strdup_printf("SESSION %s", ep->peer_id);
    soup_websocket_connection_send_text(ep->ws_conn, msg);
    g_free(msg);
    return TRUE;
}

static gboolean
register_with_server(WebRTC *ep)
{
    gchar *hello;
    gint32 our_id;

    if (soup_websocket_connection_get_state(ep->ws_conn) !=
        SOUP_WEBSOCKET_STATE_OPEN)
        return FALSE;

    our_id = g_random_int_range(10, 10000);
    g_print("Registering id %i with server\n", our_id);
    ep->app_state = SERVER_REGISTERING;

    hello = g_strdup_printf("HELLO %i", our_id);
    soup_websocket_connection_send_text(ep->ws_conn, hello);
    g_free(hello);

    return TRUE;
}

static void
on_server_closed(SoupWebsocketConnection *conn G_GNUC_UNUSED,
                 WebRTC *ep)
{
    ep->app_state = SERVER_CLOSED;
    cleanup_and_quit_loop("Server connection closed", ep, (AppState)0);
}

/* One mega message handler for our asynchronous calling mechanism */
static void
on_server_message(SoupWebsocketConnection *conn, SoupWebsocketDataType type, GBytes *message, WebRTC *ep)
{
    printf("~~~~~~~~~~~~~~~~on_server_message, ep->peer_id:%s\n", ep->peer_id);
    gsize size;
    gchar *text, *data;

    switch (type) {
        case SOUP_WEBSOCKET_DATA_BINARY:
            g_printerr("Received unknown binary message, ignoring\n");
            g_bytes_unref(message);
            return;
        case SOUP_WEBSOCKET_DATA_TEXT:
            data = (gchar *)g_bytes_unref_to_data(message, &size);
            /* Convert to NULL-terminated string */
            text = g_strndup(data, size);
            g_free(data);
            break;
        default:
            g_assert_not_reached();
    }

    /* Server has accepted our registration, we are ready to send commands */
    if (g_strcmp0(text, "HELLO") == 0) {
        if (ep->app_state != SERVER_REGISTERING) {
            cleanup_and_quit_loop("ERROR: Received HELLO when not registering", ep, APP_STATE_ERROR);
            goto out;
        }
        ep->app_state = SERVER_REGISTERED;
        g_print("==========>Registered with server\n");
        /* Ask signalling server to connect us with a specific peer */
        if (!setup_call(ep)) {
            cleanup_and_quit_loop("ERROR: Failed to setup call", ep, PEER_CALL_ERROR);
            goto out;
        }
        /* Call has been setup by the server, now we can start negotiation */
    } else if (g_strcmp0(text, "SESSION_OK") == 0) {
        if (ep->app_state != PEER_CONNECTING) {
            cleanup_and_quit_loop("ERROR: Received SESSION_OK when not calling", ep, PEER_CONNECTION_ERROR);
            goto out;
        }
        g_print("==========>SESSION_OK start_pipeline!\n");
        ep->app_state = PEER_CONNECTED;
        /* Start negotiation (exchange SDP and ICE candidates) */
        if (!start_pipeline(ep))
            cleanup_and_quit_loop("ERROR: failed to start pipeline", ep, PEER_CALL_ERROR);
        g_print("==========>SESSION_OK start_pipeline over!\n");
        /* Handle errors */
    } else if (g_str_has_prefix(text, "ERROR")) {
        switch (ep->app_state) {
            case SERVER_CONNECTING:
                ep->app_state = SERVER_CONNECTION_ERROR;
                break;
            case SERVER_REGISTERING:
                ep->app_state = SERVER_REGISTRATION_ERROR;
                break;
            case PEER_CONNECTING:
                ep->app_state = PEER_CONNECTION_ERROR;
                break;
            case PEER_CONNECTED:
            case PEER_CALL_NEGOTIATING:
                ep->app_state = PEER_CALL_ERROR;
            default:
                ep->app_state = APP_STATE_ERROR;
        }
        cleanup_and_quit_loop(text, ep, (AppState)0);
        /* Look for JSON messages containing SDP and ICE candidates */
    } else {
        g_print("\n=========>receive\n%s\n", text);
        JsonNode *root;
        JsonObject *object, *child;
        JsonParser *parser = json_parser_new();
        if (!json_parser_load_from_data(parser, text, -1, NULL)) {
            g_printerr("Unknown message '%s', ignoring", text);
            g_object_unref(parser);
            goto out;
        }

        root = json_parser_get_root(parser);
        if (!JSON_NODE_HOLDS_OBJECT(root)) {
            g_printerr("Unknown json message '%s', ignoring", text);
            g_object_unref(parser);
            goto out;
        }

        object = json_node_get_object(root);
        /* Check type of JSON message */
        if (json_object_has_member(object, "sdp")) {
            int ret;
            GstSDPMessage *sdp;
            const gchar *text, *sdptype;
            GstWebRTCSessionDescription *answer;

            g_assert_cmphex(ep->app_state, ==, PEER_CALL_NEGOTIATING);

            child = json_object_get_object_member(object, "sdp");

            if (!json_object_has_member(child, "type")) {
                cleanup_and_quit_loop("ERROR: received SDP without 'type'", ep, PEER_CALL_ERROR);
                goto out;
            }

            sdptype = json_object_get_string_member(child, "type");
            /* In this example, we always create the offer and receive one answer.
       * See tests/examples/webrtcbidirectional.c in gst-plugins-bad for how to
       * handle offers from peers and reply with answers using webrtcbin. */
            g_assert_cmpstr(sdptype, ==, "answer");

            text = json_object_get_string_member(child, "sdp");

            //   g_print ("==========>Received answer:\n%s\n", text);

            ret = gst_sdp_message_new(&sdp);
            g_assert_cmphex(ret, ==, GST_SDP_OK);

            ret = gst_sdp_message_parse_buffer((guint8 *)text, strlen(text), sdp);
            g_assert_cmphex(ret, ==, GST_SDP_OK);

            answer = gst_webrtc_session_description_new(GST_WEBRTC_SDP_TYPE_ANSWER,
                                                        sdp);
            g_assert_nonnull(answer);

            /* Set remote description on our pipeline */
            {
                GstPromise *promise = gst_promise_new();
                g_signal_emit_by_name(ep->webrtc, "set-remote-description", answer, promise);
                gst_promise_interrupt(promise);
                gst_promise_unref(promise);
            }

            ep->app_state = PEER_CALL_STARTED;
        } else if (json_object_has_member(object, "ice")) {
            g_print("==========>Received ice\n");
            const gchar *candidate;
            gint sdpmlineindex;

            child = json_object_get_object_member(object, "ice");
            candidate = json_object_get_string_member(child, "candidate");
            sdpmlineindex = json_object_get_int_member(child, "sdpMLineIndex");

            /* Add ice candidate sent by remote peer */
            g_signal_emit_by_name(ep->webrtc, "add-ice-candidate", sdpmlineindex, candidate);
        } else {
            g_printerr("Ignoring unknown JSON message:\n%s\n", text);
        }
        g_object_unref(parser);
    }

out:
    g_free(text);
}

static void
on_server_connected(SoupSession *session, GAsyncResult *res, WebRTC *ep)
{
    printf("~~~~~~~~~~~~on_server_connected: peer_id %p  ,  %s\n", ep, ep->peer_id);
    GError *error = NULL;

    ep->ws_conn = soup_session_websocket_connect_finish(session, res, &error);
    if (error) {
        cleanup_and_quit_loop(error->message, ep, SERVER_CONNECTION_ERROR);
        g_error_free(error);
        return;
    }

    g_assert_nonnull(ep->ws_conn);

    ep->app_state = SERVER_CONNECTED;
    g_print("Connected to signalling server\n");

    g_signal_connect(ep->ws_conn, "closed", G_CALLBACK(on_server_closed), ep);
    g_signal_connect(ep->ws_conn, "message", G_CALLBACK(on_server_message), ep);

    /* Register with the server so it knows about us and can accept commands */
    register_with_server(ep);
}

/*
 * Connect to the signalling server. This is the entrypoint for everything else.
 */
static void
connect_to_websocket_server_async(WebRTC *ep)
{
    SoupLogger *logger;
    SoupMessage *message;
    SoupSession *session;
    const char *https_aliases[] = {"wss", NULL};

    session = soup_session_new_with_options(SOUP_SESSION_SSL_STRICT, strict_ssl, SOUP_SESSION_SSL_USE_SYSTEM_CA_FILE, TRUE,
                                            //SOUP_SESSION_SSL_CA_FILE, "/etc/ssl/certs/ca-bundle.crt",
                                            SOUP_SESSION_HTTPS_ALIASES,
                                            https_aliases,
                                            NULL);

    logger = soup_logger_new(SOUP_LOGGER_LOG_BODY, -1);
    soup_session_add_feature(session, SOUP_SESSION_FEATURE(logger));
    g_object_unref(logger);

    message = soup_message_new(SOUP_METHOD_GET, server_url);

    g_print("Connecting to server...\n");

    /* Once connected, we will register */
    printf("~~~~~~~~~~~~connect_to_websocket_server_async: peer_id %p   ,   %s\n", ep, ep->peer_id);
    soup_session_websocket_connect_async(session, message, NULL, NULL, NULL, (GAsyncReadyCallback)on_server_connected, ep);
    ep->app_state = SERVER_CONNECTING;
}

/////////////////////////////////////////////////////////////////////////////////
WebRTC *create_ep(gchar *peer)
{
    printf("~~~~~~~~~~~~~~~~~~peer: %s\n", peer);
    WebRTC *ep = new WebRTC(peer);
    connect_to_websocket_server_async(ep);
    return ep;
}


// #define TEST
#ifdef TEST
int main(int argc, char *argv[])
{
    gst_init(NULL, NULL);

    create_ep("1234");
    create_ep("1235");

    loop = g_main_loop_new(NULL, FALSE);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    terminate();

    return 0;
}
#endif