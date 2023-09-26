/**
 * GStreamerVideoSourceFile.cpp
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
#include <iostream>

GStreamerVideoSourceFile::GStreamerVideoSourceFile(const char* fileName, CALLBACK cb, void* userData){
    std::string pipeline_str;
    userData = userData;
    cb = cb;
    pipeline_str = "filesrc location=";
    pipeline_str += fileName;
    pipeline_str += " name=source ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! appsink name=sink";
    createPipeline(pipeline_str.c_str(), userData);
}