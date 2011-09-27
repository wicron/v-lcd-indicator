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

#ifndef V_LCD_INDICATOR_ADAPTER_H
#define V_LCD_INDICATOR_ADAPTER_H

#include <string>
#include <vector>

class VLCDIndicatorAdapter
{
public:
    VLCDIndicatorAdapter();
    ~VLCDIndicatorAdapter();

    bool connectToPort(const std::string& portName);
    void disconnectFromPort();

    bool connectStatus();

    bool readState();
    bool writeData();

    void setPoints(std::vector<bool> points);
    void setPoint(int index, bool value);

    void setHex(int* bytes, int size);
    void setDecimalNumber(int decimalValue);
    void setHexNumber(std::vector<int> values);
    void setDidit(int index, int value);
    void setString(const std::string& stringValue);

    bool stillAlive();

protected:
    bool reconnectToPort();

private:
    enum StatusDataRegister { STATUS_DATA_START_REGISTER = 1150, STATUS_DATA_END_REGISTER = 1150,
                              STATUS_DATA_LENGTH = STATUS_DATA_END_REGISTER - STATUS_DATA_START_REGISTER + 1};
    enum WorkBoardRegister { WORK_DATA_START_REGISTER = 1100,  WORK_DATA_END_REGISTER = 1105,
                             WORK_DATA_LENGTH = WORK_DATA_END_REGISTER - WORK_DATA_START_REGISTER + 1};

    enum PointState { POINT_ON = 255, POINT_OFF = 0 };
    enum ErrorState { BOARD_WORK_ERROR = 255, BOARD_WORK_NORMAL = 0 };

    struct StatusBoardData;
    struct WorkBoardData;

    enum { boarBaudrate = 57600 };

    enum { boadAdress = 0x0C };
    enum { boardIdentification = 0xCC };

    enum { maxBoardConnect = 100 };

private:
    VLCDIndicatorAdapter(const VLCDIndicatorAdapter&);
    VLCDIndicatorAdapter& operator = (const VLCDIndicatorAdapter&);

    StatusBoardData*            m_pStatusBoardData;
    WorkBoardData*              m_pWorkBoardData;

    struct modbus;
    modbus*                     m_modbus;

    std::string                 m_portName;
    int                         m_boardConnect;
};

#endif // V_LCD_INDICATOR_ADAPTER_H
