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

#ifdef __arm__
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#endif

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
    connect(options, SIGNAL(autoShutdownChanged()), this, SLOT(resetIdleTime()));
    connect(options, SIGNAL(powerSavingFromChanged()), this, SLOT(resetIdleTime()));
    connect(options, SIGNAL(powerSavingToChanged()), this, SLOT(resetIdleTime()));
    connect(options, SIGNAL(idleTimeChanged()), this, SLOT(resetIdleTime()));

    QTimer *tickTimer = new QTimer(this);
    connect(tickTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    tickTimer->start(1000);

#ifdef __arm__
    wdg = open("/dev/watchdog", O_WRONLY);

    if (wdg == -1) {
        qDebug() << "Watchdog device not enabled";
    }

    int flags = WDIOS_ENABLECARD;
    ioctl(wdg, WDIOC_SETOPTIONS, &flags);
#endif
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
    keepAlive();
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

void Daemon::keepAlive()
{
#ifdef __arm__
    int dummy;
    ioctl(wdg, WDIOC_KEEPALIVE, &dummy);
#endif
}
