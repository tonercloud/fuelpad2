/*
 * This file is part of CarSails.
 *
 * Copyright (C) 2007-2012,2014 Julius Luukko <julle.luukko@quicknet.inet.fi>
 * and Chris Walker <cdw_nokiaqt@the-walker-hopusehold.co.uk>
 *
 * CarSails is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Carsails is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Carsails.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

import "../extra_imports/CommonUnits.js" as Units

Page {
    id: carsailspage

    property bool firstRun: true

    function loadFuelViewPage(dbid) {
        applicationData.setCurrentCar(dbid)
        pageStack.push(Qt.resolvedUrl("FuelViewPage.qml", CarSailsPage, {"carId": dbid}))
    }

    function clearFirstRun() {
        applicationData.clearDataBaseDidNotExist();
        firstRun = false
    }

    SilicaListView {
        id: carlistview
        model: carModel
        delegate: delegate
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Car List")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings");
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("Manage Cars")
                onClicked: pageStack.push(Qt.resolvedUrl("ManageCarsPage.qml"))
            }
            MenuItem {
                text: qsTr("Manage Drivers")
                onClicked: pageStack.push(Qt.resolvedUrl("ManageDriversPage.qml"))
            }
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutDialog.qml"))
            }
            MenuItem {
                text: qsTr("Help Page")
                onClicked: pageStack.push(Qt.resolvedUrl("HelpDialog.qml"))
            }
        }

        PushUpMenu {
            MenuItem {
                text: "Go to top"
                onClicked: carlistview.scrollToTop()
            }
        }

        Component {
            id: delegate
            Item {
                id: delegateRec
                height: (carNameText.height * 1.5) + maingrid.height
                width: parent.width
                MouseArea {
                    width: parent.width
                    height: parent.height
                    onClicked: pageStack.push(Qt.resolvedUrl("ManageCarsPage.qml",
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

            Label {
                id: carNameText
                text: mark + " " + carmodel
                font.bold: true
            }

            Grid {
                id: maingrid
                anchors {
                    top: carNameText.bottom
                }

                columns: 1

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
                    width: carlistview.width
                    color: "lightgrey"
                    }
                }
            }
        }
    }
}
