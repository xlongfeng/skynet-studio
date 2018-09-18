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

#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QTime>

#include "options.h"

class HalfDuplexLinker;
class QTimer;

class Sensor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int identity MEMBER m_identity READ identity WRITE setIdentity NOTIFY identityChanged)
    Q_PROPERTY(Options::SensorType sensorType MEMBER m_sensorType READ sensorType WRITE setSensorType NOTIFY sensorTypeChanged)

    Q_PROPERTY(quint32 requestTimes MEMBER m_requestTimes READ requestTimes NOTIFY requestTimesChanged)
    Q_PROPERTY(quint32 responseTimeoutTimes MEMBER m_responseTimeoutTimes READ responseTimeoutTimes NOTIFY responseTimeoutTimesChanged)
    Q_PROPERTY(quint32 responseLatency MEMBER m_responseLatency READ responseLatency NOTIFY responseLatencyChanged)


public:
    static Sensor *instance();

    int identity() const
    {
        return m_identity;
    }
    void setIdentity(int id);
    Q_INVOKABLE void getIdentity();

    Options::SensorType sensorType() const
    {
        return m_sensorType;
    }
    void setSensorType(Options::SensorType value);
    Q_INVOKABLE void getSensorType();

    Q_INVOKABLE void queryDevice();

    Q_INVOKABLE void testStart();
    Q_INVOKABLE void testStop();

    quint32 requestTimes()
    {
        return m_requestTimes;
    }

    quint32 responseTimeoutTimes()
    {
        return m_responseTimeoutTimes;
    }

    quint32 responseLatency()
    {
        return m_responseLatency;
    }

signals:
    void identityChanged();
    void sensorTypeChanged();
    void requestTimesChanged();
    void responseTimeoutTimesChanged();
    void responseLatencyChanged();

private slots:
    void test();
    void testTimeout();
    void response(int id, const QString &cmd , quint16 arg);

private:
    Sensor(QObject *parent = nullptr);
    Q_DISABLE_COPY(Sensor)

private:
    static Sensor *self;
    HalfDuplexLinker *linker;

    int m_identity = 0;
    Options::SensorType m_sensorType = Options::UnknownSensor;

    QTimer *testTimer;
    QTimer *testTimeroutTimer;
    quint32 m_requestTimes = 0;
    quint32 m_responseTimeoutTimes = 0;
    QTime responseLatencyTime;
    quint32 m_responseLatency = 0;
};

#endif // SENSOR_H
