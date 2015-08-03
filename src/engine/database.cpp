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

#include "database.h"

Database::Database()
{
    carDataValid = false;
}

void Database::setFileName(string name)
{
    fileName = QString::fromStdString(name);
}

const string Database::getFileName(void)
{
    return fileName.toStdString();
}

void Database::setCurrentCar(int id)
{
    carData.setId(id);
}

CarData Database::getCurrentCar(void)
{
    return carData;
}

void Database::setCurrentDriver(int id)
{
    driverData.setId(id);
}

DriverData Database::getCurrentDriver(void)
{
    return driverData;
}
