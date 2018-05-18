#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <gst/gst.h>
#include <gst/video/video.h>

tesseract::TessBaseAPI *api = NULL;
GstElement *pipeline = NULL;
void on_rtspsrc_pad_added(GstElement *src,
                          GstPad *src_pad,
                          gpointer depay)
{
    GstCaps *caps = gst_pad_query_caps(src_pad, NULL);
    GstStructure *stru = gst_caps_get_structure(caps, 0);
    const GValue *media_type = gst_structure_get_value(stru, "media");
    if (g_str_equal(g_value_get_string(media_type), "video")) {
        GstPad *sink_pad = gst_element_get_static_pad(GST_ELEMENT_CAST(depay), "sink");
        GstPadLinkReturn ret = gst_pad_link(src_pad, sink_pad);
        g_warn_if_fail(ret == GST_PAD_LINK_OK);
        gst_object_unref(sink_pad);
    }
}

static gboolean timeout(gpointer sink)
{
    int i;
    GstCaps *caps;
    GstSample *from_sample, *to_sample;
    GError *err = NULL;
    GstBuffer *buf;
    GstMapInfo map_info;
    char pic_location[50];

    g_object_get(sink, "last-sample", &from_sample, NULL);
    if (from_sample == NULL) {
        printf("Failed getting sample.\n");
        return FALSE;
    }
    caps = gst_caps_from_string("image/bmp");

    if (caps == NULL) {
        printf("Failed getting caps.\n");
        return FALSE;
    }

    to_sample = gst_video_convert_sample(from_sample, caps, GST_CLOCK_TIME_NONE, &err);
    gst_caps_unref(caps);
    gst_sample_unref(from_sample);

    if (to_sample == NULL && err) {
        printf("Failed converting frame.\n");
        printf("Error : %s\n", err->message);
        return FALSE;
    }
    sprintf(pic_location, "img/00000.bmp");
    buf = gst_sample_get_buffer(to_sample);
    printf("timestamp: %03.3f\n", (float)GST_BUFFER_TIMESTAMP(buf) / 1000000 / 1000);

    if (gst_buffer_map(buf, &map_info, GST_MAP_READ)) {
        // if (!g_file_set_contents(pic_location, (const char *)map_info.data, map_info.size, &err)) {
        //     printf("Could not save image %i.\n", i);
        // }
    }

    gst_sample_unref(to_sample);
    printf("Save image: %s.\n", pic_location);

    api->Init(NULL, "eng");
    char *outText;
    // Open input image with leptonica library
    // Pix *image = pixRead("D:\\Working\\MyProject\\Notes\\opensource\\tesseract\\img\\00000.bmp");
    Pix *image = pixReadMemBmp(map_info.data, map_info.size);
    // printf("w: %d\nh: %d\nd: %d\nspp: %d\nwpl: %d\nrefcount: %d\nxres: %d\nyres: %d\ninformat: %d\nspecial: %d\ntext: %s\ncolormap: %p\ndata: %p\n",
    //        image->w,
    //        image->h,
    //        image->d,
    //        image->spp,
    //        image->wpl,
    //        image->refcount,
    //        image->xres,
    //        image->yres,
    //        image->informat,
    //        image->special,
    //        image->text,
    //        image->colormap,
    //        image->data);
    image->xres = 70;
    image->yres = 70;
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory

    delete[] outText;

    pixDestroy(&image);
    api->End();

    return TRUE;
}
int main()
{
    api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    // if (api->Init(NULL, "eng")) {
    //     fprintf(stderr, "Could not initialize tesseract.\n");
    //     exit(1);
    // }

    gst_init(NULL, NULL);
    GMainLoop *main_loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_pipeline_new("video-pipeline");

    GstElement *source = gst_element_factory_make("rtspsrc", "source");
    g_object_set(G_OBJECT(source), "location", "rtsp://127.0.0.1:8553/test", NULL);

    GstElement *depay = gst_element_factory_make("rtph264depay", "depay");
    g_signal_connect(source, "pad-added", (GCallback)on_rtspsrc_pad_added, depay);

    GstElement *decodec = gst_element_factory_make("avdec_h264", "decodec");
    GstElement *sink = gst_element_factory_make("fakesink", "sink");
    gst_bin_add_many(GST_BIN(pipeline), source, depay, decodec, sink, NULL);
    gst_element_link_many(depay, decodec, sink, NULL);

    g_timeout_add_seconds(5, (GSourceFunc)timeout, sink);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(main_loop);

    g_main_loop_unref(main_loop);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}