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

#include "fuelrecord.h"
#include "datafield.h"

Fuelrecord::Fuelrecord(UnitSystem u) :
    date(u),
    km(u),
    trip(u),
    fill(u),
    consum(u),
    price(u),
    fueltype(u),
    ppl(u),
    ppt(u),
    service(u),
    oil(u),
    tires(u),
    lat(u),
    lon(u),
    place(u),
    notes(u),
    co2emission(u),
    id(u)
{
    unit = u;
}

void Fuelrecord::setUnitSystem(UnitSystem u)
{
    unit = u;
}

UnitSystem Fuelrecord::getUnitSystem(void)
{
    return unit;
}

void Fuelrecord::setAllValues(QString Date, double Km, double Trip, double Fill,
                         double Consum, double Price, int Fueltype, double Ppl,
                         double Ppt, double Service, double Oil, double Tires,
                         double Latitude, double Longitude, QString Place,
                         QString Notes, qlonglong Id)
{
    date.setValue(Date, Datafield::DATE);
    km.setValue(Km, Datafield::LENGTH);
    trip.setValue(Trip, Datafield::LENGTH);
    fill.setValue(Fill, Datafield::VOLUME);
    consum.setValue(Consum, Datafield::CONSUMPTION);
    price.setValue(Price, Datafield::PRICE);
    fueltype.setValue(Fueltype, Datafield::FUELTYPE);
    ppl.setValue(Ppl, Datafield::PRICEPERLITRE);
    ppt.setValue(Ppt, Datafield::PRICEPERTRIP);
    service.setValue(Service, Datafield::PRICE);
    oil.setValue(Oil, Datafield::PRICE);
    tires.setValue(Tires, Datafield::PRICE);
    lat.setValue(Latitude, Datafield::LATITUDE);
    lon.setValue(Longitude, Datafield::LONGITUDE);
    place.setValue(Place, Datafield::PLACE);
    notes.setValue(Notes, Datafield::NOTE);
    co2emission.setValue(calc_co2_emission(Consum, Fueltype), Datafield::CO2EMISSION);
    id.setValue(Id, Datafield::ID);
}

void Fuelrecord::setAllValuesUserUnit(QString Date, double Km, double Trip, double Fill,
                         double Consum, double Price, int Fueltype, double Ppl,
                         double Ppt, double Service, double Oil,
                         double Tires, double Latitude, double Longitude,
                         QString Place, QString Notes, qlonglong Id)
{
    date.setValueUserUnit(Date, Datafield::DATE);
    km.setValueUserUnit(Km, Datafield::LENGTH);
    trip.setValueUserUnit(Trip, Datafield::LENGTH);
    fill.setValueUserUnit(Fill, Datafield::VOLUME);
    consum.setValueUserUnit(Consum, Datafield::CONSUMPTION);
    price.setValueUserUnit(Price, Datafield::PRICE);
    fueltype.setValueUserUnit(Fueltype, Datafield::FUELTYPE);
    ppl.setValueUserUnit(Ppl, Datafield::PRICEPERLITRE);
    ppt.setValueUserUnit(Ppt, Datafield::PRICEPERTRIP);
    service.setValueUserUnit(Service, Datafield::PRICE);
    oil.setValueUserUnit(Oil, Datafield::PRICE);
    tires.setValueUserUnit(Tires, Datafield::PRICE);
    lat.setValueUserUnit(Latitude, Datafield::LATITUDE);
    lon.setValueUserUnit(Longitude, Datafield::LONGITUDE);
    place.setValueUserUnit(Place, Datafield::PLACE);
    notes.setValueUserUnit(Notes, Datafield::NOTE);
    co2emission.setValue(calc_co2_emission(consum.getValue().toDouble(), Fueltype), Datafield::CO2EMISSION);
    id.setValue(Id, Datafield::ID);
}

//--------------------------------------------------------
// Setters for database native units (SI)
//--------------------------------------------------------

void Fuelrecord::setDate(QString Date)
{
    date.setValue(Date, Datafield::DATE);
}

void Fuelrecord::setKm(double Km)
{
    km.setValue(Km, Datafield::LENGTH);
}

void Fuelrecord::setTrip(double Trip)
{
    trip.setValue(Trip, Datafield::LENGTH);
}

void Fuelrecord::setFill(double Fill)
{
    fill.setValue(Fill, Datafield::VOLUME);
}

void Fuelrecord::setConsum(double Consum)
{
    consum.setValue(Consum, Datafield::CONSUMPTION);
}

void Fuelrecord::setPrice(double Price)
{
    price.setValue(Price, Datafield::PRICE);
}

void Fuelrecord::setFueltype(int Fueltype)
{
    fueltype.setValue(Fueltype, Datafield::FUELTYPE);
}

void Fuelrecord::setPpl(double Ppl)
{
    ppl.setValue(Ppl, Datafield::PRICEPERLITRE);
}

void Fuelrecord::setPpt(double Ppt)
{
    ppt.setValue(Ppt, Datafield::PRICEPERTRIP);
}

void Fuelrecord::setService(double Service)
{
    service.setValue(Service, Datafield::PRICE);
}

void Fuelrecord::setOil(double Oil)
{
    oil.setValue(Oil, Datafield::PRICE);
}

void Fuelrecord::setTires(double Tires)
{
    tires.setValue(Tires, Datafield::PRICE);
}

void Fuelrecord::setLatitude(double Lat)
{
    lat.setValue(Lat, Datafield::LATITUDE);
}

void Fuelrecord::setLongitude(double Lon)
{
    lat.setValue(Lon, Datafield::LONGITUDE);
}

void Fuelrecord::setPlace(QString Place)
{
    place.setValue(Place, Datafield::PLACE);
}

void Fuelrecord::setNotes(QString Notes)
{
    notes.setValue(Notes, Datafield::NOTE);
}

void Fuelrecord::setId(qlonglong Id)
{
    id.setValue(Id, Datafield::ID);
}

//--------------------------------------------------------
// Getters for database native units (SI)
//--------------------------------------------------------

QVariant Fuelrecord::getDate(void)
{
    return date.getValue();
}

QVariant Fuelrecord::getKm(void)
{
    return km.getValue();
}

QVariant Fuelrecord::getTrip(void)
{
    return trip.getValue();
}

QVariant Fuelrecord::getFill(void)
{
    return fill.getValue();
}

QVariant Fuelrecord::getConsum(void)
{
    return consum.getValue();
}

QVariant Fuelrecord::getPrice(void)
{
    return price.getValue();
}

QVariant Fuelrecord::getFueltype(void)
{
    return fueltype.getValue();
}

QVariant Fuelrecord::getPpl(void)
{
    return ppl.getValue();
}

QVariant Fuelrecord::getPpt(void)
{
    return ppt.getValue();
}

QVariant Fuelrecord::getService(void)
{
    return service.getValue();
}

QVariant Fuelrecord::getOil(void)
{
    return oil.getValue();
}

QVariant Fuelrecord::getTires(void)
{
    return tires.getValue();
}

QVariant Fuelrecord::getLatitude(void)
{
    return lat.getValue();
}

QVariant Fuelrecord::getLongitude(void)
{
    return lon.getValue();
}

QVariant Fuelrecord::getPlace(void)
{
    return place.getValue();
}

QVariant Fuelrecord::getCO2Emission(void)
{
    return co2emission.getValue();
}

//--------------------------------------------------------
// Getters for user units
//--------------------------------------------------------

QVariant Fuelrecord::getDateUserUnit(void)
{
    return date.getValueUserUnit();
}

QVariant Fuelrecord::getKmUserUnit(void)
{
    return km.getValueUserUnit();
}

QVariant Fuelrecord::getTripUserUnit(void)
{
    return trip.getValueUserUnit();
}

QVariant Fuelrecord::getFillUserUnit(void)
{
    return fill.getValueUserUnit();
}

QVariant Fuelrecord::getConsumUserUnit(void)
{
    return consum.getValueUserUnit();
}

QVariant Fuelrecord::getPriceUserUnit(void)
{
    return price.getValueUserUnit();
}

QVariant Fuelrecord::getFueltypeUserUnit(void)
{
    return fueltype.getValueUserUnit();
}

QVariant Fuelrecord::getPplUserUnit(void)
{
    return ppl.getValueUserUnit();
}

QVariant Fuelrecord::getPptUserUnit(void)
{
    return ppt.getValueUserUnit();
}

QVariant Fuelrecord::getServiceUserUnit(void)
{
    return service.getValueUserUnit();
}

QVariant Fuelrecord::getOilUserUnit(void)
{
    return oil.getValueUserUnit();
}

QVariant Fuelrecord::getTiresUserUnit(void)
{
    return tires.getValueUserUnit();
}

QVariant Fuelrecord::getCO2EmissionUserUnit(void)
{
    return co2emission.getValueUserUnit();
}

QVariant Fuelrecord::getNotes(void)
{
    return notes.getValueUserUnit();
}

QVariant Fuelrecord::getId(void)
{
    return id.getValueUserUnit();
}

bool Fuelrecord::getNotFullFill(void)
{
    bool notFull = false;

    if ( (fill.getValue().toDouble() > 0) && (consum.getValue().toDouble() <1.0e-3) ) {
        notFull = true;
    }

    return notFull;
}

/**
 * \fn double calc_co2_emission(float fill, float trip, int fueltype)
 * \brief Returns the CO2 emission in g/km
 * \param fill The amount of fuel filled
 * \param trip The distance driven
 * \param carid The car id
 * \return CO2 emission in g/km
 *
 */
double Fuelrecord::calc_co2_emission(double consum, int fueltype)
{
  double result;

  if (fueltype<0) fueltype=0;

  if (consum>1e-5)
    result=get_emission_per_litre(fueltype)*consum/100.0;
  else
    result=0.0;

  return result;
}

/**
 * \fn float get_emission_per_litre(int fueltype)
 * \brief Returns the CO2 emission in g/l
 * \param fueltype 0 for petrol, 1 for diesel
 * \return CO2 emission in g/l
 *
 */
double Fuelrecord::get_emission_per_litre(int fueltype)
{
  double emissionperlitre[]={2350.0, 2660.0};
  double result = 0.0;

  if (fueltype>=0 && fueltype <=1) {
      result = emissionperlitre[fueltype];
  }

  return result;

}
