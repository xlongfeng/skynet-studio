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
#include <QFile>
#include <QDebug>

#include "settings.h"
#include "options.h"

Options *Options::self = nullptr;

Options::Options(QObject *parent) : QObject(parent),
    settings(new Settings(this))
{
    m_portName = settings->value("port-name", "COM1").toString();
    m_baudRate = settings->value("baud-rate", 38400).toInt();
    m_pollInterval = settings->value("poll-interval", 3).toInt();
    m_backlight = settings->value("backlight", -1).toInt();
    if (m_backlight == -1) {
        m_backlight = backlightDefault();
    } else {
        setBrightness(m_backlight);
    }
    m_autoShutdown = settings->value("auto-shutdown", false).toBool();
    m_powerSavingFrom = settings->value("power-saving-from", "2000-01-01 23:00").toDateTime();
    m_powerSavingTo = settings->value("power-saving-to", "2000-01-01 06:00").toDateTime();
    m_idleTime = settings->value("idle-time", 15).toInt();
    m_sniffer = settings->value("sniffer", false).toBool();
    m_debug = settings->value("debug", false).toBool();
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
        settings->setValue("port-name", name);
        emit portNameChanged();
    }
}

void Options::setBaudRate(qint32 rate)
{
    if (m_baudRate != rate) {
        m_baudRate = rate;
        settings->setValue("baud-rate", rate);
        emit baudRateChanged();
    }
}

void Options::setPollInterval(qint32 value)
{
    if (m_pollInterval != value) {
        m_pollInterval = value;
        settings->setValue("poll-interval", value);
        emit pollIntervalChanged();
    }
}

qint32 Options::backlightMax()
{
#ifdef __arm__
    QFile file("/sys/class/backlight/backlight-lvds/max_brightness");
#else
    QFile file("/sys/class/backlight/intel_backlight/max_brightness");
#endif
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    QTextStream in(&file);
    qint32 value;
    in >> value;
    file.close();
    return value;
}

qint32 Options::backlightDefault()
{
#ifdef __arm__
    QFile file("/sys/class/backlight/backlight-lvds/actual_brightness");
#else
    QFile file("/sys/class/backlight/intel_backlight/actual_brightness");
#endif
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    QTextStream in(&file);
    qint32 value;
    in >> value;
    file.close();
    return value;
}

void Options::setBacklight(qint32 value)
{
    if (m_backlight != value) {
        m_backlight = value;
        settings->setValue("backlight", value);
        emit backlightChanged();
        setBrightness(m_backlight);
    }
}

void Options::setBrightness(qint32 value)
{
#ifdef __arm__
    QFile file("/sys/class/backlight/backlight-lvds/brightness");
#else
    QFile file("/sys/class/backlight/intel_backlight/brightness");
#endif
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << value;
    file.close();
}

void Options::setAutoShutdown(bool enabled)
{
    if (m_autoShutdown != enabled) {
        m_autoShutdown = enabled;
        settings->setValue("auto-shutdown", enabled);
        emit autoShutdownChanged();
    }
}

void Options::setPowerSavingFrom(QDateTime time)
{
    if (m_powerSavingFrom != time) {
        m_powerSavingFrom = time;
        settings->setValue("power-saving-from", time);
        emit powerSavingFromChanged();
    }
}

void Options::setPowerSavingTo(QDateTime time)
{
    if (m_powerSavingTo != time) {
        m_powerSavingTo = time;
        settings->setValue("power-saving-to", time);
        emit powerSavingToChanged();
    }
}

void Options::setIdleTime(qint32 value)
{
    if (m_idleTime != value) {
        m_idleTime = value;
        settings->setValue("idle-time", value);
        emit idleTimeChanged();
    }
}

void Options::setSniffer(bool on)
{
    if (m_sniffer != on) {
        m_sniffer = on;
        settings->setValue("sniffer", on);
        emit snifferChanged();
    }
}

void Options::setDebug(bool on)
{
    if (m_debug != on) {
        m_debug = on;
        settings->setValue("debug", on);
        emit debugChanged();
    }
}
