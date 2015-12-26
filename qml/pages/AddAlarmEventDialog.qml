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

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import org.fuelpad.qmlui 1.0
import "CommonFuncs.js" as Funcs

FPDialog {
    id: addDialog

    property bool editMode: false
    property string oldId
    property string oldAlarmId
    property string oldRecordId
    property string oldDate
    property double oldKm
    property string oldNotes
    property double oldService
    property double oldOil
    property double oldTires

    property date currentDate: editMode ? oldDate : "2014-01-01"

    width: parent.width

    title: editMode ? qsTr("Edit event") : qsTr("Add a new event")

    // Similar function exists also in AddFuelEntryDialog
    function launchDateDialogToToday(pv) {
         if (editMode) {
             var d = new Date(pv);
         }
         else {
             var d = new Date();
         }
         dateDialog.year = d.getFullYear();
         dateDialog.month = d.getMonth()+1;
         dateDialog.day = d.getDate();
         dateDialog.open();
    }

    function dateDialogAccecpted() {
        dateField.text = "%d-%02d-%02d".$(dateDialog.year,dateDialog.month,dateDialog.day)
        currentDate = dateField.text
    }

    function addDialogAccepted() {
        if (editMode) {
            applicationData.updateAlarmEvent(oldId, oldAlarmId, oldRecordId, dateField.text, kmField.text, serviceField.text, oilField.text,
                                            tiresField.text, notesField.text)
        }
        else {
            applicationData.addAlarmEvent(oldAlarmId, dateField.text, kmField.text, serviceField.text, oilField.text,
                                          tiresField.text, notesField.text)
        }
    }

    Flickable {
        id: addDialogData
        anchors {
            fill: parent
            leftMargin: appTheme.paddingLarge
            rightMargin: appTheme.paddingLarge
        }
        width: parent.width

        FPDatePickerDialog {
            id: dateDialog
            titleText: qsTr("Event date")
            acceptButtonText: qsTr("OK")
            rejectButtonText: qsTr("Cancel")
            minimumYear: 2000
            onAccepted: addDialog.dateDialogAccecpted()
        }

        Grid {
            id: addDialogGrid
            columns: 2
            spacing: appTheme.paddingMedium

            FPListButton {
                 id: dateButton
                 text: qsTr("Pick date")
                 width: text.width
                 onClicked: addDialog.launchDateDialogToToday(currentDate)
            }
            FPTextField {
                id: dateField
                placeholderText: qsTr("Add date")
                maximumLength: 10
                readOnly: true
                text: editMode ? oldDate : ""
            }

            Text {
                text: qsTr("Km")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: kmField
                placeholderText: qsTr("Add event km")
                maximumLength: 8
                validator: DoubleValidator{bottom: 0.0}
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: editMode ? oldKm : ""
            }

            Text {
                text: qsTr("Service")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: serviceField
                placeholderText: qsTr("Add service cost")
                maximumLength: 5
                validator: DoubleValidator{bottom: 0.0}
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: editMode ? oldService : ""
            }

            Text {
                text: qsTr("Oil")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: oilField
                placeholderText: qsTr("Add oil cost")
                maximumLength: 5
                validator: DoubleValidator{bottom: 0.0}
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: editMode ? oldOil : ""
            }

            Text {
                text: qsTr("Tires")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: tiresField
                placeholderText: qsTr("Add tires cost")
                maximumLength: 5
                validator: DoubleValidator{bottom: 0.0}
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: editMode ? oldTires : ""
            }

            Text {
                text: qsTr("Notes")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: notesField
                placeholderText: qsTr("Add notes")
                maximumLength: 120
                validator: RegExpValidator{}
                text: editMode ? oldNotes : ""
            }
        }

    }

    buttons: FPButtonRow {
        anchors.horizontalCenter: parent.horizontalCenter
        FPButton {
            text: editMode ? qsTr("Apply") : qsTr("Add");
            onClicked: addDialog.accept()
        }
        FPButton {
            text: qsTr("Cancel");
            onClicked: addDialog.cancel()
        }
      }

    onAccepted: addDialogAccepted()

}
