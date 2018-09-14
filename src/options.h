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

class Settings;

class Options : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString portName MEMBER m_portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(qint32 baudRate MEMBER m_baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)

public:
    static Options *instance();

    Q_INVOKABLE void setHardwareClock(int year, int month, int day, int hour, int minute, int second);

    Q_INVOKABLE QList<QString> availablePorts() const;
    Q_INVOKABLE QList<qint32> standardBaudRates() const;

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

signals:
    void portNameChanged();
    void baudRateChanged();

public slots:

private:
    explicit Options(QObject *parent = nullptr);
    Q_DISABLE_COPY(Options)

private:
    static Options *self;
    Settings *settings;

    QString m_portName;
    qint32 m_baudRate;
};

#endif // OPTIONS_H
