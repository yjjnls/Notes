cmake_minimum_required( VERSION 2.8 )

project(webrtc-examples)

find_package(PkgConfig 0.29.1 REQUIRED)


pkg_check_modules(BASE  REQUIRED
                  gstreamer-1.0>=1.14.0
                  gstreamer-base-1.0>=1.14.0
                  gstreamer-webrtc-1.0
                  gstreamer-sdp-1.0 libsoup-2.4 json-glib-1.0)
add_definitions(-DGST_USE_UNSTABLE_API)


include_directories(${CMAKE_SOURCE_DIR})
include_directories(${BASE_INCLUDE_DIRS}) 
link_directories(${BASE_LIBRARY_DIRS})


add_executable (webrtc-simple simple/webrtc.c)
target_link_libraries(webrtc-simple ${BASE_LIBRARIES})

add_executable (webrtc-bidirectional simple/webrtcbidirectional.c)
target_link_libraries(webrtc-bidirectional ${BASE_LIBRARIES})

add_executable (webrtc-swap simple/webrtcswap.c)
target_link_libraries(webrtc-swap ${BASE_LIBRARIES})

add_executable (webrtc-sendrecv gstwebrtc-demos/sendrecv/gst/webrtc-sendrecv.c)
target_link_libraries(webrtc-sendrecv ${BASE_LIBRARIES})

add_executable (webrtc-sendrecv-mp gstwebrtc-demos/multiparty-sendrecv/gst/mp-webrtc-sendrecv.c)
target_link_libraries(webrtc-sendrecv-mp ${BASE_LIBRARIES})

add_subdirectory(mp)
add_subdirectory(mp_web)

install(TARGETS webrtc-simple webrtc-bidirectional webrtc-swap webrtc-sendrecv webrtc-sendrecv-mp
		RUNTIME DESTINATION .
        )