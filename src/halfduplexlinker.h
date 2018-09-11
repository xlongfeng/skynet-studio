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
#include <QQueue>

class QTimer;
class QSerialPort;

class HalfDuplexLinker : public QObject
{
    Q_OBJECT

public:
    static HalfDuplexLinker *instance();
    void request(int id, const QString &cmd, const QString &arg);

signals:
    void response(int id, const QString &cmd , const QString &arg);

private slots:
    void onResponseTimeout();
    void onReadyRead();

private:
    explicit HalfDuplexLinker(QObject *parent = nullptr);
    Q_DISABLE_COPY(HalfDuplexLinker)

private:
    static HalfDuplexLinker *self;
    QQueue<QString> packageQueue;

    QTimer *responseTimer;
    QSerialPort *port;
};

#endif // HALFDUPLEXLINKER_H
