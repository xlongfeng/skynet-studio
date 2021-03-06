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

#ifndef HALFDUPLEXLINKER_H
#define HALFDUPLEXLINKER_H

#include <QObject>
#include <QStateMachine>
#include <QQueue>

#include "utility.h"

class QTimer;
class AbstractDataLinker;

class HalfDuplexLinker : public QObject
{
    Q_OBJECT

public:
    static HalfDuplexLinker *instance();
    void setDataLinker(AbstractDataLinker *newLinker);
    void request(int id, const QString &cmd, quint16 arg, bool sniffer = false);

signals:
    void readyToSend();
    void requestNext();
    void queueEmpty();
    void response(int id, const QString &cmd , quint16 arg);
    void timeout(int id, const QString &cmd , quint16 arg);

private:
    struct Datagram {
        int id;
        QString cmd;
        quint16 arg;
        bool sniffer;
    };

private slots:
    void onRequestStateEntered();
    void onRequestStateExited();
    void onRequestTimeout();
    void onDataArrived(const QByteArray &bytes);

private:
    explicit HalfDuplexLinker(QObject *parent = nullptr);
    Q_DISABLE_COPY(HalfDuplexLinker)

private:
    static HalfDuplexLinker *self;
    AbstractDataLinker *linker = nullptr;
    CmdBuf cmdBuf;
    QQueue<Datagram> datagramQueue;
    QTimer *responseTimeoutTimer;
    Datagram datagramRequested;
    QStateMachine machine;
};

#endif // HALFDUPLEXLINKER_H
