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
#include <cstdarg>

#ifndef LOGGER_H
#define LOGGER_H

#define INFO(msg, ...) logger.log( LogLevel::INFO, msg, ##__VA_ARGS__)
#define WARN(msg, ...) logger.log( LogLevel::WARN, msg, ##__VA_ARGS__)
#define ERROR(msg, ...) logger.log( LogLevel::ERROR, msg, ##__VA_ARGS__)
#define DEBUG(msg, ...) logger.log( LogLevel::DEBUG, msg, ##__VA_ARGS__)

typedef enum {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
} LogLevel;

static const char* logLevelStrings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

class Logger{
    public:
        Logger(const char* name, LogLevel level = LogLevel::INFO){
            this->level = level;
            this->name = name;
        }

        void setLogLevel(LogLevel level){
            this->level = level;
        }
        void log(LogLevel lvl, const char* msg, ...){
            va_list args;
            va_start(args,msg);
            if(this->level <= lvl){
                fprintf(stderr,"[%s] %s:", this->name, logLevelStrings[lvl]);
                vfprintf(stderr, msg, args);
                fprintf(stderr, "\n");
            }
            va_end(args);
        }
        void log(LogLevel lvl, std::string msg){
            log(lvl, msg.c_str());
        }
    private:
        const char* name;
        LogLevel level;
};
#endif