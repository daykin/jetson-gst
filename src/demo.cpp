/**
 * demo.cpp
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

#include "GStreamerVPI.h"
#include <iostream>

/* static GstFlowReturn dumbCallback(GstElement* sink, void* userData){
    GstSample* sample;
    g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
    if(sample){
        std::cout<<"Got a frame!"<<std::endl;
        GstBuffer* buffer = gst_sample_get_buffer(sample);
        GstMapInfo info;
        GstPad *pad;
        GstCaps* caps;
        gst_buffer_map(buffer, &info, GST_MAP_READ);
        pad = gst_element_get_static_pad(sink, "sink");
        caps = gst_pad_get_current_caps(pad);
        if(caps != nullptr){
            gchar *str = gst_caps_to_string(caps);
            std::cout<<"Caps: "<<str<<std::endl;
            g_free(str);
        }
        gst_caps_unref(caps);
        //do something with the data in info.data

        gst_buffer_unmap(buffer, &info);
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
} */

int main(int argc, char** argv){
    gst_init(&argc, &argv);
    //default serial number
    std::string id = "";
    if(argc>1){
        if (std::string(argv[1]) == "-h"){
            std::cout<<"Usage: demo [serial number of camera, or /path/to/videofile.mp4]"<<std::endl;
            return 0;
        }
        else{
            id = std::string(argv[1]);
        }
        
    }
    //creates a 'source' object reading from camera.
    GStreamerVPI source = GStreamerVPI(id.c_str());
    source.start();
    std::cout<<"Press enter to stop the stream."<<std::endl;
    std::cin.get();
    source.stop();
    return 0;
}