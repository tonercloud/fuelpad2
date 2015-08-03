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

#include "geocodenominatim.h"

GeocodeNominatim::GeocodeNominatim() :
    serviceAddress("http://nominatim.openstreetmap.org/reverse?format=xml&lat=%1&lon=%2zoom=18&addressdetails=0")
{
}

bool GeocodeNominatim::requestAddress(double lat, double lon)
{
    QString url = QString(serviceAddress)
            .arg(lat,0,'f',8)
            .arg(lon,0,'f',8);

    return requestAddressFromService(url);
}

QString GeocodeNominatim::getAddress(void)
{
    QString address;

    parse_xml(address);

    return address;
}

bool GeocodeNominatim::retrieve_result(QString &result, QDomElement node)
{
    QDomNode cur_node;
    bool retVal = false;

    for (cur_node = node.firstChild(); !cur_node.isNull(); cur_node = cur_node.nextSibling()) {
        if (cur_node.nodeType() == QDomNode::ElementNode) {

            if (cur_node.toElement().tagName() == QString("result")) {
                retVal = true;
                result = cur_node.toElement().text();
            }

        }
    }

    return retVal;
}

bool GeocodeNominatim::parse_xml(QString &address)
{
    QDomDocument document("nominatimresponse");
    QString reply = getReplyFromService();
    int errorLine, errorColumn;
    QString errorMsg;
    bool retVal = false;

    if (!document.setContent(reply, false, &errorMsg, &errorLine, &errorColumn)) {
        QString error("Xml error: line %1, column %2:\n%3");
        error = error
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorMsg);
        qDebug(error.toStdString().c_str());
        retVal = false;
    }
    else {
        QDomElement rootElement = document.documentElement();

        if (rootElement.tagName() == QString("reversegeocode")) {

            if (retrieve_result(address,rootElement)) {
                retVal = true;
            }

        }
        else {
            qDebug("Xml error: Wrong root element tag: %s",rootElement.tagName().toStdString().c_str());
        }
    }

}
