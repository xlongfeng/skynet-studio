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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QDateTime>

class Settings;

class Options : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString portName MEMBER m_portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(qint32 baudRate MEMBER m_baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(qint32 pollInterval MEMBER m_pollInterval READ pollInterval WRITE setPollInterval NOTIFY pollIntervalChanged)
    Q_PROPERTY(qint32 backlight MEMBER m_backlight READ backlight WRITE setBacklight NOTIFY backlightChanged)

    Q_PROPERTY(bool autoShutdown MEMBER m_autoShutdown READ autoShutdown WRITE setAutoShutdown NOTIFY autoShutdownChanged)
    Q_PROPERTY(QDateTime powerSavingFrom MEMBER m_powerSavingFrom READ powerSavingFrom WRITE setPowerSavingFrom NOTIFY powerSavingFromChanged)
    Q_PROPERTY(QDateTime powerSavingTo MEMBER m_powerSavingTo READ powerSavingTo WRITE setPowerSavingTo NOTIFY powerSavingToChanged)
    Q_PROPERTY(qint32 idleTime MEMBER m_idleTime READ idleTime WRITE setIdleTime NOTIFY idleTimeChanged)

    Q_PROPERTY(bool debug MEMBER m_debug READ debug WRITE setDebug NOTIFY debugChanged)

public:
    enum LinkStatus {
        Disabled,
        Detached,
        Attached,
    };
    Q_ENUM(LinkStatus)

    enum SensorType {
        UnknownSensor,
        WaterlevelSensor,
        UltrasonicSensor,
    };
    Q_ENUM(SensorType)

    static Options *instance();

    Q_INVOKABLE void setHardwareClock(int year, int month, int day, int hour, int minute, int second);

    Q_INVOKABLE QStringList availablePorts() const;
    Q_INVOKABLE QList<QVariant> standardBaudRates() const;

    QString portName() const
    {
        return m_portName;
    }
    void setPortName(const QString &name);

    qint32 baudRate() const
    {
        return m_baudRate;
    }
    void setBaudRate(qint32 rate);

    qint32 pollInterval() const
    {
        return m_pollInterval;
    }

    void setPollInterval(qint32 value);

    Q_INVOKABLE qint32 backlightMin() const
    {
        return 1;
    }
    Q_INVOKABLE qint32 backlightMax();
    Q_INVOKABLE qint32 backlightDefault();
    qint32 backlight() const
    {
        return m_backlight;
    }
    void setBacklight(qint32 value);

    bool autoShutdown() const
    {
        return m_autoShutdown;
    }
    void setAutoShutdown(bool enabled);

    QDateTime powerSavingFrom() const
    {
        return m_powerSavingFrom;
    }
    void setPowerSavingFrom(QDateTime time);

    QDateTime powerSavingTo() const
    {
        return m_powerSavingTo;
    }
    void setPowerSavingTo(QDateTime time);

    qint32 idleTime() const
    {
        return m_idleTime;
    }
    void setIdleTime(qint32 value);

    bool debug() const
    {
        return m_debug;
    }

    void setDebug(bool on);

signals:
    void portNameChanged();
    void baudRateChanged();
    void pollIntervalChanged();
    void backlightChanged();
    void autoShutdownChanged();
    void powerSavingFromChanged();
    void powerSavingToChanged();
    void idleTimeChanged();
    void debugChanged();

public slots:

private:
    explicit Options(QObject *parent = nullptr);
    Q_DISABLE_COPY(Options)
    void setBrightness(qint32 value);

private:
    static Options *self;
    Settings *settings;

    QString m_portName;
    qint32 m_baudRate;
    qint32 m_pollInterval;
    qint32 m_backlight;

    bool m_autoShutdown;
    QDateTime m_powerSavingFrom;
    QDateTime m_powerSavingTo;
    qint32 m_idleTime;

    bool m_debug;
};

#endif // OPTIONS_H
