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

GStreamerVideoSourceLive::GStreamerVideoSourceLive(const char* serial, CALLBACK cb, void* userData): GStreamerVideoSource(WARN){
    const char* pipeline_str = "tcambin tcam-properties=tcam,ExposureAuto=Off,GainAuto=On,ExposureTime=50000,AcquisitionFrameRate=20 name=source ! videoconvert  ! appsink name=sink";
    this->userData = userData;
    this->cb = cb;
    GError* err = NULL;
    createPipeline(pipeline_str, userData);
    GstElement* source = gst_bin_get_by_name(GST_BIN(this->pipeline), "source");
    if(serial != ""){    
        GValue sn = {};
        g_value_init(&sn, G_TYPE_STRING);
        g_value_set_static_string(&sn, serial);
        g_object_set_property(G_OBJECT(source), "serial", &sn);
    }
    gst_element_set_state(pipeline, GST_STATE_READY);
}


void GStreamerVideoSourceLive::print_enum_property(const char* propName){
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               propName,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION)
    {
        printf("%s has wrong type. This should not happen.\n", propName);
    }
    else
    {
        TcamPropertyEnumeration* property_enum = TCAM_PROPERTY_ENUMERATION(property_base);
        const char* value = tcam_property_enumeration_get_value(property_enum, &err);

        if (err)
        {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            printf("%s: %s\n", propName, value);
        }
    }
    g_object_unref(property_base);
}

void GStreamerVideoSourceLive::set_enum_property(const char* propName, const char* value)
{
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               propName,
                                                                               &err);

    if (err)
    {
        ERROR("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_ENUMERATION)
    {
        ERROR("Prop has wrong type. This should not happen.\n");
    }
    else
    {
        TcamPropertyEnumeration* enum_property = TCAM_PROPERTY_ENUMERATION(property_base);

        tcam_property_enumeration_set_value(enum_property, value, &err);

        if (err)
        {
            ERROR("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            INFO("Set %s to %s\n", propName, value);
        }
    }
    g_object_unref(property_base);
}

void GStreamerVideoSourceLive::print_float_property(const char* propName){
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               propName,
                                                                               &err);

    if (err)
    {
        printf("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT)
    {
        printf("%s has wrong type. This should not happen.\n", propName);
    }
    else
    {
        TcamPropertyFloat* property_float = TCAM_PROPERTY_FLOAT(property_base);
        gdouble value = tcam_property_float_get_value(property_float, &err);

        if (err)
        {
            printf("Error while retrieving property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            printf("%s: %f\n", propName, value);
        }
    }
    g_object_unref(property_base);
}

void GStreamerVideoSourceLive::set_float_property(const char* propName, gdouble value)
{
    GError* err = NULL;
    TcamPropertyBase* property_base = tcam_property_provider_get_tcam_property(TCAM_PROPERTY_PROVIDER(source),
                                                                               propName,
                                                                               &err);

    if (err)
    {
        ERROR("Error while retrieving property: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    if (tcam_property_base_get_property_type(property_base) != TCAM_PROPERTY_TYPE_FLOAT)
    {
        ERROR("Prop has wrong type. This should not happen.\n");
    }
    else
    {
        TcamPropertyFloat* float_property = TCAM_PROPERTY_FLOAT(property_base);

        tcam_property_float_set_value(float_property, value, &err);

        if (err)
        {
            ERROR("Error while setting property: %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        else
        {
            INFO("Set %s to %f\n", propName, value);
        }
    }
    g_object_unref(property_base);
}