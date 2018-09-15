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
const int WatertowerQuantity = 6;
const int QueryInterval = 3000;
const int QeuryTimeout = QueryInterval * 3;
const int AcousticVelocity = 340;
}

QMap<int, Watertower *> Watertower::watertowerGroup;

Watertower::Watertower(int index, QObject *parent) :
    QObject(parent),
    index(index),
    settings(new Settings(this))
{
    settings->beginGroup("Watertower");
    settings->beginGroup(QString::number(index));
    m_identity = settings->value("identity", -1).toInt();
    m_onOff = settings->value("on-off", false).toBool();
    m_radius = settings->value("radius", 200).toInt();
    m_bucketHeight = settings->value("bucket-height", 200).toInt();
    m_bucketQuantity = settings->value("bucket-quantity", 1).toInt();
    m_sensorType = settings->value("sensor-type", Options::WaterlevelSensor).value<Options::SensorType>();
    m_sensorQuantity = settings->value("sensor-quantity", 1).toInt();

    linker = HalfDuplexLinker::instance();
    connect(linker, SIGNAL(response(int,QString,quint16)), this, SLOT(response(int,QString,quint16)));

    queryTimer = new QTimer(this);
    connect(queryTimer, SIGNAL(timeout()), this, SLOT(query()));
    queryTimeoutTimer =  new QTimer(this);
    connect(queryTimeoutTimer, SIGNAL(timeout()), this, SLOT(queryTimeout()));

    if (m_onOff) {
        queryTimer->start(QueryInterval);
        queryTimeoutTimer->start(QeuryTimeout);
    }
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
        return tr("Ground floor");
    case 1:
        return tr("Second floor");
    case 2:
        return tr("Seaside");
    case 3:
        return tr("Well");
    case 4:
        return tr("Boat");
    case 5:
        return tr("Roof");
    default:
        return QString::number(index);
    }
}

const QString Watertower::icon() const
{
    return QString(QString("file:") + IMAGES_PATH + "/watertower-%1.png").arg(index);
}

Options::WatertowerState Watertower::link() const
{
    if (m_onOff) {
        return m_link;
    } else {
        return Options::DisabledState;
    }
}

double Watertower::tunnage() const
{
    return m_tunnage;
}

int Watertower::percent() const
{
    return m_percent;
}

void Watertower::setIdentity(int id)
{
    m_identity = id;
    settings->setValue("identity", m_identity);
}

void Watertower::setOnOff(bool on)
{
    m_onOff = on;
    settings->setValue("on-off", on);
    emit linkChanged();
    if (m_onOff) {
        queryTimer->start(QueryInterval);
        queryTimeoutTimer->start(QeuryTimeout);
    } else {
        queryTimeoutTimer->stop();
        queryTimer->stop();
    }
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
    if (m_identity < 0)
        return;
    linker->request(m_identity, "Query", 0);
}

void Watertower::queryTimeout()
{
    m_link = Options::DisconnectedState;
    emit linkChanged();
}

void Watertower::response(int id, const QString &cmd , quint16 arg)
{
    if (id != m_identity)
        return;

    queryTimeoutTimer->start(QeuryTimeout);

    if (m_link == Options::DisconnectedState) {
        m_link = Options::ConnectedState;
        emit linkChanged();
    }

    if (cmd == "Query") {
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
        // qDebug() << "Identity" << m_identity << waterLevelCM << m_tunnage << m_percent;
    }
}
