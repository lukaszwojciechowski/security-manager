SET(COMMON_VERSION_MAJOR 1)
SET(COMMON_VERSION ${COMMON_VERSION_MAJOR}.0.2)

########################## search for packages ################################

PKG_CHECK_MODULES(TIZEN_DEP
    QUIET
    libtzplatform-config
    )

IF (TIZEN_DEP_FOUND)
    SET(BUILD_WITH_TIZEN ON)
ENDIF (TIZEN_DEP_FOUND)

PKG_CHECK_MODULES(COMMON_DEP
    REQUIRED
    cynara-admin
    cynara-client-async
    )

FIND_PACKAGE(Boost REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM
    ${SYSTEMD_DEP_INCLUDE_DIRS}
    ${SMACK_DEP_INCLUDE_DIRS}
    ${DBUTIL_DEP_INCLUDE_DIRS}
    ${TIZEN_DEP_INCLUDE_DIRS}
    ${COMMON_DEP_INCLUDE_DIRS}
    )

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATH}
    ${COMMON_PATH}/include
    ${DPL_PATH}/core/include
    ${DPL_PATH}/log/include
    ${DPL_PATH}/db/include
    ${Boost_INCLUDE_DIRS}
    )

SET(COMMON_SOURCES
    ${DPL_PATH}/log/src/abstract_log_provider.cpp
    ${DPL_PATH}/log/src/log.cpp
    ${DPL_PATH}/log/src/old_style_log_provider.cpp
    ${DPL_PATH}/core/src/assert.cpp
    ${DPL_PATH}/core/src/binary_queue.cpp
    ${DPL_PATH}/core/src/colors.cpp
    ${DPL_PATH}/core/src/exception.cpp
    ${DPL_PATH}/core/src/noncopyable.cpp
    ${DPL_PATH}/core/src/serialization.cpp
    ${DPL_PATH}/core/src/singleton.cpp
    ${DPL_PATH}/core/src/errno_string.cpp
    ${DPL_PATH}/core/src/string.cpp
    ${DPL_PATH}/db/src/naive_synchronization_object.cpp
    ${DPL_PATH}/db/src/sql_connection.cpp
    ${COMMON_PATH}/config.cpp
    ${COMMON_PATH}/connection.cpp
    ${COMMON_PATH}/cynara.cpp
    ${COMMON_PATH}/file-lock.cpp
    ${COMMON_PATH}/protocols.cpp
    ${COMMON_PATH}/message-buffer.cpp
    ${COMMON_PATH}/master-req.cpp
    ${COMMON_PATH}/privilege_db.cpp
    ${COMMON_PATH}/smack-labels.cpp
    ${COMMON_PATH}/smack-rules.cpp
    ${COMMON_PATH}/smack-check.cpp
    ${COMMON_PATH}/service_impl.cpp
    ${COMMON_PATH}/zone-utils.cpp
    )

IF(BUILD_WITH_SYSTEMD)
SET(COMMON_SOURCES ${COMMON_SOURCES}
    ${DPL_PATH}/log/src/sd_journal_provider.cpp
    )
ELSE(BUILD_WITH_SYSTEMD)
SET(COMMON_SOURCES ${COMMON_SOURCES}
    ${DPL_PATH}/log/src/syslog_provider.cpp
    )
ENDIF(BUILD_WITH_SYSTEMD)

ADD_LIBRARY(${TARGET_COMMON} SHARED ${COMMON_SOURCES})

SET_TARGET_PROPERTIES(${TARGET_COMMON}
    PROPERTIES
        COMPILE_FLAGS "-D_GNU_SOURCE -fPIC -fvisibility=default"
        SOVERSION ${COMMON_VERSION_MAJOR}
        VERSION ${COMMON_VERSION}
    )

TARGET_LINK_LIBRARIES(${TARGET_COMMON}
    ${SYSTEMD_DEP_LIBRARIES}
    ${SMACK_DEP_LIBRARIES}
    ${DBUTIL_DEP_LIBRARIES}
    ${TIZEN_DEP_LIBRARIES}
    ${COMMON_DEP_LIBRARIES}
    )

INSTALL(TARGETS ${TARGET_COMMON} DESTINATION ${LIB_INSTALL_DIR})
