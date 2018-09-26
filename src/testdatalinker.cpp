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

#include <QDebug>

#include "testdatalinker.h"
#include "utility.h"

TestDataLinker::TestDataLinker(QObject *parent) :
    AbstractDataLinker(parent)
{

}

void TestDataLinker::appendResponse(int id, const QString &cmd, quint16 arg, bool invalid)
{
    CmdBuf cmdBuf;
    if (invalid)
        cmdBufBuildInvalidCrc(&cmdBuf, id, cmd.toLatin1().data(), arg);
    else
        cmdBufBuild(&cmdBuf, id, cmd.toLatin1().data(), arg);
    datagramQueue.enqueue(cmdBuf.buf);
}

void TestDataLinker::appendResponse(const QByteArray &data)
{
    datagramQueue.enqueue(data);

}

void TestDataLinker::send(const QByteArray &bytes)
{
    qDebug() << "DataLinker Send:" << bytes;

    if (!datagramQueue.isEmpty()) {
        QByteArray data = datagramQueue.dequeue();
        emit dataArrived(data);
    }
}
