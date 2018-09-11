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

#include <QDebug>

#include "settings.h"
#include "watertower.h"

namespace {
const int WatertowerQuantity = 6;
}

QMap<int, Watertower *> Watertower::watertowerGroup;

Watertower::Watertower(QObject *parent) :
    QObject(parent),
    index(-1)
{

}

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
    m_bucketQuantity = settings->value("bucket-quantity", 1).toInt();
    m_sensorType = settings->value("sensor-type", WaterlevelSensor).value<SensorType>();
    m_sensorQuantity = settings->value("sensor-quantity", 1).toInt();
}

Watertower::~Watertower()
{
    /* index group */
    settings->endGroup();
    /* Watertower group */
    settings->endGroup();
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

Watertower::WatertowerState Watertower::state() const
{
    return DisabledState;
}

double Watertower::tunnage() const
{
    return 0.1;
}

int Watertower::percent() const
{
    return 40;
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
}

void Watertower::setRadius(int radius)
{
    m_radius = radius;
    settings->setValue("radius", radius);
}

void Watertower::setBucketQuantity(int quantity)
{
    m_bucketQuantity = quantity;
    settings->setValue("bucket-quantity", quantity);
}

void Watertower::setSensorType(SensorType type)
{
    m_sensorType = type;
    settings->setValue("sensor-type", type);
}

void Watertower::setSensorQuantity(int quantity)
{
    m_sensorQuantity = quantity;
    settings->setValue("sensor-quantity", quantity);
}
