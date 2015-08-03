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

#ifndef UNITSYSTEM_H
#define UNITSYSTEM_H

#include <QString>

class UnitSystem
{
public:
    enum unit {SI = 0,
               US,
               IMPERIAL,
               NUMUNITS};

    enum dateformat {
        DATEFMT0,
        DATEFMT1,
        DATEFMT2,
        DATEFMT3,
        DATEFMT4,
        DATEFMT5,
        DATEFMT6,
        NUMDATEFMT
    };

    UnitSystem();
    void setMainUnit(enum unit u);
    void setLengthUnit(enum unit u);
    void setVolumeUnit(enum unit u);
    void setConsumeUnit(enum unit u);
    void setMassUnit(enum unit u);
    void setIndividualUnit(bool individual);
    void setDateFormatString(int index);
    void setCurrencySymbol(QString symbol);
    UnitSystem::unit getMainUnit(void);
    UnitSystem::unit getLengthUnit(void);
    UnitSystem::unit getVolumeUnit(void);
    UnitSystem::unit getConsumeUnit(void);
    UnitSystem::unit getMassUnit(void);
    bool getIndividualUnit(void);

    double getLengthConversionFactor(void);
    double getVolumeConversionFactor(void);
    double getMassConversionFactor(void);
    QString getCurrentDateFormatString(void);
    QString getCurrencySymbol(void);
private:
    enum unit mainUnit;
    enum unit lengthUnit;
    enum unit volumeUnit;
    enum unit consumeUnit;
    enum unit massUnit;
    bool individualUnit;
    QString currency;
    int dateformat;

    /* lcf = length conversion factor */
    /* vcf = volume conversion factor */
    /* mcf = mass conversion factor */
    static const double lcf[3];
    static const double vcf[3];
    static const double mcf[3];
    static const QString dateFmtStr[NUMDATEFMT];
};

#endif // UNITSYSTEM_H
