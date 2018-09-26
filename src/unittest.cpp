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
#include "daemon.h"
#include "halfduplexlinker.h"
#include "testdatalinker.h"

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

    void backlightTest()
    {
        Options *opt = Options::instance();
        qint32 backlightMax = opt->backlightMax();
        qint32 backlightDefault = opt->backlightDefault();
        qDebug() << "backlight max" << backlightMax;
        qDebug() << "backlight default" << backlightDefault;
        qint32 backlight = backlightDefault + backlightMax * 20 / 100;
        qDebug() << "backlight lighter 20%" << backlight;
        opt->setBacklight(backlight);
        // QTest::qWait(2000);
        opt->setBacklight(backlightDefault);
    }

    void powerSavingTest()
    {
        Options *opt = Options::instance();
        Daemon *daemon = Daemon::instance();

        opt->setPowerSavingFrom(QDateTime(QDate(2000, 1, 1), QTime(22, 0)));
        opt->setPowerSavingTo(QDateTime(QDate(2000, 1, 1), QTime(6, 0)));
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(0, 0))) == true);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(4, 0))) == true);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(6, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(8, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(14, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(22, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(23, 0))) == true);

        opt->setPowerSavingFrom(QDateTime(QDate(2000, 1, 1), QTime(6, 0)));
        opt->setPowerSavingTo(QDateTime(QDate(2000, 1, 1), QTime(22, 0)));
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(0, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(4, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(6, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(8, 0))) == true);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(14, 0))) == true);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(22, 0))) == false);
        QVERIFY(daemon->isPowerSaving(QDateTime(QDate(2000, 1, 1), QTime(23, 0))) == false);
    }

    void halfDuplexLinkerTest()
    {
        HalfDuplexLinker *linker = HalfDuplexLinker::instance();
        TestDataLinker *dataLinker = new TestDataLinker();
        linker->setDataLinker(dataLinker);

        QSignalSpy readyToSendSpy(linker, SIGNAL(readyToSend()));
        QSignalSpy requestNextSpy(linker, SIGNAL(requestNext()));
        QSignalSpy queueEmpty(linker, SIGNAL(queueEmpty()));
        QSignalSpy responseSpy(linker, SIGNAL(response(int,QString,quint16)));
        QSignalSpy timeoutSpy(linker, SIGNAL(timeout(int,QString,quint16)));

        /*
         * init
         */
        QTest::qWait(100);

        QCOMPARE(readyToSendSpy.count(), 0);
        QCOMPARE(requestNextSpy.count(), 0);
        QCOMPARE(queueEmpty.count(), 0);
        QCOMPARE(responseSpy.count(), 0);
        QCOMPARE(timeoutSpy.count(), 0);

        /*
         * one request
         */
        linker->request(0x10, "Query", 0);
        QVERIFY(timeoutSpy.wait(250));

        QCOMPARE(readyToSendSpy.count(), 1);
        QCOMPARE(requestNextSpy.count(), 0);
        QCOMPARE(queueEmpty.count(), 1);
        QCOMPARE(responseSpy.count(), 0);
        QCOMPARE(timeoutSpy.count(), 1);

        /*
         * two request
         */
        readyToSendSpy.clear();
        requestNextSpy.clear();
        queueEmpty.clear();
        responseSpy.clear();
        timeoutSpy.clear();

        linker->request(0x10, "Query", 0);
        linker->request(0x10, "Query", 0);
        QTest::qWait(450);

        QCOMPARE(readyToSendSpy.count(), 2);
        QCOMPARE(requestNextSpy.count(), 0);
        QCOMPARE(queueEmpty.count(), 1);
        QCOMPARE(responseSpy.count(), 0);
        QCOMPARE(timeoutSpy.count(), 2);

        /*
         * one request, one response
         */
        readyToSendSpy.clear();
        requestNextSpy.clear();
        queueEmpty.clear();
        responseSpy.clear();
        timeoutSpy.clear();

        dataLinker->appendResponse(0x10, "Query", 33);
        linker->request(0x10, "Query", 0);

        QTest::qWait(100);

        QCOMPARE(readyToSendSpy.count(), 1);
        QCOMPARE(requestNextSpy.count(), 1);
        QCOMPARE(queueEmpty.count(), 1);
        QCOMPARE(responseSpy.count(), 1);
        QCOMPARE(timeoutSpy.count(), 0);

        /*
         * one request, one invalid response
         */
        readyToSendSpy.clear();
        requestNextSpy.clear();
        queueEmpty.clear();
        responseSpy.clear();
        timeoutSpy.clear();

        dataLinker->appendResponse(0x10, "Query", 33, true);
        linker->request(0x10, "Query", 0);

        QTest::qWait(100);

        QCOMPARE(readyToSendSpy.count(), 1);
        QCOMPARE(requestNextSpy.count(), 1);
        QCOMPARE(queueEmpty.count(), 1);
        QCOMPARE(responseSpy.count(), 0);
        QCOMPARE(timeoutSpy.count(), 0);

        /*
         * one request, no response
         * one request, one response
         * one request, one invalid response
         * one request, no response
         */
        readyToSendSpy.clear();
        requestNextSpy.clear();
        queueEmpty.clear();
        responseSpy.clear();
        timeoutSpy.clear();

        linker->request(0x10, "Query", 0);

        dataLinker->appendResponse(0x10, "Query", 33);
        linker->request(0x10, "Query", 0);

        dataLinker->appendResponse(0x10, "Query", 33, true);
        linker->request(0x10, "Query", 0);

        linker->request(0x10, "Query", 0);

        QTest::qWait(500);

        QCOMPARE(readyToSendSpy.count(), 4);
        QCOMPARE(requestNextSpy.count(), 2);
        QCOMPARE(queueEmpty.count(), 1);
        QCOMPARE(responseSpy.count(), 1);
        QCOMPARE(timeoutSpy.count(), 2);
    }
};

QTEST_MAIN(Unittest)
#include "unittest.moc"
