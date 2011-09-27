/*
 * The MIT License
 *
 * Copyright (c) 2011 Evgeny Proydakov lord.tiran@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "v_lcd_indicator_adapter.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <modbus/modbus.h>

struct VLCDIndicatorAdapter::modbus
{
    modbus_t* m_my_modbus;       
};

int charToHex(char s)
{
    return isdigit(s) ? s -'0' : tolower(s) - 'a' + 10;
}

struct VLCDIndicatorAdapter::StatusBoardData
{
    uint16_t boardStatus;
    
    StatusBoardData()
    {
        boardStatus = VLCDIndicatorAdapter::BOARD_WORK_ERROR;
    }
} __attribute__ ((packed));

struct VLCDIndicatorAdapter::WorkBoardData
{
    enum { DISPLAY_VALUE_SIZE = VLCDIndicatorAdapter::WORK_DATA_LENGTH / 2};
    
    uint16_t displayValues[DISPLAY_VALUE_SIZE];
    uint16_t displayPoints[DISPLAY_VALUE_SIZE];
    
    WorkBoardData()
    {
        for(int i = 0; i < DISPLAY_VALUE_SIZE; ++i) {
            displayValues[i] = 0;
            displayPoints[i] = VLCDIndicatorAdapter::POINT_OFF;
        }
    }
} __attribute__ ((packed));

VLCDIndicatorAdapter::VLCDIndicatorAdapter( ):
    m_pStatusBoardData(0),
    m_pWorkBoardData(0),
    m_modbus(0),
    m_portName(""),
    m_boardConnect(0)
{
    m_pStatusBoardData = new StatusBoardData;
    m_pWorkBoardData = new WorkBoardData;
    m_modbus = new modbus;
    m_modbus->m_my_modbus = 0;
}

VLCDIndicatorAdapter::~VLCDIndicatorAdapter()
{
    disconnectFromPort();
    delete m_pStatusBoardData;
    delete m_pWorkBoardData;
    delete m_modbus;
}

bool VLCDIndicatorAdapter::connectToPort(const std::string& portName)
{
    if(!m_modbus->m_my_modbus) {
        // 'E' - event work mode, 8 - data bits, 1 - stop bit - serial port configuration
        m_modbus->m_my_modbus = modbus_new_rtu(portName.c_str(), boarBaudrate, 'E', 8, 1);
        modbus_set_slave(m_modbus->m_my_modbus, boadAdress);
        
        m_portName = portName;
        ++m_boardConnect;
        //assert(m_boardConnect < maxBoardConnect);
    }
    bool connectStatus = !modbus_connect(m_modbus->m_my_modbus);
    if(!connectStatus) {
        modbus_free(m_modbus->m_my_modbus);
        m_modbus->m_my_modbus = 0;
    }
    return connectStatus;;
}

void VLCDIndicatorAdapter::disconnectFromPort()
{
    if(m_modbus->m_my_modbus) {
        modbus_close(m_modbus->m_my_modbus);
        modbus_free(m_modbus->m_my_modbus);
        m_modbus->m_my_modbus = 0;
    }
}

bool VLCDIndicatorAdapter::connectStatus()
{
    return m_modbus->m_my_modbus != 0;
}

bool VLCDIndicatorAdapter::readState()
{
    bool state = modbus_read_registers(m_modbus->m_my_modbus, STATUS_DATA_START_REGISTER,
                                       STATUS_DATA_LENGTH,
                                       reinterpret_cast<uint16_t*>(m_pStatusBoardData)) == STATUS_DATA_LENGTH;
    do {
        state = reconnectToPort();
    }
    while(!state);
    return state;
}

bool VLCDIndicatorAdapter::writeData()
{
    bool state = modbus_write_registers(m_modbus->m_my_modbus, WORK_DATA_START_REGISTER,
                                        WORK_DATA_LENGTH,
                                        reinterpret_cast<uint16_t*>(m_pWorkBoardData)) == WORK_DATA_LENGTH;
    do {
        state = reconnectToPort();
    }
    while(!state);
    return state;
}

void VLCDIndicatorAdapter::setPoints(std::vector<bool> points)
{
    for(int i = 0; i < std::min((int)points.size(), (int)WorkBoardData::DISPLAY_VALUE_SIZE); ++i)
        if(points[i])
            m_pWorkBoardData->displayPoints[i] = POINT_ON;
}

void VLCDIndicatorAdapter::setPoint(int index, bool value)
{
    bool status = (index >= 0 &&  index < (int)WorkBoardData::DISPLAY_VALUE_SIZE);
    assert(status);
    if(status) {
        if(value)
            m_pWorkBoardData->displayPoints[index] = POINT_ON;
        else
            m_pWorkBoardData->displayPoints[index] = POINT_OFF;
    }
}

void VLCDIndicatorAdapter::setHex(int* bytes, int size)
{
    memcpy(m_pWorkBoardData->displayValues, bytes, std::min(size, (int)WorkBoardData::DISPLAY_VALUE_SIZE));
}

void VLCDIndicatorAdapter::setDecimalNumber(int value)
{
    int unit = value % 10;
    int tens = (value - unit) % 100;
    int hundreds = (value - tens) % 1000;
    
    setDidit(0, unit);
    setDidit(1, tens);
    setDidit(2, hundreds);
}

void VLCDIndicatorAdapter::setHexNumber(std::vector<int> values)
{
    for(int i = 0; i < std::min((int)values.size(), (int)WorkBoardData::DISPLAY_VALUE_SIZE); ++i)
        m_pWorkBoardData->displayValues[i] = values[i];
}

void VLCDIndicatorAdapter::setDidit(int index, int hexValue)
{
    bool status = (index >= 0 && index < (int)WorkBoardData::DISPLAY_VALUE_SIZE);
    assert(status);
    if(status)
        m_pWorkBoardData->displayValues[index] = (uint16_t)hexValue;
}

void VLCDIndicatorAdapter::setString(const std::string& stringValue)
{
    int len = stringValue.size();
    if(len < (int)WorkBoardData::DISPLAY_VALUE_SIZE * 2 + 1) {
        int pointNum = 0;
        for(int i = 0; i < len; ++i) {
            if(stringValue[i] == '.')
                ++pointNum;
        }
        
        bool pointCond = true;
        for(int i = 0; i < len -1 && pointCond; ++i) {
            if(stringValue[i] == '.' && stringValue[i + 1] == '.')
                pointCond = false;
        }
        if(stringValue[0] == '.')
            pointCond = false;
        
        bool symbolCorrectCondition = true;
        for(int i = 0; i < len; ++i) {
            if((stringValue[i] < '0' || stringValue[i] > '9') &&
                    (stringValue[i] < 'a' || stringValue[i] > 'f') &&
                    (stringValue[i] < 'A' || stringValue[i] > 'F') &&
                    (stringValue[i] != '.')) {
                symbolCorrectCondition = false;
                break;
            }
        }
        
        if((pointNum < (int)WorkBoardData::DISPLAY_VALUE_SIZE + 1) &&
                pointCond && symbolCorrectCondition &&
                (len - pointNum > (int)WorkBoardData::DISPLAY_VALUE_SIZE - 1) ) {
            
            std::vector<bool> points((int)WorkBoardData::DISPLAY_VALUE_SIZE, false);
            std::vector<int> values((int)WorkBoardData::DISPLAY_VALUE_SIZE, 0);
            
            int lenN = len - 1;
            for(int i = 0, digitIntex = 0, pointIndex = (int)WorkBoardData::DISPLAY_VALUE_SIZE - 1; i < lenN; ++i) {
                if(stringValue[i] != '.') {
                    values[digitIntex] = charToHex(stringValue[i]);
                    ++digitIntex;
                    if(stringValue[i + 1] != '.')
                        --pointIndex;
                }
                else {
                    points[pointIndex] = true;
                    --pointIndex;
                }
            }
            if(stringValue[lenN] == '.')
                points[0] = true;
            else
                values[2] = charToHex(stringValue[lenN]);
            setPoints(points);
            setHexNumber(values);
        }
    }
}

bool VLCDIndicatorAdapter::stillAlive()
{
    if(m_pStatusBoardData->boardStatus == BOARD_WORK_NORMAL)
        return true;
    else
        return false;
}

bool VLCDIndicatorAdapter::reconnectToPort()
{
    modbus_free(m_modbus->m_my_modbus);
    m_modbus->m_my_modbus = 0;
    return connectToPort(m_portName);
}
