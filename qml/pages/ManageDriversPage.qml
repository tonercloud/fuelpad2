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
    id: manageDriversPage

    SilicaListView {
        id: driverlistView
        model: driverModel
        delegate: delegate
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Manage Drivers")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add a Driver")
                onClicked: pageStack.push(Qt.resolvedUrl("AddDriverDialog.qml"))
            }
        }

    Component {
        id: delegate
            Item {
                id: delegateRec
                height: (driverNameText.height * 1.5) + managedriversgrid.height
                width: parent.width - (Theme.paddingLarge * 2)
                MouseArea {
                    width: parent.width - Theme.paddingLarge
                    height: parent.height
                    onPressAndHold: Qt.resolvedUrl("DeleteDriverDialog.qml", manageDriversPage,
                                                        {databaseId: databaseid}).open()
                    onClicked: pageStack.push(Qt.resolvedUrl("AddDriverDialog.qml",manageDriversPage,
                                                             {editMode: true,
                                                              oldId: databaseid,
                                                              oldFullname: fullname,
                                                              oldNickname: nickname
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
                    id: driverNameText
                    text: nickname
                    font.bold: true
                }
                Grid {
                    id: managedriversgrid
                    anchors {
                        top: driverNameText.bottom
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
                            text: qsTr("Full name : ") + fullname
                        }
                    }

                    Rectangle {
                        id: itemSeperator
                        height: 2
                        width: driverlistView.width
                        color: "lightgrey"
                    }
                }
            }
        }
    }
}
