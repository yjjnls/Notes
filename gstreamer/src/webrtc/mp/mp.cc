#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/webrtc/webrtc.h>

#include <vector>
#include <string>


#define RTP_CAPS_OPUS "application/x-rtp,media=audio,encoding-name=OPUS,payload="
#define RTP_CAPS_VP8 "application/x-rtp,media=video,encoding-name=VP8,payload="

static std::string pattern[] = {"smpte", "ball", "snow", "blink", "circular", "pinwheel", "spokes"};

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
    }

    void Initialize()
    {
        std::string launch = "webrtcbin name=remote videotestsrc pattern=" + pattern[pattern_ + 1] + " ! queue ! vp8enc deadline=1 ! rtpvp8pay ! queue ! " + RTP_CAPS_VP8 + "96 ! remote. webrtcbin name=local videotestsrc pattern=smpte ! queue ! vp8enc deadline=1 ! rtpvp8pay ! queue ! " + RTP_CAPS_VP8 + "96 ! local.";

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

        gst_element_set_state(GST_ELEMENT(pipeline_), GST_STATE_PLAYING);
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
    g_print("Created offer:\n%s\n", desc);
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
    g_print("Created answer:\n%s\n", desc);
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
    g_print("on_webrtc_pad_added\n");
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
    // GST_ERROR_OBJECT(new_pad, "caps %" GST_PTR_FORMAT, caps);
    // g_assert(gst_caps_is_fixed(caps));
    s = gst_caps_get_structure(caps, 0);
    encoding_name = gst_structure_get_string(s, "encoding-name");
    if (g_strcmp0(encoding_name, "VP8") == 0) {
        // if (webrtc_element == webrtc->remote_webrtc_) {
        out = gst_parse_bin_from_description(
            "rtpvp8depay ! vp8dec ! videoconvert ! queue ! xvimagesink sync=false",
            TRUE,
            NULL);
        // } else {
        //     out = gst_parse_bin_from_description(
        //         "rtpvp8depay ! vp8dec ! videoconvert ! queue ! fakesink sync=false",
        //         TRUE,
        //         NULL);
        // }
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
        video_tee_ = gst_element_factory_make("tee", "video-tee");
        audio_tee_ = gst_element_factory_make("tee", "audio-tee");
        g_object_set(video_tee_, "allow-not-linked", TRUE, NULL);
        g_object_set(audio_tee_, "allow-not-linked", TRUE, NULL);
        video_input_selector_ = gst_element_factory_make("input-selector", "video-input-selector");
        audio_input_selector_ = gst_element_factory_make("input-selector", "audio-input-selector");

        main_pipeline_ = gst_pipeline_new("MultiPoints");
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_input_selector_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_input_selector_));

        g_warn_if_fail(gst_element_link(video_input_selector_, video_tee_));
        g_warn_if_fail(gst_element_link(audio_input_selector_, audio_tee_));
    }
    ~MultiPoints()
    {
        if (main_pipeline_) {
            gst_element_set_state(main_pipeline_, GST_STATE_NULL);
            g_object_unref(main_pipeline_);
            main_pipeline_ = NULL;
        }
        for (WebRTC *ep : endpoints_) {
            delete ep;
        }
        endpoints_.clear();
    }
    void generate_webrtc_enpoint(int num)
    {
        for (int i = 0; i < num; ++i) {
            WebRTC *ep = new WebRTC(i);
            ep->Initialize();
            endpoints_.push_back(ep);
            // usleep(3 * 1000000);
        }
    }

 private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_input_selector_;
    GstElement *audio_input_selector_;

    GstElement *main_pipeline_;
    std::vector<WebRTC *> endpoints_;
};
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
    room->generate_webrtc_enpoint(2);

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