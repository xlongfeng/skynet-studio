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
#include <QDebug>

#include "options.h"
#include "halfduplexlinker.h"

namespace {
const int ResponseTimeout = 200;    /* millisecond */
const int ActionLenMax = 16;
}

HalfDuplexLinker *HalfDuplexLinker::self = nullptr;

HalfDuplexLinker::HalfDuplexLinker(QObject *parent) :
    QObject(parent)
{
    options = Options::instance();

    cmdBufInit(&cmdBuf);

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

    QState *idleState = new QState();
    machine.addState(idleState);

    QState *requestState = new QState();
    idleState->addTransition(this, SIGNAL(readyToSend()), requestState);
    requestState->addTransition(this, SIGNAL(queueEmpty()), idleState);
    responseTimeoutTimer = new QTimer(this);
    responseTimeoutTimer->setSingleShot(true);
    connect(responseTimeoutTimer, SIGNAL(timeout()), this, SLOT(onRequestTimeout()));
    requestState->addTransition(responseTimeoutTimer, SIGNAL(timeout()), requestState);
    requestState->addTransition(this, SIGNAL(responseReceived()), requestState);
    connect(requestState, SIGNAL(entered()), this, SLOT(onRequestStateEntered()));
    connect(requestState, SIGNAL(exited()), this, SLOT(onRequestStateExited()));
    machine.addState(requestState);

    machine.setInitialState(idleState);
    machine.start();
}

HalfDuplexLinker *HalfDuplexLinker::instance()
{
    if (self == nullptr) {
        self = new HalfDuplexLinker();
    }

    return self;
}

void HalfDuplexLinker::request(int id, const QString &cmd, quint16 arg)
{
    CmdBuf cmdBuf;
    cmdBufBuild(&cmdBuf, id, cmd.toLatin1().data(), arg);
    packageQueue.enqueue(QString(cmdBuf.buf));
    emit readyToSend();
}

void HalfDuplexLinker::onPortSettingsChanged()
{
    port->close();
    port->setPortName(options->portName());
    port->setBaudRate(options->baudRate());
    port->open(QIODevice::ReadWrite);
}

void HalfDuplexLinker::onReadyRead()
{
    char action[ActionLenMax];
    quint8 addr;
    quint16 arg;
    char cbyte;
    pCmdBuf pCmd = &cmdBuf;

    int bytes = port->bytesAvailable();
    if (bytes > 0) {
        QByteArray buf = port->readAll();
        for (int i = 0; i < buf.size(); i++) {
            cbyte = buf[i];
            if (cbyte == '\r' || cbyte == '\n') {
                cmdBufPushEnd(pCmd);
                if (cmdBufSize(pCmd) > 0) {
                    cmdBufGetAddr(pCmd, &addr);
                    cmdBufGetAction(pCmd, action, ActionLenMax);
                    cmdBufGetArg(pCmd, &arg);
                    if (cmdBufValidation(pCmd) == CMD_BUF_OK) {
                        emit response(addr, action, arg);
                        emit responseReceived();
                    }
                }
                cmdBufReset(pCmd);
            }  else {
                cmdBufPushByte(pCmd, cbyte);
            }
        }
    }
}

void HalfDuplexLinker::onRequestStateEntered()
{
    if(!packageQueue.isEmpty()) {
        QString request = packageQueue.dequeue();
        requestString = request;
        port->write(request.toLatin1());
        responseTimeoutTimer->start(ResponseTimeout);
    } else {
        emit queueEmpty();
    }
}

void HalfDuplexLinker::onRequestStateExited()
{
    responseTimeoutTimer->stop();
}

void HalfDuplexLinker::onRequestTimeout()
{
    qDebug() << QTime::currentTime().toString() << "Request timeout" << requestString;
}
