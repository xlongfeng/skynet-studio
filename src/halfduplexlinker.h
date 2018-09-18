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

class Options;
class QTimer;
class QSerialPort;

class HalfDuplexLinker : public QObject
{
    Q_OBJECT

public:
    static HalfDuplexLinker *instance();
    void request(int id, const QString &cmd, quint16 arg);

signals:
    void response(int id, const QString &cmd , quint16 arg);

    void readyToSend();
    void responseReceived();
    void queueEmpty();

private slots:
    void onPortSettingsChanged();
    void onReadyRead();

    void onRequestStateEntered();
    void onRequestStateExited();

private:
    explicit HalfDuplexLinker(QObject *parent = nullptr);
    Q_DISABLE_COPY(HalfDuplexLinker)

private:
    static HalfDuplexLinker *self;
    Options *options;
    CmdBuf cmdBuf;
    QQueue<QString> packageQueue;
    QSerialPort *port;
    QTimer *responseTimeoutTimer;
    QStateMachine machine;
};

#endif // HALFDUPLEXLINKER_H
