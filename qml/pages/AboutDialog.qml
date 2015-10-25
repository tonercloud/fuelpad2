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

Page {
    id: aboutPage

    SilicaFlickable {
        id: aboutFlickable
        anchors.fill: parent
        contentWidth: aboutPage.width
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingLarge
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingLarge
            spacing: Theme.paddingLarge

            PageHeader {
                id: headertext
                title: qsTr("About Fuelpad2")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:///assets/icons/harbour-fuelpad2.png"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeLarge
                text: qsTr("Fuelpad2 v:")  + APP_VERSION
            }

            Rectangle {
                    x: Theme.paddingMedium
                    id: titleField
                    height: 2
                    width: headertext.width - (Theme.paddingMedium * 2)
                    color: "red"
                }

                Label {
                    width: aboutColumn.width - Theme.paddingMedium * 2
                    x: Theme.paddingMedium
                    textFormat: Text.RichText
                    font.pixelSize: Theme.fontSizeMedium
                    text: qsTr("<b>Copyright (C) Julius Luukko 2008-2011, 2015<br>"
                          + "& Chris Walker 2015<p>"
                          + "<small>This software is free software distributed under the terms of the GNU General Public License version 3 or later.</small>")
                    wrapMode: Text.WordWrap
                }

                Button {
                    id: homepage
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("<a href=\"https://github.com/tonercloud/harbour-carsails\">Sourcecode on Github</a>")
                    onClicked: {
                        Qt.openUrlExternally("https://github.com/tonercloud/harbour-carsails")
                    }
                }
        }
    }
}
