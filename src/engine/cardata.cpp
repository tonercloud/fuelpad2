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

#include "cardata.h"

CarData::CarData()
{
}

void CarData::setFuelType(enum FuelType ftype)
{
    fuelType = ftype;
}

CarData::FuelType CarData::getFuelType(void)
{
    return fuelType;
}

void CarData::setId(qlonglong Id)
{
    id = Id;
}

qlonglong CarData::getId(void)
{
    return id;
}

void CarData::setMark(QString mrk)
{
    mark = mrk;
}

QString CarData::getMark(void)
{
    return mark;
}

void CarData::setModel(QString mdl)
{
    model = mdl;
}

QString CarData::getModel(void)
{
    return model;
}

void CarData::setYear(quint16 yr)
{
    year = yr;
}

quint16 CarData::getYear(void)
{
    return year;
}

void CarData::setRegNum(QString reg)
{
    regNum = reg;
}

QString CarData::getRegNum(void)
{
    return regNum;
}

void CarData::setNotes(QString nts)
{
    notes = nts;
}

QString CarData::getNotes(void)
{
    return notes;
}
