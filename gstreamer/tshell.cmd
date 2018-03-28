
set GSTREAMER_ROOT=%~dp0
set GST_PLUGIN_SYSTEM_PATH_1_0=%GSTREAMER_ROOT%/lib/gstreamer-1.0
set GIO_EXTRA_MODULES=%GSTREAMER_ROOT%/lib/gio/modules
set GST_PLUGIN_SYSTEM_PATH=%GSTREAMER_ROOT%/lib/gstreamer-0.10
set GST_PLUGIN_SCANNER=%GSTREAMER_ROOT%/libexec/gstreamer-0.10/gst-plugin-scanner
set GST_PLUGIN_SCANNER_1_0=%GSTREAMER_ROOT%/libexec/gstreamer-1.0/gst-plugin-scanner
PATH=%GSTREAMER_ROOT%bin;%PATH%
set GI_TYPELIB_PATH=%GSTREAMER_ROOT%/lib/girepository-1.0


start cmd
