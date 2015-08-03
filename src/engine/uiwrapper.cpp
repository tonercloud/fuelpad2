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

#include "uiwrapper.h"
#include "roleitemmodel.h"
#include "mysortfilterproxymodel.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

// For debugging only
#include <iostream>

#define SEPARATOR " "

// Fuel entry fields
#define FIELD_DATE    0
#define FIELD_KM      1
#define FIELD_TRIP    2
#define FIELD_FILL    3
#define FIELD_CONSUM  4
#define FIELD_PRICE   5
#define FIELD_PPT     6
#define FIELD_PPL     7
#define FIELD_SERVICE 8
#define FIELD_OIL     9
#define FIELD_TIRES  10
#define FIELD_LAT    11
#define FIELD_LON    12
#define FIELD_PLACE  13
#define FIELD_NOTES  14
#define FIELD_ID     15

// Alarm entry fields
#define ALARM_FIELD_DESC      0
#define ALARM_FIELD_KMLIMIT   1
#define ALARM_FIELD_TIMELIMIT 2
#define ALARM_FIELD_NEXTKM    3
#define ALARM_FIELD_NEXTDATE  4
#define ALARM_FIELD_LASTKM    5
#define ALARM_FIELD_LASTDATE  6
#define ALARM_FIELD_ID        7

// Alarm event fields
#define ALARM_EVENT_FIELD_DATE      0
#define ALARM_EVENT_FIELD_KMLIMIT   1
#define ALARM_EVENT_FIELD_ID        2
#define ALARM_EVENT_FIELD_RECORDID  3

struct FuelEntry {
    enum FuelEntryRoles {
        DateRole = Qt::UserRole + 1,
        KmRole,
        TripRole,
        FillRole,
        NotFullRole,
        ConsumRole,
        CO2Role,
        PriceRole,
        PptRole,
        PplRole,
        ServiceRole,
        OilRole,
        TiresRole,
        LatRole,
        LonRole,
        PlaceRole,
        NotesRole,
        IdRole
    };
};

struct CarEntry {
    enum CarEntryRoles {
        MarkRole = Qt::UserRole + 1,
        ModelRole,
        YearRole,
        RegnumRole,
        NotesRole,
        FueltypeRole,
        TotalKmRole,
        LastMonthKmRole,
        LastYearKmRole,
        TotalFillRole,
        LastMonthFillRole,
        LastYearFillRole,
        TotalConsumptionRole,
        LastMonthConsumptionRole,
        LastYearConsumptionRole,
        IdRole
    };
};

struct DriverEntry {
    enum DriverEntryRoles {
        FullnameRole = Qt::UserRole + 1,
        NicknameRole,
        IdRole
    };
};

struct AlarmEntry {
    enum AlarmEntryRoles {
        DescrptionRole = Qt::UserRole + 1,
        LongDescriptionRole,
        KmLimitRole,
        TimeLimitRole,
        NextKmRole,
        NextDateRole,
        LastKmRole,
        LastDateRole,
        KmExpiredRole,
        DateExpiredRole,
        IdRole
    };
};

struct AlarmEventEntry {
    enum AlarmEventEntryRoles {
        DateRole = Qt::UserRole + 1,
        KmRole,
        ServiceRole,
        OilRole,
        TiresRole,
        NotesRole,
        CarIdRole,
        RecordIdRole,
        IdRole
    };
};

struct StatisticsData {
    enum StatisticsDataRoles {
        xRole = Qt::UserRole + 1,
        yRole
    };
};

struct CarStatisticsEntry {
    enum CarStatisticsEntryRoles {
        YearRole = Qt::UserRole + 1,
        MinKmRole,
        MaxKmRole,
        AverageConsumptionRole,
        TotalFillRole,
        TotalPriceRole,
        TotalTripRole,
        TotalOilRole,
        TotalServiceRole,
        TotalTiresRole
    };
};


UiWrapper::UiWrapper(Database *db, Geocode *gc)
{
    dataBase = db;
    geoCode = gc;
    unitSystem = new UnitSystem;

    // Read data from config

//    unitSystem->setMainUnit(UnitSystem::SI);
//    unitSystem->setIndividualUnit(false);
    userConfig.settings.beginGroup("Unit");
    unitSystem->setMainUnit((enum UnitSystem::unit)userConfig.settings.value("UnitSystem", (int)UnitSystem::SI).toInt());
    unitSystem->setIndividualUnit(userConfig.settings.value("IndividualUnit", false).toBool());
    unitSystem->setLengthUnit((enum UnitSystem::unit)userConfig.settings.value("LengthUnit", (int)UnitSystem::SI).toInt());
    unitSystem->setVolumeUnit((enum UnitSystem::unit)userConfig.settings.value("VolumeUnit", (int)UnitSystem::SI).toInt());
    unitSystem->setConsumeUnit((enum UnitSystem::unit)userConfig.settings.value("ConsumeUnit", (int)UnitSystem::SI).toInt());
    unitSystem->setMassUnit((enum UnitSystem::unit)userConfig.settings.value("MassUnit", (int)UnitSystem::SI).toInt());
    userConfig.settings.endGroup();

    // For testing
//    unitSystem->setMainUnit(UnitSystem::US);
//    unitSystem->setLengthUnit(UnitSystem::US);
//    unitSystem->setVolumeUnit(UnitSystem::US);
//    unitSystem->setConsumeUnit(UnitSystem::US);
//    unitSystem->setMassUnit(UnitSystem::US);

    // Comment out since settings don't seem to work with simulator
//    db->setCurrentCar(userConfig.settings.value("CurrentCar",1).toInt());
//    db->setCurrentDriver(userConfig.settings.value("CurrentDriver",1).toInt());

    // Create models needed in Qml
    createFuelEntryModel();
    createCarDataModel();
    createDriverDataModel();
    createAlarmEntryModel();
    createAlarmEventModel();
    createStatisticsModel();
    createCarStatisticsModel();

}

UiWrapper::~UiWrapper()
{
    saveConfig();
    delete unitSystem;
    delete sortModel;
}

static void setDataToFuelEntryModel(QStandardItem *it, Fuelrecord *data)
{
    it->setData(data->getDateUserUnit(), FuelEntry::DateRole);
    it->setData(data->getKmUserUnit(), FuelEntry::KmRole);
    it->setData(data->getTripUserUnit(), FuelEntry::TripRole);
    it->setData(data->getFillUserUnit(), FuelEntry::FillRole);
    it->setData(data->getNotFullFill(), FuelEntry::NotFullRole);
    it->setData(data->getConsumUserUnit(), FuelEntry::ConsumRole);
    it->setData(data->getCO2EmissionUserUnit(), FuelEntry::CO2Role);
    it->setData(data->getPriceUserUnit(), FuelEntry::PriceRole);
    it->setData(data->getPptUserUnit(), FuelEntry::PptRole);
    it->setData(data->getPplUserUnit(), FuelEntry::PplRole);
    it->setData(data->getServiceUserUnit(), FuelEntry::ServiceRole);
    it->setData(data->getOilUserUnit(), FuelEntry::OilRole);
    it->setData(data->getTiresUserUnit(), FuelEntry::TiresRole);
    it->setData(data->getLatitude(), FuelEntry::LatRole);
    it->setData(data->getLongitude(), FuelEntry::LonRole);
    it->setData(data->getPlace(), FuelEntry::PlaceRole);
    it->setData(data->getNotes(), FuelEntry::NotesRole);
    it->setData(data->getId(), FuelEntry::IdRole);
}

static void addRecordToFuelEntryModel(QStandardItemModel *model, Fuelrecord *data)
{
    QStandardItem* it = new QStandardItem();
    setDataToFuelEntryModel(it, data);
    model->appendRow(it);
//    model->insertRow(0, it);
}

static void setStatDataToCarEntryModel(QStandardItem *it, qlonglong id,
                                   double totalKm, double lastMonthKm, double lastYearKm,
                                   double totalFill, double lastMonthFill, double lastYearFill,
                                   double totalConsum, double lastMonthConsum, double lastYearConsum)
{
    it->setData(QVariant(totalKm), CarEntry::TotalKmRole);
    it->setData(QVariant(lastMonthKm), CarEntry::LastMonthKmRole);
    it->setData(QVariant(lastYearKm), CarEntry::LastYearKmRole);
    it->setData(QVariant(totalFill), CarEntry::TotalFillRole);
    it->setData(QVariant(lastMonthFill), CarEntry::LastMonthFillRole);
    it->setData(QVariant(lastYearFill), CarEntry::LastYearFillRole);
    it->setData(QVariant(totalConsum), CarEntry::TotalConsumptionRole);
    it->setData(QVariant(lastMonthConsum), CarEntry::LastMonthConsumptionRole);
    it->setData(QVariant(lastYearConsum), CarEntry::LastYearConsumptionRole);
}

static void setDataToCarEntryModel(QStandardItem *it, CarData *data,
                                   double totalKm, double lastMonthKm, double lastYearKm,
                                   double totalFill, double lastMonthFill, double lastYearFill,
                                   double totalConsum, double lastMonthConsum, double lastYearConsum)
{
    int id = data->getId();
    it->setData(data->getMark(), CarEntry::MarkRole);
    it->setData(data->getModel(), CarEntry::ModelRole);
    it->setData(data->getYear(), CarEntry::YearRole);
    it->setData(data->getRegNum(), CarEntry::RegnumRole);
    it->setData(data->getNotes(), CarEntry::NotesRole);
    it->setData(data->getFuelType(), CarEntry::FueltypeRole);
    it->setData(id, CarEntry::IdRole);

    setStatDataToCarEntryModel(it, id, totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                               totalConsum, lastMonthConsum, lastYearConsum);

}

static void addRecordToCarEntryModel(QStandardItemModel *model, CarData *data,
                                     double totalKm, double lastMonthKm, double lastYearKm,
                                     double totalFill, double lastMonthFill, double lastYearFill,
                                     double totalConsum, double lastMonthConsum, double lastYearConsum)
{
    QStandardItem* it = new QStandardItem();
    setDataToCarEntryModel(it, data, totalKm, lastMonthKm, lastYearKm,
                           totalFill, lastMonthFill, lastYearFill,
                           totalConsum, lastMonthConsum, lastYearConsum);
    model->appendRow(it);
//    model->insertRow(0, it);
}

static void setDataToDriverEntryModel(QStandardItem *it, DriverData *data)
{
    int id = data->getId();
    it->setData(data->getFullName(), DriverEntry::FullnameRole);
    it->setData(data->getNickName(), DriverEntry::NicknameRole);
    it->setData(id, DriverEntry::IdRole);
}

static void setDataToAlarmEntryModel(QStandardItem *it, AlarmtypeData *data, double currentKm)
{
    int id = data->getId();
    it->setData(data->getShortDesc(), AlarmEntry::DescrptionRole);
    it->setData(data->getDistance(), AlarmEntry::KmLimitRole);
    it->setData(data->getLongDesc(), AlarmEntry::LongDescriptionRole);
    it->setData(data->getInterval(), AlarmEntry::TimeLimitRole);
    it->setData(data->getNextKm(), AlarmEntry::NextKmRole);
    it->setData(data->getNextDate(), AlarmEntry::NextDateRole);

    it->setData(data->getLastKm(), AlarmEntry::LastKmRole);
    it->setData(data->getLastDate(), AlarmEntry::LastDateRole);

    it->setData(data->getKmExpired(currentKm), AlarmEntry::KmExpiredRole);
    it->setData(data->getDateExpired(), AlarmEntry::DateExpiredRole);

    it->setData(id, AlarmEntry::IdRole);
}

static void setDataToAlarmEventModel(QStandardItem *it, AlarmeventData *data)
{
    int id = data->getId();
    it->setData(data->getDate(), AlarmEventEntry::DateRole);
    it->setData(data->getKm(), AlarmEventEntry::KmRole);
    it->setData(data->getOil(), AlarmEventEntry::OilRole);
    it->setData(data->getTires(), AlarmEventEntry::TiresRole);
    it->setData(data->getService(), AlarmEventEntry::ServiceRole);
    it->setData(data->getNotes(), AlarmEventEntry::NotesRole);
    it->setData(data->getCarId(), AlarmEventEntry::CarIdRole);
    it->setData(data->getRecordId(), AlarmEventEntry::RecordIdRole);
    it->setData(id, AlarmEventEntry::IdRole);
}


static void addRecordToDriverEntryModel(QStandardItemModel *model, DriverData *data)
{
    QStandardItem* it = new QStandardItem();
    setDataToDriverEntryModel(it, data);
    model->appendRow(it);
//    model->insertRow(0, it);
}

static void addRecordToAlarmEntryModel(QStandardItemModel *model, AlarmtypeData *data, double currentKm)
{
    QStandardItem* it = new QStandardItem();
    setDataToAlarmEntryModel(it, data, currentKm);
    model->appendRow(it);
}

static void addRecordToAlarmEventModel(QStandardItemModel *model, AlarmeventData *data)
{
    QStandardItem* it = new QStandardItem();
    setDataToAlarmEventModel(it, data);
    model->appendRow(it);
}

static void setDataToCarStatisticsModel(QStandardItem *it, CarStatistics *data)
{
    it->setData(data->getYear(), CarStatisticsEntry::YearRole);
    it->setData(data->getMinKm(), CarStatisticsEntry::MinKmRole);
    it->setData(data->getMaxKm(), CarStatisticsEntry::MaxKmRole);
    it->setData(data->getAverageConsumption(), CarStatisticsEntry::AverageConsumptionRole);
    it->setData(data->getTotalFill(), CarStatisticsEntry::TotalFillRole);
    it->setData(data->getTotalPrice(), CarStatisticsEntry::TotalPriceRole);
    it->setData(data->getTotalTrip(), CarStatisticsEntry::TotalTripRole);
    it->setData(data->getTotalOil(), CarStatisticsEntry::TotalOilRole);
    it->setData(data->getTotalService(), CarStatisticsEntry::TotalServiceRole);
    it->setData(data->getTotalTires(), CarStatisticsEntry::TotalTiresRole);
}

static void addRecordToCarStatisticsModel(QStandardItemModel *model, CarStatistics *data)
{
    QStandardItem* it = new QStandardItem();
    setDataToCarStatisticsModel(it, data);
    model->appendRow(it);
}

bool UiWrapper::dataBaseDidNotExist(void)
{
    return dataBase->didNotExist();
}

void UiWrapper::clearDataBaseDidNotExist(void)
{
    dataBase->clearDidNotExist();
}

void UiWrapper::saveConfig(void)
{
    userConfig.settings.beginGroup("Unit");
    userConfig.settings.setValue("UnitSystem",(int)unitSystem->getMainUnit());
    userConfig.settings.setValue("IndividualUnit", (bool)unitSystem->getIndividualUnit());
    userConfig.settings.setValue("LengthUnit", (int)unitSystem->getLengthUnit());
    userConfig.settings.setValue("VolumeUnit", (int)unitSystem->getVolumeUnit());
    userConfig.settings.setValue("ConsumeUnit", (int)unitSystem->getConsumeUnit());
    userConfig.settings.setValue("MassUnit", (int)unitSystem->getMassUnit());
    userConfig.settings.endGroup();
}

void UiWrapper::setSearchExpression(QString searchExp)
{
    QRegExp regExp(searchExp, Qt::CaseInsensitive, QRegExp::Wildcard);
    sortModel->setFilterRegExp(regExp);
    sortModel->invalidate();
}

QString UiWrapper::getSearchExpression(void)
{
    QRegExp regExp = sortModel->filterRegExp();
    return regExp.pattern();
}

void UiWrapper::updateAllModels(void)
{
    qDebug("Updating models");
    sortModel->invalidate();
    alarmSortModel->invalidate();
    alarmEventSortModel->invalidate();
    qDebug("Models updated");
}

void UiWrapper::reReadAllModels(void)
{
    // Fuel entry model
    fuelEntryModel->clear();
    addAllRecordsToFuelEntryModel(fuelEntryModel);

    // Car data model
    carDataModel->clear();
    addAllRecordsToCarEntryModel(carDataModel);

    // Driver data model
    driverDataModel->clear();
    addAllRecordsToDriverEntryModel(driverDataModel);

    // Alarm types model
    alarmEntryModel->clear();
    addAllRecordsToAlarmEntryModel(alarmEntryModel);

    // Car statistics model
    carStatisticsModel->clear();
    addDataToCarStatisticsModel(carStatisticsModel);
}

QStandardItem* UiWrapper::findFuelEntry(QString id)
{
    QStandardItem *it = 0;
    QList<QModelIndex> items = fuelEntryModel->match(fuelEntryModel->index(0,0), FuelEntry::IdRole, QVariant(id));
    if (items.size() == 1) {
        it = fuelEntryModel->itemFromIndex(items.at(0));
    }

    return it;
}

QStandardItem* UiWrapper::findCar(QString id)
{
    QStandardItem *it = 0;
    QList<QModelIndex> items = carDataModel->match(carDataModel->index(0,0), CarEntry::IdRole, QVariant(id));
    if (items.size() == 1) {
        it = carDataModel->itemFromIndex(items.at(0));
    }

    return it;
}

QStandardItem* UiWrapper::findDriver(QString id)
{
    QStandardItem *it = 0;
    QList<QModelIndex> items = driverDataModel->match(driverDataModel->index(0,0), DriverEntry::IdRole, QVariant(id));
    if (items.size() == 1) {
        it = driverDataModel->itemFromIndex(items.at(0));
    }

    return it;
}

QStandardItem* UiWrapper::findAlarmEvent(QString id)
{
    QStandardItem *it = 0;
    QList<QModelIndex> items = alarmEventModel->match(fuelEntryModel->index(0,0), AlarmEventEntry::IdRole, QVariant(id));
    if (items.size() == 1) {
        it = alarmEventModel->itemFromIndex(items.at(0));
    }

    return it;
}


void UiWrapper::addAllRecordsToFuelEntryModel(QStandardItemModel *model)
{
    vector<Fuelrecord> fuelData;

    if (dataBase->isOpen()) {

        fuelData = dataBase->getRecordData(*unitSystem);

        for (vector<Fuelrecord>::size_type i=0; i < fuelData.size(); i++) {
            addRecordToFuelEntryModel(model, &fuelData[i]);
        }

    }
}

void UiWrapper::addAllRecordsToCarEntryModel(QStandardItemModel *model)
{
    // @todo Note that an item called carData is already defined in this class
    vector<CarData> carData;
    int activeIndex;

    if (dataBase->isOpen()) {

        carData = dataBase->getCarData();

        for (vector<CarData>::size_type i=0; i < carData.size(); i++) {
            double totalKm, lastMonthKm, lastYearKm;
            double totalFill, lastMonthFill, lastYearFill;
            double totalConsum, lastMonthConsum, lastYearConsum;
            totalKm = getTotalKm(carData[i].getId());
            lastMonthKm = getLastMonthKm(carData[i].getId());
            lastYearKm = getLastYearKm(carData[i].getId());
            totalFill = getTotalFill(carData[i].getId());
            lastMonthFill = getLastMonthFill(carData[i].getId());
            lastYearFill = getLastYearFill(carData[i].getId());
            totalConsum = getTotalConsum(carData[i].getId());
            lastMonthConsum = getLastMonthConsum(carData[i].getId());
            lastYearConsum = getLastYearConsum(carData[i].getId());
            addRecordToCarEntryModel(model, &carData[i], totalKm, lastMonthKm, lastYearKm,
                                     totalFill, lastMonthFill, lastYearFill,
                                     totalConsum, lastMonthConsum, lastYearConsum);
            if (carData[i].getId() == dataBase->getCurrentCar().getId()) {
                activeIndex = i;
            }
        }

    }
}

void UiWrapper::addAllRecordsToDriverEntryModel(QStandardItemModel *model)
{
    vector<DriverData> drvData;
    int activeIndex;

    if (dataBase->isOpen()) {

        drvData = dataBase->getDriverData();

        for (vector<DriverData>::size_type i=0; i < drvData.size(); i++) {
            addRecordToDriverEntryModel(model, &drvData[i]);
            if (drvData[i].getId() == dataBase->getCurrentDriver().getId()) {
                activeIndex = i;
            }
        }

    }
}

void UiWrapper::addAllRecordsToAlarmEntryModel(QStandardItemModel *model)
{
    vector<AlarmtypeData> alarmData;
    double currentKm;

    if (dataBase->isOpen()) {

        alarmData = dataBase->getAlarmTypeData();
        currentKm = dataBase->getLastKm();

        for (vector<AlarmtypeData>::size_type i=0; i < alarmData.size(); i++) {
            addRecordToAlarmEntryModel(model, &alarmData[i], currentKm);
        }

    }
}

void UiWrapper::addAllRecordsToAlarmEventModel(qlonglong alarmid)
{
    vector<AlarmeventData> alarmData;
//    int activeIndex;

    if (dataBase->isOpen()) {

        alarmEventModel->clear();
        alarmData = dataBase->getAlarmeventData(alarmid);

        for (vector<AlarmeventData>::size_type i=0; i < alarmData.size(); i++) {
            addRecordToAlarmEventModel(alarmEventModel, &alarmData[i]);
//            if (alarmData[i].getId() == dataBase->getCurrentCar().getId()) {
//                activeIndex = i;
//            }
        }

    }
}

void UiWrapper::addDataToCarStatisticsModel(QStandardItemModel *model)
{
    vector<CarStatistics> carStatistics;

    if (dataBase->isOpen()) {

        carStatistics = dataBase->getCarStatistics(*unitSystem);

        for (vector<CarStatistics>::size_type i=0; i < carStatistics.size(); i++) {
            addRecordToCarStatisticsModel(model, &carStatistics[i]);
        }

    }
}

void UiWrapper::addDataToStatisticsModel(PlotDataModel *model, double x, double y)
{
//    QStandardItem* it = new QStandardItem();
//    it->setData(QVariant(x), StatisticsData::xRole);
//    it->setData(QVariant(y), StatisticsData::yRole);
//    PlotData* it = new PlotData(x, y);
    model->appendData(PlotData(x, y));
}

void UiWrapper::getStatistics(int year, int statSelect)
{
    vector<int> month;
    vector<double> fill;
    vector<double> trip;
    vector<double> consum;
    vector<double> ppl;
    vector<int> rowIndex(12,0);
    vector<double> x(12,0.0);
    vector<double> y(12,0.0);

    for (vector<int>::size_type i=0; i < x.size(); i++) {
        rowIndex.at(i)=i;
    }

    qDebug("Querying year: %d",year);

    dataBase->getMonthlyData(year, *unitSystem, month, fill, trip, consum, ppl);

    vector<double> data;
    switch (statSelect) {
    case 0:
        data = fill;
        break;
    case 1:
        data = trip;
        break;
    case 2:
        data = consum;
        break;
    case 3:
        data = ppl;
        break;
    }

    if (month.size() > 0) {
        for (vector<int>::size_type i=0; i < month.size(); i++) {
            x.at(month.at(i)-1) = month.at(i);
            y.at(month.at(i)-1) = data.at(i);
        }
    }
    else {
        for (vector<int>::size_type i=0; i < x.size(); i++) {
            x.at(i) = i+1;
            y.at(i) = 0.0;
        }
    }

    statisticsModel->setRowData(rowIndex, x, y);
    statisticsModel->sort(0, Qt::AscendingOrder);
}

void UiWrapper::createFuelEntryModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[FuelEntry::DateRole] =  "date";
    roleNames[FuelEntry::KmRole] =  "km";
    roleNames[FuelEntry::TripRole] =  "trip";
    roleNames[FuelEntry::FillRole] =  "fill";
    roleNames[FuelEntry::NotFullRole] =  "notfull";
    roleNames[FuelEntry::ConsumRole] =  "consum";
    roleNames[FuelEntry::CO2Role] = "co2";
    roleNames[FuelEntry::PriceRole] =  "price";
    roleNames[FuelEntry::PptRole] =  "ppt";
    roleNames[FuelEntry::PplRole] =  "ppl";
    roleNames[FuelEntry::ServiceRole] =  "service";
    roleNames[FuelEntry::OilRole] =  "oil";
    roleNames[FuelEntry::TiresRole] =  "tires";
    roleNames[FuelEntry::LatRole] =  "latitude";
    roleNames[FuelEntry::LonRole] =  "longitude";
    roleNames[FuelEntry::PlaceRole] =  "place";
    roleNames[FuelEntry::NotesRole] =  "notes";
    roleNames[FuelEntry::IdRole] =  "databaseid";
    RoleItemModel *model = new RoleItemModel(roleNames);

    sortModel = new MySortFilterProxyModel(this, roleNames);

    addAllRecordsToFuelEntryModel(model);

    fuelEntryModel = model;

    sortModel->setSourceModel(fuelEntryModel);
    sortModel->sort(FIELD_DATE, Qt::DescendingOrder);
    sortModel->setDynamicSortFilter(true);
    sortModel->beginResetModel();
    sortModel->setSortRole(FuelEntry::DateRole);
    sortModel->endResetModel();
    sortModel->invalidate();

}

void UiWrapper::createCarDataModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[CarEntry::MarkRole] =  "mark";
    roleNames[CarEntry::ModelRole] =  "carmodel"; // "model" is not good for qml
    roleNames[CarEntry::YearRole] =  "year";
    roleNames[CarEntry::RegnumRole] =  "regnum";
    roleNames[CarEntry::NotesRole] =  "notes";
    roleNames[CarEntry::FueltypeRole] =  "fueltype";
    roleNames[CarEntry::IdRole] =  "databaseid";
    roleNames[CarEntry::TotalKmRole] =  "totalkm";
    roleNames[CarEntry::LastMonthKmRole] =  "lastmonthkm";
    roleNames[CarEntry::LastYearKmRole] =  "lastyearkm";
    roleNames[CarEntry::TotalFillRole] =  "totalfill";
    roleNames[CarEntry::LastMonthFillRole] =  "lastmonthfill";
    roleNames[CarEntry::LastYearFillRole] =  "lastyearfill";
    roleNames[CarEntry::TotalConsumptionRole] =  "totalconsumption";
    roleNames[CarEntry::LastMonthConsumptionRole] =  "lastmonthconsumption";
    roleNames[CarEntry::LastYearConsumptionRole] =  "lastyearconsumption";
    RoleItemModel *model = new RoleItemModel(roleNames);

    addAllRecordsToCarEntryModel(model);

    carDataModel = model;

}

void UiWrapper::createDriverDataModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[DriverEntry::FullnameRole] =  "fullname";
    roleNames[DriverEntry::NicknameRole] =  "nickname";
    roleNames[DriverEntry::IdRole] =  "databaseid";
    RoleItemModel *model = new RoleItemModel(roleNames);

    addAllRecordsToDriverEntryModel(model);

    driverDataModel = model;

}

void UiWrapper::createAlarmEntryModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AlarmEntry::DescrptionRole] =  "description";
    roleNames[AlarmEntry::LongDescriptionRole] = "longdesc";
    roleNames[AlarmEntry::KmLimitRole] =  "kmlimit";
    roleNames[AlarmEntry::TimeLimitRole] =  "timelimit";
    roleNames[AlarmEntry::NextKmRole] =  "nextkm";
    roleNames[AlarmEntry::NextDateRole] =  "nextdate";
    roleNames[AlarmEntry::LastKmRole] =  "lastkm";
    roleNames[AlarmEntry::LastDateRole] =  "lastdate";
    roleNames[AlarmEntry::KmExpiredRole] =  "kmexpired";
    roleNames[AlarmEntry::DateExpiredRole] =  "dateexpired";
    roleNames[AlarmEntry::IdRole] =  "databaseid";
    RoleItemModel *model = new RoleItemModel(roleNames);

    alarmSortModel = new MySortFilterProxyModel(this, roleNames);

    addAllRecordsToAlarmEntryModel(model);

    alarmEntryModel = model;

    alarmSortModel->setSourceModel(alarmEntryModel);
    alarmSortModel->sort(ALARM_FIELD_NEXTDATE, Qt::DescendingOrder);
    alarmSortModel->setDynamicSortFilter(true);
    alarmSortModel->beginResetModel();
    alarmSortModel->setSortRole(AlarmEntry::NextDateRole);
    alarmSortModel->endResetModel();
    alarmSortModel->invalidate();

}

void UiWrapper::createAlarmEventModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AlarmEventEntry::DateRole] =  "date";
    roleNames[AlarmEventEntry::KmRole] =  "km";
    roleNames[AlarmEventEntry::OilRole] =  "oil";
    roleNames[AlarmEventEntry::TiresRole] =  "tires";
    roleNames[AlarmEventEntry::ServiceRole] =  "service";
    roleNames[AlarmEventEntry::NotesRole] =  "notes";
    roleNames[AlarmEventEntry::CarIdRole] =  "carid";
    roleNames[AlarmEventEntry::RecordIdRole] =  "recordid";
    roleNames[AlarmEventEntry::IdRole] =  "databaseid";
    RoleItemModel *model = new RoleItemModel(roleNames);

    alarmEventSortModel = new MySortFilterProxyModel(this, roleNames);

//    addAllRecordsToAlarmEventModel(model);

    alarmEventModel = model;

    alarmEventSortModel->setSourceModel(alarmEventModel);
    alarmEventSortModel->sort(ALARM_FIELD_NEXTDATE, Qt::DescendingOrder);
    alarmEventSortModel->setDynamicSortFilter(true);
    alarmEventSortModel->beginResetModel();
    alarmEventSortModel->setSortRole(AlarmEventEntry::DateRole);
    alarmEventSortModel->endResetModel();
    alarmEventSortModel->invalidate();

}

void UiWrapper::createStatisticsModel(void)
{
    PlotDataModel *model = new PlotDataModel();

    statisticsModel = model;

    // Preadd zero data to model
    for (int i=1;i<=12;i++) {
        statisticsModel->appendData(PlotData(i,0));
    }

}

void UiWrapper::createCarStatisticsModel(void)
{
    QHash<int, QByteArray> roleNames;
    roleNames[CarStatisticsEntry::YearRole] =  "year";
    roleNames[CarStatisticsEntry::MinKmRole] =  "minkm";
    roleNames[CarStatisticsEntry::MaxKmRole] =  "maxkm";
    roleNames[CarStatisticsEntry::AverageConsumptionRole] = "avgconsum";
    roleNames[CarStatisticsEntry::TotalFillRole] =  "totalfill";
    roleNames[CarStatisticsEntry::TotalPriceRole] =  "totalprice";
    roleNames[CarStatisticsEntry::TotalTripRole] =  "totaltrip";
    roleNames[CarStatisticsEntry::TotalOilRole] =  "totaloil";
    roleNames[CarStatisticsEntry::TotalServiceRole] =  "totalservice";
    roleNames[CarStatisticsEntry::TotalTiresRole] =  "totaltires";
    RoleItemModel *model = new RoleItemModel(roleNames);

    carStatisticsSortModel = new MySortFilterProxyModel(this, roleNames);

    addDataToCarStatisticsModel(model);

    carStatisticsModel = model;

    carStatisticsSortModel->setSourceModel(carStatisticsModel);
    carStatisticsSortModel->sort(0, Qt::AscendingOrder);
    carStatisticsSortModel->setDynamicSortFilter(true);
    carStatisticsSortModel->beginResetModel();
    carStatisticsSortModel->setSortRole(CarStatisticsEntry::YearRole);
    carStatisticsSortModel->endResetModel();
    carStatisticsSortModel->invalidate();

}


MySortFilterProxyModel *UiWrapper::getFuelEntryModel(void)
{
//    return mainViewModel;
    return sortModel;
}

RoleItemModel* UiWrapper::getCarEntryModel(void)
{
    return carDataModel;
}

RoleItemModel* UiWrapper::getDriverEntryModel(void)
{
    return driverDataModel;
}

MySortFilterProxyModel *UiWrapper::getAlarmEntryModel(void)
{
    return alarmSortModel;
}

MySortFilterProxyModel *UiWrapper::getAlarmEventModel(void)
{
    return alarmEventSortModel;
}

MySortFilterProxyModel *UiWrapper::getCarStatisticsModel(void)
{
    return carStatisticsSortModel;
}

PlotDataModel* UiWrapper::getStatisticsModel(void)
{
    return statisticsModel;
}

void UiWrapper::calcCarStatistics(QString id, double &totalKm, double &lastMonthKm, double &lastYearKm,
                                    double &totalFill, double &lastMonthFill, double &lastYearFill,
                                    double &totalConsum, double &lastMonthConsum, double &lastYearConsum)
{
    qlonglong Id = id.toLongLong();

    totalKm = getTotalKm(Id);
    lastMonthKm = getLastMonthKm(Id);
    lastYearKm = getLastYearKm(Id);
    totalFill = getTotalFill(Id);
    lastMonthFill = getLastMonthFill(Id);
    lastYearFill = getLastYearFill(Id);
    totalConsum = getTotalConsum(Id);
    lastMonthConsum = getLastMonthConsum(Id);
    lastYearConsum = getLastYearConsum(Id);

}

void UiWrapper::addFuelEntry(int carid, QString date, double km, double trip, double fill, bool notFull,
                             double price, double service, double oil, double tires, double lat, double lon,
                             QString place, QString notes)
{
    setCurrentCar(carid);

    Fuelrecord *record = new Fuelrecord(*unitSystem);
    qlonglong affectedId;
    QStandardItem *affectedItem;
    QStandardItem *carItem;
    double totalKm, lastMonthKm, lastYearKm;
    double totalFill, lastMonthFill, lastYearFill;
    double totalConsum, lastMonthConsum, lastYearConsum;

    record->setAllValuesUserUnit(date,
                                 km,
                                 trip,
                                 fill,
                                 0.0 /* consumption is calculated in database add method */,
                                 price,
                                 dataBase->getCurrentCar().getFuelType(), // @todo Get fueltype from UI
                                 0.0 /* price/litre is calculated in database add method */,
                                 0.0 /* price/trip is calculated in database add method */,
                                 service,
                                 oil,
                                 tires,
                                 lat,
                                 lon,
                                 place,
                                 notes,
                                 (qlonglong)0 /* id will be generated by the database add method */);

    std::cout << "Record before adding:" << "Date: " << record->getDate().toString().toStdString() << SEPARATOR
            << "Km: " << record->getKm().toString().toStdString() << SEPARATOR
              << "Trip: " << record->getTrip().toString().toStdString() << SEPARATOR
              << "Fill: " << record->getFill().toString().toStdString() << SEPARATOR
              << "Price: " << record->getPrice().toString().toStdString() << SEPARATOR
              << "Consumption: " << record->getConsum().toString().toStdString() << SEPARATOR
              << "Price/trip: " << record->getPpt().toString().toStdString() << SEPARATOR
              << "Notes: " << record->getNotes().toString().toStdString() << SEPARATOR
              << "Latitude: " << record->getLatitude().toString().toStdString() << SEPARATOR
              << "Longitude: " << record->getLongitude().toString().toStdString() << SEPARATOR
              << "Place: " << record->getPlace().toString().toStdString() << SEPARATOR
              << "Oil: " << record->getOil().toString().toStdString() << SEPARATOR
              << "Service: " << record->getService().toString().toStdString() << SEPARATOR
              << std::endl;

    // Add to database
    affectedId = dataBase->addNewRecord(*record, notFull);

    // Add to model
    addRecordToFuelEntryModel(fuelEntryModel, record);

    // Change also the affected id in model (in case of not full fill)
    if (affectedId != 0) {
        affectedItem = findFuelEntry(QVariant(affectedId).toString());
        if (affectedItem != 0) {
            qDebug("Found affected id from model and trying to update it");
            setDataToFuelEntryModel(affectedItem, dataBase->queryOneRecord(affectedId, *unitSystem));
            qDebug("Record updated");
        }
        else {
            qDebug("Did not find affected id from model");
        }
    }

    // Update car statistics
    // Change the statistics data in the model
    carItem = findCar(QString("%1").arg(carid));
    if (carItem != 0) {
        qDebug("Found current id from model and trying to update it");
        calcCarStatistics(QString("%1").arg(carid), totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                totalConsum, lastMonthConsum, lastYearConsum);
        setStatDataToCarEntryModel(carItem, carid, totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                               totalConsum, lastMonthConsum, lastYearConsum);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side that list models have changed
    updateAllModels();

    delete record;

}

void UiWrapper::updateFuelEntry(int carid, QString id, QString date, double km, double trip, double fill, bool notFull,
                                double price, double service, double oil, double tires, double lat, double lon,
                                QString place, QString notes)
{
    setCurrentCar(carid);

    Fuelrecord *record = new Fuelrecord(*unitSystem);
    Fuelrecord* oldRecord;
    QStandardItem *currentItem;
    qlonglong affectedId;
    QStandardItem *affectedItem;
    QStandardItem *carItem;
    double totalKm, lastMonthKm, lastYearKm;
    double totalFill, lastMonthFill, lastYearFill;
    double totalConsum, lastMonthConsum, lastYearConsum;

    oldRecord = dataBase->queryOneRecord(id.toLongLong(), *unitSystem);

    record->setAllValuesUserUnit(date,
                                 (km < 0.0)    ? oldRecord->getKm().toDouble() : km,
                                 (trip < 0.0)  ? oldRecord->getTrip().toDouble() : trip,
                                 (fill < 0.0)  ? oldRecord->getFill().toDouble() : fill,
                                 0.0 /* consumption is calculated in database add method */,
                                 (price < 0.0) ? oldRecord->getPrice().toDouble() : price,
                                 dataBase->getCurrentCar().getFuelType(), // @todo Get fueltype from UI
                                 0.0 /* price/litre is calculated in database add method */,
                                 0.0 /* price/trip is calculated in database add method */,
                                 (service < 0.0) ? oldRecord->getService().toDouble() : service,
                                 (oil < 0.0) ? oldRecord->getOil().toDouble() : oil,
                                 (tires < 0.0) ? oldRecord->getTires().toDouble() : tires,
                                 (lat < -90.0) ? oldRecord->getLatitude().toDouble() : lat,
                                 (lon < -180.0) ? oldRecord->getLongitude().toDouble() : lon,
                                 (place.isNull()) ? oldRecord->getPlace().toString() : place,
                                 (notes.isNull()) ? oldRecord->getNotes().toString() : notes,
                                 id.toLongLong());

    std::cout << "Record before editing:" << "Id: " << record->getId().toString().toStdString() << SEPARATOR
              << "Date: " << record->getDate().toString().toStdString() << SEPARATOR
              << "Km: " << record->getKm().toString().toStdString() << SEPARATOR
              << "Trip: " << record->getTrip().toString().toStdString() << SEPARATOR
              << "Fill: " << record->getFill().toString().toStdString() << SEPARATOR
              << "Price: " << record->getPrice().toString().toStdString() << SEPARATOR
              << "Consumption: " << record->getConsum().toString().toStdString() << SEPARATOR
              << "Price/trip: " << record->getPpt().toString().toStdString() << SEPARATOR
              << "Notes: " << record->getNotes().toString().toStdString() << SEPARATOR
              << "Latitude: " << record->getLatitude().toString().toStdString() << SEPARATOR
              << "Longitude: " << record->getLongitude().toString().toStdString() << SEPARATOR
              << "Place: " << record->getPlace().toString().toStdString() << SEPARATOR
              << "Oil: " << record->getOil().toString().toStdString() << SEPARATOR
              << "Service: " << record->getService().toString().toStdString() << SEPARATOR
              << std::endl;

    // Update the record in database
    affectedId = dataBase->updateRecord(*record, notFull);

    // Change the data in the model
    currentItem = findFuelEntry(id);
    if (currentItem != 0) {
        qDebug("Found current id from model and trying to update it");
        setDataToFuelEntryModel(currentItem, dataBase->queryOneRecord(id.toLong(), *unitSystem));
        qDebug("Record updated");
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Change also the affected id in model (in case of not full fill)
    if (affectedId != 0) {
        affectedItem = findFuelEntry(QVariant(affectedId).toString());
        if (affectedItem != 0) {
            qDebug("Found affected id from model and trying to update it");
            setDataToFuelEntryModel(affectedItem, dataBase->queryOneRecord(affectedId, *unitSystem));
        }
        else {
            qDebug("Did not find affected id from model");
        }
    }

    // Update car statistics
    // Change the statistics data in the model
    carItem = findCar(QString("%1").arg(carid));
    if (carItem != 0) {
        qDebug("Found current id from model and trying to update it");
        calcCarStatistics(QString("%1").arg(carid), totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                totalConsum, lastMonthConsum, lastYearConsum);
        setStatDataToCarEntryModel(carItem, carid, totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                               totalConsum, lastMonthConsum, lastYearConsum);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side somehow that the model has changed
    updateAllModels();

    delete record;

}

float UiWrapper::calcTrip(double newkm, double trip)
{
    Datafield *distance = new Datafield(*unitSystem);
    float lastkm;
    float calculatedTrip=trip;

    if (trip<0.1 && newkm>0) {
      lastkm=dataBase->getLastRefill(newkm);
      if (lastkm<0.1) {
        lastkm=dataBase->getLastKm();
      }
      distance->setValueUserUnit(newkm-lastkm, Datafield::LENGTH);
      calculatedTrip = distance->getValueUserUnit().toFloat();
    }

    delete distance;

    return calculatedTrip;
}

void UiWrapper::deleteRecord(QString id)
{
    QStandardItem *currentItem;

    qDebug("%s",__PRETTY_FUNCTION__);

    //@todo Get the second parameter (event delete confirmation) from UI
    dataBase->deleteRecord(id.toLongLong(), true);
    currentItem = findFuelEntry(id);
    if (currentItem != 0) {
        fuelEntryModel->removeRows(currentItem->row(), 1);
    }
    else {
        qDebug("Did not found current record - can't delete it!");
    }

    // Notify Qml side that list models have changed
    updateAllModels();
}

QString UiWrapper::getCarMark(int carid=-1)
{
    CarData oldCarData;
    QString mark;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    mark = dataBase->getCurrentCar().getMark();

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return mark;
}

QString UiWrapper::getCarModel(int carid=-1)
{
    CarData oldCarData;
    QString model;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    model = dataBase->getCurrentCar().getModel() ;

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return model;
}

double UiWrapper::getTotalKm(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalKm = dataBase->getTotalKm(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalKm.overall;
}

double UiWrapper::getLastMonthKm(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalKm = dataBase->getTotalKm(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalKm.lastmonth;
}

double UiWrapper::getLastYearKm(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalKm = dataBase->getTotalKm(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalKm.lastyear;
}

double UiWrapper::getTotalFill(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalFill = dataBase->getTotalFill(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalFill.overall;
}

double UiWrapper::getLastMonthFill(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalFill = dataBase->getTotalFill(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalFill.lastmonth;
}

double UiWrapper::getLastYearFill(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalFill = dataBase->getTotalFill(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalFill.lastyear;
}

double UiWrapper::getTotalConsum(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalConsum = dataBase->getTotalConsumption(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalConsum.overall;
}

double UiWrapper::getLastMonthConsum(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalConsum = dataBase->getTotalConsumption(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalConsum.lastmonth;
}

double UiWrapper::getLastYearConsum(int carid=-1)
{
    CarData oldCarData;

    if (carid != -1) {
        oldCarData = dataBase->getCurrentCar();
        dataBase->setCurrentCar(carid);
    }

    Database::dbtimespan totalConsum = dataBase->getTotalConsumption(*unitSystem);

    if (carid != -1) {
        dataBase->setCurrentCar(oldCarData.getId());
    }

    return totalConsum.lastyear;
}

void UiWrapper::addDriver(QString fullname, QString nickname)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    DriverData *record = new DriverData();

    record->setFullName(fullname);
    record->setNickName(nickname);

    dataBase->addDriver(fullname.toStdString(), nickname.toStdString());

    addRecordToDriverEntryModel(driverDataModel, record);

    // Notify Qml side that list models have changed
    updateAllModels();
}

void UiWrapper::updateDriver(QString id, QString fullname, QString nickname)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    qlonglong Id = id.toLongLong();
    DriverData *record = new DriverData();
    QStandardItem *currentItem;

    qDebug("id = %s, id = %ld\n",id.toStdString().c_str(),Id);

    record->setFullName(fullname);
    record->setNickName(nickname);
    record->setId(Id);

    dataBase->updateDriver(Id, fullname.toStdString(), nickname.toStdString());

    // Change the data in the model
    currentItem = findDriver(id);
    if (currentItem != 0) {
        qDebug("Found current id from model and trying to update it");
        setDataToDriverEntryModel(currentItem, record);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side that list models have changed
    updateAllModels();
}

void UiWrapper::deleteDriver(QString id)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    QStandardItem *currentItem;

    dataBase->deleteDriver(id.toLongLong());

    currentItem = findDriver(id);
    if (currentItem != 0) {
        qDebug("Found current id from model and trying to update it");
        driverDataModel->removeRows(currentItem->row(), 1);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side that list models have changed
    updateAllModels();
}

void UiWrapper::addCar(QString mark, QString model, QString year, QString regist, QString notes, quint8 fueltype)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    CarData *record = new CarData();

    record->setMark(mark);
    record->setModel(model);
    record->setYear(year.toUInt());
    record->setRegNum(regist);
    record->setNotes(notes);
    record->setFuelType((enum CarData::FuelType)fueltype);

    dataBase->addCar(mark.toStdString(), model.toStdString(), year.toStdString(), regist.toStdString(), notes.toStdString(), fueltype);

    addRecordToCarEntryModel(carDataModel, record, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    // Notify Qml side that list models have changed
    updateAllModels();

    delete record;
}

void UiWrapper::updateCar(QString id, QString mark, QString model, QString year, QString regist, QString notes, quint8 fueltype)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    qlonglong Id = id.toLongLong();
    CarData *record = new CarData();
    QStandardItem *currentItem;
    double totalKm, lastMonthKm, lastYearKm;
    double totalFill, lastMonthFill, lastYearFill;
    double totalConsum, lastMonthConsum, lastYearConsum;

    record->setMark(mark);
    record->setModel(model);
    record->setYear(year.toUInt());
    record->setRegNum(regist);
    record->setNotes(notes);
    record->setFuelType((enum CarData::FuelType)fueltype);
    record->setId(Id);

    dataBase->updateCar(id.toLongLong(), mark.toStdString(), model.toStdString(),
                        year.toStdString(), regist.toStdString(), notes.toStdString(), fueltype);

    // Change the statistics data in the model
    currentItem = findCar(id);
    if (currentItem != 0) {
        qDebug("Found current id from model and trying to update it");
        calcCarStatistics(id, totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                totalConsum, lastMonthConsum, lastYearConsum);
        setDataToCarEntryModel(currentItem, record, totalKm, lastMonthKm, lastYearKm, totalFill, lastMonthFill, lastYearFill,
                               totalConsum, lastMonthConsum, lastYearConsum);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side that list models have changed
    updateAllModels();

    delete record;
}

void UiWrapper::deleteCar(QString id)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    QStandardItem *currentItem;

    dataBase->deleteCar(id.toLongLong());

    currentItem = findCar(id);
    if (currentItem != 0) {
        qDebug("Found current id from model and trying to update it");
        carDataModel->removeRows(currentItem->row(), 1);
    }
    else {
        qDebug("Did not find current id from model");
    }

    // Notify Qml side that list models have changed
    updateAllModels();
}

//-------------------------------------------------------------------
//
// Alarms
//
//-------------------------------------------------------------------
void UiWrapper::addAlarmType(qlonglong carId, QString shortDesc, quint32 interval, quint32 distance, QString longDesc)
{
    AlarmtypeData *alarmtype = new AlarmtypeData();

    alarmtype->setCarId(carId);
    alarmtype->setShortDesc(shortDesc);
    alarmtype->setInterval(interval);
    alarmtype->setDistance(distance);
    alarmtype->setLongDesc(longDesc);

    dataBase->addAlarmType(*alarmtype);

    addRecordToAlarmEntryModel(alarmEntryModel, alarmtype, dataBase->getLastKm());

    delete alarmtype;

    // Notify Qml side that list models have changed
    updateAllModels();
}

void UiWrapper::addAlarmEvent(qlonglong alarmId, QString date,
                              double km, double service, double oil, double tires, QString notes)
{
    AlarmeventData *event = new AlarmeventData();

    event->setAlarmId(alarmId);
    event->setDate(date);
    event->setKm(km);
    event->setService(service);
    event->setOil(oil);
    event->setTires(tires);
    event->setNotes(notes);

    dataBase->addNewAlarmEvent(*event, *unitSystem);

    addAllRecordsToAlarmEventModel(alarmId);

    delete event;
}

void UiWrapper::updateAlarmEvent(qlonglong id, qlonglong alarmid, qlonglong recordId, QString date,
                                 double km, double service, double oil, double tires, QString notes)
{
    AlarmeventData *event = new AlarmeventData();

    // Update the entry in fuel database
    updateFuelEntry(-1, // Do not change the current car
                    QString("%1").arg(recordId),
                    date,
                    km,
                    -1.0, // Do not change trip
                    -1.0, // Do not change fill
                    false, // Just guessing, could go wrong if the user has altered in fuel database
                    -1.0, // Do not change price
                    service,
                    oil,
                    tires,
                    -360.0, // Do not change the latitude
                    -360.0, // Do not change the longitude
                    QString(), // Do not change place
                    QString() // Do not change notes
                    );

    event->setId(id);
    event->setAlarmId(alarmid);
    event->setRecordId(recordId);
    event->setDate(date);
    event->setKm(km);
    event->setService(service);
    event->setOil(oil);
    event->setTires(tires);
    event->setNotes(notes);

    dataBase->updateAlarmEvent(*event, *unitSystem);

    addAllRecordsToAlarmEventModel(alarmid);

    delete event;
}

void UiWrapper::deleteEvent(QString id)
{
    qDebug("%s called!\n",__PRETTY_FUNCTION__);
    QStandardItem *currentItem;

    //@todo Get the second parameter (event delete confirmation) from UI
    dataBase->deleteEvent(id.toLongLong(), true);

    currentItem = findAlarmEvent(id);
    if (currentItem != 0) {
        alarmEventModel->removeRows(currentItem->row(), 1);
    }
    else {
        qDebug("Did not find current id from model, can't delete it");
    }

    addAllRecordsToAlarmEventModel(id.toLongLong());
}


void UiWrapper::setSortColumn(int col, Qt::SortOrder order = Qt::DescendingOrder)
{
    sortModel->beginResetModel();
    sortModel->sort(col, order);
    sortModel->endResetModel();
}

void UiWrapper::setCurrentCar(int carid)
{
    if ((carid != -1) && (dataBase->getCurrentCar().getId() != carid)) {
        dataBase->setCurrentCar(carid);

        reReadAllModels();
        updateAllModels();
    }
}

void UiWrapper::setMainUnit(int unit, bool individualUnit)
{
    if (unit >= UnitSystem::SI || unit < UnitSystem::NUMUNITS) {
        unitSystem->setMainUnit((UnitSystem::unit)unit);

        unitSystem->setIndividualUnit(individualUnit);

        reReadAllModels();
        updateAllModels();
    }
}

void UiWrapper::setLengthUnit(int unit)
{
    if ((UnitSystem::unit)unit != unitSystem->getLengthUnit() && (unit >= UnitSystem::SI || unit < UnitSystem::NUMUNITS)) {
        unitSystem->setLengthUnit((UnitSystem::unit)unit);

        reReadAllModels();
        updateAllModels();
    }
}

void UiWrapper::setMassUnit(int unit)
{
    if ((UnitSystem::unit)unit != unitSystem->getMassUnit() && (unit >= UnitSystem::SI || unit < UnitSystem::NUMUNITS)) {
        unitSystem->setMassUnit((UnitSystem::unit)unit);

        reReadAllModels();
        updateAllModels();
    }

}

void UiWrapper::setVolumeUnit(int unit)
{
    if ((UnitSystem::unit)unit != unitSystem->getVolumeUnit() && (unit >= UnitSystem::SI || unit < UnitSystem::NUMUNITS)) {
        unitSystem->setVolumeUnit((UnitSystem::unit)unit);

        reReadAllModels();
        updateAllModels();
    }
}

void UiWrapper::setConsumeUnit(int unit)
{
    if ((UnitSystem::unit)unit != unitSystem->getConsumeUnit() && (unit >= UnitSystem::SI || unit < UnitSystem::NUMUNITS)) {
        unitSystem->setConsumeUnit((UnitSystem::unit)unit);

        reReadAllModels();
        updateAllModels();
    }
}

//-------------------------------------------------------------------
//
// Units
//
//-------------------------------------------------------------------
int UiWrapper::getMainUnit(void)
{
    return unitSystem->getMainUnit();
}

bool UiWrapper::getIndividualUnit(void)
{
    return unitSystem->getIndividualUnit();
}

int UiWrapper::getLengthUnit(void)
{
    return unitSystem->getLengthUnit();
}

int UiWrapper::getMassUnit(void)
{
    return unitSystem->getMassUnit();
}

int UiWrapper::getVolumeUnit(void)
{
    return unitSystem->getVolumeUnit();
}

int UiWrapper::getConsumeUnit(void)
{
    return unitSystem->getConsumeUnit();
}

QString UiWrapper::getCurrencySymbol(void)
{
    return unitSystem->getCurrencySymbol();
}

QString UiWrapper::requestAddress(double lat, double lon)
{
    connect(geoCode, SIGNAL(addressReady()),
            this, SLOT(replyFinished()));

    geoCode->requestAddress(lat,lon);

    return QString("");
}

void UiWrapper::replyFinished(void)
{
    qDebug("Address available");
    geoAddress = geoCode->getAddress();

    emit addressReady();
}

QString UiWrapper::getAddress(void)
{
    return geoAddress;
}

#if 0
void UiWrapper::on_action_Add_record_triggered()
{
    qlonglong affectedId;
    AddRecordDialog dialog(this, dataBase, unitSystem);

    qDebug("File|Add record selected\n");

    if (dialog.exec() == QDialog::Accepted) {
        Fuelrecord *record = new Fuelrecord(*unitSystem);
        qlonglong carId = dialog.getField("carid").toLongLong();
        qlonglong driverId = dialog.getField("driverid").toLongLong();

        if (carId != dataBase->getCurrentCar().getId()) {
            qDebug("Changing carid to %ld",carId);
            dataBase->setCurrentCar(carId);
        }

        if (driverId != dataBase->getCurrentDriver().getId()) {
            qDebug("Changing driverid to %ld",driverId);
            dataBase->setCurrentDriver(driverId);
        }

        record->setAllValuesUserUnit(dialog.getField("date").toString(),
                                     dialog.getField("km").toDouble(),
                                     dialog.getField("trip").toDouble(),
                                     dialog.getField("fill").toDouble(),
                                     0.0 /* consumption is calculated in database add method */,
                                     dialog.getField("price").toDouble(),
                                     0.0 /* price/litre is calculated in database add method */,
                                     0.0 /* price/trip is calculated in database add method */,
                                     dialog.getField("service").toDouble(),
                                     dialog.getField("oil").toDouble(),
                                     dialog.getField("tires").toDouble(),
                                     dialog.getField("notes").toString(),
                                     (qlonglong)0 /* id will be generated by the database add method */);

        affectedId = dataBase->addNewRecord(*record, dialog.getField("notfull").toBool());

        // @todo Use also affectedId to update a affected record
//        addRecordToModel(ui->mainTreeView->model(), record);

        delete record;

    }
    else {
        qDebug("Canceled\n");
    }
}

void UiWrapper::on_action_Edit_record_triggered()
{
    qlonglong affectedId;
    const QModelIndex selectedIndex = ui->mainTreeView->selectionModel()->currentIndex();
    int row = selectedIndex.row();
    qlonglong selectedId = selectedIndex.sibling(row,FIELD_ID).data().toLongLong();
//    QString selectedText = selectedIndex.data(Qt::DisplayRole).toString();

    AddRecordDialog dialog(this, dataBase, unitSystem);

    dialog.setWindowTitle(QObject::tr("Edit record"));

    // @todo Get driver and car from record, now current car and driver are used
    dialog.setCar(dataBase->getCurrentCar().getId());
    dialog.setDriver(dataBase->getCurrentDriver().getId());
    // @todo Remember user unit
    dialog.setField("date", selectedIndex.sibling(row, FIELD_DATE).data());
    dialog.setField("km",   selectedIndex.sibling(row, FIELD_KM).data());
    dialog.setField("trip", selectedIndex.sibling(row, FIELD_TRIP).data());
    dialog.setField("fill", selectedIndex.sibling(row, FIELD_FILL).data());
    dialog.setField("notfull", (selectedIndex.sibling(row, FIELD_FILL).data().toFloat() > 0.0) &
                    (selectedIndex.sibling(row, FIELD_CONSUM).data().toFloat() < 1.0e-4) );
    dialog.setField("price", selectedIndex.sibling(row, FIELD_PRICE).data());
    dialog.setField("notes", selectedIndex.sibling(row, FIELD_NOTES).data());
    dialog.setField("oil", selectedIndex.sibling(row, FIELD_OIL).data());
    dialog.setField("service", selectedIndex.sibling(row, FIELD_SERVICE).data());

    if (dialog.exec() == QDialog::Accepted) {
        qDebug("Accepted");
        Fuelrecord *record = new Fuelrecord(*unitSystem);
        qlonglong carId = dialog.getField("carid").toLongLong();
        qlonglong driverId = dialog.getField("driverid").toLongLong();

        if (carId != dataBase->getCurrentCar().getId()) {
            qDebug("Changing carid to %ld",carId);
            dataBase->setCurrentCar(carId);
        }

        if (driverId != dataBase->getCurrentDriver().getId()) {
            qDebug("Changing driverid to %ld",driverId);
            dataBase->setCurrentDriver(driverId);
        }

        std::cout << "Carid: " << dialog.getField("carid").toString().toStdString() << SEPARATOR
                  << "Date: " << dialog.getField("date").toString().toStdString() << SEPARATOR
                  << "Km: " << dialog.getField("km").toString().toStdString() << SEPARATOR
                  << "Trip: " << dialog.getField("trip").toString().toStdString() << SEPARATOR
                  << "Fill: " << dialog.getField("fill").toString().toStdString() << SEPARATOR
                  << (dialog.getField("notfull").toBool() ? "Not full" : "Full") << SEPARATOR << "Tank" << SEPARATOR
                  << "Price: " << dialog.getField("price").toString().toStdString() << SEPARATOR
                  << "Notes: " << dialog.getField("notes").toString().toStdString() << SEPARATOR
                  << "Oil: " << dialog.getField("oil").toString().toStdString() << SEPARATOR
                  << "Service: " << dialog.getField("service").toString().toStdString() << SEPARATOR
                  << std::endl;

        record->setAllValuesUserUnit(dialog.getField("date").toString(),
                                     dialog.getField("km").toDouble(),
                                     dialog.getField("trip").toDouble(),
                                     dialog.getField("fill").toDouble(),
                                     0.0 /* consumption is calculated in database add method */,
                                     dialog.getField("price").toDouble(),
                                     0.0 /* price/litre is calculated in database add method */,
                                     0.0 /* price/trip is calculated in database add method */,
                                     dialog.getField("service").toDouble(),
                                     dialog.getField("oil").toDouble(),
                                     dialog.getField("tires").toDouble(),
                                     dialog.getField("notes").toString(),
                                     selectedId);

        std::cout << "Record before edit:" << "Date: " << record->getDate().toString().toStdString() << SEPARATOR
                << "Km: " << record->getKm().toString().toStdString() << SEPARATOR
                  << "Trip: " << record->getTrip().toString().toStdString() << SEPARATOR
                  << "Fill: " << record->getFill().toString().toStdString() << SEPARATOR
                  << "Price: " << record->getPrice().toString().toStdString() << SEPARATOR
                  << "Consumption: " << record->getConsum().toString().toStdString() << SEPARATOR
                  << "Price/trip: " << record->getPpt().toString().toStdString() << SEPARATOR
                  << "Notes: " << record->getNotes().toString().toStdString() << SEPARATOR
                  << "Oil: " << record->getOil().toString().toStdString() << SEPARATOR
                  << "Service: " << record->getService().toString().toStdString() << SEPARATOR
                  << std::endl;

        affectedId = dataBase->updateRecord(*record, dialog.getField("notfull").toBool());

        // @todo Update also affectedId
        setDataToModel(ui->mainTreeView->model(), record, row);

        std::cout << "Record after edit:" << "Date: " << record->getDate().toString().toStdString() << SEPARATOR
                << "Km: " << record->getKm().toString().toStdString() << SEPARATOR
                  << "Trip: " << record->getTrip().toString().toStdString() << SEPARATOR
                  << "Fill: " << record->getFill().toString().toStdString() << SEPARATOR
                  << "Price: " << record->getPrice().toString().toStdString() << SEPARATOR
                  << "Consumption: " << record->getConsum().toString().toStdString() << SEPARATOR
                  << "Price/trip: " << record->getPpt().toString().toStdString() << SEPARATOR
                  << "Notes: " << record->getNotes().toString().toStdString() << SEPARATOR
                  << "Oil: " << record->getOil().toString().toStdString() << SEPARATOR
                  << "Service: " << record->getService().toString().toStdString() << SEPARATOR
                  << "Id: " << record->getId().toString().toStdString() << SEPARATOR
                  << std::endl;

        delete record;
    }
    else {
        qDebug("Canceled\n");
    }

    qDebug("File|Edit record selected\n");
    std::cout << "Selected row = " << row << " database id = " << selectedIndex.sibling(row,FIELD_ID).data().toString().toStdString() << std::endl;
}

void UiWrapper::on_action_Delete_record_triggered()
{
    qlonglong affectedId;
    const QModelIndex selectedIndex = ui->mainTreeView->selectionModel()->currentIndex();
    int row = selectedIndex.row();
    qlonglong selectedId = selectedIndex.sibling(row,FIELD_ID).data().toLongLong();

    qDebug("File|Delete record selected\n");

    std::cout << "Selected row = " << row << " database id = " << selectedId << std::endl;

    DeleteConfirmDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        qDebug("Accepted");

        // @todo Actual removal of a record
        dataBase->deleteRecord(selectedId);
        ui->mainTreeView->model()->removeRow(row, selectedIndex.parent());
    }
    else {
        qDebug("Canceled\n");
    }
}

void UiWrapper::on_actionStatistics_triggered()
{
    qDebug("Report|Statistics selected\n");
}

void UiWrapper::on_actionReport_triggered()
{
    qDebug("Report|Report selected\n");
}

void UiWrapper::on_action_Settings_triggered()
{
    SettingsDialog dialog(this, dataBase);

    qDebug("File|Settings selected\n");

    if (dialog.exec() == QDialog::Accepted) {
        qDebug("Accepted");

        // @todo Do something with the new settings
    }
    else {
        qDebug("Canceled\n");
    }

}

void UiWrapper::on_action_Sort_by_triggered()
{
    qDebug("View|Sort by selected\n");
}

void UiWrapper::on_actionSort_ascending_triggered()
{
    qDebug("View|Sort order|Ascending selected\n");
}

void UiWrapper::on_actionSort_descending_triggered()
{
    qDebug("View|Sort order|Descending selected\n");
}

void UiWrapper::on_action_Reminders_triggered()
{
    qDebug("Tools|Reminders selected\n");
}

void UiWrapper::on_action_Driving_log_triggered()
{
    qDebug("Tools|Driving log selected\n");
}
#endif
