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

#ifndef UIENGINE_H
#define UIENGINE_H

#include "uiwrapper.h"
#include "database.h"
#include "databasesqlite.h"
#include "geocode.h"
#include "geocodenominatim.h"

class UiEngine
{
public:
    UiEngine(void);
    ~UiEngine(void);

    UiWrapper* getApplicationData(void);
    MySortFilterProxyModel *getFuelEntryModel(void);
    RoleItemModel *getCarEntryModel(void);
    RoleItemModel *getDriverEntryModel(void);
    MySortFilterProxyModel *getAlarmEntryModel(void);
    MySortFilterProxyModel *getAlarmEventModel(void);
    MySortFilterProxyModel* getCarStatisticsModel(void);
    PlotDataModel *getStatisticsModel(void);

private:
    DatabaseSqlite sqliteDatabase;
    Database *dataBase;
    GeocodeNominatim nominatimGeoCode;
    Geocode *geoCode;

    UiWrapper *uiWrapper;
    MySortFilterProxyModel *fuelEntryModel;
    RoleItemModel *carEntryModel;
    RoleItemModel *driverEntryModel;
    MySortFilterProxyModel *alarmEntryModel;
    MySortFilterProxyModel *alarmEventModel;
    MySortFilterProxyModel *carStatisticsModel;
    PlotDataModel *statisticsModel;

};

#endif // UIENGINE_H
