/**
 * GStreamerVideoSource.h
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

// An interface to get video frames from GStreamer,
// via an .mp4 file or a TISCamera live stream.
// A factory method will create either a GStreamerVideoSourceFile
// or a GStreamerVideoSourceLive. The callback will provide this frame
// to a VPIStream instance.

#pragma once
#ifndef GStreamerVideoSource_h
#define GStreamerVideoSource_h

extern "C"{
#include "tcam-property-1.0.h"
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/video/video.h>
}

class GStreamerVideoSource
{
    public:
        typedef GstFlowReturn (*CALLBACK)(GstElement*, void*);
        // everything attached to pipeline is torn down automagically
        ~GStreamerVideoSource() {gst_object_unref(GST_OBJECT(this->pipeline));}
        void start();
        void stop();
        //Abstract factory to create either a GStreamerVideoSourceFile or a GStreamerVideoSourceLive.
        static GStreamerVideoSource* create(const char* id, CALLBACK cb, void* userData);

    protected:
        GStreamerVideoSource(CALLBACK, void*){}
        GStreamerVideoSource(){}
        int createPipeline(const char* pipeline_str, void* myData);
        void* userData;
        CALLBACK cb;
        GstElement *pipeline;
        GstElement *source;
        GstElement *sink;
};

class GStreamerVideoSourceFile : public GStreamerVideoSource
{
    public:
        GStreamerVideoSourceFile(const char* path, CALLBACK, void* userData);
        GStreamerVideoSourceFile(){}
        GStreamerVideoSourceFile(const GStreamerVideoSourceFile&);
        ~GStreamerVideoSourceFile();

    private:
        const char* path;
};

class GStreamerVideoSourceLive : public GStreamerVideoSource
{
    public:
        //pass camName==NULL to use the default camera
        GStreamerVideoSourceLive(const char*, CALLBACK, void*);
        GStreamerVideoSourceLive();
        //copy constructor
        GStreamerVideoSourceLive(const GStreamerVideoSourceLive&);
        ~GStreamerVideoSourceLive();

    private:
        const char* camName;
};
#endif