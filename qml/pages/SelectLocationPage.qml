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

import QtQuick 1.1
import org.fuelpad.qmlui 1.0
import "CommonFuncs.js" as Funcs
import "CommonUnits.js" as Units

FPDialog {
    id: locationDialog

    FPApplicationTheme {
        id: appTheme
    }

    width: parent.width

    title: qsTr("Retrieve location")

    function toggleGPS() {
        if (enableGPS.checked) {
            positionSource.start()
        }
        else {
            positionSource.stop()
        }
    }

    function locationDialogAccepted() {
            addDialogData.addressFound(placeField.text, latField.text, lonField.text);
    }


    Flickable {
        id: locationDialogData
        anchors {
            fill: parent
            leftMargin: appTheme.paddingLarge
            rightMargin: appTheme.paddingLarge
        }
        contentWidth: column.width
        contentHeight: column.height
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Connections {
            target: applicationData
            onAddressReady: {
                placeField.text = applicationData.getAddress()
                console.log(applicationData.getAddress())
            }
        }

        Column {
            id: column
            spacing: appTheme.paddingMedium

            Grid {
                columns: 2
                spacing: appTheme.paddingMedium

                FPLabel {
                    text: qsTr("Enable GPS")
                }

                // todo Get checked from global settings
                FPSwitch {
                    id: enableGPS
                    checked: positionSource.active
                    onCheckedChanged: locationDialog.toggleGPS()
                }

                FPLabel {
                    text: qsTr("Latitude")
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPLabel {
                    id: latField
                    text: positionSource.position.coordinate.latitude.toFixed(8)
                }

                FPLabel {
                    text: qsTr("Longitude")
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPLabel {
                    id: lonField
                    text: positionSource.position.coordinate.longitude.toFixed(8)
                }

                FPLabel {
                    text: qsTr("Position valid")
                }

                FPLabel {
                    text: (positionSource.position.latitudeValid && positionSource.position.longitudeValid) ?
                              qsTr("valid") : qsTr("invalid")
                }

                FPLabel {
                    text: qsTr("Horizontal accuracy")
                }

                FPLabel {
                    text: positionSource.position.horizontalAccuracy.toFixed(1)
                }

                FPLabel {
                    text: qsTr("Vertical accuracy")
                }

                FPLabel {
                    text: positionSource.position.verticalAccuracy.toFixed(1)
                }

            }

            FPButton {
//                    anchors.horizontalCenter: locationDialog.horizontalCenter
                text: qsTr("Retrieve address")
                onClicked: applicationData.requestAddress(latField.text, lonField.text)
            }

            FPTextArea {
                id: placeField
                width: locationDialog.width-2*appTheme.paddingLarge
                height: 4*appTheme.fontSizeMedium
                placeholderText: qsTr("Place")
//                readOnly: true
            }

        }
    }
    buttons: FPButtonRow {
        anchors.horizontalCenter: parent.horizontalCenter
        FPButton {
            text: qsTr("Apply")
            onClicked: locationDialog.accept()
        }
        FPButton {
            text: qsTr("Cancel");
            onClicked: locationDialog.cancel()
        }
    }

    onAccepted: locationDialogAccepted()

}
