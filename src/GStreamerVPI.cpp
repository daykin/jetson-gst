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
    vpiContextCreate(VPI_BACKEND_CPU, &(this->ctx));
    vpiContextSetCurrent(this->ctx);
    vpiStreamCreate(VPI_BACKEND_CPU, &(this->stream));
    this->backend = VPI_BACKEND_CPU;
    this->imgDataIn = VPIImageData();
    this->planeIn = VPIImagePlane();
    this->fmtIn = VPI_IMAGE_FORMAT_INVALID;
    this->imgIn = NULL;
    this->imgOut = NULL;
    this->fmtOut = VPI_IMAGE_FORMAT_INVALID;
    this->sameDimensions = false;
    this->sameFormat = false;
}

GStreamerVPI::~GStreamerVPI()
{
    // Destroy the VPI parameters
    vpiStreamDestroy(this->stream);
    vpiPayloadDestroy(this->payload);
    // Destroy the GStreamerVideoSource
    delete this->source;
}

void GStreamerVPI::getVPIFormatIn(GstCaps *caps)
{
    VPIImageFormat fmtOld = this->fmtIn;
    GstStructure *structure = gst_caps_get_structure(caps, 0);
    const char *format = gst_structure_get_string(structure, "format");
    if (strcmp(format, "RGB") == 0)
    {
        this->fmtIn = VPI_IMAGE_FORMAT_RGB8;
    }
    else if (strcmp(format, "GRAY8") == 0)
    {
        this->fmtIn = VPI_IMAGE_FORMAT_U8;
    }
    else
    {
        std::cout << "Send me unsigned 8-bit data, please" << std::endl;
        this->fmtIn = VPI_IMAGE_FORMAT_INVALID;
    }
    this->sameFormat = (gboolean)(this->fmtIn == fmtOld);
}

void GStreamerVPI::getDimensionsFromCaps(GstCaps *caps)
{
    GstStructure *w = gst_caps_get_structure(caps, 1);
    GstStructure *h = gst_caps_get_structure(caps, 2);
    gint oldWidth = this->planeIn.width;
    gint oldHeight = this->planeIn.height;
    gst_structure_get_int(w, "width", &(this->planeIn.width));
    gst_structure_get_int(h, "height", &(this->planeIn.height));
    this->sameDimensions = (gboolean)(this->planeIn.width == oldWidth && this->planeIn.height == oldHeight);
}

gboolean GStreamerVPI::structureHasChanged()
{
    return !(this->sameDimensions && this->sameFormat);
}

void GStreamerVPI::updateImageData()
{
    if (this->imgIn != NULL)
    {
        vpiImageDestroy(this->imgIn);
    }
    // tell VPI about the data
    // a 'plane' refers to a 2D array of data
    // color images are still 1 plane, we just stride by 3 bytes instead of 1
    if (this->fmtIn == VPI_IMAGE_FORMAT_RGB8)
    {
        this->planeIn.pitchBytes = this->planeIn.width * 3;
    }
    else
    {
        this->planeIn.pitchBytes = this->planeIn.width;
    }

    this->imgDataIn.numPlanes = 1;
    this->planeIn.data = this->source->getData();
    this->imgDataIn.format = this->fmtIn;
    this->imgDataIn.planes[0] = this->planeIn;
}

void GStreamerVPI::unrefSourceBuffer()
{
    this->source->bufferUnref();
}

// pass a pointer to ourselves into the callback, so it knows our state
GstFlowReturn GStreamerVPI::vpi_callback(GstElement *sink, void *_vpi)
{
    GStreamerVPI *vpi = (GStreamerVPI *)_vpi;
    if (!sink)
    {
        std::cout << "Sink is null!" << std::endl;
        return GST_FLOW_ERROR;
    }
    GstPad *pad;
    GstCaps *caps;
    GstSample *sample;
    const VPIImageData *pData = &(vpi->imgDataIn);
    g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
    if (sample)
    {
        std::cout << "Got a frame!" << std::endl;
        vpi->source->setBuf(gst_sample_get_buffer(sample));

        // get the dimensions and type of the frame
        pad = gst_element_get_static_pad(sink, "sink");
        caps = gst_pad_get_current_caps(pad);
        vpi->getDimensionsFromCaps(caps);
        vpi->getVPIFormatIn(caps);

        if (vpi->fmtIn != VPI_IMAGE_FORMAT_U8 && vpi->fmtIn != VPI_IMAGE_FORMAT_RGB8)
        {
            return GST_FLOW_ERROR;
        }
        if (vpi->structureHasChanged() || vpi->imgIn == NULL)
        {
            // destroy the old image if it exists
            vpi->updateImageData();
            vpiImageCreateHostMemWrapper(pData, 0, &(vpi->imgIn));
        }
        else
        { // structure hasn't changed and we already have an Image object
            vpiImageSetWrappedHostMem(vpi->imgIn, pData);
        }
        // do something with the image
        if(vpi->imgIn != NULL){
            std::cout<<"Image is not null!"<<std::endl;
        }
        vpi->unrefSourceBuffer();
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}
