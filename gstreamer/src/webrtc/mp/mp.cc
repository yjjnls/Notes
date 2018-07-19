#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/remote_webrtc/remote_webrtc.h>

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

        main_pipeline_ = gst_pipeline_new("multipoint");
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_tee_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), video_input_selector_));
        g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), audio_input_selector_));

        g_warn_if_fail(gst_element_link(video_input_selector_, video_tee_));
        g_warn_if_fail(gst_element_link(audio_input_selector_, audio_tee_));
    }
    ~multipoint()
    {
        if (main_pipeline_) {
            gst_element_set_state(main_pipeline_, GST_STATE_NULL);
            g_object_unref(main_pipeline_);
            main_pipeline_ = NULL;
        }
    }
    void generate_webrtc_enpoint(int num)
    {
        std::string pattern[] = {"white", "red", "yellow", "blue"};
        for (int i = 0; i < num; ++i) {
            std::string launch = "webrtcbin name=remote videotestsrc pattern=" + pattern[i] + " ! videoconvert ! queue ! vp8enc deadline=1 ! rtpvp8pay ! queue ! " + RTP_CAPS_VP8 + "96 ! sendrecv. audiotestsrc wave=red-noise ! audioconvert ! audioresample ! queue ! alawenc ! rtppcmapay ! queue ! " + RTP_CAPS_OPUS + "8 ! sendrecv. ";
            
            printf("%s\n", launch.c_str());

            GError *error = NULL;

            GstElement *remote_pipe = gst_parse_launch(launch.c_str(), &error);

            if (error) {
                g_printerr("Failed to parse launch: %s\n", error->message);
                g_error_free(error);
                continue;
            }


            GstElement *remote_webrtc = gst_bin_get_by_name(GST_BIN(remote_pipe), to_string(i));
            g_assert_nonnull(remote_webrtc);

            /* This is the gstwebrtc entry point where we create the offer and so on. It
            * will be called when the pipeline goes to PLAYING. */
            g_signal_connect(remote_webrtc, "on-negotiation-needed", G_CALLBACK(on_negotiation_needed), NULL);
            /* We need to transmit this ICE candidate to the browser via the websockets
            * signalling server. Incoming ice candidates from the browser need to be
            * added by us too, see on_server_message() */
            g_signal_connect(remote_webrtc, "on-ice-candidate", G_CALLBACK(send_ice_candidate_message), NULL);
            /* Incoming streams will be exposed via this signal */
            g_signal_connect(remote_webrtc, "pad-added", G_CALLBACK(on_incoming_stream), remote_pipe);
            /* Lifetime is the same as the pipeline itself */
            gst_object_unref(remote_webrtc);

            gst_element_set_state(GST_ELEMENT(remote_pipe), GST_STATE_PLAYING);
            remote_pipelines_.push_back(remote_pipe);
        }

    }

 private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_input_selector_;
    GstElement *audio_input_selector_;

    GstElement *main_pipeline_;
    std::vector<GstElement *> remote_pipelines_;
    std::vector<GstElement *> local_pipelines_;
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