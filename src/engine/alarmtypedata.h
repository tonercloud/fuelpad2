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

#ifndef ALARMTYPEDATA_H
#define ALARMTYPEDATA_H

#include <QtGlobal>
#include <QString>

class AlarmtypeData
{
public:
    AlarmtypeData();

    void setId(qlonglong Id);
    qlonglong getId(void);

    void setCarId(qlonglong Id);
    qlonglong getCarId(void);

    void setShortDesc(QString desc);
    QString getShortDesc(void);

    void setLongDesc(QString desc);
    QString getLongDesc(void);

    void setDistance(quint32 dist);
    quint32 getDistance(void);

    void setInterval(quint32 inter);
    quint32 getInterval(void);

    void setLastDate(QString date);
    QString getLastDate(void);

    void setLastKm(double km);
    double getLastKm(void);

    double getNextKm(void);
    QString getNextDate(void);

    bool getKmExpired(double currentKm);
    bool getDateExpired(void);

private:
    qlonglong id;
    qlonglong carId;
    QString shortdesc;
    quint32 distance;
    quint32 interval;
    QString longdesc;
    QString lastDate;
    double lastKm;
};

#endif // ALARMTYPEDATA_H
