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

#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QDateTime>

class Daemon : public QObject
{
    Q_OBJECT

public:
    static Daemon *instance();
    void start();
    bool isPowerSaving(QDateTime current = QDateTime(QDate(2000, 1, 1), QTime::currentTime()));

signals:

public slots:
    void resetIdleTime();

private slots:
    void onTick();

private:
    explicit Daemon(QObject *parent = nullptr);
    Q_DISABLE_COPY(Daemon)

private:
    static Daemon *self;
    QTime idleTime;
};

#endif // DAEMON_H
