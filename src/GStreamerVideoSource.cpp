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
#include <string>
#include <cstring>
#include <iostream>

GStreamerVideoSource* GStreamerVideoSource::create(const char* id, GStreamerVideoSource::CALLBACK cb, void* userData){
    std::string strId(id);
    if (strId.size() >= 4 && strId.substr(strId.size() - 4) == ".mp4"){
        std::cout<<"file"<<std::endl;
        return new GStreamerVideoSourceFile(id, cb, userData);
    //else create a GStreamerVideoSourceLive
    }else{
        return new GStreamerVideoSourceLive(id, cb, userData);
    }

}

int GStreamerVideoSource::createPipeline(const char* pipeline_str, void* myData){
    GError* err = NULL;
    //create pipeline from pipeline_str
    this->pipeline = gst_parse_launch(pipeline_str, &err);
    if (this->pipeline == NULL)
    {
        printf("Could not create pipeline. Cause: %s\n", err->message);
        return 1;
    }
    //get source and sink elements from pipeline
    this->source = gst_bin_get_by_name(GST_BIN(pipeline), "source");
    this->sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    //set callback function for sink
    g_object_set(G_OBJECT(this->sink), "emit-signals", TRUE, NULL);
    g_signal_connect(this->sink, "new-sample", G_CALLBACK(this->cb), myData);
    gst_object_unref(this->sink);
    return 0;
}

void GStreamerVideoSource::start(){
    gst_element_set_state(this->pipeline, GST_STATE_PLAYING);
}

void GStreamerVideoSource::stop(){
    gst_element_set_state(this->pipeline, GST_STATE_NULL);
}

void GStreamerVideoSource::setBuf(GstBuffer* buf){
    this->buf = buf;
    gst_buffer_map(this->buf, this->info, GST_MAP_READ);
}

void *GStreamerVideoSource::getData()
{
    if(this->info->data != NULL){
        return this->info->data;
    }
}

void GStreamerVideoSource::bufferUnref(){
    gst_buffer_unmap(this->buf, this->info);
    gst_buffer_unref(this->buf);
}