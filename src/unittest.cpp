/*
 * skynet studio
 *
 * Copyright (C) 2018 xlongfeng <xlongfeng@126.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QTest>
#include <QSignalSpy>
#include <QDebug>

#include "settings.h"
#include "options.h"

class Unittest: public QObject
{
    Q_OBJECT

private slots:
    void settingsTest()
    {
        Settings *settings = new Settings();
        qDebug() << settings->fileName();
    }

    void optionsInstanceTest()
    {
        Options *opt1 = Options::instance();
        QVERIFY(opt1 != nullptr);
        Options *opt2 = Options::instance();
        QVERIFY(opt2 != nullptr);
        QVERIFY(opt1 == opt2);
    }

    void setHardwareClockTest()
    {
        Options *opt = Options::instance();
        opt->setHardwareClock(2018, 8, 5, 13, 55, 20);
    }

    void availablePortsTest()
    {
        Options *opt = Options::instance();
        auto portNames = opt->availablePorts();
        for (auto name: portNames) {
            qDebug() << name;
        }
    }

    void standardBaudRatesTest()
    {
        Options *opt = Options::instance();
        auto standardBaudRates = opt->standardBaudRates();
        for (auto rate: standardBaudRates) {
            qDebug() << rate;
        }
    }

    void portSettingsTest()
    {
        Options *opt = Options::instance();
        opt->setPortName("COMX");
        opt->setBaudRate(1200);
        QSignalSpy portNameChangedSpy(opt, SIGNAL(portNameChanged()));
        QSignalSpy baudRateChangedSpy(opt, SIGNAL(baudRateChanged()));
        opt->setPortName("COM1");
        opt->setBaudRate(38400);
        QCOMPARE(opt->portName(), "COM1");
        QCOMPARE(opt->baudRate(), 38400);
        QCOMPARE(portNameChangedSpy.count(), 1);
        QCOMPARE(baudRateChangedSpy.count(), 1);
        opt->setPortName("COM2");
        opt->setBaudRate(38400);
        QCOMPARE(portNameChangedSpy.count(), 2);
        QCOMPARE(baudRateChangedSpy.count(), 1);
    }
};

QTEST_MAIN(Unittest)
#include "unittest.moc"
