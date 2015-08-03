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

#ifndef ALARMEVENTDATA_H
#define ALARMEVENTDATA_H

#include <QtGlobal>
#include <QString>

class AlarmeventData
{
public:
    AlarmeventData();

    void setId(qlonglong Id);
    qlonglong getId(void);

    void setAlarmId(qlonglong Id);
    qlonglong getAlarmId(void);

    void setCarId(qlonglong Id);
    qlonglong getCarId(void);

    void setRecordId(qlonglong Id);
    qlonglong getRecordId(void);

    void setDate(QString day);
    QString getDate(void);

    void setKm(double kilom);
    double getKm(void);

    void setOil(double Oil);
    double getOil(void);

    void setTires(double Tires);
    double getTires(void);

    void setService(double Service);
    double getService(void);

    void setNotes(QString Notes);
    QString getNotes(void);

private:
    // Database id: "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    qlonglong id;

    // Alarm id: to which alarm type this event belongs to
    qlonglong alarmId;

    // Car id to which car this event belongs to
    qlonglong carId;

    // Link to record in fuel database
    qlonglong recordId;

    // Date when event occured
    QString date;

    // Km when event occured
    double km;

    // Cost of oil
    double oil;

    // Cost of tires
    double tires;

    // Cost of service
    double service;

    // Notes
    QString notes;
};

#endif // ALARMEVENTDATA_H
