/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012, 2014 Julius Luukko <julle.luukko@quicknet.inet.fi>
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

#ifndef CARSTATISTICS_H
#define CARSTATISTICS_H

#include <QtGlobal>
#include <QString>

#include "unitsystem.h"

class CarStatistics
{
public:
    CarStatistics(UnitSystem u);

    void setYear(QString year);
    QString getYear(void);

    void setMaxKm(double maxKm);
    double getMaxKm(void);

    void setMinKm(double minKm);
    double getMinKm(void);

    void setTotalTrip(double totalTrip);
    double getTotalTrip(void);

    void setTotalFill(double totalFill);
    double getTotalFill(void);

    void setTotalPrice(double totalPrice);
    double getTotalPrice(void);

    void setTotalOil(double totalOil);
    double getTotalOil(void);

    void setTotalService(double totalService);
    double getTotalService(void);

    void setTotalTires(double totalTires);
    double getTotalTires(void);

    double getAverageConsumption(void);
private:
    UnitSystem m_unit;
    QString m_year;
    double m_maxKm;
    double m_minKm;
    double m_totalTrip;
    double m_totalFill;
    double m_totalPrice;
    double m_totalOil;
    double m_totalService;
    double m_totalTires;
};

#endif // CARSTATISTICS_H
