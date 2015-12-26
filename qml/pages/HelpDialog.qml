/*
 * This file is part of Fuelpad2.
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
 * along with Fuelpad2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: helppage

    SilicaFlickable {
        id: contentFlickable
        anchors.fill: parent
        contentHeight: contentColumn.height + Theme.paddingLarge
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: contentColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingLarge
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingLarge
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Help Page")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:///assets/icons/harbour-fuelpad2.png"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeLarge
                text: qsTr("Fuelpad2")
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Vehicle Records Database")
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                textFormat: Text.RichText
                text: qsTr("<style>a{color:" + Theme.highlightColor + "}</style>Fuelpad2 allows you to store details of your vehicle usage. When the program starts, you will not have defined any vehicle. You are therefore presented with a vehicle of random name. At this point you can choose to modify that name or create a new one. But you are advised that the first thing you should do is check the Settings page to allow you to choose the units pertinent to your country or indeed, your own particular needs.")
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("There is no practical limit to the amount of your personal information which is stored, save for the amount of memory in the device.")
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Acknowledgements")
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                textFormat: Text.RichText
                text: qsTr("<style>a{color:" + Theme.highlightColor + "}</style>This software was originally developed for use on a Nokia N900 and has been ported to Sailfish.")
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }
        }
        VerticalScrollDecorator { flickable: contentFlickable }
    }
}
