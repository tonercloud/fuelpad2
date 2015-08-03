/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012 Julius Luukko <julle.luukko@quicknet.inet.fi>
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

#ifndef CARDATA_H
#define CARDATA_H

#include <QtGlobal>
#include <QString>

class CarData
{
public:
    CarData();
    enum FuelType {
        PETROL,
        DIESEL,
        ETHANOL,
        OTHER
    };
    void setFuelType(enum FuelType ftype);
    enum FuelType getFuelType(void);

    void setId(qlonglong Id);
    qlonglong getId(void);

    void setMark(QString mrk);
    QString getMark(void);

    void setModel(QString mdl);
    QString getModel(void);

    void setYear(quint16 yr);
    quint16 getYear(void);

    void setRegNum(QString reg);
    QString getRegNum(void);

    void setNotes(QString nts);
    QString getNotes(void);


private:
    enum FuelType fuelType;
    qlonglong id;
    QString mark;
    QString model;
    quint16 year;
    QString regNum;
    QString notes;
};

#endif // CARDATA_H
