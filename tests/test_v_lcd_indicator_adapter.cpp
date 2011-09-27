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

#include <test_v_lcd_indicator_adapter.h>

#include <QtTest/QtTest>

#include <vector>

const QString TestVLCDIndicatorAdapter::connectionError = "    Connect faild ERROR    ";
const QString TestVLCDIndicatorAdapter::writeDataError  = "    Write data ERROR    ";
const QString TestVLCDIndicatorAdapter::readDataError = "    Read data ERROR    ";
const QString TestVLCDIndicatorAdapter::deadTimeError = "    Dead Time ERROR    ";

void TestVLCDIndicatorAdapter::initTestCase()
{
    m_adapter = new VLCDIndicatorAdapter();

    std::string myport=("/dev/ttyUSB0");
    qDebug() << myport.c_str();

    if(!m_adapter->connectToPort(myport)) {
        QSKIP(connectionError.toAscii(),SkipAll);
    }
    else {
        if(!m_adapter->writeData())
            QSKIP(writeDataError.toAscii(), SkipAll);
        qDebug() << "\n";
    }
}

void TestVLCDIndicatorAdapter::cleanupTestCase()
{
    if(m_adapter->connectStatus()) {
        m_adapter->disconnectFromPort();
    }
    qDebug() << "\n";

    delete m_adapter;
}

void TestVLCDIndicatorAdapter::init()
{
    qDebug() << "init\n";
    QTest::qSleep(testTimeOut/2);
}

void TestVLCDIndicatorAdapter::cleanup()
{
    qDebug() << "cleanup\n";
    QTest::qSleep(testTimeOut/2);
}

void TestVLCDIndicatorAdapter::testDisplay()
{
    qDebug() << "Printf on display";
    m_adapter->setString("888");
    std::vector<bool> point(3, false);
    m_adapter->setPoints(point);
    QVERIFY2(m_adapter->writeData(), writeDataError.toAscii());
    QTest::qSleep(displyTimeOut);
}

void TestVLCDIndicatorAdapter::testError()
{
    m_adapter->readState();
    qDebug() << m_adapter->stillAlive();
}

void TestVLCDIndicatorAdapter::testReadPerformanceState()
{
    QBENCHMARK {
        m_adapter->readState();
    }
}

void TestVLCDIndicatorAdapter::testWritePerformanceState()
{
    QBENCHMARK {
        m_adapter->writeData();
    }
}

QTEST_MAIN(TestVLCDIndicatorAdapter)
