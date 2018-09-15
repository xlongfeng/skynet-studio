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

#include "watertower.h"
#include "watertowermodel.h"

WatertowerModel::WatertowerModel(QObject *parent) :
    QAbstractListModel(parent)
{
    for (int row = 0; row < Watertower::count(); row++) {
        Watertower *watertower = Watertower::instance(row);
        connect(watertower, &Watertower::linkStatusChanged, [=] () {
            this->dataChanged(index(row, 0), index(row, 0), { LinkStatusRole });
        });
        connect(watertower, &Watertower::tunnageChanged, [=] () {
            this->dataChanged(index(row, 0), index(row, 0), { TunnageRole });
        });
        connect(watertower, &Watertower::percentChanged, [=] () {
            this->dataChanged(index(row, 0), index(row, 0), { PercentRole });
        });
    }
}

WatertowerModel::~WatertowerModel()
{
    for (int row = 0; row < Watertower::count(); row++) {
        Watertower *watertower = Watertower::instance(row);
        disconnect(watertower, &Watertower::linkStatusChanged, nullptr, nullptr);
        disconnect(watertower, &Watertower::tunnageChanged, nullptr, nullptr);
        disconnect(watertower, &Watertower::percentChanged, nullptr, nullptr);
    }
}

int WatertowerModel::rowCount(const QModelIndex &) const
{
    return Watertower::count();
}

Qt::ItemFlags WatertowerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return WatertowerModel::flags(index) | Qt::ItemIsEditable;
}

QVariant WatertowerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    switch (role) {
    case NameRole:
        return Watertower::instance(row)->name();
    case IconRole:
        return Watertower::instance(row)->icon();
    case LinkStatusRole:
        return Watertower::instance(row)->linkStatus();
    case TunnageRole:
        return QString::number(Watertower::instance(row)->tunnage(), 'f', 1);
    case PercentRole:
        return Watertower::instance(row)->percent();
    case IdentityRole:
        return Watertower::instance(row)->identity();
    case OnOffRole:
        return Watertower::instance(row)->onOff();
    case RadiusRole:
        return Watertower::instance(row)->radius();
    case BucketHeightRole:
        return Watertower::instance(row)->bucketHeight();
    case BucketQuantityRole:
        return Watertower::instance(row)->bucketQuantity();
    case SensorTypeRole:
        return Watertower::instance(row)->sensorType();
    case SensorQuantityRole:
        return Watertower::instance(row)->sensorQuantity();
    default:
        return QVariant();
    }
}

bool WatertowerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int row = index.row();
    switch (role) {
    case IdentityRole:
        Watertower::instance(row)->setIdentity(value.toInt());
        break;
    case OnOffRole:
        Watertower::instance(row)->setOnOff(value.toBool());
        break;
    case RadiusRole:
        Watertower::instance(row)->setRadius(value.toInt());
        break;
    case BucketHeightRole:
        Watertower::instance(row)->setBucketHeight(value.toInt());
        break;
    case BucketQuantityRole:
        Watertower::instance(row)->setBucketQuantity(value.toInt());
        break;
    case SensorTypeRole:
        Watertower::instance(row)->setSensorType(qvariant_cast<Options::SensorType>(value));
        break;
    case SensorQuantityRole:
        Watertower::instance(row)->setSensorQuantity(value.toInt());
        break;
    default:
        return false;
    }

    return true;
}

QHash<int, QByteArray> WatertowerModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { NameRole, "name" },
        { IconRole, "icon" },
        { LinkStatusRole, "linkStatus" },
        { TunnageRole, "tunnage" },
        { PercentRole, "percent" },
        { IdentityRole, "identity" },
        { OnOffRole, "onOff" },
        { RadiusRole, "radius" },
        { BucketHeightRole, "bucketHeight" },
        { BucketQuantityRole, "bucketQuantity" },
        { SensorTypeRole, "sensorType" },
        { SensorQuantityRole, "sensorQuantity" }
    };

    return roles;
}
