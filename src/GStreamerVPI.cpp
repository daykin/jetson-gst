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

void GStreamerVPI::init(const char* id, LogLevel level){
    vpiContextCreate(0, &(ctx));
    vpiContextSetCurrent(ctx);
    vpiStreamCreate(0, &(stream));
    imgDataIn = VPIImageData();
    planeIn = VPIImagePlane();
    //tell the source to update the VPI dimension/format parameters when they change
    gst_pad_add_probe(source->getPad(), GST_PAD_PROBE_TYPE_EVENT_BOTH, dimensions_changed, this, nullptr);
    fmtIn = VPI_IMAGE_FORMAT_INVALID;
    imgIn = NULL;
    imgOut = NULL;
    logger.setLogLevel(level);
    fmtOut = VPI_IMAGE_FORMAT_INVALID;
}

//create a GStreamerVPI with our example callback
GStreamerVPI::GStreamerVPI(const char *id, LogLevel level)
{
    source = GStreamerVideoSource::create(id, GStreamerVPI::vpi_callback, this);
    init(id,level);
    INFO("Created GStreamerVPI Object with id %s", id);
}

// Create a GStreamerVPI object with a custom callback
GStreamerVPI::GStreamerVPI(const char* id, GStreamerVideoSource::CALLBACK cb, LogLevel level){
    source = GStreamerVideoSource::create(id, cb, this);
    init(id, level);
    INFO("Created GStreamerVPI Object with id %s", id);
}

GStreamerVPI::~GStreamerVPI()
{
    INFO("Destroying GStreamerVPI Object.");
    // Destroy the VPI parameters
    vpiImageDestroy(imgIn);
    vpiStreamDestroy(stream);
    //vpiPayloadDestroy(payload);
    // Destroy the GStreamerVideoSource
    //delete source;
}

GstPadProbeReturn GStreamerVPI::dimensions_changed(GstPad *pad, GstPadProbeInfo *info, gpointer _vpi){
    GStreamerVPI* vpi = (GStreamerVPI*) _vpi;
    GstEvent *event = GST_PAD_PROBE_INFO_EVENT(info);
    if(GST_EVENT_CAPS == GST_EVENT_TYPE(event)){
        GstCaps *caps = gst_caps_new_any();
        gst_event_parse_caps(event, &caps);

        GstStructure *s = gst_caps_get_structure(caps, 0);
        std::string fmt;
        gboolean result;
        result = gst_structure_get_int(s, "width", &(vpi->planeIn.width));
        result |= gst_structure_get_int(s, "height", &(vpi->planeIn.height));
        fmt = gst_structure_get_string(s, "format");
        if(fmt == "RGB"){
            vpi->fmtIn = VPI_IMAGE_FORMAT_RGB8;
            vpi->planeIn.pitchBytes = vpi->planeIn.width * 3;
        }
        else if(fmt == "GRAY8"){
            vpi->fmtIn = VPI_IMAGE_FORMAT_U8;
            vpi->planeIn.pitchBytes = vpi->planeIn.width;
        }
        else if(fmt == "I420"){
            //discard the UV plane, just keep the Y as a grayscale U8
            vpi->fmtIn = VPI_IMAGE_FORMAT_Y8_ER;
            vpi->planeIn.pitchBytes = vpi->planeIn.width;
        }
        else{
            vpi->ERROR("Send me unsigned 8-bit data (GRAY8 or RGB), please. You're sending %s.", fmt.c_str());
            vpi->fmtIn = VPI_IMAGE_FORMAT_INVALID;
        }
        if(!result){
            vpi->WARN("No dimensions!");
        }
        vpi->imgDataIn.numPlanes = 1;
        vpi->updateImageData();
        vpi->imgDataIn.format = vpi->fmtIn;
        vpi->imgDataIn.planes[0] = vpi->planeIn;
        vpi->INFO("GST_EVENT_CAPS dims: %dx%d", vpi->planeIn.width, vpi->planeIn.height);
    }
    return GST_PAD_PROBE_OK;
}


void GStreamerVPI::updateImageData()
{
    DEBUG("Updating image plane");
    planeIn.data = source->getData();
    imgDataIn.planes[0] = planeIn;
}

void GStreamerVPI::unrefSourceBuffer()
{
    DEBUG("UnrefSourceBuffer");
    source->bufferUnref();
}

void GStreamerVPI::createOrUpdateImgIn()
{
    const VPIImageData *pData = &(imgDataIn);
    if (imgIn == NULL)
    {
        INFO("Creating a new VPIImage Object");
        vpiImageCreateHostMemWrapper(pData, 0, &(imgIn));
    }
    else
    {
        DEBUG("Updating existing VPIImage");
        vpiImageSetWrappedHostMem(imgIn, pData);
    }
}

// pass a pointer to ourselves into the callback, so it knows our state
// This one is an example, you don't have to use it.
GstFlowReturn GStreamerVPI::vpi_callback(GstElement *sink, void *_vpi)
{
    GStreamerVPI *vpi = (GStreamerVPI *)_vpi;
    if (!sink)
    {
        vpi->WARN("Sink is null!");
        return GST_FLOW_ERROR;
    }
    GstSample *sample;
    g_signal_emit_by_name(sink, "pull-sample", &sample, NULL);
    if (sample)
    {
        vpi->DEBUG("Got a frame");
        vpi->source->setBuf(gst_buffer_ref(gst_sample_get_buffer(sample)));
        vpi->updateImageData();
        vpi->createOrUpdateImgIn();
        // do something with imgIn here, e.g. notify your own stuff that it's here
        
        if(vpi->imgIn == NULL){
            vpi->WARN("Image is null!");
        }
        //be sure to block (e.g. with vpiStreamSync) until you've copied the data out of imgIn!
        vpi->unrefSourceBuffer();
        vpi->DEBUG("Unref'd source buffer");
        INVALIDATE_IMGIN
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}
