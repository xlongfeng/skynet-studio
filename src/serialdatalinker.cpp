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

#include <QSerialPort>

#include "options.h"
#include "serialdatalinker.h"

SerialDataLinker::SerialDataLinker(QObject *parent) :
    AbstractDataLinker(parent)
{
    options = Options::instance();

    port = new QSerialPort(options->portName(), this);
    port->setBaudRate(options->baudRate());
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->open(QIODevice::ReadWrite);
    connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(options, SIGNAL(portNameChanged()), this, SLOT(onPortSettingsChanged()));
    connect(options, SIGNAL(baudRateChanged()), this, SLOT(onPortSettingsChanged()));
}

void SerialDataLinker::send(const QByteArray &bytes)
{
    port->write(bytes);
}

void SerialDataLinker::onPortSettingsChanged()
{
    port->close();
    port->setPortName(options->portName());
    port->setBaudRate(options->baudRate());
    port->open(QIODevice::ReadWrite);
}

void SerialDataLinker::onReadyRead()
{
    int bytes = port->bytesAvailable();
    if (bytes > 0)
        emit dataArrived(port->readAll());
}
