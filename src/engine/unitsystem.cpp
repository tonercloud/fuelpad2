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

//-------------------------------------------
// Qt includes
//-------------------------------------------

// currencySymbol() will be in Qt 4.8
//#include <QLocale>

//-------------------------------------------
// Application includes
//-------------------------------------------
#include "unitsystem.h"

const double UnitSystem::lcf[3]={1.0, 1.609344, 1.609344};
const double UnitSystem::vcf[3]={1.0, 3.785411784, 4.54609};
const double UnitSystem::mcf[3]={1.0, 453.59237, 453.59237};

const QString UnitSystem::dateFmtStr[UnitSystem::NUMDATEFMT]={"yyyy-MM-dd", "dd.MM.yyyy", "dd/MM/yyyy", "dd/MM/yy",
                                      "dd-MM-yyyy", "MM/dd/yyyy", "MM/dd/yy"};

UnitSystem::UnitSystem() :
    mainUnit(SI),
    lengthUnit(SI),
    volumeUnit(SI),
    consumeUnit(SI),
    massUnit(SI),
    individualUnit(false),
    dateformat(0)
// @todo Dateformat uninitialized
{
    // This would work with Qt 4.8
//    currency = currencySymbol(QLocale::CurrencySymbol);
    currency = QString("%1").arg(QChar(8364));
}

void UnitSystem::setMainUnit(enum unit u)
{
    mainUnit = u;
}

void UnitSystem::setLengthUnit(enum unit u)
{
    lengthUnit = u;
}

void UnitSystem::setVolumeUnit(enum unit u)
{
    volumeUnit = u;
}

void UnitSystem::setConsumeUnit(enum unit u)
{
    consumeUnit = u;
}

void UnitSystem::setMassUnit(enum unit u)
{
    massUnit = u;
}

void UnitSystem::setIndividualUnit(bool individual)
{
    individualUnit = individual;

    if (!individualUnit) {
        lengthUnit = mainUnit;
        volumeUnit = mainUnit;
        consumeUnit = mainUnit;
        massUnit = mainUnit;
    }
}

void UnitSystem::setDateFormatString(int index)
{
    dateformat = index;
}

void UnitSystem::setCurrencySymbol(QString symbol)
{
    currency = symbol;
}

UnitSystem::unit UnitSystem::getMainUnit(void)
{
    return mainUnit;
}

UnitSystem::unit UnitSystem::getLengthUnit(void)
{
    return lengthUnit;
}

UnitSystem::unit UnitSystem::getVolumeUnit(void)
{
    return volumeUnit;
}

UnitSystem::unit UnitSystem::getConsumeUnit(void)
{
    return consumeUnit;
}

UnitSystem::unit UnitSystem::getMassUnit(void)
{
    return massUnit;
}

bool UnitSystem::getIndividualUnit(void)
{
    return individualUnit;
}

double UnitSystem::getLengthConversionFactor(void)
{
    return UnitSystem::lcf[(int)lengthUnit];
}

double UnitSystem::getVolumeConversionFactor(void)
{
    return UnitSystem::vcf[(int)volumeUnit];
}

double UnitSystem::getMassConversionFactor(void)
{
    return UnitSystem::mcf[(int)massUnit];
}

QString UnitSystem::getCurrentDateFormatString(void)
{
    return dateFmtStr[dateformat];
}

QString UnitSystem::getCurrencySymbol(void)
{
    return currency;
}
