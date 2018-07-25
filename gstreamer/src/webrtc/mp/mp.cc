#include <unistd.h>
#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/webrtc/webrtc.h>

#include <vector>
#include <list>
#include <string>
#include <mutex>
#include <algorithm>
#include "pipejoint.h"


#define RTP_CAPS_OPUS "application/x-rtp,media=audio,encoding-name=OPUS,payload="
#define RTP_CAPS_VP8 "application/x-rtp,media=video,encoding-name=VP8,payload="

// static std::string pattern[] = {"smpte", "ball", "snow", "blink", "circular", "pinwheel", "spokes"};
static std::string pattern[] = {"white", "red", "green", "blue", "smpte", "ball", "snow", "blink", "circular", "pinwheel", "spokes"};
struct sink_link
{
    GstElement *upstream_joint;
    GstPad *tee_pad;
    void *pipeline;
    gboolean video_probe_invoke_control;
    gboolean audio_probe_invoke_control;

    sink_link(GstPad *pad, GstElement *joint, void *pipe)
        : upstream_joint(joint)
        , tee_pad(pad)
        , pipeline(pipe)
        , video_probe_invoke_control(FALSE)
        , audio_probe_invoke_control(FALSE)
    {
    }
};
class WebRTC
{
 public:
    WebRTC(int i)
        : pattern_(i) {}
    ~WebRTC()
    {
        if (pipeline_) {
            gst_element_set_state(pipeline_, GST_STATE_NULL);
            g_object_unref(pipeline_);
            pipeline_ = NULL;
        }
        gst_object_unref(remote_webrtc_);
        gst_object_unref(local_webrtc_);

        destroy_pipe_joint(&video_joint_);
    }

    void Initialize()
    {
        std::string launch = "webrtcbin name=remote videotestsrc pattern=" + pattern[pattern_ + 1] + " ! queue ! vp8enc deadline=1 ! rtpvp8pay ! queue ! " + RTP_CAPS_VP8 + "96 ! remote. webrtcbin name=local queue name=video_joint ! rtpvp8pay !" + RTP_CAPS_VP8 + " 96 ! local.";


        printf("%s\n", launch.c_str());

        GError *error = NULL;

        pipeline_ = gst_parse_launch(launch.c_str(), &error);

        if (error) {
            g_printerr("Failed to parse launch: %s\n", error->message);
            g_error_free(error);
            return;
        }

        remote_webrtc_ = gst_bin_get_by_name(GST_BIN(pipeline_), "remote");
        g_assert_nonnull(remote_webrtc_);
        g_signal_connect(remote_webrtc_, "on-negotiation-needed", G_CALLBACK(WebRTC::on_negotiation_needed), this);
        g_signal_connect(remote_webrtc_, "pad-added", G_CALLBACK(WebRTC::on_webrtc_pad_added), this);

        local_webrtc_ = gst_bin_get_by_name(GST_BIN(pipeline_), "local");
        g_assert_nonnull(local_webrtc_);
        g_signal_connect(local_webrtc_, "pad-added", G_CALLBACK(WebRTC::on_webrtc_pad_added), this);

        g_signal_connect(remote_webrtc_, "on-ice-candidate", G_CALLBACK(WebRTC::on_ice_candidate), local_webrtc_);
        g_signal_connect(local_webrtc_, "on-ice-candidate", G_CALLBACK(WebRTC::on_ice_candidate), remote_webrtc_);

        static int session_count = 1;
        static std::string media_type = "video";
        std::string pipejoint_name = std::string("webrtc_video_endpoint_joint_") +
                                     std::to_string(session_count);
        video_joint_ = make_pipe_joint(media_type, pipejoint_name);
        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), video_joint_.downstream_joint));
        GstElement *video_joint = gst_bin_get_by_name_recurse_up(GST_BIN(pipeline_), "video_joint");
        g_warn_if_fail(gst_element_link(video_joint_.downstream_joint, video_joint));

        gst_element_set_state(GST_ELEMENT(pipeline_), GST_STATE_PLAYING);
        session_count++;
    }

    GstElement *get_video_pipejoint()
    {
        return video_joint_.upstream_joint;
    }

    static void on_negotiation_needed(GstElement *element, gpointer user_data);
    static void on_offer_created(GstPromise *promise, gpointer user_data);
    static void on_answer_created(GstPromise *promise, gpointer user_data);
    static void on_ice_candidate(GstElement *webrtc, guint mlineindex, gchar *candidate, GstElement *other);
    static void on_webrtc_pad_added(GstElement *webrtc_element, GstPad *new_pad, gpointer user_data);

 private:
    int pattern_;
    GstElement *pipeline_;
    GstElement *remote_webrtc_;
    GstElement *local_webrtc_;
    PipeJoint video_joint_;
};

void WebRTC::on_negotiation_needed(GstElement *element, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstPromise *promise;

    promise = gst_promise_new_with_change_func(WebRTC::on_offer_created, user_data, NULL);
    g_signal_emit_by_name(webrtc->remote_webrtc_, "create-offer", NULL, promise);
}
void WebRTC::on_offer_created(GstPromise *promise, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstWebRTCSessionDescription *sdp = NULL;
    const GstStructure *reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, "offer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &sdp, NULL);
    gst_promise_unref(promise);

    gchar *desc = gst_sdp_message_as_text(sdp->sdp);
    // g_print("-----\n%d Created offer:\n%s\n", webrtc->pattern_, desc);
    g_free(desc);

    gst_sdp_media_add_attribute((GstSDPMedia *)&g_array_index(sdp->sdp->medias, GstSDPMedia, 0),
                                "fmtp",
                                "96 profile-level-id=42e01f");
    g_signal_emit_by_name(webrtc->remote_webrtc_, "set-local-description", sdp, NULL);
    g_signal_emit_by_name(webrtc->local_webrtc_, "set-remote-description", sdp, NULL);


    promise = gst_promise_new_with_change_func(WebRTC::on_answer_created, user_data, NULL);
    g_signal_emit_by_name(webrtc->local_webrtc_, "create-answer", NULL, promise);

    gst_webrtc_session_description_free(sdp);
}
void WebRTC::on_answer_created(GstPromise *promise, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);
    GstWebRTCSessionDescription *answer = NULL;
    const GstStructure *reply;

    g_assert(gst_promise_wait(promise) == GST_PROMISE_RESULT_REPLIED);
    reply = gst_promise_get_reply(promise);
    gst_structure_get(reply, "answer", GST_TYPE_WEBRTC_SESSION_DESCRIPTION, &answer, NULL);
    gst_promise_unref(promise);

    gchar *desc = gst_sdp_message_as_text(answer->sdp);
    // g_print("%d Created answer:\n%s\n", webrtc->pattern_, desc);
    g_free(desc);

    g_signal_emit_by_name(webrtc->remote_webrtc_, "set-remote-description", answer, NULL);
    promise = gst_promise_new();
    g_signal_emit_by_name(webrtc->local_webrtc_, "set-local-description", answer, NULL);
    gst_promise_interrupt(promise);
    gst_promise_unref(promise);

    gst_webrtc_session_description_free(answer);
}
void WebRTC::on_ice_candidate(GstElement *webrtc, guint mlineindex, gchar *candidate, GstElement *other)
{
    g_signal_emit_by_name(other, "add-ice-candidate", mlineindex, candidate);
}
void WebRTC::on_webrtc_pad_added(GstElement *webrtc_element, GstPad *new_pad, gpointer user_data)
{
    WebRTC *webrtc = static_cast<WebRTC *>(user_data);

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
    if (g_strcmp0(encoding_name, "VP8") == 0) {
        if (webrtc_element == webrtc->remote_webrtc_) {
            out = gst_parse_bin_from_description(
                "rtpvp8depay ! vp8dec ! videoconvert ! queue ! xvimagesink",
                TRUE,
                NULL);
            g_print("on_webrtc_pad_added %d\n", webrtc->pattern_);
        } else {
            out = gst_parse_bin_from_description(
                // "rtpvp8depay ! vp8dec ! videoconvert ! queue ! xvimagesink",
                "tee name=local_tee ! queue ! fakesink",
                TRUE,
                NULL);
        }
    } else if (g_strcmp0(encoding_name, "OPUS") == 0) {
        out = gst_parse_bin_from_description(
            "rtpopusdepay ! opusdec ! "
            "audioconvert ! audioresample ! audiorate ! queue ! autovideosink",
            TRUE,
            NULL);
    } else {
        g_critical("Unknown encoding name %s", encoding_name);
        g_assert_not_reached();
    }
    gst_bin_add(GST_BIN(webrtc->pipeline_), out);
    gst_element_sync_state_with_parent(out);
    sink = (GstPad *)out->sinkpads->data;

    gst_pad_link(new_pad, sink);

    gst_caps_unref(caps);
}

/*---------------------------------------------------------------------------------------------*/
class MultiPoints
{
 public:
    MultiPoints()
    {
        // video_tee_ = gst_element_factory_make("tee", "video-tee");
        // audio_tee_ = gst_element_factory_make("tee", "audio-tee");
        // g_object_set(video_tee_, "allow-not-linked", TRUE, NULL);
        // g_object_set(audio_tee_, "allow-not-linked", TRUE, NULL);
        // video_input_selector_ = gst_element_factory_make("input-selector", "video-input-selector");
        // g_object_set(G_OBJECT(video_input_selector_), "sync-streams", false, NULL);
        // audio_input_selector_ = gst_element_factory_make("input-selector", "audio-input-selector");

        // main_pipeline_ = gst_pipeline_new("MultiPoints");
        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_tee_));
        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_tee_));
        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_input_selector_));
        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_input_selector_));

        // g_warn_if_fail(gst_element_link(video_input_selector_, video_tee_));
        // g_warn_if_fail(gst_element_link(audio_input_selector_, audio_tee_));

        // GstElement *src = gst_element_factory_make("videotestsrc", NULL);
        // GstElement *enc = gst_element_factory_make("vp8enc", NULL);
        // g_object_set(G_OBJECT(enc), "deadline", 1, NULL);


        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), src));
        // g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), enc));
        // g_warn_if_fail(gst_element_link_many(src, enc, video_input_selector_, NULL));
        std::string launch = "videotestsrc pattern=white ! timeoverlay valignment=3 halignment=4 time-mode=2 xpos=0 ypos=0 color=4278190080 font-desc=\"Sans 48\" draw-shadow=false draw-outline=true outline-color=4278190080 ! vp8enc ! tee name=video-tee allow-not-linked=true";

        GError *error = NULL;

        main_pipeline_ = gst_parse_launch(launch.c_str(), &error);

        if (error) {
            g_printerr("Failed to parse launch: %s\n", error->message);
            g_error_free(error);
            return;
        }

        video_tee_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "video-tee");
        video_input_selector_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "video-input-selector");

        // GstPad *pad = gst_element_get_static_pad(video_input_selector_, "src");
        // gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, on_monitor_data, NULL, NULL);
        // gst_object_unref(pad);

        gst_element_set_state(main_pipeline_, GST_STATE_PLAYING);
    }
    ~MultiPoints()
    {
        if (main_pipeline_) {
            gst_element_set_state(main_pipeline_, GST_STATE_NULL);
            g_object_unref(main_pipeline_);
            main_pipeline_ = NULL;
        }
        for (WebRTC *ep : endpoints_) {
            remove_pipe_joint(ep->get_video_pipejoint());
            usleep(1 * 100000);
            delete ep;
        }
        endpoints_.clear();
    }
    void generate_webrtc_enpoint(int num)
    {
        for (int i = 0; i < num; ++i) {
            WebRTC *ep = new WebRTC(i);
            ep->Initialize();
            add_audience(ep);
            // usleep(1 * 1000000);
        }
    }
    void add_audience(WebRTC *ep)
    {
        add_pipe_joint(ep->get_video_pipejoint());
        endpoints_.push_back(ep);
    }
    void remove_audience(WebRTC *ep)
    {
        remove_pipe_joint(ep->get_video_pipejoint());
        auto it = std::find(endpoints_.begin(), endpoints_.end(), ep);
        if (it != endpoints_.end()) {
            endpoints_.erase(it);
        }
    }
    void add_pipe_joint(GstElement *upstream_joint)
    {
        joint_mutex_.lock();
        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(upstream_joint), "media-type");
        if (g_str_equal(media_type, "video")) {
            GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(video_tee_), "src_%u");
            GstPad *pad = gst_element_request_pad(video_tee_, templ, NULL, NULL);
            sink_link *info = new sink_link(pad, upstream_joint, this);

            g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), upstream_joint));
            gst_element_sync_state_with_parent(upstream_joint);

            GstPad *sinkpad = gst_element_get_static_pad(upstream_joint, "sink");
            GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
            g_warn_if_fail(ret == GST_PAD_LINK_OK);
            gst_object_unref(sinkpad);

            sinks_.push_back(info);
        } else if (g_str_equal(media_type, "audio")) {
            // GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(audio_tee_), "src_%u");
            // GstPad *pad = gst_element_request_pad(audio_tee_, templ, NULL, NULL);
            // sink_link *info = new sink_link(pad, upstream_joint, this);

            // g_warn_if_fail(gst_bin_add(GST_BIN(pipeline()), upstream_joint));
            // gst_element_sync_state_with_parent(upstream_joint);

            // GstPad *sinkpad = gst_element_get_static_pad(upstream_joint, "sink");
            // GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
            // g_warn_if_fail(ret == GST_PAD_LINK_OK);
            // gst_object_unref(sinkpad);

            // sinks_.push_back(info);
        }
        joint_mutex_.unlock();
    }

    void remove_pipe_joint(GstElement *upstream_joint)
    {
        joint_mutex_.lock();
        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(upstream_joint), "media-type");
        if (g_str_equal(media_type, "video")) {
            auto it = sinks_.begin();
            for (; it != sinks_.end(); ++it) {
                if ((*it)->upstream_joint == upstream_joint) {
                    break;
                }
            }
            if (it == sinks_.end()) {
                g_warn_if_reached();
                // TODO(yuanjunjie) notify application
                return;
            }
            (*it)->video_probe_invoke_control = TRUE;
            gst_pad_add_probe((*it)->tee_pad, GST_PAD_PROBE_TYPE_IDLE, on_tee_pad_remove_video_probe, *it, NULL);
            sinks_.erase(it);
            GST_DEBUG("[livestream] remove video joint completed");
        } else if (g_str_equal(media_type, "audio")) {
            // auto it = sinks_.begin();
            // for (; it != sinks_.end(); ++it) {
            //     if ((*it)->upstream_joint == upstream_joint) {
            //         break;
            //     }
            // }
            // if (it == sinks_.end()) {
            //     g_warn_if_reached();
            //     // TODO(yuanjunjie) notify application
            //     return;
            // }
            // (*it)->audio_probe_invoke_control = TRUE;
            // gst_pad_add_probe((*it)->tee_pad, GST_PAD_PROBE_TYPE_IDLE, on_tee_pad_remove_audio_probe, *it, NULL);
            // sinks_.erase(it);
            // GST_DEBUG("[livestream] remove audio joint completed");
        }
        joint_mutex_.unlock();
    }

    static GstPadProbeReturn
    on_tee_pad_remove_video_probe(GstPad *pad,
                                  GstPadProbeInfo *probe_info,
                                  gpointer data);
    static GstPadProbeReturn
    on_monitor_data(GstPad *pad,
                    GstPadProbeInfo *info,
                    gpointer rtspclient);

 private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_input_selector_;
    GstElement *audio_input_selector_;

    GstElement *main_pipeline_;
    std::list<WebRTC *> endpoints_;
    std::list<sink_link *> sinks_;
    static std::mutex joint_mutex_;
};
std::mutex MultiPoints::joint_mutex_;

GstPadProbeReturn MultiPoints::on_monitor_data(GstPad *pad,
                                               GstPadProbeInfo *info,
                                               gpointer rtspclient)
{

    printf(".");
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn MultiPoints::on_tee_pad_remove_video_probe(GstPad *teepad, GstPadProbeInfo *probe_info, gpointer data)
{
    sink_link *info = static_cast<sink_link *>(data);
    if (!g_atomic_int_compare_and_exchange(&info->video_probe_invoke_control, TRUE, FALSE)) {
        return GST_PAD_PROBE_OK;
    }

    GstElement *upstream_joint = info->upstream_joint;
    MultiPoints *pipeline = static_cast<MultiPoints *>(info->pipeline);

    // remove pipeline dynamicaly
    GstPad *sinkpad = gst_element_get_static_pad(upstream_joint, "sink");
    gst_pad_unlink(info->tee_pad, sinkpad);
    gst_object_unref(sinkpad);
    gst_element_set_state(upstream_joint, GST_STATE_NULL);
    g_warn_if_fail(gst_bin_remove(GST_BIN(pipeline->main_pipeline_), upstream_joint));

    gst_element_release_request_pad(pipeline->video_tee_, info->tee_pad);
    gst_object_unref(info->tee_pad);
    delete static_cast<sink_link *>(data);
    return GST_PAD_PROBE_REMOVE;
}
/*---------------------------------------------------------------------------------------------*/
static GMainLoop *main_loop = NULL;
static GThread *main_thread = NULL;
static GMainContext *main_context = NULL;
void MainLoop()
{
    main_context = g_main_context_new();
    main_loop = g_main_loop_new(main_context, FALSE);
    g_main_context_push_thread_default(main_context);

    MultiPoints *room = new MultiPoints();
    room->generate_webrtc_enpoint(3);

    g_main_loop_run(main_loop);

    delete room;

    g_main_context_pop_thread_default(main_context);

    g_main_loop_unref(main_loop);

    gst_deinit();
    main_loop = NULL;
    main_context = NULL;
}

int main(int argc, char *argv[])
{
    gst_init(NULL, NULL);
    main_thread = g_thread_new("mp_main_loop",
                               (GThreadFunc)MainLoop,
                               NULL);
    getchar();
    // while (getchar() != 'q')
    //     printf("press q to exit!\n");
    g_main_loop_quit(main_loop);
    return 0;
}