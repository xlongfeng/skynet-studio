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

#ifndef WATERTOWERLISTMODEL_H
#define WATERTOWERLISTMODEL_H

#include <QAbstractListModel>

class WatertowerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum WatertowerRole {
        NameRole = Qt::DisplayRole,
        IconRole = Qt::UserRole,
        LinkRole,
        TunnageRole,
        PercentRole,
        IdentityRole,
        OnOffRole,
        RadiusRole,
        BucketHeightRole,
        BucketQuantityRole,
        SensorTypeRole,
        SensorQuantityRole,
    };
    Q_ENUM(WatertowerRole)

    explicit WatertowerModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // WATERTOWERLISTMODEL_H
