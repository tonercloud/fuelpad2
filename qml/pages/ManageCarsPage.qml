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
import "../extra_imports/CommonUnits.js" as Units

Page {
    id: manageCarsPage

    SilicaListView {
        id: carlistView
        model: carModel
        delegate: delegate
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Manage Cars")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add a Car")
                onClicked: pageStack.push(Qt.resolvedUrl("AddCarDialog.qml"))
            }
        }

    Component {
        id: delegate
            Item {
                id: delegateRec
                x: Theme.paddingMedium
                height: (carNameText.height * 1.5) + managecarsgrid.height
                width: parent.width - (Theme.paddingMedium * 2)
                MouseArea {
                    width: parent.width
                    height: parent.height
                    onPressAndHold: Qt.resolvedUrl("DeleteCarDialog.qml", manageCarsPage,
                                                        {databaseId: databaseid}).open()
                    onClicked: pageStack.push(Qt.resolvedUrl("AddCarDialog.qml",manageCarsPage,
                                                   {editMode: true,
                                                    oldId: databaseid,
                                                    oldMark: mark,
                                                    oldModel: carmodel,
                                                    oldYear: year,
                                                    oldRegnum: regnum,
                                                    oldFueltype: fueltype,
                                                    oldNotes: notes
                                                   }))
                }

                states: [
                    State {
                        name: "selected"
                        when: (databaseid==selectedId)
                        PropertyChanges {target: delegateRec;}
                    }
                ]

                Label {
                    id: carNameText
                    text: mark + " " + carmodel
                    font.bold: true
                }
                Grid {
                    id: managecarsgrid
                    anchors {
                        top: carNameText.bottom
                    }

                    columns: 1

                    Row {
                        Text {
                            text: databaseid
                            visible: false
                        }
                    }

                    Row {
                        Label {
                            text: qsTr("Model year : ") + year
                        }
                    }
                    Row {
                        Label {
                            text: qsTr("Registration number : ") + regnum
                        }
                    }

                    Row {
                        Label {
                            text: qsTr("Notes : ") + notes
                        }
                    }

                    Row {
                        Label {
                            text: qsTr("Overall distance : ") + totalkm.toFixed(0) + " " + Units.getLengthUnit()
                        }
                    }

                    Row {
                        Label {
                            text: qsTr("Last month : ") + lastmonthkm.toFixed(0) + " " + Units.getLengthUnit()
                        }
                    }

                    Row {
                        Label {
                            text: qsTr("Last year : ") + lastyearkm.toFixed(0) + " " + Units.getLengthUnit()
                        }
                    }

                    Rectangle {
                        id: itemSeperator
                        height: 2
                        width: carlistView.width - (Theme.paddingMedium * 2)
                        color: "lightgrey"
                    }
                }
            }
        }
    }
}
