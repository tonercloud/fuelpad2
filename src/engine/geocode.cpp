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

#include "geocode.h"

Geocode::Geocode()
{
    manager = new QNetworkAccessManager(this);
}

Geocode::~Geocode()
{
    delete manager;
}

bool Geocode::requestAddressFromService(QUrl url)
{
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(url)));

    return true;
}

void Geocode::replyFinished(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {
            replyFromService = QString(reply->readAll());
            emit addressReady();
            qDebug(replyFromService.toStdString().c_str());
        } else {
            // Handle the error
        }
    reply->deleteLater();
}

QString Geocode::getReplyFromService(void)
{
    return replyFromService;
}
