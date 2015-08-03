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

#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

// Standard libraries
#include <string>
using namespace std;

#include <vector>

// Application includes
#include "fuelrecord.h"
#include "unitsystem.h"
#include "cardata.h"
#include "carstatistics.h"
#include "driverdata.h"
#include "alarmtypedata.h"
#include "alarmeventdata.h"

class Database
{
public:
    typedef struct {
        double overall;
        double lastyear;
        double lastmonth;
    } dbtimespan;

    Database();

    // Database open and close
    virtual bool isOpen(void) =0;
    virtual bool openConnection(void) =0;
    virtual void closeConnection(void) =0;
    void setFileName(string);
    const string getFileName(void);
    virtual bool didNotExist(void) =0;
    virtual void clearDidNotExist(void) =0;

    // Current car and driver
    virtual void setCurrentCar(int id);
    virtual CarData getCurrentCar(void);
    virtual void setCurrentDriver(int id);
    virtual DriverData getCurrentDriver(void);

    // Record querying
    virtual bool initRecordQuery(void) =0;
    virtual void resetRecordQuery(void) =0;
    virtual bool stepRecordQuery(void) =0;
    virtual Fuelrecord *getValuesRecordQuery(UnitSystem unit) =0;
    virtual vector<Fuelrecord> getRecordData(UnitSystem unit) =0;
    virtual Fuelrecord *queryOneRecord(qlonglong id, UnitSystem unit) =0;
    virtual float getLastRefill(float newkm) =0;
    virtual float getLastKm(void) =0;

    // Adding record
    virtual qlonglong addNewRecord(Fuelrecord &record, bool notFull) =0;
    virtual bool getNextFullFill(float km, Fuelrecord &record) =0;
    virtual bool getPrevNotFullData(float km, Fuelrecord &record) =0;

    // Updating record
    virtual qlonglong updateRecord(Fuelrecord &record, bool notFull) =0;

    // Deleting record
    virtual qlonglong deleteRecord(qlonglong id, bool deleteEvents) =0;

    // Querying simple statistics
    virtual dbtimespan getTotalKm(UnitSystem unit) =0;
    virtual dbtimespan getTotalFill(UnitSystem unit) =0;
    virtual dbtimespan getTotalConsumption(UnitSystem unit) =0;

    // Querying monthly statistics
    virtual bool getMonthlyData(int year, UnitSystem unit, vector<int> &month,
                                vector<double> &fill, vector<double> &trip,
                                vector<double> &consum, vector<double> &ppl) = 0;

    // Add a new driver
    virtual bool addDriver(string fullname, string nickname) =0;

    // Update driver data
    virtual bool updateDriver(qlonglong id, string fullname, string nickname) =0;

    // Delete a driver
    virtual bool deleteDriver(qlonglong id) =0;

    // Add a new car
    virtual bool addCar(string mark, string model, string year, string regist, string notes, quint8 fueltype) =0;

    // Update car data
    virtual bool updateCar(qlonglong id, string mark, string model, string year, string regist, string notes, quint8 fueltype) =0;

    // Delete a car
    virtual bool deleteCar(qlonglong id) =0;

    virtual vector<CarData> getCarData(void) =0;
    virtual vector<DriverData> getDriverData(void) =0;

    virtual vector<AlarmtypeData> getAlarmTypeData(void) =0;
    virtual bool addAlarmType(AlarmtypeData &alarmType) =0;
    virtual bool getLastEvent(qlonglong alarmid, QString &date, double &km) =0;
    virtual vector<AlarmeventData> getAlarmeventData(qlonglong alarmid) =0;
    virtual qlonglong addNewAlarmEvent(AlarmeventData &event, UnitSystem unit) =0;
    virtual qlonglong updateAlarmEvent(AlarmeventData &event, UnitSystem unit) =0;
    virtual int getNumOfRecordEvents(qlonglong id) =0;
    virtual bool deleteEventWithRecordId(qlonglong id) =0;
    virtual bool deleteEvent(qlonglong id, bool deleteFuelRecord) =0;

    // Car statistics
    virtual vector<CarStatistics> getCarStatistics(UnitSystem unit) =0;

protected:
    QString fileName;
    CarData carData;
    DriverData driverData;
    bool carDataValid;
//    int carId;
};

#endif // DATABASE_H
