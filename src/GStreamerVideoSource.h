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
#include "Logger.h"

class GStreamerVideoSource
{
    public:
        typedef GstFlowReturn (*CALLBACK)(GstElement*, void*);
        GStreamerVideoSource(LogLevel);
        // everything attached to pipeline is torn down automagically
        ~GStreamerVideoSource() {
            gst_buffer_unmap(buf, info);
            delete info;
            gst_object_unref(pad);
            gst_object_unref(GST_OBJECT(pipeline));
        }
        void start();
        void setLogLevel(LogLevel level){
            logger.setLogLevel(level);
        };
        void stop();
        void setBuf(GstBuffer* buf);
        void* getData();
        GstPad* getPad(){return pad;}
        void bufferUnref();
        //Factory to create either a GStreamerVideoSourceFile or a GStreamerVideoSourceLive.
        static GStreamerVideoSource* create(const char* id, CALLBACK cb, void* userData);

    protected:
        int createPipeline(const char* pipeline_str, void* myData);
        void* userData;
        CALLBACK cb;
        GstElement *pipeline;
        GstElement *source;
        GstPad     *pad;
        GstElement *sink;
        GstBuffer  *buf;
        GstMapInfo *info;
        Logger logger = Logger("GStreamerVideoSource", WARN);     
};

class GStreamerVideoSourceFile : public GStreamerVideoSource
{
    public:
        GStreamerVideoSourceFile(const char* path, CALLBACK, void* userData);
        GStreamerVideoSourceFile(const GStreamerVideoSourceFile&);
        ~GStreamerVideoSourceFile();

    private:
        const char* path;
};

class GStreamerVideoSourceLive : public GStreamerVideoSource
{
    public:
        //pass camName==NULL to use the default camera
        GStreamerVideoSourceLive(const char*, CALLBACK, void* userData);
        GStreamerVideoSourceLive(const GStreamerVideoSourceLive&);
        ~GStreamerVideoSourceLive();
        void print_enum_property(const char* propName);
        void print_float_property(const char* propName);
        void set_enum_property(const char* propName, const char* value);
        void set_float_property(const char* propName, gdouble value);
    private:
        const char* camName;
};
#endif