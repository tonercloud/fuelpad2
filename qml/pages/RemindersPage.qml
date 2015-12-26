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
import "../pages"
import "../extra_imports"
import "../extra_imports/CommonFuncs.js" as Funcs
import "../extra_imports/CommonUnits.js" as Units
import "../extra_imports/DialogStatus.js" as DialogStatus

Page {
    id: remindersPage

    property variant tools

    tools: remindersTools

    function loadAlarmTypePage(dbid) {
        applicationData.setCurrentCar(dbid)
        pageStack.push(Funcs.loadComponent("AlarmTypePage.qml",remindersPage, {"carId": dbid}))
    }

    PageHeader {
        id: applicationHeader
        title: qsTr("Reminders")
    }

    FPToolBarLayout {
        id: remindersTools
        visible: false
        FPToolIcon {
            iconId: "toolbar-back"
            onClicked: { pageStack.pop(); }
        }
//        FPToolIcon {
//            platformIconId: "toolbar-view-menu"
//            anchors.right: (parent === undefined) ? undefined : parent.right
//            onClicked: (fuelViewMenu.status === DialogStatus.Closed) ? fuelViewMenu.open() : fuelViewMenu.close()
//        }
    }

    SilicaFlickable {
        id: content

        width: remindersPage.width
        anchors.top: applicationHeader.bottom
        anchors.bottom: parent.bottom

        contentHeight: contentColumn.height

        Column {
            id: contentColumn
            spacing: 10

            ListView {
                id: listView
                model: carModel
                delegate: delegate
                anchors {
                    top: applicationHeader.bottom
//                    left: parent.left
//                    right: parent.right
//                    bottom: parent.bottom
                    leftMargin: appTheme.paddingLarge
                    rightMargin: appTheme.paddingLarge
                }
                height: content.height
                width: content.width
                clip: true
            }

            FPScrollDecorator {
                flickableItem: listView
            }

            Component {
                id: delegate
                    Item {
                        id: delegateRec
                        height: carNameText.height*1.5 + + grid.height
                        width: parent.width
                        MouseArea {
                            width: parent.width
                            height: parent.height
        //                    onPressAndHold: Funcs.loadComponent("DeleteCarDialog.qml", remindersPage,
        //                                                        {databaseId: databaseid}).open()
                            onClicked: loadAlarmTypePage(databaseid)
                        }

        //                states: [
        //                    State {
        //                        name: "selected"
        //                        when: (databaseid==selectedId)
        //                        PropertyChanges {target: delegateRec; color: "red"}
        //                    }
        //                ]

                        Image {
                            id: subIndicatorArrow
                            width: sourceSize.width

                            anchors {
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                                rightMargin: appTheme.scrollDecoratorMarginShort
                            }

                            smooth: true
                            source: "image://theme/icon-m-common-drilldown-arrow"
                                    + (theme.inverted ? "-inverse" : "");
                        }

                        FPLabel {
                            id: carNameText
                            text: mark + " " + carmodel
                            platformStyle: MyLabelStyleTitle{}
                            font.bold: true
                        }
                        Grid {
                            id: grid
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
                                LabelText {
                                    text: qsTr("Overall distance:") + " "
                                }
                                ElementText {
                                    text: totalkm.toFixed(0) + " " + Units.getLengthUnit()
                                }
                            }

                            Rectangle {
                                id: itemSeperator
                                height: 2
                                width: listView.width
                                color: appTheme.separatorColor
                            }
                    }

                }
            }
        }
    }


}
