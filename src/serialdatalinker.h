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

#ifndef SERIALDATALINKER_H
#define SERIALDATALINKER_H

#include "abstractdatalinker.h"

class Options;
class QSerialPort;

class SerialDataLinker : public AbstractDataLinker
{
    Q_OBJECT
public:
    explicit SerialDataLinker(QObject *parent = nullptr);

public slots:
    void send(const QByteArray &bytes) override;

private slots:
    void onPortSettingsChanged();
    void onReadyRead();

private:
    Options *options;
    QSerialPort *port;
};

#endif // SERIALDATALINKER_H
