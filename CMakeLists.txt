# Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
# @file        CMakeLists.txt
# @author
# @brief
#

############################# Check minimum CMake version #####################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT("security-manager")

############################# cmake packages ##################################

INCLUDE(FindPkgConfig)
INCLUDE(CheckCXXCompilerFlag)
INCLUDE(GNUInstallDirs)

########################## search for packages ################################

PKG_CHECK_MODULES(SYSTEMD_DEP
    QUIET
    libsystemd-daemon
    libsystemd-journal
    )

IF (SYSTEMD_DEP_FOUND)
    SET(BUILD_WITH_SYSTEMD ON)
ENDIF (SYSTEMD_DEP_FOUND)

PKG_CHECK_MODULES(SMACK_DEP
    QUIET
    libsmack
    )

IF (SMACK_DEP_FOUND)
    SET(BUILD_WITH_SMACK ON)
ENDIF (SMACK_DEP_FOUND)

########################  directory configuration  ############################

SET(LIB_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    CACHE PATH
    "Object code libraries directory")

SET(BIN_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
    CACHE PATH
    "User executables directory")

SET(INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}"
    CACHE PATH
    "Header files directory")

SET(SYSTEMD_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/lib/systemd/system"
    CACHE PATH
    "Systemd system unit file directory")

SET(DB_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/var/security-manager/db"
    CACHE PATH
    "Database directory")

ADD_DEFINITIONS("-DLIB_INSTALL_DIR=\"${LIB_INSTALL_DIR}\"")
ADD_DEFINITIONS("-DBIN_INSTALL_DIR=\"${BIN_INSTALL_DIR}\"")
ADD_DEFINITIONS("-DINCLUDE_INSTALL_DIR=\"${INCLUDE_INSTALL_DIR}\"")
ADD_DEFINITIONS("-DSYSTEMD_INSTALL_DIR=\"${SYSTEMD_INSTALL_DIR}\"")
ADD_DEFINITIONS("-DDB_INSTALL_DIR=\"${DB_INSTALL_DIR}\"")

############################# compiler flags ##################################

SET(CMAKE_C_FLAGS_PROFILING    "-g -O0 -pg -Wp,-U_FORTIFY_SOURCE")
SET(CMAKE_CXX_FLAGS_PROFILING  "-g -O0 -pg -Wp,-U_FORTIFY_SOURCE")
SET(CMAKE_C_FLAGS_DEBUG        "-g -O0 -ggdb -Wp,-U_FORTIFY_SOURCE")
SET(CMAKE_CXX_FLAGS_DEBUG      "-g -O0 -ggdb -Wp,-U_FORTIFY_SOURCE")
SET(CMAKE_C_FLAGS_RELEASE      "-g -O2")
SET(CMAKE_CXX_FLAGS_RELEASE    "-g -O2")
SET(CMAKE_C_FLAGS_CCOV         "-g -O2 --coverage")
SET(CMAKE_CXX_FLAGS_CCOV       "-g -O2 --coverage")

# Check for C++11 support and enable proper compilation flags
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
IF(COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    IF(COMPILER_SUPPORTS_CXX0X)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    ELSE()
        MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    ENDIF()
ENDIF()

# If supported for the target machine, emit position-independent code,suitable
# for dynamic linking and avoiding any limit on the size of the global offset
# table. This option makes a difference on the m68k, PowerPC and SPARC.
# (BJ: our ARM too?)
ADD_DEFINITIONS("-fPIC")

# Set compiler warning flags
ADD_DEFINITIONS("-Werror")                      # Make all warnings into errors.
ADD_DEFINITIONS("-Wall")                        # Generate all warnings
ADD_DEFINITIONS("-Wextra")                      # Generate even more extra warnings

STRING(REGEX MATCH "([^.]*)" API_VERSION "${VERSION}")
ADD_DEFINITIONS("-DAPI_VERSION=\"$(API_VERSION)\"")

ADD_DEFINITIONS("-DSMACK_ENABLED")

IF (CMAKE_BUILD_TYPE MATCHES "DEBUG")
    ADD_DEFINITIONS("-DTIZEN_DEBUG_ENABLE")
    ADD_DEFINITIONS("-DBUILD_TYPE_DEBUG")
ENDIF (CMAKE_BUILD_TYPE MATCHES "DEBUG")

IF (BUILD_WITH_SYSTEMD)
    ADD_DEFINITIONS("-DBUILD_WITH_SYSTEMD")
ENDIF (BUILD_WITH_SYSTEMD)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(pc)
IF (BUILD_WITH_SYSTEMD)
    ADD_SUBDIRECTORY(systemd)
ENDIF (BUILD_WITH_SYSTEMD)
ADD_SUBDIRECTORY(db)
ADD_SUBDIRECTORY(policy)
