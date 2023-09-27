/**
 * GStreamerVPI.h
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

// Adapts GStreamerVideoSource to a VPIStream.
// Creates a GStreamerVideoSource and provides a callback function
// to the GStreamerVideoSource that will provide the frame to the VPIStream.

#pragma once
#ifndef GStreamerVPI_h
#define GStreamerVPI_h
#include <opencv2/core/version.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <vpi/Context.h>
#include <vpi/Array.h>
#include <vpi/Image.h>
#include <vpi/ImageFormat.h>
#include <vpi/Stream.h>
#include <vpi/Status.h>
#include <vpi/Event.h>
#include <vpi/algo/ConvertImageFormat.h>

#include "Logger.h"
#include "GStreamerVideoSource.h"

#define INVALIDATE_IMGIN {if(vpi->imgIn != NULL){ vpiImageInvalidate(vpi->imgIn); }}
#define INVALIDATE_IMGOUT {if(vpi->imgOut != NULL){ vpiImageInvalidate(vpi->imgOut); }}

class GStreamerVPI{
    public:
        GStreamerVPI(const char* id, LogLevel level = WARN);
        GStreamerVPI(const char* id, GStreamerVideoSource::CALLBACK cb, LogLevel level = WARN);
        ~GStreamerVPI();
        gboolean structureHasChanged();
        void* getInData();  
        void createOrUpdateImgIn();
        void start(){this->source->start();};
        void stop(){this->source->stop();}
    protected:
        void updateImageData();
        void unrefSourceBuffer();
        GStreamerVideoSource* source;
        Logger logger = Logger("GStreamerVPI", WARN);
        VPIContext ctx;
        VPIStream stream;
        VPIImage imgIn;
        VPIImageData imgDataIn;
        VPIImageFormat fmtIn;
        VPIImagePlane planeIn;
        VPIImage imgOut;
        VPIBackend backend;
        VPIImageFormat fmtOut;
        VPIPayload payload;
    private:
        long frameCount = 0;
        void init(const char* id, LogLevel lvl);
        static GstFlowReturn vpi_callback(GstElement* sink, void* _vpi);
        static GstPadProbeReturn dimensions_changed(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
};

#endif