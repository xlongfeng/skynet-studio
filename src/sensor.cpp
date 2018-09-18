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

#include <cmath>
#include <QTimer>
#include <QDebug>

#include "settings.h"
#include "sensor.h"
#include "halfduplexlinker.h"

namespace {
const int AcousticVelocity = 340;
}

Sensor *Sensor::self = nullptr;

Sensor::Sensor(QObject *parent) :
    QObject(parent)
{
    linker = HalfDuplexLinker::instance();
    connect(linker, SIGNAL(response(int,QString,quint16)), this, SLOT(response(int,QString,quint16)));

    testTimer = new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(test()));
    testTimeroutTimer =  new QTimer(this);
    testTimeroutTimer->setSingleShot(true);
    connect(testTimeroutTimer, SIGNAL(timeout()), this, SLOT(testTimeout()));
}

Sensor *Sensor::instance()
{
    if (self == nullptr) {
        self = new Sensor();
    }

    return self;
}

void Sensor::setIdentity(int id)
{
    if (id > 0)
        linker->request(0x7e, "SetID", id);
}

void Sensor::getIdentity()
{
    linker->request(0x7e, "GetID", 0);
}

void Sensor::setSensorType(Options::SensorType value)
{
    linker->request(m_identity, "SetType", value);
}

void Sensor::getSensorType()
{
    linker->request(m_identity, "GetType", 0);
}

void Sensor::queryDevice()
{
    linker->request(m_identity, "Query", 0);
}

void Sensor::testStart()
{
    m_requestTimes = 0;
    emit requestTimesChanged();
    m_responseTimeoutTimes = 0;
    emit responseTimeoutTimesChanged();
    testTimer->start(500);
    responseLatencyTime.start();
}

void Sensor::testStop()
{
    testTimeroutTimer->stop();
    testTimer->stop();
}

void Sensor::test()
{
    ++m_requestTimes;
    emit requestTimesChanged();
    linker->request(m_identity, "Heartbeat", 1000);
    responseLatencyTime.restart();
    testTimeroutTimer->start(200);
}

void Sensor::testTimeout()
{
    ++m_responseTimeoutTimes;
    emit responseTimeoutTimesChanged();
    m_responseLatency = responseLatencyTime.elapsed();
    emit responseLatencyChanged();
}

void Sensor::response(int id, const QString &cmd , quint16 arg)
{
    if (cmd == "Heartbeat") {
        testTimeroutTimer->stop();
        m_responseLatency = responseLatencyTime.elapsed();
        emit responseLatencyChanged();
    } else if (cmd == "Query") {
        switch (m_sensorType) {
        case Options::WaterlevelSensor:
            break;
        case Options::UltrasonicSensor:
            arg = arg * AcousticVelocity / 10000 / 2;
            break;
        default:
            return;
        }
    } else if (cmd == "SetID" || cmd == "GetID") {
        if (m_identity != arg) {
            m_identity = arg;
            emit identityChanged();
        }
    } else if (cmd == "SetType" || cmd == "GetType") {
        if (m_sensorType != static_cast<Options::SensorType>(arg)) {
            m_sensorType = static_cast<Options::SensorType>(arg);
            emit sensorTypeChanged();
        }
    }
}
