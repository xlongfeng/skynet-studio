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

#ifndef WATERTOWER_H
#define WATERTOWER_H

#include <QObject>
#include <QMap>

#include "options.h"

class HalfDuplexLinker;
class QTimer;

class Watertower : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString icon READ icon)
    Q_PROPERTY(int identity MEMBER m_identity READ identity)
    Q_PROPERTY(Options::LinkStatus linkStatus MEMBER m_linkStatus READ linkStatus NOTIFY linkStatusChanged)
    Q_PROPERTY(double tunnage MEMBER m_tunnage READ tunnage NOTIFY tunnageChanged)
    Q_PROPERTY(int percent MEMBER m_percent READ percent NOTIFY percentChanged)
    Q_PROPERTY(int requestTimes MEMBER m_requestTimes READ requestTimes NOTIFY requestTimesChanged)
    Q_PROPERTY(int timeoutTimes MEMBER m_timeoutTimes READ timeoutTimes NOTIFY timeoutTimesChanged)

    Q_PROPERTY(bool onOff MEMBER m_onOff READ onOff WRITE setOnOff)
    Q_PROPERTY(int radius MEMBER m_radius READ radius WRITE setRadius)
    Q_PROPERTY(int bucketHeight MEMBER m_bucketHeight READ bucketHeight MEMBER m_bucketHeight WRITE setBucketHeight)
    Q_PROPERTY(int bucketQuantity MEMBER m_bucketQuantity READ bucketQuantity WRITE setBucketQuantity)
    Q_PROPERTY(Options::SensorType sensorType MEMBER m_sensorType READ sensorType WRITE setSensorType)
    Q_PROPERTY(int sensorQuantity MEMBER m_sensorQuantity READ sensorQuantity WRITE setSensorQuantity)

public:
    static Watertower *instance(int index);
    static int count();

    const QString name() const;
    const QString icon() const;

    int identity() const
    {
        return m_identity;
    }

    Options::LinkStatus linkStatus() const
    {
        if (m_onOff) {
            return m_linkStatus;
        } else {
            return Options::Disabled;
        }
    }

    double tunnage() const
    {
        return m_tunnage;
    }

    int percent() const
    {
        return m_percent;
    }

    int requestTimes() const
    {
        return m_requestTimes;
    }

    int timeoutTimes() const
    {
        return m_timeoutTimes;
    }

    bool onOff() const
    {
        return m_onOff;
    }

    void setOnOff(bool on);

    int radius() const
    {
        return m_radius;
    }

    void setRadius(int radius);

    int bucketHeight() const
    {
        return m_bucketHeight;
    }

    void setBucketHeight(int value);

    int bucketQuantity() const
    {
        return m_bucketQuantity;
    }

    void setBucketQuantity(int quantity);

    Options::SensorType sensorType() const
    {
        return m_sensorType;
    }

    void setSensorType(Options::SensorType type);

    int sensorQuantity() const
    {
        return m_sensorQuantity;
    }

    void setSensorQuantity(int quantity);

signals:
    void linkStatusChanged();
    void tunnageChanged();
    void percentChanged();
    void requestTimesChanged();
    void timeoutTimesChanged();

public slots:

private slots:
    void query();
    void onQueryTimeChanged();
    void response(int id, const QString &cmd , quint16 arg);
    void responseTimeout(int id, const QString &cmd , quint16 arg);

private:
    Watertower(int index, QObject *parent = nullptr);
    Q_DISABLE_COPY(Watertower)

private:
    static QMap<int, Watertower *> watertowerGroup;

    int index;
    int m_identity;

    Settings *settings;
    Options *options;

    Options::LinkStatus m_linkStatus = Options::Detached;

    double m_tunnage = 0.0;
    int m_percent = 0;
    int m_requestTimes = 0;
    int m_timeoutTimes = 0;

    bool m_onOff;
    int m_radius;
    int m_bucketHeight;
    int m_bucketQuantity;
    Options::SensorType m_sensorType;
    int m_sensorQuantity;

    HalfDuplexLinker *linker;
    QTimer *queryTimer;
    qint32 queryIntervalSec;
    qint32 queryTimeoutTimes = 0;
};

#endif // WATERTOWER_H
