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

#ifndef UIWRAPPER_H
#define UIWRAPPER_H

#include <QObject>
#include <QStandardItemModel>
#include <QNetworkAccessManager>

#include "database.h"
#include "cardata.h"
#include "userconfig.h"
#include "geocode.h"
#include "roleitemmodel.h"
#include "mysortfilterproxymodel.h"
#include "plotdatamodel.h"

class UiWrapper : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(NOTIFY addressReady)
public:

    /*explicit*/ UiWrapper(Database *db = 0, Geocode *gc = 0);
    virtual ~UiWrapper();

    MySortFilterProxyModel* getFuelEntryModel(void);
    RoleItemModel* getCarEntryModel(void);
    RoleItemModel* getDriverEntryModel(void);
    MySortFilterProxyModel* getAlarmEntryModel(void);
    MySortFilterProxyModel* getAlarmEventModel(void);
    MySortFilterProxyModel* getCarStatisticsModel(void);
    PlotDataModel* getStatisticsModel(void);
    void calcCarStatistics(QString id, double &totalKm, double &lastMonthKm, double &lastYearKm,
                             double &totalFill, double &lastMonthFill, double &lastYearFill,
                             double &totalConsum, double &lastMonthConsum, double &lastYearConsum);

    // Exposed to Qml
    Q_INVOKABLE bool dataBaseDidNotExist(void);
    Q_INVOKABLE void clearDataBaseDidNotExist(void);

    Q_INVOKABLE void saveConfig(void);

    Q_INVOKABLE void setSearchExpression(QString searchExp);

    Q_INVOKABLE QString getSearchExpression(void);

    Q_INVOKABLE void addFuelEntry(int carid, QString date, double km, double trip, double fill, bool notFull,
                                  double price, double service, double oil, double tires, double lat, double lon,
                                  QString place, QString notes);

    Q_INVOKABLE void updateFuelEntry(int carid, QString id, QString date, double km, double trip, double fill, bool notFull,
                                     double price, double service, double oil, double tires, double lat, double lon,
                                     QString place, QString notes);

    Q_INVOKABLE float calcTrip(double newkm, double trip);

    Q_INVOKABLE void deleteRecord(QString id);

    Q_INVOKABLE void getStatistics(int year, int statSelect);

    Q_INVOKABLE QString getCarMark(int carid);

    Q_INVOKABLE QString getCarModel(int carid);

    Q_INVOKABLE double getTotalKm(int carid);

    Q_INVOKABLE double getLastMonthKm(int carid);

    Q_INVOKABLE double getLastYearKm(int carid);

    Q_INVOKABLE double getTotalFill(int carid);

    Q_INVOKABLE double getLastMonthFill(int carid);

    Q_INVOKABLE double getLastYearFill(int carid);

    Q_INVOKABLE double getTotalConsum(int carid);

    Q_INVOKABLE double getLastMonthConsum(int carid);

    Q_INVOKABLE double getLastYearConsum(int carid);

    Q_INVOKABLE void addDriver(QString fullname, QString nickname);

    Q_INVOKABLE void updateDriver(QString id, QString fullname, QString nickname);

    Q_INVOKABLE void deleteDriver(QString id);

    Q_INVOKABLE void addCar(QString mark, QString model, QString year, QString regist, QString notes, quint8 fueltype);

    Q_INVOKABLE void updateCar(QString id, QString mark, QString model, QString year, QString regist, QString notes, quint8 fueltype);

    Q_INVOKABLE void deleteCar(QString id);

    Q_INVOKABLE void addAlarmType(qlonglong carId, QString shortDesc, quint32 interval, quint32 distance, QString longDesc);

    Q_INVOKABLE void addAlarmEvent(qlonglong alarmId, QString date,
                                              double km, double service, double oil, double tires, QString notes);

    Q_INVOKABLE void updateAlarmEvent(qlonglong id, qlonglong alarmid, qlonglong recordId, QString date,
                                                 double km, double service, double oil, double tires, QString notes);

    Q_INVOKABLE void deleteEvent(QString id);

    Q_INVOKABLE void setSortColumn(int col, Qt::SortOrder order);

    Q_INVOKABLE void setCurrentCar(int carid);

    Q_INVOKABLE void setMainUnit(int unit, bool individualUnit);

    Q_INVOKABLE void setLengthUnit(int unit);

    Q_INVOKABLE void setMassUnit(int unit);

    Q_INVOKABLE void setVolumeUnit(int unit);

    Q_INVOKABLE void setConsumeUnit(int unit);

    Q_INVOKABLE int getMainUnit(void);

    Q_INVOKABLE bool getIndividualUnit(void);

    Q_INVOKABLE int getLengthUnit(void);

    Q_INVOKABLE int getMassUnit(void);

    Q_INVOKABLE int getVolumeUnit(void);

    Q_INVOKABLE int getConsumeUnit(void);

    Q_INVOKABLE QString getCurrencySymbol(void);

    Q_INVOKABLE QString requestAddress(double lat, double lon);

    Q_INVOKABLE QString getAddress(void);

    Q_INVOKABLE void addAllRecordsToAlarmEventModel(qlonglong alarmid);

public slots:
    void replyFinished(void);

signals:
    void addressReady(void);


private:
    Database *dataBase;
    UnitSystem *unitSystem;
    CarData *carData;
    UserConfig userConfig;
    Geocode *geoCode;
    QString geoAddress;

    RoleItemModel *fuelEntryModel;
    RoleItemModel *carDataModel;
    RoleItemModel *driverDataModel;
    RoleItemModel *alarmEntryModel;
    RoleItemModel *alarmEventModel;
    RoleItemModel *carStatisticsModel;
    MySortFilterProxyModel *sortModel;
    MySortFilterProxyModel *alarmSortModel;
    MySortFilterProxyModel *alarmEventSortModel;
    MySortFilterProxyModel *carStatisticsSortModel;
    PlotDataModel *statisticsModel;

    void updateAllModels(void);
    void reReadAllModels(void);
    QStandardItem *findFuelEntry(QString id);
    QStandardItem* findCar(QString id);
    QStandardItem* findDriver(QString id);
    QStandardItem* findAlarmEvent(QString id);
    void addAllRecordsToCarEntryModel(QStandardItemModel *model);
    void addAllRecordsToFuelEntryModel(QStandardItemModel *model);
    void addAllRecordsToDriverEntryModel(QStandardItemModel *model);
    void addAllRecordsToAlarmEntryModel(QStandardItemModel *model);
    void addDataToStatisticsModel(PlotDataModel *model, double x, double y);
    void addDataToCarStatisticsModel(QStandardItemModel *model);

    void createFuelEntryModel(void);
    void createCarDataModel(void);
    void createDriverDataModel(void);
    void createAlarmEntryModel(void);
    void createAlarmEventModel(void);
    void createStatisticsModel(void);
    void createCarStatisticsModel(void);
};

#endif // UIWRAPPER_H
