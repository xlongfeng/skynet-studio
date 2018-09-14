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

#include <QProcess>
#include <QSerialPortInfo>
#include <QDebug>

#include "settings.h"
#include "options.h"

Options *Options::self = nullptr;

Options::Options(QObject *parent) : QObject(parent),
    settings(new Settings(this))
{
    m_portName = settings->value("port-name", "COM1").toString();
    m_baudRate = settings->value("baud-rate", 38400).toInt();
    m_autoShutdown = settings->value("auto-shutdown", false).toBool();
    m_shutdownHour = settings->value("shutdown-hour", 23).toInt();
    m_shutdownMinute = settings->value("shutdown-minute", 0).toInt();
}

Options *Options::instance()
{
    if (self == nullptr)
        self = new Options();
    return self;
}

void Options::setHardwareClock(int year, int month, int day, int hour, int minute, int second)
{
    const QString &program = "date";
    QStringList options =
            QStringList() << "-s" << QString("%1-%2-%3 %4:%5:%6")
               .arg(year).arg(month).arg(day)
               .arg(hour).arg(minute).arg(second);
    qDebug() << program << options;
#ifdef __arm__
    QProcess::execute(program, options);
    QProcess::execute("hwclock", QStringList() << "-w");
#endif
}

QStringList Options::availablePorts() const
{
    auto ports = QSerialPortInfo::availablePorts();
    QStringList portNames;
    auto cbegin = ports.cbegin();
    auto cend = ports.cend();
    for (auto i = cbegin; i < cend; i++) {
        portNames.append(i->portName());
    }
    return portNames;
}

QList<QVariant> Options::standardBaudRates() const
{
    QList<QVariant> baudRates;
    for (qint32 rate: QSerialPortInfo::standardBaudRates()) {
        if (rate < 2400 || rate > 115200)
            continue;
        baudRates.append(rate);
    }
    return baudRates;
}

void Options::setPortName(const QString &name)
{
    if (m_portName != name) {
        m_portName = name;
        settings->setValue("port-name", m_portName);
        emit portNameChanged();
    }
}

void Options::setBaudRate(qint32 rate)
{
    if (m_baudRate != rate) {
        m_baudRate = rate;
        settings->setValue("baud-rate", m_baudRate);
        emit baudRateChanged();
    }
}

void Options::setAutoShutdown(bool enabled)
{
    if (m_autoShutdown != enabled) {
        m_autoShutdown = enabled;
        settings->setValue("auto-shutdown", m_autoShutdown);
        emit autoShutdownChanged();
    }
}

void Options::setShutdownHour(qint32 value)
{
    if (m_shutdownHour != value) {
        m_shutdownHour = value;
        settings->setValue("shutdown-hour", m_shutdownHour);
        emit shutdownHourChanged();
    }
}

void Options::setShutdownMinute(qint32 value)
{
    if (m_shutdownMinute != value) {
        m_shutdownMinute = value;
        settings->setValue("shutdown-minute", m_shutdownMinute);
        emit shutdownMinuteChanged();
    }
}
