
set(GLIB_INCLUDE_DIRS "/usr/lib/glib-2.0/include" "/usr/include/glib-2.0" "/usr/lib/aarch64-linux-gnu/glib-2.0/include")
set(GSTREAMER_INCLUDE_DIR "/usr/include/gstreamer-1.0")
set(OPENCV_INCLUDE_DIR "/usr/include/opencv4/opencv" "/usr/include/opencv4")
set(NVVPI_INCLUDE_DIRS "/opt/nvidia/vpi1/include")
set(NVVPI_LIBRARIES "/opt/nvidia/vpi1/lib64/libnvvpi.so")
execute_process(COMMAND "pkg-config --libs opencv4" OUTPUT_VARIABLE OPENCV_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
set(GSTREAMER_LIBRARIES "/usr/lib/aarch64-linux-gnu/libgstreamer-1.0.so" "/usr/lib/aarch64-linux-gnu/libgobject-2.0.so" "/usr/lib/aarch64-linux-gnu/libglib-2.0.so")
set(GLIB_LIBRARIES "/usr/lib/aarch64-linux-gnu/libglib-2.0.so")
set(TCAM_LIBRARIES "/usr/lib/libtcam-network.so" "/usr/lib/libtcam-property.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgsttcambin.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgsttcamsrc.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libtcamconvert.so" "/usr/lib/libtcam.so")

# set OPENCV_LIBRARIES to output of pkg-config --libs opencv4

include_directories(${GLIB_INCLUDE_DIRS})


set( srcs
    GStreamerVideoSource.cpp
    GStreamerVideoSourceFile.cpp
    GStreamerVideoSourceLive.cpp
    GStreamerVideoSource.h
    GStreamerVPI.h
    GStreamerVPI.cpp
)

#create a shared library target
add_library( ${PROJECT_NAME} SHARED ${srcs} )
add_executable(demo demo.cpp)
target_include_directories(demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${TCAM_LIBRARIES} ${OPENCV_LIBRARIES} ${NVVPI_LIBRARIES})
target_link_libraries(demo ${PROJECT_NAME} ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${TCAM_LIBRARIES} ${OPENCV_LIBRARIES} ${NVVPI_LIBRARIES})

target_include_directories(demo PUBLIC ${GSTREAMER_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIR} ${NVVPI_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIR} ${NVVPI_INCLUDE_DIRS})