/**
 * GStreamerVideoSourceLive.cpp
 * Author: Evan Daykin
 * Copyright (C) 2023 MSU Board of Trustees
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *           http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#include "GStreamerVideoSource.h"
#include <iostream> 

GStreamerVideoSourceLive::GStreamerVideoSourceLive(const char* serial, CALLBACK cb, void* userData){
    const char* pipeline_str = "tcambin name=source ! videoconvert ! appsink name=sink";
    userData = userData;
    cb = cb;
    GError* err = NULL;
    createPipeline(pipeline_str, userData); 
    if(serial != ""){
        GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), "source");
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);
        g_object_set_property(G_OBJECT(source), "serial", &val);
    }
}

//trivial copy constructor
GStreamerVideoSourceLive::GStreamerVideoSourceLive(const GStreamerVideoSourceLive& other){
    pipeline = other.pipeline;
    source = other.source;
    sink = other.sink;
    pad = other.pad;
    buf = other.buf;
    info = other.info;
}