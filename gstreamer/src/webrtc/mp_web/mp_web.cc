#ifndef _WIN32
#include <unistd.h>
#endif

#include <gst/gst.h>
#include <gst/sdp/sdp.h>

#include <gst/webrtc/webrtc.h>

#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

#include <vector>
#include <list>
#include <string>
#include <mutex>
#include <algorithm>
#include "media.h"


#define RTP_CAPS_OPUS "application/x-rtp,media=audio,encoding-name=PCMA,payload="
#define RTP_CAPS_VP8 "application/x-rtp,media=video,encoding-name=VP8,payload="

// static std::string pattern[] = {"smpte", "ball", "snow", "blink", "circular", "pinwheel", "spokes"};
static std::string pattern[] = {"white", "green", "blue", "blink", "snow", "red", "circular", "pinwheel", "spokes"};
static std::string freq[] = {"500", "800", "2000", "10000", "30000", "40000", "50000"};

struct sink_link
{
    GstElement *joint;
    GstPad *request_pad;
    void *pipeline;
    gboolean video_probe_invoke_control;
    gboolean audio_probe_invoke_control;
    bool is_output;

    sink_link(GstPad *pad, GstElement *joint_element, void *pipe, bool output = true)
        : joint(joint_element)
        , request_pad(pad)
        , pipeline(pipe)
        , video_probe_invoke_control(FALSE)
        , audio_probe_invoke_control(FALSE)
        , is_output(output)
    {
    }
};
/*---------------------------------------------------------------------------------------------*/
class MultiPoints
{
 public:
    MultiPoints()
        : video_tee_(NULL)
        , audio_tee_(NULL)
        , video_selector_(NULL)
        , audio_selector_(NULL)
        , default_video_src_(NULL)
        , default_audio_src_(NULL)
        , main_pipeline_(NULL)
        , speaker_(NULL)
    {
        std::string launch = "videotestsrc ! timeoverlay valignment=3 halignment=4 time-mode=2 xpos=0 ypos=0 color=4278190080 font-desc=\"Sans 48\" draw-shadow=false draw-outline=true outline-color=4278190080 ! vp8enc ! rtpvp8pay ! rtpvp8depay name=default_video_src input-selector name=video-input-selector ! tee name=video-tee allow-not-linked=true  audiotestsrc ! alawenc ! rtppcmapay ! rtppcmadepay name=default_audio_src input-selector name=audio-input-selector ! tee name=audio-tee allow-not-linked=true";

        GError *error = NULL;

        main_pipeline_ = gst_parse_launch(launch.c_str(), &error);

        if (error) {
            g_printerr("Failed to parse launch: %s\n", error->message);
            g_error_free(error);
            return;
        }

        video_tee_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "video-tee");
        video_selector_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "video-input-selector");
        audio_tee_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "audio-tee");
        audio_selector_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "audio-input-selector");

        // GstPad *pad = gst_element_get_static_pad(audio_selector_, "src");
        // gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, on_monitor_data, NULL, NULL);
        // gst_object_unref(pad);

        default_video_src_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "default_video_src");
        // g_warn_if_fail(gst_element_link(default_video_src_, video_selector_));
        default_audio_src_ = gst_bin_get_by_name(GST_BIN(main_pipeline_), "default_audio_src");
        // g_warn_if_fail(gst_element_link(default_audio_src_, audio_selector_));

        gst_element_set_state(main_pipeline_, GST_STATE_PLAYING);
    }
    ~MultiPoints()
    {
        if (speaker_) {
            // video
            // g_warn_if_fail(gst_element_link(default_video_src_, video_selector_));
            GstPad *video_output_src_pad = gst_element_get_static_pad(default_video_src_, "src");
            GstPad *video_selector_sink_pad = gst_pad_get_peer(video_output_src_pad);
            g_object_set(G_OBJECT(video_selector_), "active-pad", video_selector_sink_pad, NULL);
            gst_object_unref(video_output_src_pad);
            // audio
            // g_warn_if_fail(gst_element_link(default_audio_src_, audio_selector_));
            GstPad *audio_output_src_pad = gst_element_get_static_pad(default_audio_src_, "src");
            GstPad *audio_selector_sink_pad = gst_pad_get_peer(audio_output_src_pad);
            g_object_set(G_OBJECT(audio_selector_), "active-pad", audio_selector_sink_pad, NULL);
            gst_object_unref(audio_output_src_pad);
            usleep(1 * 1000000);
        }
        for (WebRTC *ep : members_) {
            remove_stream_output_joint(ep->video_output_pipejoint());
            remove_stream_input_joint(ep->video_input_pipejoint());
            remove_stream_output_joint(ep->audio_output_pipejoint());
            remove_stream_input_joint(ep->audio_input_pipejoint());
            delete ep;
        }
        members_.clear();

        if (main_pipeline_) {
            gst_element_set_state(main_pipeline_, GST_STATE_NULL);
            g_object_unref(main_pipeline_);
            main_pipeline_ = NULL;
        }
        gst_object_unref(video_tee_);
        gst_object_unref(video_selector_);
        gst_object_unref(default_video_src_);
        gst_object_unref(audio_tee_);
        gst_object_unref(audio_selector_);
        gst_object_unref(default_audio_src_);
    }
    void generate_webrtc_enpoint(int num)
    {
        int id = 1234;
        for (int i = 0; i < num; ++i) {
            WebRTC *ep = create_ep((gchar *)std::to_string(id++).c_str());
            members_.push_back(ep);
        }
    }
    void join_room_all()
    {
        for (WebRTC *ep : members_) {
            add_member(ep);
        }
    }

    void add_member(WebRTC *ep)
    {
        GstElement *video_downstream_joint = ep->video_input_pipejoint();
        link_stream_input_joint(video_downstream_joint);
        GstElement *video_upstream_joint = ep->video_output_pipejoint();
        link_stream_output_joint(video_upstream_joint);
        GstElement *audio_downstream_joint = ep->audio_input_pipejoint();
        link_stream_input_joint(audio_downstream_joint);
        GstElement *audio_upstream_joint = ep->audio_output_pipejoint();
        link_stream_output_joint(audio_upstream_joint);

        if (speaker_ == NULL) {
            speaker_ = ep;
        }
    }
    void remove_member(WebRTC *ep)
    {
        GstElement *upstream_joint = ep->video_output_pipejoint();
        remove_stream_output_joint(upstream_joint);
        auto it = std::find(members_.begin(), members_.end(), ep);
        if (it != members_.end()) {
            members_.erase(it);
            printf("remove audience successfully!\n");
        }

        GstElement *downstream_joint = ep->video_input_pipejoint();
        remove_stream_input_joint(downstream_joint);
    }

    void link_stream_output_joint(GstElement *upstream_joint)
    {
        g_assert_nonnull(upstream_joint);
        std::lock_guard<std::mutex> lck(tee_mutex_);

        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(upstream_joint), "media-type");
        g_assert_nonnull(media_type);
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

            tee_sinks_.push_back(info);
        } else if (g_str_equal(media_type, "audio")) {
            GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(audio_tee_), "src_%u");
            GstPad *pad = gst_element_request_pad(audio_tee_, templ, NULL, NULL);
            sink_link *info = new sink_link(pad, upstream_joint, this);

            g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), upstream_joint));
            gst_element_sync_state_with_parent(upstream_joint);

            GstPad *sinkpad = gst_element_get_static_pad(upstream_joint, "sink");
            GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
            g_warn_if_fail(ret == GST_PAD_LINK_OK);
            gst_object_unref(sinkpad);

            tee_sinks_.push_back(info);
            printf("~~~~~~~~~tee ---> audio\n");
        }
    }

    void remove_stream_output_joint(GstElement *upstream_joint)
    {
        std::lock_guard<std::mutex> lck(tee_mutex_);


        auto it = tee_sinks_.begin();
        for (; it != tee_sinks_.end(); ++it) {
            if ((*it)->joint == upstream_joint) {
                break;
            }
        }
        if (it == tee_sinks_.end()) {
            g_warn_if_reached();
            // TODO(yuanjunjie) notify application
            return;
        }


        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(upstream_joint), "media-type");
        if (g_str_equal(media_type, "video")) {
            (*it)->video_probe_invoke_control = TRUE;
            gst_pad_add_probe((*it)->request_pad, GST_PAD_PROBE_TYPE_IDLE, on_request_pad_remove_video_probe, *it, NULL);
        } else if (g_str_equal(media_type, "audio")) {
            (*it)->audio_probe_invoke_control = TRUE;
            gst_pad_add_probe((*it)->request_pad, GST_PAD_PROBE_TYPE_IDLE, on_request_pad_remove_audio_probe, *it, NULL);
        }
        tee_sinks_.erase(it);
    }

    void link_stream_input_joint(GstElement *downstream_joint)
    {
        g_assert_nonnull(downstream_joint);
        std::lock_guard<std::mutex> lck(selector_mutex_);

        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(downstream_joint), "media-type");
        g_assert_nonnull(media_type);

        if (g_str_equal(media_type, "video")) {
            GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(video_selector_), "sink_%u");
            GstPad *pad = gst_element_request_pad(video_selector_, templ, NULL, NULL);
            sink_link *info = new sink_link(pad, downstream_joint, this, false);

            g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), downstream_joint));
            gst_element_sync_state_with_parent(downstream_joint);

            GstPad *srcpad = gst_element_get_static_pad(downstream_joint, "src");
            GstPadLinkReturn ret = gst_pad_link(srcpad, pad);
            g_warn_if_fail(ret == GST_PAD_LINK_OK);
            gst_object_unref(srcpad);

            selector_sinks_.push_back(info);
        } else if (g_str_equal(media_type, "audio")) {
            GstPadTemplate *templ = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(audio_selector_), "sink_%u");
            GstPad *pad = gst_element_request_pad(audio_selector_, templ, NULL, NULL);
            // gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, on_monitor_data, NULL, NULL);

            sink_link *info = new sink_link(pad, downstream_joint, this, false);

            g_warn_if_fail(gst_bin_add(GST_BIN(main_pipeline_), downstream_joint));
            gst_element_sync_state_with_parent(downstream_joint);

            GstPad *srcpad = gst_element_get_static_pad(downstream_joint, "src");

            // GstPad *testpad = gst_element_get_static_pad(downstream_joint, "src");
            // gst_pad_add_probe(testpad, GST_PAD_PROBE_TYPE_BUFFER, on_monitor_data, NULL, NULL);
            // gst_object_unref(testpad);

            GstPadLinkReturn ret = gst_pad_link(srcpad, pad);
            g_warn_if_fail(ret == GST_PAD_LINK_OK);
            gst_object_unref(srcpad);

            selector_sinks_.push_back(info);
            printf("~~~~~~~~~audio ---> selector\n");
        }
    }
    void remove_stream_input_joint(GstElement *downstream_joint)
    {
        std::lock_guard<std::mutex> lck(selector_mutex_);

        auto it = selector_sinks_.begin();
        for (; it != selector_sinks_.end(); ++it) {
            if ((*it)->joint == downstream_joint) {
                break;
            }
        }
        if (it == selector_sinks_.end()) {
            g_warn_if_reached();
            // TODO(yuanjunjie) notify application
            return;
        }

        gchar *media_type = (gchar *)g_object_get_data(G_OBJECT(downstream_joint), "media-type");
        if (g_str_equal(media_type, "video")) {
            (*it)->video_probe_invoke_control = TRUE;
            gst_pad_add_probe((*it)->request_pad, GST_PAD_PROBE_TYPE_IDLE, on_request_pad_remove_video_probe, *it, NULL);
        } else if (g_str_equal(media_type, "audio")) {
            (*it)->audio_probe_invoke_control = TRUE;
            gst_pad_add_probe((*it)->request_pad, GST_PAD_PROBE_TYPE_IDLE, on_request_pad_remove_audio_probe, *it, NULL);
        }
        selector_sinks_.erase(it);
    }
    void set_speaker(WebRTC *ep)
    {
        // video
        GstElement *video_downstream_joint = ep->video_input_pipejoint();
        GstPad *video_output_src_pad = gst_element_get_static_pad(video_downstream_joint, "src");
        GstPad *video_selector_sink_pad = gst_pad_get_peer(video_output_src_pad);
        g_object_set(G_OBJECT(video_selector_), "active-pad", video_selector_sink_pad, NULL);
        gst_object_unref(video_output_src_pad);
        // audio
        GstElement *audio_downstream_joint = ep->audio_input_pipejoint();
        GstPad *audio_output_src_pad = gst_element_get_static_pad(audio_downstream_joint, "src");
        GstPad *audio_selector_sink_pad = gst_pad_get_peer(audio_output_src_pad);
        g_object_set(G_OBJECT(audio_selector_), "active-pad", audio_selector_sink_pad, NULL);
        gst_object_unref(audio_output_src_pad);

        speaker_ = ep;
    }
    // for test
    void change_speaker()
    {
        static int member = 0;
        int current = (member++) % members_.size();
        auto it = members_.begin();
        for (int i = 0; i < current; ++i) {
            ++it;
        }
        set_speaker(*it);
    }
    static GstPadProbeReturn
    on_request_pad_remove_video_probe(GstPad *pad, GstPadProbeInfo *probe_info, gpointer data);
    static GstPadProbeReturn
    on_request_pad_remove_audio_probe(GstPad *pad, GstPadProbeInfo *probe_info, gpointer data);

    static GstPadProbeReturn
    on_monitor_data(GstPad *pad, GstPadProbeInfo *info, gpointer data);

 private:
    GstElement *video_tee_;
    GstElement *audio_tee_;
    GstElement *video_selector_;
    GstElement *audio_selector_;
    GstElement *default_video_src_;
    GstElement *default_audio_src_;

    GstElement *main_pipeline_;
    std::list<WebRTC *> members_;
    WebRTC *speaker_;

    std::list<sink_link *> tee_sinks_;
    std::list<sink_link *> selector_sinks_;
    static std::mutex tee_mutex_;
    static std::mutex selector_mutex_;
};
std::mutex MultiPoints::tee_mutex_;
std::mutex MultiPoints::selector_mutex_;

GstPadProbeReturn MultiPoints::on_monitor_data(GstPad *pad,
                                               GstPadProbeInfo *info,
                                               gpointer data)
{

    printf("*");
    return GST_PAD_PROBE_OK;
}

GstPadProbeReturn MultiPoints::on_request_pad_remove_video_probe(GstPad *teepad, GstPadProbeInfo *probe_info, gpointer data)
{
    sink_link *info = static_cast<sink_link *>(data);
    if (!g_atomic_int_compare_and_exchange(&info->video_probe_invoke_control, TRUE, FALSE)) {
        return GST_PAD_PROBE_OK;
    }

    GstElement *joint = info->joint;
    MultiPoints *pipeline = static_cast<MultiPoints *>(info->pipeline);

    // remove pipeline dynamicaly
    if (info->is_output) {
        GstPad *sinkpad = gst_element_get_static_pad(joint, "sink");
        gst_pad_unlink(info->request_pad, sinkpad);
        gst_object_unref(sinkpad);
        gst_element_set_state(joint, GST_STATE_NULL);
        g_warn_if_fail(gst_bin_remove(GST_BIN(pipeline->main_pipeline_), joint));
        gst_element_release_request_pad(pipeline->video_tee_, info->request_pad);
        gst_object_unref(info->request_pad);
        printf("~~~~~~~remove video tee pad\n");
    } else {
        GstPad *srckpad = gst_element_get_static_pad(joint, "src");
        gst_pad_unlink(srckpad, info->request_pad);
        gst_object_unref(srckpad);
        gst_element_set_state(joint, GST_STATE_NULL);
        g_warn_if_fail(gst_bin_remove(GST_BIN(pipeline->main_pipeline_), joint));
        gst_element_release_request_pad(pipeline->video_selector_, info->request_pad);
        gst_object_unref(info->request_pad);
        printf("~~~~~~~remove video selector pad\n");
    }
    delete static_cast<sink_link *>(data);
    return GST_PAD_PROBE_REMOVE;
}
GstPadProbeReturn MultiPoints::on_request_pad_remove_audio_probe(GstPad *teepad, GstPadProbeInfo *probe_info, gpointer data)
{
    sink_link *info = static_cast<sink_link *>(data);
    if (!g_atomic_int_compare_and_exchange(&info->audio_probe_invoke_control, TRUE, FALSE)) {
        return GST_PAD_PROBE_OK;
    }

    GstElement *joint = info->joint;
    MultiPoints *pipeline = static_cast<MultiPoints *>(info->pipeline);

    // remove pipeline dynamicaly
    if (info->is_output) {
        GstPad *sinkpad = gst_element_get_static_pad(joint, "sink");
        gst_pad_unlink(info->request_pad, sinkpad);
        gst_object_unref(sinkpad);
        gst_element_set_state(joint, GST_STATE_NULL);
        g_warn_if_fail(gst_bin_remove(GST_BIN(pipeline->main_pipeline_), joint));
        gst_element_release_request_pad(pipeline->audio_tee_, info->request_pad);
        gst_object_unref(info->request_pad);
        printf("~~~~~~~remove audio tee pad\n");
    } else {
        GstPad *srckpad = gst_element_get_static_pad(joint, "src");
        gst_pad_unlink(srckpad, info->request_pad);
        gst_object_unref(srckpad);
        gst_element_set_state(joint, GST_STATE_NULL);
        g_warn_if_fail(gst_bin_remove(GST_BIN(pipeline->main_pipeline_), joint));
        gst_element_release_request_pad(pipeline->audio_selector_, info->request_pad);
        gst_object_unref(info->request_pad);
        printf("~~~~~~~remove audio selector pad\n");
    }
    delete static_cast<sink_link *>(data);
    return GST_PAD_PROBE_REMOVE;
}
/*---------------------------------------------------------------------------------------------*/
static GMainLoop *main_loop = NULL;
static GThread *main_thread = NULL;
static GMainContext *main_context = NULL;
MultiPoints *room = NULL;
GSource *test_source = NULL;

static gboolean test(gpointer data)
{
    MultiPoints *room = static_cast<MultiPoints *>(data);
    printf("\nchange speaker\n");
    room->change_speaker();
    return TRUE;
}
void start_test()
{
    room->join_room_all();
    usleep(1 * 1000000);

    test_source = g_timeout_source_new_seconds(3);
    g_source_set_callback(test_source, (GSourceFunc)test, room, NULL);
    guint id = g_source_attach(test_source, main_context);
}
void MainLoop()
{
    main_context = g_main_context_new();
    main_loop = g_main_loop_new(main_context, FALSE);
    g_main_context_push_thread_default(main_context);

    room = new MultiPoints();
    room->generate_webrtc_enpoint(2);

    g_main_loop_run(main_loop);
    printf("main loop quit\n");

    g_source_destroy(test_source);
    test_source = NULL;
    delete room;
    room = NULL;

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


    char ret = 0;
    do {
        ret = getchar();
        if (ret == 't')
            start_test();
    } while (ret != 'q');

    g_main_loop_quit(main_loop);
    g_thread_join(main_thread);
    return 0;
}