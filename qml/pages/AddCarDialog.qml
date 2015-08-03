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
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        VerticalScrollDecorator {}

        Column {
            id: column
            anchors { left: parent.left; right: parent.right }
            spacing: Theme.paddingLarge

            PageHeader { title: "Add a new Car" }

            ListModel {
                id: fueltypeModel
                ListElement { name: "Petrol" }
                ListElement { name: "Diesel" }
                ListElement { name: "Ethanol" }
                ListElement { name: "Other" }
            }

            TextField {
                id: markfield
                anchors { left: parent.left; right: parent.right }
                inputMethodHints: Qt.ImhNoPredictiveText
                focus: true; label: "Mark"; placeholderText: "Mark / Marque"
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: modelfield.focus = true
            }

            TextField {
                id: modelfield
                anchors { left: parent.left; right: parent.right }
                label: "Model"; placeholderText: "Model of car"
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: yearfield.focus = true
            }

            TextField {
                id: yearfield
                anchors { left: parent.left; right: parent.right }
                validator: RegExpValidator { regExp: /^[0-9]{4,}$/ }
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                label: "Model year"; placeholderText: "Registration year of car (4 chars)"
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked:  regnumfield.focus = true
            }

            TextField {
                id: regnumfield
                anchors { left: parent.left; right: parent.right }
                label: "Registration number"; placeholderText: label
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: notesfield.focus = true
            }

            TextField {
                id: notesfield
                width: parent.width
                anchors { left: parent.left; right: parent.right }
                focus: true; label: "Add Notes"; placeholderText: "Notes"
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: fueltypefield.focus = true
            }

            TextField {
                id: fueltypefield
                width: parent.width
                anchors { left: parent.left; right: parent.right }
                focus: true; label: "Primary fuel type"; placeholderText: "Select primary fuel type"
                selectedIndex: 0
                model: fueltypeModel
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: {
                    if (errorHighlight)
                        markfield.focus = true
                    else
                        addCarToDatabase();
                    pageStack.clear();
                        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
                    }
                }
            }
        }

    Component {
        id: nextPage
        Page {
            backNavigation: true

            Column {
                anchors { left: parent.left; right: parent.right }
                spacing: Theme.paddingLarge

                PageHeader { title: "Car Added" }

                Label {
                    anchors { left: parent.left; right: parent.right }
                    horizontalAlignment: Text.AlignHCenter
                    text: "User Information"
                }

                Label {
                    anchors { left: parent.left; right: parent.right }
                    horizontalAlignment: Text.AlignHCenter
                    text: firstname.text
                }

                Label {
                    anchors { left: parent.left; right: parent.right }
                    horizontalAlignment: Text.AlignHCenter
                    text: lastname.text
                }

                Label {
                    anchors { left: parent.left; right: parent.right }
                    horizontalAlignment: Text.AlignHCenter
                    text: email.text
                }
            }
        }
    }
}
