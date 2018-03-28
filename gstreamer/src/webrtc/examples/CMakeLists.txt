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



# add_executable (dynamic-tee-vsink dynamic-tee-vsink.c)
# target_link_libraries(dynamic-tee-vsink ${BASE_LIBRARIES})

#add_dependencies(transceiver libwebrtctransport )

#add_subdirectory( libwebrtctransport  )
#add_subdirectory( tests/transceiver  )
#install(DIRECTORY web DESTINATION testkits/openwebrtc )

install(TARGETS webrtc-simple
		RUNTIME DESTINATION .
        )