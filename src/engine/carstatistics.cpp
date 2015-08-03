#include "carstatistics.h"
#include "datafield.h"

CarStatistics::CarStatistics(UnitSystem u) : m_unit(u)
{
}

void CarStatistics::setYear(QString year)
{
    m_year = year;
}

QString CarStatistics::getYear(void)
{
    return m_year;
}

void CarStatistics::setMaxKm(double maxKm)
{
    m_maxKm = maxKm;
}

double CarStatistics::getMaxKm(void)
{
    return m_maxKm;
}

void CarStatistics::setMinKm(double minKm)
{
    m_minKm = minKm;
}

double CarStatistics::getMinKm(void)
{
    return m_minKm;
}

void CarStatistics::setTotalTrip(double totalTrip)
{
    m_totalTrip = totalTrip;
}

double CarStatistics::getTotalTrip(void)
{
    return m_totalTrip;
}

void CarStatistics::setTotalFill(double totalFill)
{
    m_totalFill = totalFill;
}

double CarStatistics::getTotalFill(void)
{
    return m_totalFill;
}

void CarStatistics::setTotalPrice(double totalPrice)
{
    m_totalPrice = totalPrice;
}

double CarStatistics::getTotalPrice(void)
{
    return m_totalPrice;
}

void CarStatistics::setTotalOil(double totalOil)
{
    m_totalOil = totalOil;
}

double CarStatistics::getTotalOil(void)
{
    return m_totalOil;
}

void CarStatistics::setTotalService(double totalService)
{
    m_totalService = totalService;
}

double CarStatistics::getTotalService(void)
{
    return m_totalService;
}

void CarStatistics::setTotalTires(double totalTires)
{
    m_totalTires = totalTires;
}

double CarStatistics::getTotalTires(void)
{
    return m_totalTires;
}

double CarStatistics::getAverageConsumption(void)
{
    Datafield consum(m_unit);

    consum.setValue(m_totalFill/m_totalTrip*100.0, Datafield::CONSUMPTION);

    return consum.getValueUserUnit().toDouble();
}
