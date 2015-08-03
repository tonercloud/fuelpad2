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

#include "datafield.h"

Datafield::Datafield(UnitSystem u) : unit(u)
{
}

void Datafield::setValue(QVariant val, enum fieldtype t)
{
    value = val;
    type = t;
}

QVariant Datafield::getValue(void)
{
    return value;
}

// @todo Not finished
void Datafield::setValueUserUnit(QVariant val, enum fieldtype t)
{
    type = t;

    switch (type) {
    case DATE:
        value.setValue(val);
        break;
    case LENGTH:
        value.setValue(val.toDouble()*unit.getLengthConversionFactor());
        break;
    case VOLUME:
        value.setValue(val.toDouble()*unit.getVolumeConversionFactor());
        break;
    case CONSUMPTION:
        if (unit.getConsumeUnit() == UnitSystem::SI) {
            value.setValue(val);
        }
        else {
            value.setValue(unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/val.toDouble());
        }
        break;
    case PRICE:
        value.setValue(val);
        break;
    case FUELTYPE:
        value.setValue(val);
        break;
    case PRICEPERTRIP:
        value.setValue(val.toDouble()/unit.getLengthConversionFactor());
        break;
    case PRICEPERLITRE:
        value.setValue(val.toDouble()/unit.getVolumeConversionFactor());
        break;
    case CO2EMISSION:
        if (unit.getConsumeUnit() == UnitSystem::SI) {
            value.setValue(val); // @todo Still missing the right conversion
        }
        else {
            value.setValue(val); // @todo Still missing the right conversion
        }
        break;
    case LATITUDE:
    case LONGITUDE:
    case PLACE:
    case NOTE:
        value.setValue(val);
        break;
    case ID:
        value.setValue(val);
        break;
    }
}

QVariant Datafield::getValueUserUnit(void)
{
    QVariant retVal;

    switch (type) {
    case DATE:
        retVal = value;
        break;
    case LENGTH:
        retVal = value;
        retVal.setValue(retVal.toDouble()/unit.getLengthConversionFactor());
        break;
    case VOLUME:
        retVal = value;
        retVal.setValue(retVal.toDouble()/unit.getVolumeConversionFactor());
        break;
    case CONSUMPTION:
        retVal = value;
        if (unit.getConsumeUnit() != UnitSystem::SI) {
            retVal.setValue(unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/retVal.toDouble());
        }
        break;
    case PRICE:
        retVal = value;
        break;
    case FUELTYPE:
        retVal = value;
        break;
    case PRICEPERTRIP:
        retVal= value;
        retVal.setValue(retVal.toDouble()*unit.getLengthConversionFactor());
        break;
    case PRICEPERLITRE:
        retVal = value;
        retVal.setValue(retVal.toDouble()*unit.getVolumeConversionFactor());
        break;
        // todo: not finished
    case CO2EMISSION:
        retVal = value;

        if (unit.getLengthUnit() != UnitSystem::SI) {
            // mass unit/miles
            retVal.setValue(retVal.toDouble()*unit.getLengthConversionFactor());
        }

        if (unit.getMassUnit() != UnitSystem::SI) {
            // pounds/100 length units
            retVal.setValue(100.0*retVal.toDouble()/unit.getMassConversionFactor());
        }
        break;
    case LONGITUDE:
    case LATITUDE:
    case PLACE:
    case NOTE:
        retVal = value;
        break;
    case ID:
        retVal = value;
        break;
    }
    return retVal;
}
