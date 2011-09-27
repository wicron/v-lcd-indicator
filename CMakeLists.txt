#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Welcome to the CMake build system for LCD  INDICATOR.
# This is the main file where we prepare the general build environment
# and provide build configuration options.
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(v_lcd_indicator)
MESSAGE(STATUS "CREATE  PROJECT  LCD  INDICATOR")

# temp setting
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

# build libraries type
IF(NOT BUILD_SHARED_LIBS)
    SET(BUILD_SHARED_LIBS ON)
ENDIF(NOT BUILD_SHARED_LIBS)

SET(ROBOGUN_BASE_DIRECTORY ${PROJECT_SOURCE_DIR})

# build directory config
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# work with cmake
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# call system info script
INCLUDE(${CMAKE_MODULE_PATH}/FindOS.cmake)

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(TESTS_PATH ${PROJECT_SOURCE_DIR}/tests)

# find modbus library
FIND_PACKAGE(MODBUS)
INCLUDE_DIRECTORIES(${MODBUS_INCLUDE_DIR})

SET(DOC_FILES
    README)

SET(HEADER_FILES
    v_lcd_indicator_adapter.h)

SET(SOURCE_FILES
    v_lcd_indicator_adapter.cpp)

ADD_LIBRARY(v_lcd_indicator ${HEADER_FILES}  ${SOURCE_FILES} ${DOC_FILES})
TARGET_LINK_LIBRARIES(v_lcd_indicator ${MODBUS_LIBRARIES})

# test setting
IF(NOT DISABLE_TEST)
    INCLUDE(CTest)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${TESTS_PATH})
ENDIF(NOT DISABLE_TEST)
