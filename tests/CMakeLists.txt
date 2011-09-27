#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for test  lcd_indicator
#==============================================================================

MESSAGE(STATUS "CREATE  LCD  INDICATOR  TEST")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${LCD_INDICATOR_BASE_DIRECTORY})

FIND_PACKAGE(Qt4 COMPONENTS QtGui QtCore QtTest REQUIRED)

IF(QT_FOUND)

INCLUDE(${QT_USE_FILE})

SET(HEADER_FILES
        test_v_lcd_indicator_adapter.h)

SET(SOURCE_FILES
        test_v_lcd_indicator_adapter.cpp)

QT4_WRAP_CPP(MOC_SOURCES ${HEADER_FILES})

ADD_EXECUTABLE(test_v_lcd_indicator ${HEADER_FILES} ${SOURCE_FILES} ${MOC_SOURCES})
ADD_TEST(NAME test_v_lcd_indicator COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_v_lcd_indicator)
TARGET_LINK_LIBRARIES(test_v_lcd_indicator v_lcd_indicator)
TARGET_LINK_LIBRARIES(test_v_lcd_indicator ${QT_LIBRARIES})

ENDIF(QT_FOUND)
