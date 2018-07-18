#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>


class multipoint
{
public:
    multipoint()
    {
        video_tee_ = gst_element_factory_make("tee", "video-tee");
        audio_tee_ = gst_element_factory_make("tee", "audio-tee");
        g_object_set(video_tee, "allow-not-linked", TRUE, NULL);
        g_object_set(audio_tee, "allow-not-linked", TRUE, NULL);
        video_input_selector_ = gst_element_factory_make("input-selector", "video-input-selector");
        audio_input_selector_ = gst_element_factory_make("input-selector", "audio-input-selector");

        pipeline_ = gst_pipeline_new("multipoint");
        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), video_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), audio_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), video_input_selector_));
        g_warn_if_fail(gst_bin_add(GST_BIN(pipeline_), audio_input_selector_));

        g_warn_if_fail(gst_element_link(video_input_selector_, video_tee_));
        g_warn_if_fail(gst_element_link(audio_input_selector_, audio_tee_));
    }
    ~multipoint()
    {
        if (pipeline_)
        {
            gst_element_set_state(pipeline_, GST_STATE_NULL);
            g_object_unref(pipeline_);
            pipeline_ = NULL;
        }
    }

private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_input_selector_;
    GstElement *audio_input_selector_;

    GstElement *pipeline_;
};
int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    pipe1 =
        gst_parse_launch(
            "webrtcbin name=smpte webrtcbin name=ball "
            "videotestsrc pattern=smpte ! queue ! vp8enc ! rtpvp8pay ! queue ! "
            "application/x-rtp,media=video,payload=96,encoding-name=VP8 ! smpte.sink_0 "
            "audiotestsrc ! opusenc ! rtpopuspay ! queue ! "
            "application/x-rtp,media=audio,payload=97,encoding-name=OPUS ! smpte.sink_1 "
            "videotestsrc pattern=ball ! queue ! vp8enc ! rtpvp8pay ! queue ! "
            "application/x-rtp,media=video,payload=96,encoding-name=VP8 ! ball.sink_1 "
            "audiotestsrc wave=saw ! opusenc ! rtpopuspay ! queue ! "
            "application/x-rtp,media=audio,payload=97,encoding-name=OPUS ! ball.sink_0 ",
            NULL);
    bus1 = gst_pipeline_get_bus(GST_PIPELINE(pipe1));
    gst_bus_add_watch(bus1, (GstBusFunc)_bus_watch, pipe1);

    webrtc1 = gst_bin_get_by_name(GST_BIN(pipe1), "smpte");
    g_signal_connect(webrtc1, "on-negotiation-needed",
                     G_CALLBACK(_on_negotiation_needed), NULL);
    g_signal_connect(webrtc1, "pad-added", G_CALLBACK(_webrtc_pad_added),
                     pipe1);
    webrtc2 = gst_bin_get_by_name(GST_BIN(pipe1), "ball");
    g_signal_connect(webrtc2, "pad-added", G_CALLBACK(_webrtc_pad_added),
                     pipe1);
    g_signal_connect(webrtc1, "on-ice-candidate",
                     G_CALLBACK(_on_ice_candidate), webrtc2);
    g_signal_connect(webrtc2, "on-ice-candidate",
                     G_CALLBACK(_on_ice_candidate), webrtc1);

    g_print("Starting pipeline\n");
    gst_element_set_state(GST_ELEMENT(pipe1), GST_STATE_PLAYING);

    g_main_loop_run(loop);

    gst_element_set_state(GST_ELEMENT(pipe1), GST_STATE_NULL);
    g_print("Pipeline stopped\n");

    gst_object_unref(webrtc1);
    gst_object_unref(webrtc2);
    gst_bus_remove_watch(bus1);
    gst_object_unref(bus1);
    gst_object_unref(pipe1);

    gst_deinit();

    return 0;
}