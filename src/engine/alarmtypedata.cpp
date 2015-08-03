/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012,2014 Julius Luukko <julle.luukko@quicknet.inet.fi>
 *
 * Fuelpad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fuelpad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fuelpad.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "alarmtypedata.h"

#include <QDate>

AlarmtypeData::AlarmtypeData()
{
}

void AlarmtypeData::setId(qlonglong Id)
{
    id = Id;
}

qlonglong AlarmtypeData::getId(void)
{
    return id;
}

void AlarmtypeData::setCarId(qlonglong Id)
{
    carId = Id;
}

qlonglong AlarmtypeData::getCarId(void)
{
    return carId;
}

void AlarmtypeData::setShortDesc(QString desc)
{
    shortdesc = desc;
}

QString AlarmtypeData::getShortDesc(void)
{
    return shortdesc;
}

void AlarmtypeData::setLongDesc(QString desc)
{
    longdesc = desc;
}

QString AlarmtypeData::getLongDesc(void)
{
    return longdesc;
}

void AlarmtypeData::setDistance(quint32 dist)
{
    distance = dist;
}

quint32 AlarmtypeData::getDistance(void)
{
    return distance;
}

void AlarmtypeData::setInterval(quint32 inter)
{
    interval = inter;
}

quint32 AlarmtypeData::getInterval(void)
{
    return interval;
}

void AlarmtypeData::setLastDate(QString date)
{
    lastDate = date;
}

QString AlarmtypeData::getLastDate(void)
{
    return lastDate;
}

void AlarmtypeData::setLastKm(double km)
{
    lastKm = km;
}

double AlarmtypeData::getLastKm(void)
{
    return lastKm;
}

double AlarmtypeData::getNextKm(void)
{
    // @todo Add estimation of next km based on interval
    return lastKm+distance;
}

QString AlarmtypeData::getNextDate(void)
{
    QDate nextDate(QDate::fromString(lastDate, "yyyy-MM-dd"));

    // @todo Add estimation of next date based on recent driving
    return nextDate.addMonths(interval).toString("yyyy-MM-dd");
}

bool AlarmtypeData::getKmExpired(double currentKm)
{
    return getNextKm() < currentKm;
}

bool AlarmtypeData::getDateExpired(void)
{
    QDate today = QDate::currentDate();

    return QDate(QDate::fromString(getNextDate(), "yyyy-MM-dd")) < today;
}
