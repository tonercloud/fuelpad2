/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012,2014 Julius Luukko <julle.luukko@quicknet.inet.fi>
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

#ifndef GEOCODENOMINATIM_H
#define GEOCODENOMINATIM_H

#include <QtXml/QDomDocument>

#include "geocode.h"

class GeocodeNominatim : public Geocode
{
public:
    GeocodeNominatim();

    bool requestAddress(double lat, double lon);
    QString getAddress(void);

private:
    const QString serviceAddress;

    bool retrieve_result(QString &result, QDomElement node);
    bool parse_xml(QString &address);
};

#endif // GEOCODENOMINATIM_H
