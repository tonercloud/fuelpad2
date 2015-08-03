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

#ifndef DATAFIELD_H
#define DATAFIELD_H

#include <QVariant>

#include "unitsystem.h"

class Datafield
{
public:
    Datafield(UnitSystem u);
    enum fieldtype {
        DATE = 0,
        LENGTH,
        VOLUME,
        CONSUMPTION,
        PRICE,
        FUELTYPE,
        PRICEPERTRIP,
        PRICEPERLITRE,
        CO2EMISSION,
        LATITUDE,
        LONGITUDE,
        PLACE,
        NOTE,
        ID,
        NUM_FIELDS
    };
    void setValue(QVariant val, enum fieldtype t);
    QVariant getValue(void);

    void setValueUserUnit(QVariant val, enum fieldtype t);
    QVariant getValueUserUnit(void);
private:
    QVariant value;
    enum fieldtype type;
    UnitSystem unit;
};

#endif // DATAFIELD_H
