#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/webrtc/webrtc.h>

#include <vector>
#include <string>


#define RTP_CAPS_OPUS "application/x-rtp,media=audio,encoding-name=PCMA,payload="
#define RTP_CAPS_VP8 "application/x-rtp,media=video,encoding-name=VP8,payload="

class multipoint
{
 public:
    multipoint()
    {
        video_tee_ = gst_element_factory_make("tee", "video-tee");
        audio_tee_ = gst_element_factory_make("tee", "audio-tee");
        g_object_set(video_tee_, "allow-not-linked", TRUE, NULL);
        g_object_set(audio_tee_, "allow-not-linked", TRUE, NULL);
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
        if (pipeline_) {
            gst_element_set_state(pipeline_, GST_STATE_NULL);
            g_object_unref(pipeline_);
            pipeline_ = NULL;
        }
    }
    void generate_webrtc_enpoint(int num)
    {
        std::string pattern[] = {"white", "red", "yellow", "blue"};
        for (int i = 0; i < num; ++i) {
            std::string launch = "webrtcbin name=" + std::to_string(i) + " videotestsrc pattern=" + pattern[i] + " ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay ! queue ! " + RTP_CAPS_VP8 + "96 ! sendrecv. audiotestsrc wave=red-noise ! audioconvert ! audioresample ! queue ! alawenc ! rtppcmapay ! queue ! " + RTP_CAPS_OPUS + "8 ! sendrecv. ";
            printf("%s\n", launch.c_str());
        }

        // gst_element_set_state(GST_ELEMENT(pipe1), GST_STATE_PLAYING);
    }

 private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_input_selector_;
    GstElement *audio_input_selector_;

    GstElement *pipeline_;
    std::vector<GstElement *> endpoints_;
};

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    multipoint *room = new multipoint();
    room->generate_webrtc_enpoint(2);
    // g_main_loop_run(loop);

    gst_deinit();

    return 0;
}