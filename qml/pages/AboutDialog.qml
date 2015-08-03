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
    id: aboutPage

    SilicaFlickable {
        id: aboutFlickable
        anchors.fill: parent
        contentWidth: aboutPage.width
        contentHeight: pagecolumn.height

        Column {
            width: parent.width
            id: pagecolumn

            PageHeader {
                id: headertext
                title: qsTr("About Fuelpad2")
            }

            Label {
                x: Theme.paddingMedium
                text: "Fuelpad2 v:" + APP_VERSION
                    font.pixelSize: Theme.fontSizeExtraLarge
                    font.weight: Font.Bold
                    color: "white"
                }
                Rectangle {
                    x: Theme.paddingMedium
                    id: titleField
                    height: 2
                    width: headertext.width - (Theme.paddingMedium * 2)
                    color: "red"
                }

                Label {
                    // anchors.verticalCenter: parent.verticalCenter
                    width: pagecolumn.width - Theme.paddingMedium * 2
                    x: Theme.paddingMedium
                    textFormat: Text.RichText
                    font.pixelSize: Theme.fontSizeMedium
                    text: "<b>Copyright (C) Julius Luukko 2008-2011, 2015<br>"
                          + "& Chris Walker<p>"
                          + "<small>This software is free software distributed under the terms of "
                          + "the GNU General Public License version 3 or later.</small>"
                    wrapMode: Text.WordWrap
                }

                Button {
                    id: homepage
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "<a href=\"https://github.com/tonercloud/harbour-carsails\">Sourcecode on Github</a>"
                    onClicked: {
                        Qt.openUrlExternally("https://github.com/tonercloud/harbour-carsails")
                    }
                }
        }
    }
}
