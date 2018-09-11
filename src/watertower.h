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

class Settings;

class Watertower : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int identity READ identity MEMBER m_identity WRITE setIdentity)
    Q_PROPERTY(bool onOff READ onOff MEMBER m_onOff WRITE setOnOff)
    Q_PROPERTY(int radius READ radius MEMBER m_radius WRITE setRadius)
    Q_PROPERTY(int bucketQuantity READ bucketQuantity MEMBER m_bucketQuantity WRITE setBucketQuantity)
    Q_PROPERTY(SensorType sensorType READ sensorType MEMBER m_sensorType WRITE setSensorType)
    Q_PROPERTY(int sensorQuantity READ sensorQuantity MEMBER m_sensorQuantity WRITE setSensorQuantity)

public:
    enum WatertowerState {
        DisabledState,
        DisconnectedState,
        ConnectedState,
    };
    Q_ENUM(WatertowerState)

    enum SensorType {
        WaterlevelSensor,
        UltrasonicSensor,
    };
    Q_ENUM(SensorType)

    explicit Watertower(QObject *parent = nullptr);
    ~Watertower();

    static Watertower *instance(int index);
    static int count();

    const QString name() const;
    const QString icon() const;
    WatertowerState state() const;
    double tunnage() const;
    int percent() const;

    int identity()
    {
        return m_identity;
    }

    void setIdentity(int id);

    bool onOff()
    {
        return m_onOff;
    }

    void setOnOff(bool on);

    int radius()
    {
        return m_radius;
    }

    void setRadius(int radius);

    int bucketQuantity()
    {
        return m_bucketQuantity;
    }

    void setBucketQuantity(int quantity);

    SensorType sensorType()
    {
        return m_sensorType;
    }

    void setSensorType(SensorType type);

    int sensorQuantity()
    {
        return m_sensorQuantity;
    }

    void setSensorQuantity(int quantity);

signals:
    void stateChanged();
    void tunnageChanged();
    void percentChanged();

public slots:

private:
    Watertower(int index, QObject *parent = nullptr);
    Q_DISABLE_COPY(Watertower)

private:
    static QMap<int, Watertower *> watertowerGroup;

    int index;

    Settings *settings;

    int m_identity;
    bool m_onOff;
    int m_radius;
    int m_bucketQuantity;
    SensorType m_sensorType;
    int m_sensorQuantity;
};

#endif // WATERTOWER_H
