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

#ifndef GEOCODE_H
#define GEOCODE_H

#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Geocode : public QObject
{
    Q_OBJECT
public:
    Geocode();
    ~Geocode();

    virtual bool requestAddress(double lat, double lon) =0;
    virtual QString getAddress(void) =0;

public slots:

    void replyFinished(QNetworkReply* reply);

signals:
    void addressReady(void);

protected:
    QNetworkAccessManager *manager;
    QString replyFromService;

    bool requestAddressFromService(QUrl url);
    QString getReplyFromService();
};

#endif // GEOCODE_H
