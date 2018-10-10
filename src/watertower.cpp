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
#include "watertower.h"
#include "halfduplexlinker.h"

namespace {
const int IdentityBase = 0x10;
const int WatertowerQuantity = 6;
const quint16 InvalidArg = 0xffff;
const int AcousticVelocity = 340;
}

QMap<int, Watertower *> Watertower::watertowerGroup;

Watertower::Watertower(int index, QObject *parent) :
    QObject(parent),
    index(index),
    settings(new Settings(this))
{
    m_identity = IdentityBase + index;

    options = Options::instance();
    connect(options, SIGNAL(pollIntervalChanged()), this, SLOT(onQueryTimeChanged()));

    settings->beginGroup("Watertower");
    settings->beginGroup(QString::number(index));
    m_onOff = settings->value("on-off", false).toBool();
    m_radius = settings->value("radius", 150).toInt();
    m_bucketHeight = settings->value("bucket-height", 200).toInt();
    m_bucketQuantity = settings->value("bucket-quantity", 1).toInt();
    m_sensorType = settings->value("sensor-type", Options::WaterlevelSensor).value<Options::SensorType>();
    m_sensorQuantity = settings->value("sensor-quantity", 4).toInt();

    linker = HalfDuplexLinker::instance();
    connect(linker, SIGNAL(response(int,QString,quint16)), this, SLOT(response(int,QString,quint16)));
    connect(linker, SIGNAL(timeout(int,QString,quint16)), this, SLOT(responseTimeout(int,QString,quint16)));

    queryTimer = new QTimer(this);
    connect(queryTimer, SIGNAL(timeout()), this, SLOT(query()));

    onQueryTimeChanged();
}

Watertower *Watertower::instance(int index)
{
    Watertower *watertower = nullptr;

    if (index < WatertowerQuantity) {
        auto iter = watertowerGroup.find(index);
        if (iter != watertowerGroup.end()) {
            watertower = iter.value();
        } else {
            watertower = new Watertower(index);
            watertowerGroup[index] = watertower;
        }
    }

    return watertower;
}

int Watertower::count()
{
    return WatertowerQuantity;
}

const QString Watertower::name() const
{
    switch(index) {
    case 0:
        return tr("1");
    case 1:
        return tr("2");
    case 2:
        return tr("3");
    case 3:
        return tr("4");
    case 4:
        return tr("5");
    case 5:
        return tr("6");
    default:
        return QString::number(index);
    }
}

const QString Watertower::icon() const
{
    return QString(QString("file:") + IMAGES_PATH + "/watertower-%1.png").arg(index + 1);
}

void Watertower::setOnOff(bool on)
{
    m_onOff = on;
    settings->setValue("on-off", on);
    emit linkStatusChanged();
    onQueryTimeChanged();
}

void Watertower::setRadius(int radius)
{
    m_radius = radius;
    settings->setValue("radius", radius);
}

void Watertower::setBucketHeight(int value)
{
    if (m_bucketHeight != value) {
        m_bucketHeight = value;
        settings->setValue("bucket-height", value);
    }
}

void Watertower::setBucketQuantity(int quantity)
{
    m_bucketQuantity = quantity;
    settings->setValue("bucket-quantity", quantity);
}

void Watertower::setSensorType(Options::SensorType type)
{
    m_sensorType = type;
    settings->setValue("sensor-type", type);
}

void Watertower::setSensorQuantity(int quantity)
{
    m_sensorQuantity = quantity;
    settings->setValue("sensor-quantity", quantity);
}

void Watertower::query()
{
    linker->request(m_identity, "Query", InvalidArg, Options::instance()->sniffer());
    ++m_requestTimes;
    emit requestTimesChanged();
}

void Watertower::onQueryTimeChanged()
{
    queryIntervalSec = options->pollInterval() * 1000;
    queryTimeoutTimes = 0;

    if (m_onOff) {
        queryTimer->start(queryIntervalSec);
    } else {
        queryTimer->stop();
    }
}

void Watertower::response(int id, const QString &cmd , quint16 arg)
{
    if (id != m_identity)
        return;

    queryTimeoutTimes = 0;

    if (m_linkStatus != Options::Attached) {
        m_linkStatus = Options::Attached;
        emit linkStatusChanged();
    }

    if (cmd == "Query") {
        if (arg == InvalidArg) {
            return;
        }

        int waterLevelCM;

        switch (m_sensorType) {
        case Options::WaterlevelSensor:
            waterLevelCM = m_bucketHeight * arg / m_sensorQuantity;
            break;
        case Options::UltrasonicSensor:
            waterLevelCM = m_bucketHeight - arg * AcousticVelocity / 10000 / 2;
            break;
        default:
            return;
        }

        waterLevelCM = waterLevelCM > m_bucketHeight ? m_bucketHeight : waterLevelCM;
        waterLevelCM = waterLevelCM < 0 ? 0 : waterLevelCM;
        m_tunnage = 3.1415926 * pow(m_radius / 100.0, 2) * (waterLevelCM / 100.0) * m_bucketQuantity;
        emit tunnageChanged();
        m_percent = waterLevelCM * 100 / m_bucketHeight;
        emit percentChanged();
        // qDebug() << "Identity" << m_identity << arg << waterLevelCM << m_tunnage << m_percent;
    }
}

void Watertower::responseTimeout(int id, const QString &cmd , quint16 arg)
{
    if (id != m_identity)
        return;

    ++m_timeoutTimes;
    emit timeoutTimesChanged();

    if (++queryTimeoutTimes > 2) {
        queryTimeoutTimes = 2;
        m_linkStatus = Options::Detached;
        emit linkStatusChanged();
    }
    // qDebug() << "Response timeout" << id << cmd << arg << queryTimeoutTimes;
}
