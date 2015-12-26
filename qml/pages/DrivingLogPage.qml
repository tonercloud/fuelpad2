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

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../pages"
import "../extra_imports"
import "../extra_imports/CommonFuncs.js" as Funcs
import "../extra_imports/CommonUnits.js" as Units
import "../extra_imports/DialogStatus.js" as DialogStatus

Page {

    property variant tools

    tools: commonTools

    PageHeader {
        id: applicationHeader
        title: qsTr("Driving log")
    }

    Text {
        wrapMode: Text.WordWrap
        text: "Driving log functionality has not been implemented yet"
        font.pixelSize: appTheme.fontSizeExtraLarge
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
    }

}
