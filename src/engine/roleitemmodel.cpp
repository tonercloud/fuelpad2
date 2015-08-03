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

#include "roleitemmodel.h"


/* Example usage:


Enumerate the role ID's somewhere
---------------------------------

struct RedditEntry {

    enum RedditRoles {
        UrlRole = Qt::UserRole + 1,
        DescRole,
        ...
    };
    ...
}

Instantiate the class
---------------------


    QHash<int, QByteArray> roleNames;
    roleNames[RedditEntry::UrlRole] =  "url";
    roleNames[RedditEntry::ScoreRole] = "score";
    m_linksmodel = new RoleItemModel(roleNames);



Populate with data:
-------------------

    QStandardItem* it = new QStandardItem();
    it->setData(e.desc, RedditEntry::DescRole);
    it->setData(e.score, RedditEntry::ScoreRole);

    m_linksmodel->appendRow(it);

Expose to QML:
-------------

QDeclarativeContext *ctx = ...

ctx->setContextProperty("mdlLinks", m_linksmodel);

*/


RoleItemModel::RoleItemModel(const QHash<int, QByteArray> &roleNames)
{
    m_roleNames = roleNames;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(m_roleNames);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
QHash<int, QByteArray> RoleItemModel::roleNames() const
{
    return m_roleNames;
}
#endif

QVariantMap RoleItemModel::getModelData(const QAbstractItemModel* mdl, int row)
{
    QHash<int,QByteArray> names = mdl->roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
     while (i.hasNext()) {
        i.next();
        QModelIndex idx = mdl->index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
         //cout << i.key() << ": " << i.value() << endl;
     }
     return res;
}


