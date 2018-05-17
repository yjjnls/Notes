#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <gst/gst.h>
#include <gst/video/video.h>

int test()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead("D:\\Working\\MyProject\\Notes\\opensource\\tesseract\\img\\00000.bmp");
    printf("w: %d\nh: %d\nd: %d\nspp: %d\nwpl: %d\nrefcount: %d\nxres: %d\nyres: %d\ninformat: %d\nspecial: %d\ntext: %s\ncolormap: %p\ndata: %p\n",
           image->w,
           image->h,
           image->d,
           image->spp,
           image->wpl,
           image->refcount,
           image->xres,
           image->yres,
           image->informat,
           image->special,
           image->text,
           image->colormap,
           image->data);
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory

    delete[] outText;
    api->End();
    pixDestroy(&image);

    return 0;
}