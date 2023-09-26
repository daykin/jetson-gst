/**
 * GStreamerVideoSource.cpp
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
#include "Logger.h"
#include <string>
#include <cstring>
#include <iostream>

GStreamerVideoSource::GStreamerVideoSource(LogLevel logLevel = WARN){
    pipeline = NULL;
    source = NULL;
    sink = NULL;
    buf = NULL;
    info = new GstMapInfo();
    logger.setLogLevel(logLevel);
}

GStreamerVideoSource* GStreamerVideoSource::create(const char* id, GStreamerVideoSource::CALLBACK cb, void* userData){
    std::string strId(id);
    if (strId.size() >= 4 && strId.substr(strId.size() - 4) == ".mp4"){
        return new GStreamerVideoSourceFile(id, cb, userData);
    //else create a GStreamerVideoSourceLive
    }else{
        return new GStreamerVideoSourceLive(id, cb, userData);
    }

}

int GStreamerVideoSource::createPipeline(const char* pipeline_str, void* myData){
    GError* err = NULL;
    //create pipeline from pipeline_str
    INFO("Creating pipeline");
    DEBUG(pipeline_str);
    pipeline = gst_parse_launch(pipeline_str, &err);
    if (pipeline == NULL)
    {
        ERROR("Could not create pipeline. Cause:\n");
        ERROR(err->message);
        return 1;
    }
    //get source and sink elements from pipeline
    DEBUG("Getting source and sink elements");
    source = gst_bin_get_by_name(GST_BIN(pipeline), "source");
    sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    pad = gst_element_get_static_pad(sink, "sink");
    DEBUG("Attaching callback to GStreamer sink");
    g_object_set(G_OBJECT(sink), "emit-signals", TRUE, NULL);
    g_signal_connect(sink, "new-sample", G_CALLBACK(cb), myData);
    gst_object_unref(sink);
    INFO("Done creating pipeline.");
    return 0;
}

void GStreamerVideoSource::start(){
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void GStreamerVideoSource::stop(){
    gst_element_set_state(pipeline, GST_STATE_NULL);
}

void GStreamerVideoSource::setBuf(GstBuffer* gbuf){
    this->buf = gbuf;
    gst_buffer_map(this->buf, info, GST_MAP_READ);
}

//return a pointer to the array holding the image data
void *GStreamerVideoSource::getData()
{
    if(info->data != NULL){
        return info->data;
    }
}

void GStreamerVideoSource::bufferUnref(){
    gst_buffer_unmap(buf, info);
    gst_buffer_unref(buf);
}