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

#include "plotdatamodel.h"

PlotData::PlotData(const double xc, const double yc)
{
    m_xc = xc;
    m_yc = yc;
}

QVariant PlotData::data(int role) const
{
    switch (role) {
    case xRole:
        return m_xc;
    case yRole:
        return m_yc;
    default:
        return 0;
    }
}

double PlotData::xc(void) const
{
    return m_xc;
}

double PlotData::yc(void) const
{
    return m_yc;
}

PlotDataModel::PlotDataModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[PlotData::xRole] =  "xc";
    m_roleNames[PlotData::yRole] = "yc";

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(m_roleNames);
#endif
}

PlotDataModel::~PlotDataModel(void)
{
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
QHash<int, QByteArray> PlotDataModel::roleNames() const
{
    return m_roleNames;
}
#endif


int PlotDataModel::count(void) const
{
    return rowCount();
}

int PlotDataModel::rowCount ( const QModelIndex & parent ) const
{
    return m_data.count();
}

void PlotDataModel::appendData(const PlotData &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << item;
    const QModelIndex idx = PlotDataModel::index(m_data.count(), 0);
    emit dataChanged(idx, idx);
    endInsertRows();
}

void PlotDataModel::appendData(const vector<double> &x, const vector<double> &y)
{
    int beginRow = rowCount();
    int endRow = beginRow + x.size();

    if (x.size() == y.size()) {
        beginInsertRows(QModelIndex(), beginRow, endRow);
        for (vector<PlotData>::size_type i=0; i < x.size(); i++) {
            m_data << PlotData(x.at(i),y.at(i));
        }
        endInsertRows();
        emit dataChanged(PlotDataModel::index(beginRow, 0), PlotDataModel::index(endRow, 0));
    }
}

void PlotDataModel::setRowData(const int row, const PlotData &item)
{
    if (row >= 0 && row < m_data.count()) {
        m_data.replace(row,item);
        const QModelIndex idx = PlotDataModel::index(row, 0);
        emit dataChanged(idx, idx);
    }
}

void PlotDataModel::setRowData(const vector<int> row, const vector<double> &x, const vector<double> &y)
{
    for (vector<PlotData>::size_type i=0; i < row.size(); i++) {
        m_data.replace(row.at(i),PlotData(x.at(i),y.at(i)));
    }
    const QModelIndex idx1 = PlotDataModel::index(row.at(0), 0);
    const QModelIndex idx2 = PlotDataModel::index(row.at(row.size()-1), 0);
    emit dataChanged(idx1, idx2);
}

QVariant PlotDataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_data.count()) {
        return QVariant();

        const PlotData &data = m_data[index.row()];
        if (role == PlotData::xRole) {
            return data.xc();
        }
        else if (role == PlotData::yRole) {
            return data.yc();
        }
        return QVariant();
    }
}

QVariant PlotDataModel::get(int index) const
{
    const PlotData &item = m_data.at(index);
    QMap<QString, QVariant> itemData;
    QHashIterator<int, QByteArray> hashItr(roleNames());
    while(hashItr.hasNext()){
        hashItr.next();
        itemData.insert(hashItr.value(),item.data(hashItr.key()).toString());
    }
    return QVariant(itemData);
}

void PlotDataModel::clear(void) {
    qDebug("Clearing plot data model");
    beginResetModel();
    m_data.clear();
    endResetModel();
}
