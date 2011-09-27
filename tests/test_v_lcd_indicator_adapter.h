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

#ifndef TEST_V_LCD_INDICATOR_ADAPTER_H
#define TEST_V_LCD_INDICATOR_ADAPTER_H

#include <QObject>
#include <v_lcd_indicator_adapter.h>

class TestVLCDIndicatorAdapter : public QObject
{
    Q_OBJECT

public:
    TestVLCDIndicatorAdapter() : m_adapter(0) { }

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDisplay();
    void testError();

    void testReadPerformanceState();
    void testWritePerformanceState();

protected:
    enum { timeScale = 1 };
    enum { testTimeOut = 500/timeScale };
    enum { displyTimeOut = 1500/timeScale };

    enum { testIteration = 10 };

    static const QString connectionError;
    static const QString writeDataError;
    static const QString readDataError;
    static const QString deadTimeError;

private:
    VLCDIndicatorAdapter *m_adapter;
};

#endif // TEST_V_LCD_INDICATOR_ADAPTER_H
