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

void printHelp(){
    std::cout<<"Usage: demo [-h] [-v|-vv] [serial number of camera, or /path/to/videofile.mp4] "<<std::endl;
    std::cout<<"-v: verbose"<<std::endl;
    std::cout<<"-vv: very verbose"<<std::endl;
}

int main(int argc, char** argv){
    gst_init(&argc, &argv);
    //default serial number
    std::string id = "";
    LogLevel ll = WARN;
    for(int i=1; i<argc; i++){
        if (std::string(argv[i]) == "-h"){
            printHelp();
            return 0;
        }
        if(std::string(argv[i]) == "-v"){
           ll = INFO;
        }
        else if(std::string(argv[i]) == "-vv"){
            ll = DEBUG;
        }
        else if(i==argc-1 && argv[i][0] != '-'){
            id = argv[i];
        }
        else if(argv[i][0] == '-'){
            std::cout<<"Unknown option: "<<argv[i]<<std::endl;
            printHelp();
            return 0;
        }
        else{
            printHelp();
            return 0;
        }
    }

    GStreamerVPI source = GStreamerVPI(id.c_str(), ll);
    source.start();
    std::cout<<"Press enter to stop the stream."<<std::endl;
    std::cin.get();
    source.stop();
    return 0;
}