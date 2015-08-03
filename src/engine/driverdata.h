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

#ifndef DRIVERDATA_H
#define DRIVERDATA_H

#include <QString>

class DriverData
{
public:
    DriverData();

    void setFullName(QString name);
    QString getFullName(void);

    void setNickName(QString nick);
    QString getNickName(void);

    void setId(qlonglong Id);
    qlonglong getId(void);

private:
    QString fullName;
    QString nickName;
    qlonglong id;
};

#endif // DRIVERDATA_H
