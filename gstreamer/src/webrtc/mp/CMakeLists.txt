cmake_minimum_required( VERSION 2.8 )

project(webrtc-examples-mp)

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

add_definitions(-std=c++11)
add_executable (mp mp.cc)
target_link_libraries(mp ${BASE_LIBRARIES})

install(TARGETS mp
		RUNTIME DESTINATION .
        )