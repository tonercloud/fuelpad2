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

// Library includes
#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

#include <math.h>

// Hopefully just for debuggin
#include <iostream>

// Application includes
#include "databasesqlite.h"
#include "cardata.h"
#include "alarmtypedata.h"
#include "alarmeventdata.h"

// Local functions

bool DatabaseSqlite::create_database(void)
{
    bool failure = false;

    QSqlQuery query;

    // Qt Bug: not possible to handle multiple SQL statements at once
    // https://bugreports.qt.nokia.com//browse/QTBUG-2144
    query.exec(
               "CREATE TABLE driver ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
               "fullname TEXT,"
               "nickname TEXT); "
                );
    failure = query.lastError().type() != QSqlError::NoError;

    query.exec(
               "CREATE TABLE car ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
               "mark TEXT,"
               "model TEXT,"
               "year NUMBER,"
               "register TEXT,"
               "notes TEXT,"
               "fueltype INTEGER);"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
               "CREATE TABLE record("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "carid INTEGER,"
               "driverid INTEGER,"
               "day TIMESTAMP,"
               "km REAL,"
               "trip REAL,"
               "fill REAL,"
               "consum REAL,"
               "price REAL,"
               "fueltype INTEGER,"
               "priceperlitre REAL,"
               "pricepertrip REAL,"
               "service REAL,"
               "oil REAL,"
               "tires REAL,"
               "insurance REAL,"
               "other REAL,"
               "lat NUMBER,"
               "lon NUMBER,"
               "place TEXT,"
               "notes TEXT);"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
               "CREATE TABLE alarmtype ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
               "carid INTEGER,"
               "shortdesc TEXT,"
               "distance NUMBER,"
               "interval INTEGER,"
               "longdesc TEXT);"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
               "CREATE TABLE alarmevent ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "alarmid INTEGER,"
               "carid INTEGER,"
               "driverid INTEGER,"
               "recordid INTEGER,"
               "day TIMESTAMP,"
               "km REAL);"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
               "INSERT INTO driver(fullname,nickname) "
               "VALUES('Fuelpad Driver','Driver');"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
               "INSERT INTO car(mark,model,year,register,notes) "
               "VALUES('My Car','Mark I',2014,'ABC-123','');"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    failure |= !create_drivinglog_tables();

    return !failure;
}

bool DatabaseSqlite::create_drivinglog_tables(void)
{
    bool failure = false;

    QSqlQuery query;

    // Qt Bug: not possible to handle multiple SQL statements at once
    // https://bugreports.qt.nokia.com//browse/QTBUG-2144
    query.exec(
                "CREATE TABLE drivinglog ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                "carid INTEGER,"
                "driverid INTEGER,"
                "distance NUMBER,"
                "starttime DATETIME,"
                "endtime DATETIME,"
                "startlat NUMBER,"
                "startlon NUMBER,"
                "startplace TEXT,"
                "endlat NUMBER,"
                "endlon NUMBER,"
                "endplace TEXT,"
                "effectivetime TIMESTAMP,"
                "explanation TEXT);"
                );
    failure = query.lastError().type() != QSqlError::NoError;

    query.exec(
                "CREATE TABLE locationalias ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                "latitude NUMBER,"
                "longitude NUMBER,"
                "place TEXT,"
                "alias TEXT);"
                );
    failure |= query.lastError().type() != QSqlError::NoError;

    return !failure;

}

bool DatabaseSqlite::add_location_fields(void)
{
    bool failure = false;

    QSqlQuery query;

    query.exec(
                "ALTER TABLE record "
                "ADD COLUMN lat NUMBER;"
               );
    failure = query.lastError().type() != QSqlError::NoError;

    query.exec(
                "ALTER TABLE record "
                "ADD COLUMN lon NUMBER;"
               );
    failure |= query.lastError().type() != QSqlError::NoError;

    query.exec(
                "ALTER TABLE record "
                "ADD COLUMN place TEXT;"
               );
    failure |= query.lastError().type() != QSqlError::NoError;

    return !failure;
}

bool DatabaseSqlite::add_fueltype_field(void)
{
    bool failure = false;

    QSqlQuery query;

    query.exec(
                "ALTER TABLE record "
                "ADD COLUMN fueltype INTEGER;"
               );
    failure = query.lastError().type() != QSqlError::NoError;

    return !failure;
}

bool DatabaseSqlite::prepare_queries(void)
{
    bool retVal;

    // Statements with a ready implementation
    ppStmtRecords = new QSqlQuery;
    ppStmtOneRecord = new QSqlQuery;
    ppStmtOneCar = new QSqlQuery;
    ppStmtLastRefill = new QSqlQuery;
    ppStmtLastKm = new QSqlQuery;
    ppStmtMonthlyData = new QSqlQuery;
    ppStmtCar = new QSqlQuery;
    ppStmtOneDriver = new QSqlQuery;
    ppStmtDriver = new QSqlQuery;
    ppStmtNextFull = new QSqlQuery;
    ppStmtPrevFull = new QSqlQuery;
    ppStmtAddRecord = new QSqlQuery;
    ppStmtUpdateRecord = new QSqlQuery;
    ppStmtDeleteRecord = new QSqlQuery;
    ppStmtGetKmOverall = new QSqlQuery;
    ppStmtGetKmLastMonth = new QSqlQuery;
    ppStmtGetKmLastYear = new QSqlQuery;
    ppStmtGetFillOverall = new QSqlQuery;
    ppStmtGetFillLastMonth = new QSqlQuery;
    ppStmtGetFillLastYear = new QSqlQuery;
    ppStmtGetConsumOverall = new QSqlQuery;
    ppStmtGetConsumLastMonth = new QSqlQuery;
    ppStmtGetConsumLastYear = new QSqlQuery;
    ppStmtAddDriver = new QSqlQuery;
    ppStmtAddCar = new QSqlQuery;
    ppStmtUpdateDriver = new QSqlQuery;
    ppStmtUpdateCar = new QSqlQuery;
    ppStmtGetAlarmtype = new QSqlQuery;
    ppStmtGetLastEvent = new QSqlQuery;
    ppStmtGetEvents = new QSqlQuery;
    ppStmtAddEvent = new QSqlQuery;
    ppStmtUpdateEvent = new QSqlQuery;
    ppStmtAddAlarmtype = new QSqlQuery;
    ppStmtGetNumOfRecordEvents = new QSqlQuery;
    ppStmtDeleteEventwithRecordid = new QSqlQuery;
    ppStmtDeleteEvent = new QSqlQuery;
    ppStmtGetOneEvent = new QSqlQuery;
    ppStmtGetReport = new QSqlQuery;

    // Statements without a ready implementation
    ppStmtCurCar = new QSqlQuery;
    ppStmtExport = new QSqlQuery;
    ppStmtExportCar = new QSqlQuery;
    ppStmtGetOneAlarmtype = new QSqlQuery;
    ppStmtUpdateAlarmtype = new QSqlQuery;

    ppStmtGetYears = new QSqlQuery;
    ppStmtAddLog = new QSqlQuery;
    ppStmtDeleteTrip = new QSqlQuery;
    ppStmtUpdateTrip = new QSqlQuery;
    ppStmtLogs = new QSqlQuery;
    ppStmtOneTrip = new QSqlQuery;
    ppStmtAddLocationAlias = new QSqlQuery;
    ppStmtFindNearestLocationAlias = new QSqlQuery;
    ppStmtLocations = new QSqlQuery;
    ppStmtUpdateLocation = new QSqlQuery;
    ppStmtDeleteLocation = new QSqlQuery;


    retVal = true;

    //--------------------------------------------------------------------------
    // Query all records
    //--------------------------------------------------------------------------
    // Bind with query.bindValue(":carid", 1001);
    retVal = retVal |
            ppStmtRecords->prepare( "SELECT day,km,trip,fill,consum,price,fueltype,"
                           "priceperlitre,pricepertrip,service,oil,tires,lat,lon,place,notes,"
                           "id FROM record WHERE carid=:carid ORDER BY km;");

    //--------------------------------------------------------------------------
    // Query one record
    // Note: the order of fields has been changed from the GTK version
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtOneRecord->prepare(
                "SELECT day,km,trip,fill,consum,price,fueltype,"
                "priceperlitre,pricepertrip,service,oil,tires,lat,lon,place,notes,"
                "id FROM record WHERE id=:id;");

    //--------------------------------------------------------------------------
    // Query one car
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtOneCar->prepare("SELECT mark,model,year,register,notes,fueltype FROM car where id=:carid;");

    //--------------------------------------------------------------------------
    // Query last refill
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtLastRefill->prepare("SELECT km FROM record WHERE carid=:carid AND trip>0 AND km<:km ORDER BY km DESC LIMIT 1;");

    //--------------------------------------------------------------------------
    // Query last km = maximum km
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtLastKm->prepare("SELECT max(km) FROM record WHERE carid=:carid;");

    //--------------------------------------------------------------------------
    // Query monthly statistics
    //--------------------------------------------------------------------------

    retVal = retVal |
            ppStmtMonthlyData->prepare("SELECT STRFTIME(:monthtag1,DATE(day)),STRFTIME(:yeartag1,DATE(day)),"
                                      "SUM(fill),SUM(trip),SUM(fill)/SUM(trip)*100,SUM(price)/SUM(fill) "
                                      "FROM record WHERE carid=:carid AND fill > 0 AND STRFTIME(:yeartag2,DATE(day))==:year "
                                      "GROUP BY STRFTIME(:monthtag2,day);");
//        ppStmtMonthlyData->prepare("SELECT STRFTIME(?,DATE(day)) "
//                              "FROM record WHERE carid=? AND fill > 0 AND STRFTIME(?,DATE(day))==? "
//                              "GROUP BY STRFTIME(?,day);");

    //--------------------------------------------------------------------------
    // Query all cars
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtCar->prepare("SELECT mark,model,year,register,notes,fueltype,id FROM car;");

    //--------------------------------------------------------------------------
    // Query one driver
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtOneDriver->prepare("SELECT fullname,nickname FROM driver where id=:driverid;");

    //--------------------------------------------------------------------------
    // Query all drivers
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtDriver->prepare("SELECT fullname,nickname,id FROM driver;");

    //--------------------------------------------------------------------------
    // Next full record
    // Consumption is marked as nonzero for full records
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtNextFull->prepare("SELECT day,km,trip,fill,consum,price,fueltype,"
                                    "priceperlitre,pricepertrip,service,oil,tires,notes,"
                                    "id FROM record WHERE carid=:carid AND km>:km AND fill>0 AND consum>0 ORDER BY km LIMIT 1;");

    //--------------------------------------------------------------------------
    // Previous records (for obtaning not full data)
    // Consumption is marked as zero for not full records
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtPrevFull->prepare("SELECT day,km,trip,fill,consum,price,"
                                    "priceperlitre,pricepertrip,service,oil,tires,notes,"
                                    "id FROM record WHERE carid=:carid AND km<:km AND fill>0 ORDER BY km DESC;");

    //--------------------------------------------------------------------------
    // Add a fuel record
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtAddRecord->prepare("INSERT INTO "
                                     "record(carid,driverid,day,km,trip,fill,consum,price,priceperlitre,pricepertrip,service,oil,tires,"
                                     "lat,lon,place,notes) "
                                     "VALUES(:carid,:driverid,:day,:km,:trip,:fill,:consum,:price,:ppl,:ppt,:service,:oil,:tires,"
                                     ":lat,:lon,:place,:notes);");

    //--------------------------------------------------------------------------
    // Update a fuel record
    // Note! Changed from GTK Fuelpad: Also carid and driverid are updated
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtUpdateRecord->prepare("UPDATE record "
                                        "SET carid=:carid, driverid=:driverid, day=:day, km=:km, trip=:trip, "
                                        "fill=:fill, consum=:consum, price=:price, priceperlitre=:ppl, "
                                        "pricepertrip=:ppt, service=:service, oil=:oil, tires=:tires, lat=:lat, lon=:lon, "
                                        "place=:place, notes=:notes WHERE id=:id;");

    //--------------------------------------------------------------------------
    // Delete a fuel record
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtDeleteRecord->prepare("DELETE FROM record "
                                        "WHERE id=:id;");

    //--------------------------------------------------------------------------
    // Simple statistics
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtGetKmOverall->prepare("SELECT MAX(km)-MIN(km) FROM record "
                                        "WHERE carid=:carid;");

    retVal = retVal |
            ppStmtGetKmLastMonth->prepare("SELECT SUM(trip) FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 month') AND DATE('now');");

    retVal = retVal |
            ppStmtGetKmLastYear->prepare("SELECT SUM(trip) FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 year') AND DATE('now');");

    retVal = retVal |
            ppStmtGetFillOverall->prepare("SELECT SUM(fill) FROM record "
                                        "WHERE carid=:carid;");

    retVal = retVal |
            ppStmtGetFillLastMonth->prepare("SELECT SUM(fill) FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 month') AND DATE('now');");

    retVal = retVal |
            ppStmtGetFillLastYear->prepare("SELECT SUM(fill) FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 year') AND DATE('now');");

    // SQL-lauseet päivittämättä
    retVal = retVal |
            ppStmtGetConsumOverall->prepare("SELECT SUM(fill)/SUM(trip)*100 FROM record "
                                        "WHERE carid=:carid;");

    retVal = retVal |
            ppStmtGetConsumLastMonth->prepare("SELECT SUM(fill)/SUM(trip)*100 FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 month') AND DATE('now');");

    retVal = retVal |
            ppStmtGetConsumLastYear->prepare("SELECT SUM(fill)/SUM(trip)*100 FROM record "
                                        "WHERE carid=:carid AND day BETWEEN DATE('now','-1 year') AND DATE('now');");

    //--------------------------------------------------------------------------
    // Add a new driver
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtAddDriver->prepare("INSERT INTO driver(fullname,nickname) VALUES(:fullname,:nickname);");

    //--------------------------------------------------------------------------
    // Update driver data
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtUpdateDriver->prepare("UPDATE driver "
                                        "SET fullname=:fullname, nickname=:nickname WHERE id=:id;");

    //--------------------------------------------------------------------------
    // Add a new car
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtAddCar->prepare("INSERT INTO car(mark,model,year,register,notes,fueltype) "
                                  "VALUES(:mark,:model,:year,:register,:notes,:fueltype);");

    //--------------------------------------------------------------------------
    // Update car data
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtUpdateCar->prepare("UPDATE car "
                                     "SET mark=:mark, model=:model, year=:year, register=:register, notes=:notes, fueltype=:fueltype WHERE id=:id;");


    //--------------------------------------------------------------------------
    // Alarms
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtGetAlarmtype->prepare("SELECT shortdesc,longdesc,distance,interval,id "
                                        "FROM alarmtype WHERE carid=:carid;");

    retVal = retVal |
            ppStmtAddAlarmtype->prepare("INSERT "
                                        "INTO alarmtype(carid,shortdesc,distance,interval,longdesc) "
                                        "VALUES(:carid,:shortdesc,:distance,:interval,:longdesc);");

    retVal = retVal |
            ppStmtGetLastEvent->prepare("SELECT day, km FROM alarmevent WHERE alarmid=:alarmid "
                                        "ORDER BY km DESC LIMIT 1;");

    retVal = retVal |
            ppStmtGetEvents->prepare("SELECT day,km,id,recordid "
                                     "FROM alarmevent WHERE alarmid=:alarmid;");

    retVal = retVal |
            ppStmtAddEvent->prepare("INSERT "
                                    "INTO alarmevent(alarmid,carid,driverid,recordid,day,km) "
                                    "VALUES(:alarmid,:carid,:driverid,:recordid,:day,:km);");

    retVal = retVal |
            ppStmtUpdateEvent->prepare("UPDATE alarmevent "
                                       "SET day=:day, km=:km WHERE id=:id;");

    retVal = retVal |
            ppStmtGetNumOfRecordEvents->prepare("SELECT COUNT(*) FROM alarmevent WHERE recordid=:recordid;");

    retVal = retVal |
            ppStmtDeleteEventwithRecordid->prepare("DELETE FROM alarmevent WHERE recordid=:recordid;");

    retVal = retVal |
            ppStmtDeleteEvent->prepare("DELETE FROM alarmevent WHERE id=:id;");

    retVal = retVal |
            ppStmtGetOneEvent->prepare("SELECT day,km,recordid FROM alarmevent WHERE id=:id;");

    //--------------------------------------------------------------------------
    // Reporting
    //--------------------------------------------------------------------------
    retVal = retVal |
            ppStmtGetReport->prepare("SELECT STRFTIME('%Y',day),MIN(km),MAX(km),MAX(km)-MIN(km),"
/*                                     "SUM(fill),SUM(price),SUM(fill)/SUM(trip)*100,"            */
                                     "SUM(fill),SUM(price),SUM(trip),"
                                     "SUM(price)/SUM(fill),SUM(price)/SUM(trip),SUM(oil),"
                                     "SUM(oil)/(MAX(km)-MIN(km)),SUM(service),"
                                     "SUM(service)/(MAX(km)-MIN(km)),SUM(tires),"
                                     "SUM(tires)/(MAX(km)-MIN(km)) FROM record "
                                     "WHERE carid=:carid AND km NOT IN (0) "
                                     "GROUP BY STRFTIME('%Y',DATE(day)) "
                                     "UNION "
                                     "SELECT 'Total',MIN(km),MAX(km),MAX(km)-MIN(km),"
/*                                     "SUM(fill),SUM(price),SUM(fill)/SUM(trip)*100,"            */
                                     "SUM(fill),SUM(price),SUM(trip),"
                                     "SUM(price)/SUM(fill),SUM(price)/SUM(trip),SUM(oil),"
                                     "SUM(oil)/(MAX(km)-MIN(km)),SUM(service),"
                                     "SUM(service)/(MAX(km)-MIN(km)),SUM(tires),"
                                     "SUM(tires)/(MAX(km)-MIN(km)) FROM record "
                                     "WHERE carid=:carid2 AND km NOT IN (0);");

    return retVal;
}

bool DatabaseSqlite::unprepare_queries(void)
{
    delete ppStmtRecords;
    delete ppStmtOneRecord;
    delete ppStmtOneCar;
    delete ppStmtLastRefill;
    delete ppStmtLastKm;
    delete ppStmtMonthlyData;
    delete ppStmtCar;
    delete ppStmtOneDriver;
    delete ppStmtDriver;
    delete ppStmtNextFull;
    delete ppStmtPrevFull;
    delete ppStmtAddRecord;
    delete ppStmtUpdateRecord;
    delete ppStmtDeleteRecord;
    delete ppStmtGetKmOverall;
    delete ppStmtGetKmLastMonth;
    delete ppStmtGetKmLastYear;
    delete ppStmtGetFillOverall;
    delete ppStmtGetFillLastMonth;
    delete ppStmtGetFillLastYear;
    delete ppStmtGetConsumOverall;
    delete ppStmtGetConsumLastMonth;
    delete ppStmtGetConsumLastYear;
    delete ppStmtAddDriver;
    delete ppStmtAddCar;
    delete ppStmtUpdateDriver;
    delete ppStmtUpdateCar;
    delete ppStmtGetAlarmtype;
    delete ppStmtGetLastEvent;
    delete ppStmtGetEvents;
    delete ppStmtAddEvent;
    delete ppStmtUpdateEvent;
    delete ppStmtAddAlarmtype;
    delete ppStmtGetNumOfRecordEvents;
    delete ppStmtDeleteEventwithRecordid;
    delete ppStmtGetReport;

    delete ppStmtCurCar;
    delete ppStmtExport;
    delete ppStmtExportCar;
    delete ppStmtGetOneAlarmtype;
    delete ppStmtUpdateAlarmtype;
    delete ppStmtGetOneEvent;
    delete ppStmtDeleteEvent;

    delete ppStmtGetYears;
    delete ppStmtAddLog;
    delete ppStmtDeleteTrip;
    delete ppStmtUpdateTrip;
    delete ppStmtLogs;
    delete ppStmtOneTrip;
    delete ppStmtAddLocationAlias;
    delete ppStmtFindNearestLocationAlias;
    delete ppStmtLocations;
    delete ppStmtUpdateLocation;
    delete ppStmtDeleteLocation;

    return true;

}

// Class methods
DatabaseSqlite::DatabaseSqlite(void)
{
    dbDidNotExist = false;
}

DatabaseSqlite::~DatabaseSqlite(void)
{
    closeConnection();
}


bool DatabaseSqlite::openConnection(void)
{
    bool retVal = false;

    if (fileName.isEmpty()) {
        retVal = false;
    }
    else {
        if (isOpen()) {
            closeConnection();
        }

        db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(fileName);

        retVal = db.open();

        if (!retVal) {
            std::cout << "Something wrong with opening database" << std::endl;
        }
        else {
            QSqlQuery query;

            // Check that database exists
            query.exec("SELECT nickname FROM driver LIMIT 1");
            query.first();

            if (query.lastError().type() != QSqlError::NoError) {
                qDebug("Creating database");
                dbDidNotExist = true;
                if (!create_database()) {
                    qDebug("Creating failed");
                }
                else {
                    qDebug("Database created");
                }
            }
            else { // we did find a database but we'll have to check if this is an old version
                /// @todo Add a lot of checks...
                qDebug("Database already exists");

                // Check if the driving log exists (GTK Fuelpad 0.90.4 onwards)
                query.exec("SELECT id FROM drivinglog LIMIT 1;");
                query.first();

                if (query.lastError().type() != QSqlError::NoError) {
                        // We'll need to create the driving log
                    if (!create_drivinglog_tables()) {
                        qDebug("Creating drivinglog tables failed");
                    }
                    else {
                        qDebug("Created drivinglog tables");
                    }
                }

                // Check if the new fuel entry location fields exist
                // (GTK Fuelpad's did not)
                query.exec("SELECT lat FROM record LIMIT 1;");
                query.first();

                if (query.lastError().type() != QSqlError::NoError) {
                        // We'll need to add the location fields
                        if (!add_location_fields()) {
                            qDebug("Adding location fields failed");
                        }
                        else {
                            qDebug("Added location fields");
                        }
                }

                // Check if the new fuel entry fueltype field exists
                // (GTK Fuelpad's did not)
                query.exec("SELECT fueltype FROM record LIMIT 1;");
                query.first();

                if (query.lastError().type() != QSqlError::NoError) {
                        // We'll need to add the location fields
                        if (!add_fueltype_field()) {
                            qDebug("Adding fueltype field failed");
                        }
                        else {
                            qDebug("Added fueltype field");
                            // @todo Set the fueltype field of all existing records
                        }
                }
            }

            if (!prepare_queries()) {
                qDebug("Something wrong with preparing queries!");
            }
        }

    }

    return retVal;
}

void DatabaseSqlite::closeConnection(void)
{
    std::cout << "Closing database" << std::endl;
    unprepare_queries();
    db.close();
}

bool DatabaseSqlite::isOpen(void)
{
    return db.isOpen();
}

bool DatabaseSqlite::didNotExist(void)
{
    return dbDidNotExist;
}

void DatabaseSqlite::clearDidNotExist(void)
{
    dbDidNotExist = false;
}

//--------------------------------------------------------------------------
// Reimplement the inhereted current car and driver setter/getter functions
//--------------------------------------------------------------------------
void DatabaseSqlite::setCurrentCar(int id)
{
    CarData *data;

    data = getCarDataFromDB(id);
    carData = *data;
    delete(data);
}

CarData DatabaseSqlite::getCurrentCar(void)
{
    return carData;
}

void DatabaseSqlite::setCurrentDriver(int id)
{
    DriverData *data;

    data = getDriverDataFromDB(id);
    driverData = *data;
    delete(data);
}

DriverData DatabaseSqlite::getCurrentDriver(void)
{
    return driverData;
}

//--------------------------------------------------------------------------
// Query functions for querying all records one at a time
//--------------------------------------------------------------------------
bool DatabaseSqlite::initRecordQuery(void)
{
    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtRecords->bindValue(":carid",getCurrentCar().getId());

    return ppStmtRecords->exec();
}

void DatabaseSqlite::resetRecordQuery(void)
{
    // Not sure if this is needed: http://doc.qt.nokia.com/4.7/qsqlquery.html#clear
    ppStmtRecords->clear();
}

bool DatabaseSqlite::stepRecordQuery(void)
{
    return ppStmtRecords->next();
}

Fuelrecord *DatabaseSqlite::getValuesRecordQuery(UnitSystem unit)
{
    Fuelrecord *record = new Fuelrecord(unit);

    record->setAllValues(ppStmtRecords->value(0).toString(), ppStmtRecords->value(1).toDouble(),
                            ppStmtRecords->value(2).toDouble(), ppStmtRecords->value(3).toDouble(),
                            ppStmtRecords->value(4).toDouble(), ppStmtRecords->value(5).toDouble(),
                            ppStmtRecords->value(6).toInt(), ppStmtRecords->value(7).toDouble(),
                            ppStmtRecords->value(8).toDouble(), ppStmtRecords->value(9).toDouble(),
                            ppStmtRecords->value(10).toDouble(), ppStmtRecords->value(11).toDouble(),
                            ppStmtRecords->value(12).toDouble(), ppStmtRecords->value(13).toDouble(),
                            ppStmtRecords->value(14).toString(), ppStmtRecords->value(15).toString(),
                            ppStmtRecords->value(16).toLongLong());

    return record;
}

//--------------------------------------------------------------------------
// Query current car's fuel record data and return it as a vector
//--------------------------------------------------------------------------
vector<Fuelrecord> DatabaseSqlite::getRecordData(UnitSystem unit)
{
    vector<Fuelrecord> data;
    // QSqlQuery.bindValue is void, we'll have to assume it worked

    ppStmtRecords->bindValue(":carid",getCurrentCar().getId());

    if (ppStmtRecords->exec()) {
        while (ppStmtRecords->next()) {
            Fuelrecord fuelRecord(unit);

            fuelRecord.setAllValues(ppStmtRecords->value(0).toString(), ppStmtRecords->value(1).toDouble(),
                                    ppStmtRecords->value(2).toDouble(), ppStmtRecords->value(3).toDouble(),
                                    ppStmtRecords->value(4).toDouble(), ppStmtRecords->value(5).toDouble(),
                                    ppStmtRecords->value(6).toInt(), ppStmtRecords->value(7).toDouble(),
                                    ppStmtRecords->value(8).toDouble(), ppStmtRecords->value(9).toDouble(),
                                    ppStmtRecords->value(10).toDouble(), ppStmtRecords->value(11).toDouble(),
                                    ppStmtRecords->value(12).toDouble(), ppStmtRecords->value(13).toDouble(),
                                    ppStmtRecords->value(14).toString(), ppStmtRecords->value(15).toString(),
                                    ppStmtRecords->value(16).toLongLong());
            // Store to vector
            data.push_back(fuelRecord);
        }
    }

    return data;
}

//--------------------------------------------------------------------------
// Query one record
//--------------------------------------------------------------------------
Fuelrecord *DatabaseSqlite::queryOneRecord(qlonglong id, UnitSystem unit)
{
    Fuelrecord *record = new Fuelrecord(unit);

    ppStmtOneRecord->bindValue(":id",id);
    if (ppStmtOneRecord->exec() && ppStmtOneRecord->next()) {
        record->setAllValues(ppStmtOneRecord->value(0).toString(), ppStmtOneRecord->value(1).toDouble(),
                                ppStmtOneRecord->value(2).toDouble(), ppStmtOneRecord->value(3).toDouble(),
                                ppStmtOneRecord->value(4).toDouble(), ppStmtOneRecord->value(5).toDouble(),
                                ppStmtOneRecord->value(6).toInt(), ppStmtOneRecord->value(7).toDouble(),
                                ppStmtOneRecord->value(8).toDouble(), ppStmtOneRecord->value(9).toDouble(),
                                ppStmtOneRecord->value(10).toDouble(), ppStmtOneRecord->value(11).toDouble(),
                                ppStmtOneRecord->value(12).toDouble(), ppStmtOneRecord->value(13).toDouble(),
                                ppStmtOneRecord->value(14).toString(), ppStmtOneRecord->value(15).toString(),
                             ppStmtOneRecord->value(16).toLongLong());

    }

    return record;
}

//--------------------------------------------------------------------------
// Query last refill
//--------------------------------------------------------------------------
float DatabaseSqlite::getLastRefill(float newkm)
{
    float lastRefill = 0.0;

    ppStmtLastRefill->bindValue(":carid",getCurrentCar().getId());
    ppStmtLastRefill->bindValue(":km",newkm);
    if (ppStmtLastRefill->exec() && ppStmtLastRefill->next()) {
        lastRefill = ppStmtLastRefill->value(0).toFloat();
    }

    return lastRefill;
}

//--------------------------------------------------------------------------
// Query last km = maximum km
//--------------------------------------------------------------------------
float DatabaseSqlite::getLastKm(void)
{
    float lastKm = 0.0;

    ppStmtLastKm->bindValue(":carid",getCurrentCar().getId());
    if (ppStmtLastKm->exec() && ppStmtLastKm->next()) {
        lastKm = ppStmtLastKm->value(0).toFloat();
    }

    return lastKm;
}

//--------------------------------------------------------------------------
// Adding and deleting record related methods
//--------------------------------------------------------------------------

// @todo Maybe this method should change the record given as a parameter,
// so it can be used to update the values in GUI
qlonglong DatabaseSqlite::addNewRecord(Fuelrecord &record, bool notFull)
{
    double km;
    double trip;
    double fill;
    double price;
    double consum;
    double pricepertrip;
    double ppl;
    double service;
    double oil;
    double tires;
    double lat;
    double lon;
    qlonglong retFullId = 0;

    // Update calculated fields and check if other records need to be updated
    retFullId = internalSetRecord(record, notFull);

    // Get the values from the record object for easier access and recalculations
    km  = record.getKm().toDouble();
    trip = record.getTrip().toDouble();
    fill = record.getFill().toDouble();
    price = record.getPrice().toDouble();
    service = record.getService().toDouble();
    oil = record.getOil().toDouble();
    tires = record.getTires().toDouble();
    consum = record.getConsum().toDouble();
    pricepertrip = record.getPpt().toDouble();
    ppl = record.getPpl().toDouble();
    lat = record.getLatitude().toDouble();
    lon = record.getLongitude().toDouble();

    // The actual adding of the record
    ppStmtAddRecord->bindValue(":carid",getCurrentCar().getId());
    ppStmtAddRecord->bindValue(":driverid",getCurrentDriver().getId());
    ppStmtAddRecord->bindValue(":day",record.getDate().toString());
    ppStmtAddRecord->bindValue(":km",km);
    ppStmtAddRecord->bindValue(":trip",trip);
    ppStmtAddRecord->bindValue(":fill",fill);
    ppStmtAddRecord->bindValue(":consum",consum);
    ppStmtAddRecord->bindValue(":price",price);
    ppStmtAddRecord->bindValue(":ppl",ppl);
    ppStmtAddRecord->bindValue(":ppt",pricepertrip);
    ppStmtAddRecord->bindValue(":service",service);
    ppStmtAddRecord->bindValue(":oil",oil);
    ppStmtAddRecord->bindValue(":tires",tires);
    ppStmtAddRecord->bindValue(":lat",lat);
    ppStmtAddRecord->bindValue(":lon",lon);
    ppStmtAddRecord->bindValue(":place",record.getPlace().toString());
    ppStmtAddRecord->bindValue(":notes",record.getNotes().toString());

    // Finally, do the update
    if (ppStmtAddRecord->exec()) {
        // @todo do we need to ppStmtAddRecord->next()
        qDebug("Adding a record was succesful");
    }
    else {
        qDebug("Adding a record was not succesful");
    }


    // Get the id for just added record and update it to the record
    record.setId(ppStmtAddRecord->lastInsertId().toLongLong());

    return retFullId;
}

qlonglong DatabaseSqlite::updateRecord(Fuelrecord &record, bool notFull)
{
    double km;
    double trip;
    double fill;
    double price;
    double consum;
    double pricepertrip;
    double ppl;
    double service;
    double oil;
    double tires;
    double lat;
    double lon;
    qlonglong retFullId = 0;

    qDebug("%s called",__PRETTY_FUNCTION__);

    // Update calculated fields and check if other records need to be updated
    retFullId = internalSetRecord(record, notFull);

    // Get the values from the record object for easier access and recalculations
    km  = record.getKm().toDouble();
    trip = record.getTrip().toDouble();
    fill = record.getFill().toDouble();
    price = record.getPrice().toDouble();
    service = record.getService().toDouble();
    oil = record.getOil().toDouble();
    tires = record.getTires().toDouble();
    consum = record.getConsum().toDouble();
    pricepertrip = record.getPpt().toDouble();
    ppl = record.getPpl().toDouble();
    lat = record.getLatitude().toDouble();
    lon = record.getLongitude().toDouble();

    // The actual updating of the record
    ppStmtUpdateRecord->bindValue(":carid",getCurrentCar().getId());
    ppStmtUpdateRecord->bindValue(":driverid",getCurrentDriver().getId());
    ppStmtUpdateRecord->bindValue(":day",record.getDate().toString());
    ppStmtUpdateRecord->bindValue(":km",km);
    ppStmtUpdateRecord->bindValue(":trip",trip);
    ppStmtUpdateRecord->bindValue(":fill",fill);
    ppStmtUpdateRecord->bindValue(":consum",consum);
    ppStmtUpdateRecord->bindValue(":price",price);
    ppStmtUpdateRecord->bindValue(":ppl",ppl);
    ppStmtUpdateRecord->bindValue(":ppt",pricepertrip);
    ppStmtUpdateRecord->bindValue(":service",service);
    ppStmtUpdateRecord->bindValue(":oil",oil);
    ppStmtUpdateRecord->bindValue(":tires",tires);
    ppStmtUpdateRecord->bindValue(":lat",lat);
    ppStmtUpdateRecord->bindValue(":lon",lon);
    ppStmtUpdateRecord->bindValue(":place",record.getPlace().toString());
    ppStmtUpdateRecord->bindValue(":notes",record.getNotes().toString());
    ppStmtUpdateRecord->bindValue(":id",record.getId().toLongLong());

    // Finally, do the update
    if (ppStmtUpdateRecord->exec()) {
        // @todo do we need to ppStmtUpdateRecord->next()
        qDebug("Updating a record was succesful");
    }
    else {
        qDebug("Updating a record was not succesful");
    }

    return retFullId;
}

qlonglong DatabaseSqlite::deleteRecord(qlonglong id, bool deleteEvents)
{
    double km;
    double trip;
    double fill;
    double price;
    double consum;
    double pricepertrip;
    double ppl;
    double service;
    double oil;
    double tires;
    qlonglong retFullId = 0;
    UnitSystem unitSystem;
    Fuelrecord *record = queryOneRecord(id, unitSystem);
    int numEvents = getNumOfRecordEvents(id);

    // Get the values from the record object for easier access and recalculations
    km  = record->getKm().toDouble();
    trip = record->getTrip().toDouble();
    fill = record->getFill().toDouble();
    price = record->getPrice().toDouble();
    service = record->getService().toDouble();
    oil = record->getOil().toDouble();
    tires = record->getTires().toDouble();

    // Consumption and price/trip are calculated, never used directly from the record
    consum = fill/trip*100;
    pricepertrip = price/trip;

    // Price per litre also calculated
    ppl = price/fill;

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtDeleteRecord->bindValue(":id",record->getId().toLongLong());

    if (ppStmtDeleteRecord->exec() ) {
        // && ppStmtDeleteRecord->next() ?

        // Check if this was a fill record and was it part of not full records
        if (fill > 0) {

            // Was this a not full record
            if (fabs(consum) < 1.0e-5) {
                Fuelrecord fullRecord(record->getUnitSystem());

                if (getNextFullFill(km, fullRecord)) {
                    double fullFill = fullRecord.getFill().toDouble();
                    double fullKm = fullRecord.getKm().toDouble();
                    double fullPrice = fullRecord.getPrice().toDouble();

                    double fullConsum;
                    double fullPricePerTrip;

                    fullConsum = fullFill/fullKm*100;
                    fullPricePerTrip = fullPrice/fullKm;

                    fullRecord.setConsum(fullConsum);
                    fullRecord.setPpt(fullPricePerTrip);

                    // Update the full record in the database
                    internalUpdateRecord(fullRecord);

                    // Return the id of the record that we needed to update
                    retFullId = fullRecord.getId().toLongLong();

                }
            }
        }

        // Remove the associated alarm events
        if (numEvents > 0 && deleteEvents) {
            deleteEventWithRecordId(id);
        }

    }

    delete record;

    return retFullId;
}

bool DatabaseSqlite::getNextFullFill(float km, Fuelrecord &record)
{
    bool retVal = false;

    ppStmtNextFull->bindValue(":carid",getCurrentCar().getId());
    ppStmtNextFull->bindValue(":km",km);

    if (ppStmtNextFull->exec() && ppStmtNextFull->next()) {
        record.setAllValues(ppStmtNextFull->value(0).toString(), ppStmtNextFull->value(1).toDouble(),
                                ppStmtNextFull->value(2).toDouble(), ppStmtNextFull->value(3).toDouble(),
                                ppStmtNextFull->value(4).toDouble(), ppStmtNextFull->value(5).toDouble(),
                                ppStmtNextFull->value(6).toInt(), ppStmtNextFull->value(7).toDouble(),
                                ppStmtNextFull->value(8).toDouble(), ppStmtNextFull->value(9).toDouble(),
                                ppStmtNextFull->value(10).toDouble(), ppStmtNextFull->value(11).toDouble(),
                                ppStmtNextFull->value(12).toDouble(), ppStmtNextFull->value(13).toDouble(),
                                ppStmtNextFull->value(14).toString(), ppStmtNextFull->value(15).toString(),
                            ppStmtNextFull->value(16).toLongLong());
        retVal = true;
    }

    return retVal;
}

bool DatabaseSqlite::getPrevNotFullData(float km, Fuelrecord &record)
{
    bool retVal = false;
    float fullFill = 0.0;
    float fullKm = 0.0;
    float fullPrice = 0.0;

    ppStmtPrevFull->bindValue(":carid",getCurrentCar().getId());
    ppStmtPrevFull->bindValue(":km",km);

    if (ppStmtPrevFull->exec()) {
        while (ppStmtPrevFull->next()) {
            if (ppStmtPrevFull->value(4).toFloat(NULL) > 1.0e-3) { // consum > 0.0 ?
                retVal = true;                               // We found a full record
                break;
            }
            else {
                qDebug("Inside loop finding previous not full data");
                fullFill += ppStmtPrevFull->value(3).toFloat(NULL);
                fullKm += ppStmtPrevFull->value(2).toFloat(NULL);
                fullPrice += ppStmtPrevFull->value(5).toFloat(NULL);
            }
        }
    }

    record.setFill(fullFill);
    record.setKm(fullKm);
    record.setPrice(fullPrice);

    qDebug("Leaving getPrevNotFullData()");

    return retVal;

}

qlonglong DatabaseSqlite::internalSetRecord(Fuelrecord &record, bool notFull)
{
    double km;
    double trip;
    double fill;
    double price;
    double consum;
    double pricepertrip;
    double ppl;
    double service;
    double oil;
    double tires;
    qlonglong retFullId = 0;

    // Get the values from the record object for easier access and recalculations
    km  = record.getKm().toDouble();
    trip = record.getTrip().toDouble();
    fill = record.getFill().toDouble();
    price = record.getPrice().toDouble();
    service = record.getService().toDouble();
    oil = record.getOil().toDouble();
    tires = record.getTires().toDouble();

    // Consumption and price/trip are calculated, never used directly from the record
    consum = fill/trip*100;
    pricepertrip = price/trip;

    // Price per litre also calculated
    ppl = price/fill;

    if (notFull) {
        Fuelrecord fullRecord(record.getUnitSystem());

        // Consumption and price/trip are stored as zero for not full records
        consum = 0.0;
        pricepertrip = 0.0;


        // We'll find a full record, if there is one with more km, and then update it
        if (getNextFullFill(km, fullRecord)) {
            double fullFill = fullRecord.getFill().toDouble();
            double fullKm = fullRecord.getKm().toDouble();
            double fullPrice = fullRecord.getPrice().toDouble();

            double fullConsum;
            double fullPricePerTrip;

            fullConsum = (fullFill+fill)/(fullKm+trip)*100;
            fullPricePerTrip = (fullPrice+price)/(fullKm+trip);

            fullRecord.setConsum(fullConsum);
            fullRecord.setPpt(fullPricePerTrip);

            // Update the full record in the database
            internalUpdateRecord(fullRecord);

            retFullId = fullRecord.getId().toLongLong();

        }
        else {
            // There is no full fill yet (i.e. data entered in normal order)
            // nothing to do then
        }
    }
    else { // if (notFull) // Full fill entered
        Fuelrecord notFullData(record.getUnitSystem());

        // Find if there are any not full fills before this
        // and update this new records consumption and price per trip
        if (getPrevNotFullData(km, notFullData)) {
            double notFullFill = notFullData.getFill().toDouble();
            double notFullKm = notFullData.getKm().toDouble();
            double notFullPrice = notFullData.getPrice().toDouble();

            consum = (notFullFill+fill)/(notFullKm+trip)*100;
            pricepertrip = (notFullPrice+price)/(notFullKm+trip);

            qDebug("Full fill: notFullFill = %.2f, notFullKm = %.2f", notFullFill, notFullKm);

        }
        else {
            qDebug("getPrevNotFullData() returned false");
        }
    }

    // Update the calculated consumption and price/trip to the callee
    record.setConsum(consum);
    record.setPpt(pricepertrip);
    record.setPpl(ppl);

    qDebug("current driver id = %ld",getCurrentDriver().getId());

    return retFullId;
}

bool DatabaseSqlite::internalUpdateRecord(Fuelrecord record)
{
    bool retVal = false;

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtUpdateRecord->bindValue(":carid",getCurrentCar().getId());
    ppStmtUpdateRecord->bindValue(":driverid",getCurrentDriver().getId());
    ppStmtUpdateRecord->bindValue(":day",record.getDate().toString());
    ppStmtUpdateRecord->bindValue(":km",record.getKm().toDouble());
    ppStmtUpdateRecord->bindValue(":trip",record.getTrip().toDouble());
    ppStmtUpdateRecord->bindValue(":fill",record.getFill().toDouble());
    ppStmtUpdateRecord->bindValue(":consum",record.getConsum().toDouble());
    ppStmtUpdateRecord->bindValue(":price",record.getPrice().toDouble());
    ppStmtUpdateRecord->bindValue(":ppl",record.getPpl().toDouble());
    ppStmtUpdateRecord->bindValue(":ppt",record.getPpt().toDouble());
    ppStmtUpdateRecord->bindValue(":service",record.getService().toDouble());
    ppStmtUpdateRecord->bindValue(":oil",record.getOil().toDouble());
    ppStmtUpdateRecord->bindValue(":tires",record.getTires().toDouble());
    ppStmtUpdateRecord->bindValue(":notes",record.getNotes().toString());
    ppStmtUpdateRecord->bindValue(":id",record.getId().toLongLong());

    if (ppStmtUpdateRecord->exec() && ppStmtUpdateRecord->next()) {
        retVal = true;
    }

    return retVal;
}

bool DatabaseSqlite::createFillView(void) {
    bool retVal = false;
    QSqlQuery q;
//    q.prepare("CREATE TEMP VIEW fillview AS SELECT * FROM record WHERE carid=:carid ORDER BY km LIMIT -1 OFFSET 1;");
    q.prepare("CREATE TEMP VIEW fillview AS SELECT * FROM record;");
//    q.bindValue(":carid", getCurrentCar().getId());
    if (q.exec()) {
        qDebug("Fillview exec ok");
//        if (q.next()) {
        if (q.first()) {
            qDebug("Fillview created succesfully");
            retVal = true;
        }
    }
    else {
        qDebug("Fillview creation unsuccesful");
        std::cout << "error type =" << q.lastError().type() << " database error = " << q.lastError().databaseText().toStdString() << std::endl;
        std::cout << "driver error= " << q.lastError().driverText().toStdString() << std::endl;
        retVal = false;
    }
    return retVal;
}

bool DatabaseSqlite::dropFillView(void)
{
    bool retVal = false;
    QSqlQuery q("DROP VIEW fillview;");
    if (q.exec()) {
        qDebug("Fillview drop exec ok");
        if (q.next()) {
            qDebug("Fillview dropped succesfully");
            retVal = true;
        }
    }
    else {
        qDebug("Fillview drop unsuccesful");
        std::cout << "database error = " << q.lastError().databaseText().toStdString() << std::endl;
        std::cout << "driver error= " << q.lastError().driverText().toStdString() << std::endl;
        retVal = false;
    }
    return retVal;
}

//--------------------------------------------------------------------------
// Query monthly statistics
//--------------------------------------------------------------------------
bool DatabaseSqlite::getMonthlyData(int year, UnitSystem unit, vector<int> &month, vector<double> &fill,
                                    vector<double> &trip, vector<double> &consum, vector<double> &ppl)
{
    bool retVal = false;
//    if (createFillView()) {
        ppStmtMonthlyData->bindValue(":monthtag1", "'%m'");
        ppStmtMonthlyData->bindValue(":yeartag1", "'%Y'");
        ppStmtMonthlyData->bindValue(":carid", getCurrentCar().getId());
        ppStmtMonthlyData->bindValue(":yeartag2", "'%Y'");
        ppStmtMonthlyData->bindValue(":year", (QString("'") + QString("%1").arg(year) + QString("'")));
        ppStmtMonthlyData->bindValue(":monthtag2", "'%m'");

        month.clear();
        fill.clear();
        trip.clear();
        consum.clear();
        ppl.clear();
        if (ppStmtMonthlyData->exec()) {
            std::cout << "Queried = "  << ppStmtMonthlyData->lastQuery().toStdString() << std::endl;
            while (ppStmtMonthlyData->next()) {
                // Obtain data from query
                month.push_back(ppStmtMonthlyData->value(0).toString().remove("'").toInt());
                fill.push_back(ppStmtMonthlyData->value(2).toDouble()/unit.getVolumeConversionFactor());
                trip.push_back(ppStmtMonthlyData->value(3).toDouble()/unit.getLengthConversionFactor());
                if (unit.getConsumeUnit() != UnitSystem::SI) {
                    consum.push_back(unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/ppStmtMonthlyData->value(4).toDouble());
                }
                else { // SI
                    consum.push_back(ppStmtMonthlyData->value(4).toDouble());
                }
                ppl.push_back(ppStmtMonthlyData->value(5).toDouble()*unit.getVolumeConversionFactor());

            }
            retVal = true;
        }
        else {
            qDebug("%s exec() failed",__PRETTY_FUNCTION__);
            std::cout << "database error = " << ppStmtMonthlyData->lastError().databaseText().toStdString() << std::endl;
            std::cout << "driver error= " << ppStmtMonthlyData->lastError().driverText().toStdString() << std::endl;
            retVal = false;
        }

//    }
//    else {
//        retVal = false;
//    }
//    dropFillView();

    return retVal;
}

//--------------------------------------------------------------------------
// Add a new driver
//--------------------------------------------------------------------------
bool DatabaseSqlite::addDriver(string fullname, string nickname)
{
    bool retVal = false;

    ppStmtAddDriver->bindValue(":fullname",QString::fromUtf8(fullname.c_str()));
    ppStmtAddDriver->bindValue(":nickname",QString::fromUtf8(nickname.c_str()));

    // Finally, do the update
    if (ppStmtAddDriver->exec()) {
        // @todo do we need to ppStmtAddDriver->next()
        qDebug("Adding a driver was succesful");
        retVal = true;
    }
    else {
        qDebug("Adding a driver was not succesful");
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Update driver data
//--------------------------------------------------------------------------
bool DatabaseSqlite::updateDriver(qlonglong id, string fullname, string nickname)
{
    bool retVal = false;

    ppStmtUpdateDriver->bindValue(":id",id);
    ppStmtUpdateDriver->bindValue(":fullname",QString::fromUtf8(fullname.c_str()));
    ppStmtUpdateDriver->bindValue(":nickname",QString::fromUtf8(nickname.c_str()));

    // Finally, do the update
    if (ppStmtUpdateDriver->exec()) {
        // @todo do we need to ppStmtUpdateDriver->next()
        qDebug("Updating a driver was succesful");
        retVal = true;
    }
    else {
        qDebug("Updating a driver was not succesful");
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Delete a driver
//--------------------------------------------------------------------------
bool DatabaseSqlite::deleteDriver(qlonglong id)
{
    bool retVal = false;
    QSqlQuery q;

    q.prepare("DELETE FROM driver WHERE id=:driverid;");
    q.bindValue(":driverid", id);
    if (q.exec()) {
        qDebug("Driver deleted ok");
        if (q.first()) {
            QSqlQuery q2;

            q2.prepare("DELETE FROM record WHERE driverid=:driverid;");
            q2.bindValue(":driverid", id);
            if (q2.exec()) {
                qDebug("Driver's records deleted ok");
                if (q2.first()) {
                    QSqlQuery q3;

                    q3.prepare("DELETE FROM alarmevent WHERE driverid=:driverid;");
                    q3.bindValue(":driverid", id);
                    if (q3.exec()) {
                        qDebug("Driver's alarm events deleted ok");
                        if (q3.first()) {

                        }
                    }
                }
            }
            retVal = true;
        }
    }
    else {
        qDebug("Car deletion unsuccesful");
        std::cout << "error type =" << q.lastError().type() << " database error = " << q.lastError().databaseText().toStdString() << std::endl;
        std::cout << "driver error= " << q.lastError().driverText().toStdString() << std::endl;
        retVal = false;
    }
    return retVal;
}


//--------------------------------------------------------------------------
// Add a new car
//--------------------------------------------------------------------------
bool DatabaseSqlite::addCar(string mark, string model, string year, string regist, string notes, quint8 fueltype)
{
    bool retVal = false;

    ppStmtAddCar->bindValue(":mark",QString::fromUtf8(mark.c_str()));
    ppStmtAddCar->bindValue(":model",QString::fromUtf8(model.c_str()));
    ppStmtAddCar->bindValue(":year",QString::fromUtf8(year.c_str()));
    ppStmtAddCar->bindValue(":register",QString::fromUtf8(regist.c_str()));
    ppStmtAddCar->bindValue(":notes",QString::fromUtf8(notes.c_str()));
    ppStmtAddCar->bindValue(":fueltype",fueltype);

    // Finally, do the update
    if (ppStmtAddCar->exec()) {
        // @todo do we need to ppStmtAddCar->next()
        qDebug("Adding a car was succesful");
        retVal = true;
    }
    else {
        qDebug("Adding a car was not succesful");
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Update car data
//--------------------------------------------------------------------------
bool DatabaseSqlite::updateCar(qlonglong id, string mark, string model, string year, string regist, string notes, quint8 fueltype)
{
    bool retVal = false;

    ppStmtUpdateCar->bindValue(":id",id);
    ppStmtUpdateCar->bindValue(":mark",QString::fromUtf8(mark.c_str()));
    ppStmtUpdateCar->bindValue(":model",QString::fromUtf8(model.c_str()));
    ppStmtUpdateCar->bindValue(":year",QString::fromUtf8(year.c_str()));
    ppStmtUpdateCar->bindValue(":register",QString::fromUtf8(regist.c_str()));
    ppStmtUpdateCar->bindValue(":notes",QString::fromUtf8(notes.c_str()));
    ppStmtUpdateCar->bindValue(":fueltype",fueltype);

    // Finally, do the update
    if (ppStmtUpdateCar->exec()) {
        // @todo do we need to ppStmtUpdateCar->next()
        qDebug("Updating a car was succesful");
        retVal = true;
    }
    else {
        qDebug("Updating a car was not succesful");
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Delete a car
//--------------------------------------------------------------------------
bool DatabaseSqlite::deleteCar(qlonglong id)
{
    bool retVal = false;
    QSqlQuery q;

    q.prepare("DELETE FROM car WHERE id=:carid;");
    q.bindValue(":carid", id);
    if (q.exec()) {
        qDebug("Car deleted ok");
        if (q.first()) {
            QSqlQuery q2;

            q2.prepare("DELETE FROM record WHERE carid=:carid;");
            q2.bindValue(":carid", id);
            if (q2.exec()) {
                qDebug("Car's records deleted ok");
                if (q2.first()) {
                    QSqlQuery q3;

                    q3.prepare("DELETE FROM alarmevent WHERE carid=:carid;");
                    q3.bindValue(":carid", id);
                    if (q3.exec()) {
                        qDebug("Car's alarm events deleted ok");
                        if (q3.first()) {

                        }
                    }
                }
            }
            retVal = true;
        }
    }
    else {
        qDebug("Car deletion unsuccesful");
        std::cout << "error type =" << q.lastError().type() << " database error = " << q.lastError().databaseText().toStdString() << std::endl;
        std::cout << "driver error= " << q.lastError().driverText().toStdString() << std::endl;
        retVal = false;
    }
    return retVal;
}


//--------------------------------------------------------------------------
// Query all car data and return in as a vector
//--------------------------------------------------------------------------
vector<CarData> DatabaseSqlite::getCarData(void)
{
    vector<CarData> data;

    if (ppStmtCar->exec()) {
        while (ppStmtCar->next()) {
            CarData carRecord;

            carRecord.setMark(ppStmtCar->value(0).toString());
            carRecord.setModel(ppStmtCar->value(1).toString());
            carRecord.setYear(ppStmtCar->value(2).toInt());
            carRecord.setRegNum(ppStmtCar->value(3).toString());
            carRecord.setNotes(ppStmtCar->value(4).toString());
            carRecord.setFuelType((enum CarData::FuelType)ppStmtCar->value(5).toInt());
            carRecord.setId(ppStmtCar->value(6).toInt());

            // Store to vector
            data.push_back(carRecord);
        }
    }

    return data;
}

//--------------------------------------------------------------------------
// Query one car
//--------------------------------------------------------------------------
CarData *DatabaseSqlite::getCarDataFromDB(qlonglong id)
{
    CarData *data = new CarData;

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtOneCar->bindValue(":carid",id);

    if (ppStmtOneCar->exec() && ppStmtOneCar->next()) {
        data->setId(id);
        data->setMark(ppStmtOneCar->value(0).toString());
        data->setModel(ppStmtOneCar->value(1).toString());
        data->setYear(ppStmtOneCar->value(2).toInt());
        data->setRegNum(ppStmtOneCar->value(3).toString());
        data->setNotes(ppStmtOneCar->value(4).toString());
        data->setFuelType((enum CarData::FuelType)ppStmtOneCar->value(5).toInt());
    }
    else {
    }

    return data;
}

//--------------------------------------------------------------------------
// Query all driver data and return in as a vector
//--------------------------------------------------------------------------
vector<DriverData> DatabaseSqlite::getDriverData(void)
{
    vector<DriverData> data;

    if (ppStmtDriver->exec()) {
        while (ppStmtDriver->next()) {
            DriverData driverRecord;

            driverRecord.setFullName(ppStmtDriver->value(0).toString());
            driverRecord.setNickName(ppStmtDriver->value(1).toString());
            driverRecord.setId(ppStmtDriver->value(2).toInt());

            // Store to vector
            data.push_back(driverRecord);
        }
    }

    return data;
}

//--------------------------------------------------------------------------
// Query one driver
//--------------------------------------------------------------------------
DriverData *DatabaseSqlite::getDriverDataFromDB(qlonglong id)
{
    DriverData *data = new DriverData;

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtOneDriver->bindValue(":driverid",id);

    if (ppStmtOneDriver->exec() && ppStmtOneDriver->next()) {
        data->setId(id);
        data->setFullName(ppStmtOneDriver->value(0).toString());
        data->setNickName(ppStmtOneDriver->value(1).toString());
    }
    else {
    }

    return data;
}

//--------------------------------------------------------------------------
// Query simple statistics
//--------------------------------------------------------------------------
Database::dbtimespan DatabaseSqlite::getTotalKm(UnitSystem unit)
{
    bool success = false;
    dbtimespan retVal = {0, 0, 0};

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtGetKmOverall->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetKmLastMonth->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetKmLastYear->bindValue(":carid",getCurrentCar().getId());

    if (ppStmtGetKmOverall->exec() && ppStmtGetKmOverall->next() ) {
        retVal.overall = ppStmtGetKmOverall->value(0).toDouble()/unit.getLengthConversionFactor();
        success = true;
    }
    else {
        success = false;
    }

    if (success) {
        if (ppStmtGetKmLastMonth->exec() && ppStmtGetKmLastMonth->next() ) {
            retVal.lastmonth = ppStmtGetKmLastMonth->value(0).toDouble()/unit.getLengthConversionFactor();
            success = true;
        }
        else {
            success = false;
        }
    }
    if (success) {
        if (ppStmtGetKmLastYear->exec() && ppStmtGetKmLastYear->next() ) {
            retVal.lastyear = ppStmtGetKmLastYear->value(0).toDouble()/unit.getLengthConversionFactor();
            success = true;
        }
        else {
            success = false;
        }
    }

    return retVal;
}

Database::dbtimespan DatabaseSqlite::getTotalFill(UnitSystem unit)
{
    bool success = false;
    dbtimespan retVal = {0, 0, 0};

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtGetFillOverall->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetFillLastMonth->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetFillLastYear->bindValue(":carid",getCurrentCar().getId());

    if (ppStmtGetFillOverall->exec() && ppStmtGetFillOverall->next() ) {
        retVal.overall = ppStmtGetFillOverall->value(0).toDouble()/unit.getVolumeConversionFactor();
        success = true;
    }
    else {
        success = false;
    }

    if (success) {
        if (ppStmtGetFillLastMonth->exec() && ppStmtGetFillLastMonth->next() ) {
            retVal.lastmonth = ppStmtGetFillLastMonth->value(0).toDouble()/unit.getVolumeConversionFactor();
            success = true;
        }
        else {
            success = false;
        }
    }
    if (success) {
        if (ppStmtGetFillLastYear->exec() && ppStmtGetFillLastYear->next() ) {
            retVal.lastyear = ppStmtGetFillLastYear->value(0).toDouble()/unit.getVolumeConversionFactor();
            success = true;
        }
        else {
            success = false;
        }
    }

    return retVal;
}

Database::dbtimespan DatabaseSqlite::getTotalConsumption(UnitSystem unit)
{
    bool success = false;
    dbtimespan retVal = {0, 0, 0};

    // QSqlQuery.bindValue is void, we'll have to assume it worked
    ppStmtGetConsumOverall->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetConsumLastMonth->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetConsumLastYear->bindValue(":carid",getCurrentCar().getId());

    if (ppStmtGetConsumOverall->exec() && ppStmtGetConsumOverall->next() ) {
        if (unit.getConsumeUnit() == UnitSystem::SI) {
            retVal.overall = ppStmtGetConsumOverall->value(0).toDouble();
        }
        else {
            retVal.overall = unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/ppStmtGetConsumOverall->value(0).toDouble();
        }
        success = true;
    }
    else {
        success = false;
    }

    if (success) {
        if (ppStmtGetConsumLastMonth->exec() && ppStmtGetConsumLastMonth->next() ) {
            if (unit.getConsumeUnit() == UnitSystem::SI) {
                retVal.lastmonth = ppStmtGetConsumLastMonth->value(0).toDouble();
            }
            else {
                retVal.lastmonth = unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/ppStmtGetConsumLastMonth->value(0).toDouble();
            }
            success = true;
        }
        else {
            success = false;
        }
    }
    if (success) {
        if (ppStmtGetConsumLastYear->exec() && ppStmtGetConsumLastYear->next() ) {
            if (unit.getConsumeUnit() == UnitSystem::SI) {
                retVal.lastyear = ppStmtGetConsumLastYear->value(0).toDouble();
            }
            else {
                retVal.lastyear = unit.getVolumeConversionFactor()/unit.getLengthConversionFactor()*100.0/ppStmtGetConsumLastYear->value(0).toDouble();
            }
            success = true;
        }
        else {
            success = false;
        }
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Query all alarm type data and return it as a vector
//--------------------------------------------------------------------------
vector<AlarmtypeData> DatabaseSqlite::getAlarmTypeData(void)
{
    vector<AlarmtypeData> data;

    ppStmtGetAlarmtype->bindValue(":carid",getCurrentCar().getId());

    if (ppStmtGetAlarmtype->exec()) {
        while (ppStmtGetAlarmtype->next()) {
            AlarmtypeData alarmtypeRecord;
            QString lastDate;
            double lastKm;
            qlonglong alarmId;

            alarmId = ppStmtGetAlarmtype->value(4).toInt();

            alarmtypeRecord.setShortDesc(ppStmtGetAlarmtype->value(0).toString());
            alarmtypeRecord.setLongDesc(ppStmtGetAlarmtype->value(1).toString());
            alarmtypeRecord.setDistance(ppStmtGetAlarmtype->value(2).toInt());
            alarmtypeRecord.setInterval(ppStmtGetAlarmtype->value(3).toInt());
            alarmtypeRecord.setId(alarmId);

            getLastEvent(alarmId, lastDate, lastKm);

            alarmtypeRecord.setLastDate(lastDate);
            alarmtypeRecord.setLastKm(lastKm);

            // Store to vector
            data.push_back(alarmtypeRecord);
        }
    }

    return data;
}

//--------------------------------------------------------------------------
// Add new alarm type
//--------------------------------------------------------------------------
bool DatabaseSqlite::addAlarmType(AlarmtypeData &alarmType)
{
    bool retVal = false;

    ppStmtAddAlarmtype->bindValue(":carid",getCurrentCar().getId());
    ppStmtAddAlarmtype->bindValue(":shortdesc",alarmType.getShortDesc());
    ppStmtAddAlarmtype->bindValue(":distance",alarmType.getDistance());
    ppStmtAddAlarmtype->bindValue(":interval",alarmType.getInterval());
    ppStmtAddAlarmtype->bindValue(":longdesc",alarmType.getLongDesc());

    // Finally, do the update
    if (ppStmtAddAlarmtype->exec()) {
        // @todo do we need to ppStmtAddAlarmtype->next()
        qDebug("Adding an alarm type was succesful");
        retVal = true;
    }
    else {
        qDebug("Adding an alarm type was not succesful");
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Query all alarm type data and return it as a vector
//--------------------------------------------------------------------------
bool DatabaseSqlite::getLastEvent(qlonglong alarmid, QString &date, double &km)
{
    bool retVal = false;

    ppStmtGetLastEvent->bindValue(":alarmid",alarmid);

    if (ppStmtGetLastEvent->exec() && ppStmtGetLastEvent->next()) {
            date = ppStmtGetLastEvent->value(0).toString();
            km = ppStmtGetLastEvent->value(1).toDouble();
            retVal = true;
    }
    else {
        date = QString("");
        km = 0.0;
        retVal = false;
    }

    return retVal;
}

//--------------------------------------------------------------------------
// Query event data and return in as a vector
//--------------------------------------------------------------------------
vector<AlarmeventData> DatabaseSqlite::getAlarmeventData(qlonglong alarmid)
{
    vector<AlarmeventData> data;
    UnitSystem unitSystem;

    ppStmtGetEvents->bindValue(":alarmid",alarmid);

    if (ppStmtGetEvents->exec()) {
        while (ppStmtGetEvents->next()) {
            qlonglong recordId;
            AlarmeventData eventRecord;
            Fuelrecord* fuelRecord;

            recordId = ppStmtGetEvents->value(3).toInt();
            eventRecord.setDate(ppStmtGetEvents->value(0).toString());
            eventRecord.setKm(ppStmtGetEvents->value(1).toDouble());
            eventRecord.setId(ppStmtGetEvents->value(2).toInt());
            eventRecord.setRecordId(recordId);

            // Fetch corresponding fuel record data and store it to eventRecord:
            fuelRecord = queryOneRecord(recordId, unitSystem);
            eventRecord.setOil(fuelRecord->getOil().toDouble());
            eventRecord.setTires(fuelRecord->getTires().toDouble());
            eventRecord.setService(fuelRecord->getService().toDouble());
            eventRecord.setNotes(fuelRecord->getNotes().toString());

            // Store to vector
            data.push_back(eventRecord);
        }
    }

    return data;
}

qlonglong DatabaseSqlite::addNewAlarmEvent(AlarmeventData &event, UnitSystem unit)
{
    Fuelrecord *record = new Fuelrecord(unit);
    qlonglong affectedId;
    qlonglong addedId=0;

    qlonglong alarmid = event.getAlarmId();
    QString date = event.getDate();
    double km = event.getKm();
    double service = event.getService();
    double oil = event.getOil();
    double tires = event.getTires();
    QString notes = event.getNotes();

    record->setAllValuesUserUnit(date, km, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
                                 service, oil, tires,
                                 0.0, 0.0, "", /* @todo Add geographical location */
                                 notes,
                                 (qlonglong)0 /* id will be generated by the database add method */);

    // Add to fuel database
    affectedId = addNewRecord(*record, false);

    // Add to alarm event database
    ppStmtAddEvent->bindValue(":alarmid",alarmid);
    ppStmtAddEvent->bindValue(":carid",getCurrentCar().getId());
    ppStmtAddEvent->bindValue(":driverid",getCurrentDriver().getId());
    ppStmtAddEvent->bindValue(":recordid",record->getId());
    ppStmtAddEvent->bindValue(":day",date);
    ppStmtAddEvent->bindValue(":km",km);

    if (ppStmtAddEvent->exec()) {
        addedId = ppStmtAddEvent->lastInsertId().toLongLong();
    }

    delete record;

    return addedId;
}

qlonglong DatabaseSqlite::updateAlarmEvent(AlarmeventData &event, UnitSystem unit)
{
    Fuelrecord *record = new Fuelrecord(unit);
    qlonglong affectedId;
    qlonglong addedId=0;

    qlonglong id = event.getId();
    qlonglong recordid = event.getRecordId();
    QString date = event.getDate();
    double km = event.getKm();
    double service = event.getService();
    double oil = event.getOil();
    double tires = event.getTires();
    QString notes = event.getNotes();

    record->setAllValuesUserUnit(date, km, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
                                 service, oil, tires,
                                 0.0, 0.0, "", /* @todo Add geographical location */
                                 notes,
                                 recordid);

    // Update fuel database
    affectedId = updateRecord(*record, false);

    // Update alarm event database
    ppStmtUpdateEvent->bindValue(":id",id);
    ppStmtUpdateEvent->bindValue(":day",date);
    ppStmtUpdateEvent->bindValue(":km",km);

    if (ppStmtUpdateEvent->exec()) {
        addedId = ppStmtUpdateEvent->lastInsertId().toLongLong();
    }

    delete record;

    return addedId;
}

int DatabaseSqlite::getNumOfRecordEvents(qlonglong id)
{
    int numEvents = 0;

    ppStmtGetNumOfRecordEvents->bindValue(":recordid",id);

    if (ppStmtGetNumOfRecordEvents->exec() && ppStmtGetNumOfRecordEvents->next()) {
        numEvents = ppStmtGetNumOfRecordEvents->value(0).toInt();
    }

    return numEvents;
}

bool DatabaseSqlite::deleteEventWithRecordId(qlonglong id)
{
    int success = false;

    ppStmtDeleteEventwithRecordid->bindValue(":recordid",id);

    if (ppStmtDeleteEventwithRecordid->exec() && ppStmtDeleteEventwithRecordid->next()) {
        success = true;
    }

    return success;
}

bool DatabaseSqlite::deleteEvent(qlonglong id, bool deleteFuelRecord)
{
    bool success = false;
    qlonglong recordId;

    ppStmtDeleteEvent->bindValue(":id",id);
    ppStmtGetOneEvent->bindValue(":id",id);

    // 1. Fetch the record from database
    if (ppStmtGetOneEvent->exec() && ppStmtGetOneEvent->next()) {
        // 2. Take a note of the recordid
        recordId = ppStmtGetOneEvent->value(2).toLongLong();

        // 3. Remove the event
        if (ppStmtDeleteEvent->exec() /*&& ppStmtDeleteEvent->next()*/) {

            success = true;

            // 4. Remove the record from fuel database
            if (deleteFuelRecord) {
                deleteRecord(recordId, false);
            }
        }

    }

    return success;
}

//--------------------------------------------------------------------------
// Query all car data and return in as a vector
//--------------------------------------------------------------------------
vector<CarStatistics> DatabaseSqlite::getCarStatistics(UnitSystem unit)
{
    vector<CarStatistics> data;

    ppStmtGetReport->bindValue(":carid",getCurrentCar().getId());
    ppStmtGetReport->bindValue(":carid2",getCurrentCar().getId());

    if (ppStmtGetReport->exec()) {
        while (ppStmtGetReport->next()) {
            CarStatistics carStatistics(unit);

            carStatistics.setYear(ppStmtGetReport->value(0).toString());
            carStatistics.setMinKm(ppStmtGetReport->value(1).toDouble());
            carStatistics.setMaxKm(ppStmtGetReport->value(2).toDouble());
            carStatistics.setTotalFill(ppStmtGetReport->value(4).toDouble());
            carStatistics.setTotalPrice(ppStmtGetReport->value(5).toDouble());
            carStatistics.setTotalTrip(ppStmtGetReport->value(6).toDouble());
            carStatistics.setTotalOil(ppStmtGetReport->value(9).toDouble());
            carStatistics.setTotalService(ppStmtGetReport->value(11).toDouble());
            carStatistics.setTotalTires(ppStmtGetReport->value(13).toDouble());

            // Store to vector
            data.push_back(carStatistics);
        }
    }

    return data;
}
