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

#ifndef TESTDATALINKER_H
#define TESTDATALINKER_H

#include <QQueue>

#include "abstractdatalinker.h"

class TestDataLinker : public AbstractDataLinker
{
    Q_OBJECT

public:
    explicit TestDataLinker(QObject *parent = nullptr);
    void appendResponse(int id, const QString &cmd, quint16 arg, bool invalid = false);
    void appendResponse(const QByteArray &data);

public slots:
    void send(const QByteArray &bytes) override;

private:
    QQueue<QByteArray> datagramQueue;
};

#endif // TESTDATALINKER_H
