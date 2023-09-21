/**
 * GStreamerVPI.cpp
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

GStreamerVPI::GStreamerVPI(const char *id)
{
    this->source = GStreamerVideoSource::create(id, GStreamerVPI::vpi_callback, this);
    vpiContextCreate(VPI_BACKEND_CUDA, &(this->ctx));
    vpiContextSetCurrent(this->ctx);
    vpiStreamCreate(VPI_BACKEND_CUDA, &(this->stream));
}

GStreamerVPI::~GStreamerVPI()
{
    //Destroy the VPI parameters
    vpiStreamDestroy(this->stream);
    vpiPayloadDestroy(this->payload);
    //Destroy the GStreamerVideoSource
    delete this->source;
}

VPIImageFormat GStreamerVPI::getVPIFormat(GstCaps *caps)
{
    VPIImageFormat fmt;
    GstStructure *structure = gst_caps_get_structure(caps, 0);
    const char *format = gst_structure_get_string(structure, "format");
    if (strcmp(format, "RGB") == 0)
    {
        fmt = VPI_IMAGE_FORMAT_RGB8;
    }
    else if (strcmp(format, "GRAY8") == 0)
    {
        fmt = VPI_IMAGE_FORMAT_U8;
    }
    else
    {
        fmt = VPI_IMAGE_FORMAT_INVALID;
    }
    return fmt;
}

Dimension GStreamerVPI::getDimensionsFromCaps(GstCaps *caps)
{
    GstStructure *w = gst_caps_get_structure(caps, 1);
    GstStructure *h = gst_caps_get_structure(caps, 2);
    //initialize a Dimension struct
    
    Dimension dim = Dimension();
    gst_structure_get_int(w, "width",  &(dim.width));
    gst_structure_get_int(h, "height", &(dim.height));
    return dim;
}

//pass a pointer to ourselves into the callback, so it knows our state
GstFlowReturn GStreamerVPI::vpi_callback(GstElement *sink, void *_vpi)
{
    GStreamerVPI *vpi = (GStreamerVPI *)_vpi;
    if(!sink){
        std::cout<<"Sink is null!"<<std::endl;
        return GST_FLOW_ERROR;
    }
    GstPad *pad;
    GstCaps* caps;
    GstSample* sample;
    g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
    if(sample){
        std::cout<<"Got a frame!"<<std::endl;
        GstBuffer* buffer = gst_sample_get_buffer(sample);
        GstMapInfo info;
        pad = gst_element_get_static_pad(sink, "sink");
        caps = gst_pad_get_current_caps(pad);
        vpi->fmtIn = GStreamerVPI::getVPIFormat(caps);
        gst_buffer_map(buffer, &info, GST_MAP_READ);
        int dmabuf_fd;
        gst_buffer_unmap(buffer, &info);
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}
