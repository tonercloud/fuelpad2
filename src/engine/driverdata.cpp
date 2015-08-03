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

#include "driverdata.h"

DriverData::DriverData()
{
}

void DriverData::setFullName(QString name)
{
    fullName = name;
}

QString DriverData::getFullName(void)
{
    return fullName;
}

void DriverData::setNickName(QString nick)
{
    nickName = nick;
}

QString DriverData::getNickName(void)
{
    return nickName;
}

void DriverData::setId(qlonglong Id)
{
    id = Id;
}

qlonglong DriverData::getId(void)
{
    return id;
}
