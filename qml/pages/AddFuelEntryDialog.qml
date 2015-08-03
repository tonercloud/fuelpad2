/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012 Julius Luukko <julle.luukko@quicknet.inet.fi>
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

    property int carId: -1
    property bool editMode: false
    property string oldId
    property string oldDate
    property double oldKm
    property double oldTrip
    property double oldFill
    property bool oldNotFull
    property double oldPrice
    property string oldNotes
    property double oldService
    property double oldOil
    property double oldTires
    property double oldLat
    property double oldLon
    property string oldPlace

    property date currentDate: editMode ? oldDate : "2014-01-01"

    width: parent.width

    title: editMode ? qsTr("Edit fuel record") : qsTr("Add a new fuel record")

    function addDialogAccepted() {
        if (editMode) {
            applicationData.updateFuelEntry(carId, oldId, dateField.text, kmField.text, tripField.text, fillField.text,
                                         notFullFill.checked, priceField.text, serviceField.text,
                                         oilField.text, tiresField.text, latField.text, lonField.text,
                                            placeField.text, notesField.text)
        } else {
            applicationData.addFuelEntry(carId, dateField.text, kmField.text, tripField.text, fillField.text,
                                         notFullFill.checked, priceField.text, serviceField.text,
                                         oilField.text, tiresField.text, latField.text, lonField.text,
                                         placeField.text, notesField.text)
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
        clip: true

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

        FPDatePickerDialog {
            id: dateDialog
            titleText: qsTr("Entry date")
            acceptButtonText: qsTr("OK")
            rejectButtonText: qsTr("Cancel")
            minimumYear: 2000
            onAccepted: addDialogData.dateDialogAccecpted()
        }

        function addressFound(address, lat, lon) {
            placeField.text = address;
            latField.text = lat;
            lonField.text = lon
        }

        FocusScope {
            x: column.x; y: column.y
            width: column.width; height: column.height
            Column{
                id: column
                Grid {
                    columns: 1
                    spacing: appTheme.paddingMedium

                    FPListButton {
                         id: dateButton
                         text: qsTr("Pick date")
                         width: text.width
                         onClicked: addDialogData.launchDateDialogToToday(currentDate)
                    }
                    FPTextField {
                        id: dateField
                        width: addDialog.width-2*appTheme.paddingLarge
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
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add overall km")
                        maximumLength: 8
                        validator: DoubleValidator{bottom: 0.0}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        onActiveFocusChanged: {
                            if (!focus) {
                                tripField.text = applicationData.calcTrip(kmField.text, tripField.text)
                            }
                        }
                        text: editMode ? oldKm : ""
                    }
                    Text {
                        text: qsTr("Trip")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPTextField {
                        id: tripField
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add trip")
                        maximumLength: 5
                        validator: DoubleValidator{bottom: 0.0}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: editMode ? oldTrip : ""
                    }
                    Text {
                        text: qsTr("Fill")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPTextField {
                        id: fillField
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add fill")
                        maximumLength: 5
                        validator: DoubleValidator{bottom: 0.0}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: editMode ? oldFill : ""
                    }
                    Text {
                        text: qsTr("Not full fill")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPSwitch {
                        id: notFullFill
                        checked: editMode ? oldNotFull : false
                    }
                    Text {
                        text: qsTr("Price")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPTextField {
                        id: priceField
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add price")
                        maximumLength: 10
                        validator: DoubleValidator{}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: editMode ? oldPrice : ""
                    }
                    Text {
                        text: qsTr("Notes")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPTextField {
                        id: notesField
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add notes")
                        maximumLength: 120
                        validator: RegExpValidator{}
                        text: editMode ? oldNotes : ""
                    }
                    Text {
                        text: qsTr("Service")
                        font.pixelSize: appTheme.fontSizeMedium
                    }
                    FPTextField {
                        id: serviceField
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add service cost")
                        maximumLength: 10
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
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add oil cost")
                        maximumLength: 10
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
                        width: addDialog.width-2*appTheme.paddingLarge
                        placeholderText: qsTr("Add tires cost")
                        maximumLength: 10
                        validator: DoubleValidator{bottom: 0.0}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        text: editMode ? oldTires : ""
                    }

                    Text {
                        text: qsTr("Latitude")
                        font.pixelSize: appTheme.fontSizeMedium
    //                    visible: false
                    }
                    FPTextField {
                        id: latField
                        width: addDialog.width-2*appTheme.paddingLarge
                        text: editMode ? oldLat : positionSource.position.coordinate.latitude.toFixed(8)
    //                    visible: false
                    }

                    Text {
                        text: qsTr("Longitude")
                        font.pixelSize: appTheme.fontSizeMedium
    //                    visible: false
                    }
                    FPTextField {
                        id: lonField
                        width: addDialog.width-2*appTheme.paddingLarge
                        text: editMode ? oldLon : positionSource.position.coordinate.longitude.toFixed(8)
    //                    visible: false
                    }

                    FPListButton {
                        text: qsTr("Get address")
                        onClicked: pageStack.push(Funcs.loadComponent("SelectLocationPage.qml",addDialogData, {}))
                    }
                    FPTextArea {
                        id: placeField
                        width: addDialog.width-2*appTheme.paddingLarge
                        height: 4*appTheme.fontSizeMedium
                        placeholderText: qsTr("Add place")
                        text: editMode ? oldPlace : ""
                    }

                }

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
