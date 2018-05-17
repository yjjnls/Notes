project(tesseract-test)
cmake_minimum_required(VERSION 2.8)

macro( project_group source_files sgbd_cur_dir)
    if(MSVC)
        foreach(sgbd_file ${${source_files}})

            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
			string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
			
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
			
            source_group(${sgbd_group_name} FILES ${sgbd_file})
			
        endforeach(sgbd_file)
    endif()
endmacro(project_group)



IF(MSVC)
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_DEPRECATE  )
	ADD_DEFINITIONS( -D_CRT_NONSTDC_NO_DEPRECATE )
	ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS   )
	
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	
ENDIF()



find_package(PkgConfig 0.29.1 REQUIRED)

file(GLOB_RECURSE  LWS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch] ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
project_group(LWS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})

foreach( filename ${LWS_SOURCES} )
message(${filename})
endforeach()


pkg_check_modules(GST_MODULES  REQUIRED  
                  gstreamer-1.0>=1.14.0
                  gstreamer-base-1.0>=1.14.0  
                  gstreamer-video-1.0              
                  tesseract)
link_directories (${GST_MODULES_LIBRARY_DIRS})

include_directories (${GST_MODULES_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../out/include)
# add_library(libwebstreamer  SHARED ${LWS_SOURCES} )
add_executable(tesseract-test ${LWS_SOURCES})
target_link_libraries(tesseract-test ${GST_MODULES_LIBRARIES} pvt.cppan.demo.danbloomberg.leptonica-1.76.0)