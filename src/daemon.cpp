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

#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#include <QDebug>

#include "options.h"
#include "daemon.h"

Daemon *Daemon::self = nullptr;

Daemon::Daemon(QObject *parent) :
    QObject(parent)
{
    options = Options::instance();
    QTimer *tickTimer = new QTimer(this);
    connect(tickTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    tickTimer->start(1000);

}

Daemon *Daemon::instance()
{
    if (self == nullptr) {
        self = new Daemon();
    }

    return self;
}

void Daemon::start()
{
    idleTime.start();
}

bool Daemon::isPowerSaving(QDateTime current)
{
    QDateTime from = options->powerSavingFrom();
    QDateTime to = options->powerSavingTo();

    if (to < from) {
        if (current > from || current < to)
            return true;
    } else {
        if (current > from && current < to)
            return true;
    }

    return false;
}

void Daemon::resetIdleTime()
{
    idleTime.restart();
}

void Daemon::onTick()
{
    if (options->autoShutdown() && isPowerSaving()) {
        if (idleTime.elapsed() > options->idleTime() * 60 * 1000) {
#ifdef __arm__
            QProcess::execute("poweroff");
#else
            qDebug() << "Power Saving";
            qApp->quit();
#endif
        }
    }
}
