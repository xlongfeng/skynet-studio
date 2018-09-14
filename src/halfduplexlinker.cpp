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

#include <QTimer>
#include <QSerialPort>

#include "options.h"
#include "halfduplexlinker.h"

namespace {
const int ResponseTimeout = 200;    /* millisecond */
}

HalfDuplexLinker *HalfDuplexLinker::self = nullptr;

HalfDuplexLinker::HalfDuplexLinker(QObject *parent) :
    QObject(parent)
{
    options = Options::instance();

    responseTimer = new QTimer(this);
    responseTimer->setSingleShot(true);
    connect(responseTimer, SIGNAL(timeout()), this, SLOT(onResponseTimeout()));

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

HalfDuplexLinker *HalfDuplexLinker::instance()
{
    if (self == nullptr) {
        self = new HalfDuplexLinker();
    }

    return self;
}

void HalfDuplexLinker::request(int id, const QString &cmd, const QString &arg)
{
    QString package = QString("@%1,%2,%3").arg(id, 0, 16).arg(cmd).arg(arg);
    packageQueue.enqueue(package);
    if (!responseTimer->isActive())
        responseTimer->start(5);
}

void HalfDuplexLinker::onPortSettingsChanged()
{
    port->close();
    port->setPortName(options->portName());
    port->setBaudRate(options->baudRate());
    port->open(QIODevice::ReadWrite);
}

void HalfDuplexLinker::onResponseTimeout()
{
    if(!packageQueue.isEmpty()) {
        packageQueue.dequeue();
        responseTimer->start(ResponseTimeout);
    }
}

void HalfDuplexLinker::onReadyRead()
{

}
