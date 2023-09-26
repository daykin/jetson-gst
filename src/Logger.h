/**
 * Logger.h
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

#include <string>
#include <iostream>

#ifndef LOGGER_H
#define LOGGER_H

#define INFO(msg) logger.log(msg, LogLevel::INFO)
#define WARN(msg) logger.log(msg, LogLevel::WARN)
#define ERROR(msg) logger.log(msg, LogLevel::ERROR)
#define DEBUG(msg) logger.log(msg, LogLevel::DEBUG)

typedef enum {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
} LogLevel;

class Logger{
    public:
        Logger(const char* name, LogLevel level = LogLevel::INFO){
            this->level = level;
            this->name = name;
        }

        void setLogLevel(LogLevel level){
            this->level = level;
        }
        void log(const char* msg, LogLevel lvl){
            if(this->level <= lvl){
                std::cerr<<name<<": "<<msg<<std::endl;
            }
        }
        void log(std::string msg, LogLevel lvl){
            log(msg.c_str(), lvl);
        }
    private:
        const char* name;
        LogLevel level;
};
#endif