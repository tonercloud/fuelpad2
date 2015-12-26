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
import "CommonUnits.js" as Units

FPDialog {
    id: addDialog

    property string carId
    property bool editMode: false
    property string oldId
    property string oldShort
    property string oldIntervalKm
    property string oldIntervalMonths
    property string oldLong

    width: parent.width

    title: qsTr("Add alarm type")

    function addDialogAccepted() {
        if (editMode) {
            console.log("Would update if backend existed...");
//            applicationData.updateAlarmType(oldId, shortDescField.text, intervalMonthsField.text, intervalKmField.text, longDescField.text);
        }
        else {
            console.log("Adding a new alarm type...");
            applicationData.addAlarmType(carId, shortDescField.text, intervalMonthsField.text, intervalKmField.text, longDescField.text);
        }
    }

    Flickable {
        id: addDialogData
        anchors {
            fill: parent
            leftMargin: appTheme.paddingLarge
            rightMargin: appTheme.paddingLarge
        }
        contentWidth: column.width
        contentHeight: column.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: column
            spacing: appTheme.paddingMedium
            Grid {
                id: addDialogGrid
                columns: 1
                spacing: appTheme.paddingMedium
                Text {
                    text: qsTr("Short description")
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPTextField {
                    id: shortDescField
                    width: addDialog.width-2*appTheme.paddingLarge
                    placeholderText: qsTr("Add short description")
                    maximumLength: 40
                    validator: RegExpValidator{}
                    text: editMode ? oldShort : ""
                }
                Text {
                    text: qsTr("Service interval ") + "(" + Units.getLengthUnit() + ")"
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPTextField {
                    id: intervalKmField
                    width: addDialog.width-2*appTheme.paddingLarge
                    placeholderText: qsTr("Set to zero if only time limit used")
                    maximumLength: 40
                    validator: IntValidator{bottom: 0}
                    text: editMode ? oldIntervalKm : ""
                }
                Text {
                    text: qsTr("Service interval (months)")
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPTextField {
                    id: intervalMonthsField
                    width: addDialog.width-2*appTheme.paddingLarge
                    placeholderText: qsTr("Set to zero if only length limit used")
                    maximumLength: 4
                    validator: IntValidator{bottom: 0; top: 240}
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    text: editMode ? oldIntervalMonths : ""
                }
                Text {
                    text: qsTr("Long description")
                    font.pixelSize: appTheme.fontSizeMedium
                }
                FPTextArea {
                    id: longDescField
                    width: addDialog.width-2*appTheme.paddingLarge
                    height: 4*appTheme.fontSizeMedium
                    placeholderText: qsTr("Add long description")
                    text: editMode ? oldLong : ""
                }
            }
        }

    }

    buttons: FPButtonRow {
        anchors.horizontalCenter: parent.horizontalCenter
        FPButton {
            text: qsTr("Add");
            onClicked: addDialog.accept()
        }
        FPButton {
            text: qsTr("Cancel");
            onClicked: addDialog.cancel()
        }
      }

    onAccepted: addDialogAccepted()

}
