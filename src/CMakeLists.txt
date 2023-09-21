
set(GLIB_INCLUDE_DIRS "/usr/lib/glib-2.0/include" "/usr/include/glib-2.0" "/usr/lib/aarch64-linux-gnu/glib-2.0/include")
set(GSTREAMER_INCLUDE_DIR "/usr/include/gstreamer-1.0")
set(GSTREAMER_LIBRARIES "/usr/lib/aarch64-linux-gnu/libgstreamer-1.0.so" "/usr/lib/aarch64-linux-gnu/libgobject-2.0.so" "/usr/lib/aarch64-linux-gnu/libglib-2.0.so")
set(GLIB_LIBRARIES "/usr/lib/aarch64-linux-gnu/libglib-2.0.so")
#Set TCAM_INCLUDE_DIR to output of pkg-config --cflags tcam
set(TCAM_LIBRARIES "/usr/lib/libtcam-network.so" "/usr/lib/libtcam-property.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgsttcambin.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libgsttcamsrc.so" "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/libtcamconvert.so" "/usr/lib/libtcam.so")
include_directories(${GLIB_INCLUDE_DIRS})


set( srcs
    GStreamerVideoSource.cpp
    GStreamerVideoSourceFile.cpp
    GStreamerVideoSourceLive.cpp
    GStreamerVideoSource.h
)

#create a shared library target
add_library( ${PROJECT_NAME} SHARED ${srcs} )
add_executable(demo demo.cpp)
target_include_directories(demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${TCAM_LIBRARIES})
target_link_libraries(demo ${PROJECT_NAME} ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${TCAM_LIBRARIES})

target_include_directories(demo PUBLIC ${GSTREAMER_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} )
include_directories(${GSTREAMER_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS})