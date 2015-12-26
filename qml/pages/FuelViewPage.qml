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
    id: fuelViewPage

    property variant tools

    tools: fuelViewTools

    property int carId: -1

    property bool searchBarVisible: false

    function loadStatisticsPage(dbid) {
        var today = new Date()
        var thisyear = Qt.formatDate(today, "yyyy");
        applicationData.getStatistics(thisyear,2);
        pageStack.push(Funcs.loadComponent("StatisticsPage.qml",fuelViewPage, {year: thisyear, stattype: 2}));
    }

    function loadReportPage(dbid) {
        pageStack.push(Funcs.loadComponent("ReportPage.qml",fuelViewPage, {}));
    }

    function updateSearchExp(searchExp) {
        applicationData.setSearchExpression(searchExp)
    }

    PageHeader {
        id: applicationHeader
        title: applicationData.getCarMark(-1) + " " + applicationData.getCarModel(-1)
    }

    FPToolBarLayout {
        id: fuelViewTools
        visible: false
        FPToolIcon {
            iconId: "toolbar-back"
            onClicked: { fuelViewMenu.close(); pageStack.pop(); }
        }
        FPToolIcon {
            iconId: "toolbar-add"
            onClicked: pageStack.push(Funcs.loadComponent("AddFuelEntryDialog.qml",fuelViewPage, {carId: carId}))
        }
        FPToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (fuelViewMenu.status === DialogStatus.Closed) ? fuelViewMenu.open() : fuelViewMenu.close()
        }
    }

    property list<FPMenuAction> fuelMenuModel

    fuelMenuModel: [
        FPMenuAction {
            text: qsTr("Statistics")
            onClicked: loadStatisticsPage()
        },
        FPMenuAction {
            text: qsTr("Report")
            onClicked: loadReportPage()
        }
    ]

    SilicaFlickable {
        id: content

        width: fuelViewPage.width
        anchors.top: applicationHeader.bottom
        anchors.bottom: parent.bottom

        contentHeight: contentColumn.height

        FPMenu {
            id: fuelViewMenu
            visualParent: pageStack
            items: fuelMenuModel
        }

        Column {
            id: contentColumn
            spacing: 10

            FPTextField {
                id: searchBar
                anchors.top: applicationHeader.bottom
                width: content.width
                placeholderText: applicationData.getSearchExpression() == "" ? qsTr("Search") : ""
                text: applicationData.getSearchExpression() == "" ? "" : applicationData.getSearchExpression()
                onActiveFocusChanged: updateSearchExp(searchBar.text)
                visible: searchBarVisible
            }

            FPToolIcon {
                id: clearSearchBar
                iconId: "toolbar-cancle"
                anchors.right: executeSearch.left
                anchors.top: searchBar.top
                onClicked: searchBar.text = ""
                visible: searchBarVisible
            }

            FPToolIcon {
                id: executeSearch
                iconId: "toolbar-search"
                anchors.right: searchBar.right
                anchors.top: searchBar.top
                onClicked: updateSearchExp(searchBar.text)
                visible: searchBarVisible
            }

            ListView {
                id: listView
                model: fuelModel
                delegate: delegate
                anchors {
                      top: searchBarVisible ? searchBar.bottom : applicationHeader.bottom
//                    left: parent.left
//                    right: parent.right
//                    bottom: parent.bottom
                    leftMargin: appTheme.paddingLarge
                    rightMargin: appTheme.paddingLarge
                }
                height: content.height
                width: content.width
                clip: true
                onContentYChanged: {
                    if (contentY < -50) {
                                       searchBarVisible = true
                                   }
                }
            }

            FPScrollDecorator {
                flickableItem: listView
            }

            Component {
                id: delegate
                    Item {
                        id: delegateRec
                        height: dateText.height*1.5 + grid.height
                        width: parent.width
                        MouseArea {
                            width: parent.width
                            height: parent.height
                            onPressAndHold: Funcs.loadComponent("DeleteFuelEntryDialog.qml", fuelViewPage,
                                                                {databaseId: databaseid}).open()
                            onClicked: pageStack.push(Funcs.loadComponent("AddFuelEntryDialog.qml",fuelViewPage,
                                                           {carId: carId,
                                                            editMode: true,
                                                            oldId: databaseid,
                                                            oldDate: date,
                                                            oldKm: km,
                                                               oldTrip: trip,
                                                               oldFill: fill,
                                                               oldNotFull: notfull,
                                                               oldPrice: price,
                                                               oldNotes: notes,
                                                               oldService: service,
                                                               oldOil: oil,
                                                               oldTires: tires,
                                                               oldLat: latitude,
                                                               oldLon: longitude,
                                                               oldPlace: place
                                                           }))
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
                            id: dateText
                            text: date + ": " + km.toFixed(0) + " " + Units.getLengthUnit() + " " + (price+service+oil+tires).toFixed(2)
                                  + applicationData.getCurrencySymbol()
                            platformStyle: MyLabelStyleTitle{}
                            font.bold: true
                        }
                        Grid {
                            id: grid
                            anchors {
                                top: dateText.bottom
                            }

                            columns: 1

                            Row {
                                Text {
                                    text: databaseid
                                    visible: false
                                }

                                Text {
                                    text: notfull
                                    visible: false
                                }
                            }

                            Row {
                                visible: (trip > 0)? true : false
                                LabelText {
                                    id: tripLabel
                                    text: qsTr("Trip")
                                }
                                ElementText {
                                    text: trip.toFixed(1) + " " + Units.getLengthUnit()
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (fill > 0)? true : false
                                LabelText {
                                    text: qsTr("Fill")
                                }
                                ElementText {
                                    text: fill.toFixed(2) + " " + Units.getVolumeUnit()
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (fill > 0)? true : false
                                LabelText {
                                    text: qsTr("Consum")
                                }
                                ElementText {
                                    text: consum.toFixed(2) + " " + Units.getConsumeUnit()
                                }
                                ElementText {
                                    text: "(" + co2.toFixed(0) + " " + Units.getEmissionUnit() + ")"
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (price > 0)? true : false
                                LabelText {
                                    text: qsTr("Price/trip")
                                }
                                ElementText {
                                    text: ppt.toFixed(2) + " " + applicationData.getCurrencySymbol()
                                          + "/" + Units.getLengthUnit()
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (price > 0)? true : false
                                LabelText {
                                    text: applicationData.getVolumeUnit() == 0 ? qsTr("Price/litre") : qsTr("Price/gallon")
                                }
                                ElementText {
                                    text: ppl.toFixed(2) + " " + applicationData.getCurrencySymbol()
                                          + "/" + Units.getVolumeUnit()
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (service > 0)? true : false
                                LabelText {
                                    text: qsTr("Service")
                                }
                                ElementText {
                                    text: service
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (oil > 0)? true : false
                                LabelText {
                                    text: qsTr("Oil")
                                }
                                ElementText {
                                    text: oil
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (notes == "")? false : true
                                LabelText {
                                    text: qsTr("Notes")
                                }
                                ElementText {
                                    text: notes
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (latitude == 0)? false : true
                                LabelText {
                                    text: qsTr("Latitude")
                                }
                                ElementText {
                                    text: latitude
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (longitude == 0)? false : true
                                LabelText {
                                    text: qsTr("Longitude")
                                }
                                ElementText {
                                    text: longitude
                                }
                            }

                            Row {
                                spacing: 10
                                visible: (place == "")? false : true
                                LabelText {
                                    text: qsTr("Address")
                                }
                                ElementText {
                                    text: place
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
