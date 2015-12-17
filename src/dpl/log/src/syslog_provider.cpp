/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/*
 * @file        syslog_provider.cpp
 * @author      Lukasz Wojciechowski <l.wojciechow@partner.samsung.com>
 * @version     1.0
 * @brief       This file contains the implementation of syslog log provider
 */

#include <dpl/log/syslog_provider.h>
#include <string>
#include <sstream>
#include <syslog.h>

#define UNUSED __attribute__((unused))

namespace SecurityManager {
namespace Log {
std::string SyslogProvider::FormatMessage(const char *message,
                                          const char *filename,
                                          int line,
                                          const char *function)
{
    std::ostringstream val;

    val << std::string("[") <<
    LocateSourceFileName(filename) << std::string(":") << line <<
    std::string("] ") << function << std::string("(): ") << message;

    return val.str();
}

SyslogProvider::SyslogProvider()
{}

SyslogProvider::~SyslogProvider()
{}

void SyslogProvider::SetTag(const char *tag)
{
    m_tag = std::string(tag);
}

void SyslogProvider::Debug(const char *message,
                           const char *filename,
                           int line,
                           const char *function)
{
    syslog(LOG_DEBUG, "%s",
           (FormatMessage(message, filename, line, function)).c_str());
}

void SyslogProvider::Info(const char *message,
                          const char *filename,
                          int line,
                          const char *function)
{
    syslog(LOG_INFO, "%s",
           (FormatMessage(message, filename, line, function)).c_str());
}

void SyslogProvider::Warning(const char *message,
                             const char *filename,
                             int line,
                             const char *function)
{
    syslog(LOG_WARNING, "%s",
           (FormatMessage(message, filename, line, function)).c_str());
}

void SyslogProvider::Error(const char *message,
                           const char *filename,
                           int line,
                           const char *function)
{
    syslog(LOG_ERR, "%s",
           (FormatMessage(message, filename, line, function)).c_str());
}

// All Pedantic logs are translated to Debug
void SyslogProvider::Pedantic(const char *message,
                              const char *filename,
                              int line,
                              const char *function)
{
    Debug(message, filename, line, function);
}

} // namespace Log
} // namespace SecurityManager
