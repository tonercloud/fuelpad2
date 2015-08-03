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

#ifndef PLOTDATAMODEL_H
#define PLOTDATAMODEL_H

#include <vector>
using namespace std;

#include <QStandardItemModel>
#include <QStandardItem>

class PlotData
{
public:
    enum RoleNames {
        xRole = Qt::UserRole + 1,
        yRole
    };
    PlotData(const double xc, const double yc);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE double xc(void) const;
    Q_INVOKABLE double yc(void) const;

private:
    double m_xc;
    double m_yc;
};

class PlotDataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
//    Q_PROPERTY(QVariant get READ get)

public:

    PlotDataModel(QObject *parent = 0);
    virtual ~PlotDataModel(void);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QHash<int, QByteArray> roleNames() const;
#endif

    int count(void) const;

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    void appendData(const PlotData &item);

    void appendData(const vector<double> &x, const vector<double> &y);

    void setRowData(const int row, const PlotData &item);

    void setRowData(const vector<int> row, const vector<double> &x, const vector<double> &y);

    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QVariant get(int index) const;

    void clear(void);

private:
     QList<PlotData> m_data;
     QHash<int, QByteArray> m_roleNames;

signals:
    void countChanged();
};

#endif // PLOTDATAMODEL_H
