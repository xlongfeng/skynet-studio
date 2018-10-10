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
#include <QDebug>

#include "halfduplexlinker.h"
#include "serialdatalinker.h"

namespace {
const int ResponseTimeout = 200;    /* millisecond */
const int ActionLenMax = 16;
}

HalfDuplexLinker *HalfDuplexLinker::self = nullptr;

HalfDuplexLinker::HalfDuplexLinker(QObject *parent) :
    QObject(parent)
{
    cmdBufInit(&cmdBuf);

    QState *idleState = new QState();
    machine.addState(idleState);

    QState *requestState = new QState();
    idleState->addTransition(this, SIGNAL(readyToSend()), requestState);
    requestState->addTransition(this, SIGNAL(queueEmpty()), idleState);
    responseTimeoutTimer = new QTimer(this);
    responseTimeoutTimer->setSingleShot(true);
    connect(responseTimeoutTimer, SIGNAL(timeout()), this, SLOT(onRequestTimeout()));
    requestState->addTransition(responseTimeoutTimer, SIGNAL(timeout()), requestState);
    requestState->addTransition(this, SIGNAL(requestNext()), requestState);
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
        self->setDataLinker(new SerialDataLinker());
    }

    return self;
}

void HalfDuplexLinker::setDataLinker(AbstractDataLinker *newLinker)
{
    if (this->linker != nullptr)
        delete this->linker;
    this->linker = newLinker;
    newLinker->setParent(this);
    connect(this->linker, SIGNAL(dataArrived(QByteArray)), this, SLOT(onDataArrived(QByteArray)));
}

void HalfDuplexLinker::request(int id, const QString &cmd, quint16 arg, bool sniffer)
{
    datagramQueue.enqueue({id, cmd, arg, sniffer});
    emit readyToSend();
}

void HalfDuplexLinker::onRequestStateEntered()
{
    if(!datagramQueue.isEmpty()) {
        Datagram request = datagramQueue.dequeue();
        datagramRequested = request;
        CmdBuf cmdBuf;
        cmdBufBuild(&cmdBuf, request.id, request.cmd.toLatin1().data(), request.arg);
        if (request.sniffer) {
            responseTimeoutTimer->start(ResponseTimeout * 6);
        } else {
            linker->send(cmdBuf.buf);
            responseTimeoutTimer->start(ResponseTimeout);
        }
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
    emit timeout(datagramRequested.id, datagramRequested.cmd, datagramRequested.arg);
}

void HalfDuplexLinker::onDataArrived(const QByteArray &bytes)
{
    pCmdBuf pCmd = &cmdBuf;

    for (int i = 0; i < bytes.size(); i++) {
        char cbyte = bytes[i];
        if (cbyte == '\r' || cbyte == '\n') {
            cmdBufPushEnd(pCmd);
            if (cmdBufSize(pCmd) > 0) {
                char action[ActionLenMax];
                quint8 addr;
                quint16 arg;
                cmdBufGetAddr(pCmd, &addr);
                cmdBufGetAction(pCmd, action, ActionLenMax);
                cmdBufGetArg(pCmd, &arg);
                if (cmdBufValidation(pCmd) == CMD_BUF_OK) {
                    emit response(addr, action, arg);
                }
                emit requestNext();
            }
            cmdBufReset(pCmd);
        }  else {
            cmdBufPushByte(pCmd, cbyte);
        }
    }
}
