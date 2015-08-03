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
    id: statisticsPage

    property variant tools

    tools: statisticsTools

    property int year: 2014
    property int stattype: 2

    function changeYear(dy) {
        year = year + dy;
        applicationData.getStatistics(year,stattype);
    }

    function changeStatType(st) {
        stattype = st
        applicationData.getStatistics(year,stattype);
    }

    PageHeader {
        id: statisticsHeader
        title: applicationData.getCarMark(-1) + " " + applicationData.getCarModel(-1)
    }

    FPToolBarLayout {
        id: statisticsTools
//        visible: false
        FPToolIcon {
            iconId: "toolbar-back"
            onClicked: { statisticsMenu.close(); pageStack.pop(); }
        }
        FPToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (statisticsMenu.status === DialogStatus.Closed) ? statisticsMenu.open() : statisticsMenu.close()
        }
    }

    ListModel {
        id: testData
        ListElement {
            xc: 1.0
            yc: 10.222925023141
        }
        ListElement {
            xc: 2.0
            yc: 10.6830122591944
        }
        ListElement {
            xc: 3.0
            yc: 11.6581777023203
        }
        ListElement {
            xc: 4.0
            yc: 10.4933857704684
        }
        ListElement {
            xc: 5.0
            yc: 0.0
        }
        ListElement {
            xc: 6.0
            yc: 9.39358257285841
        }
        ListElement {
            xc: 7.0
            yc: 8.94745635285273
        }
        ListElement {
            xc: 8.0
            yc: 9.16784953867992
        }
        ListElement {
            xc: 9.0
            yc: 10.7719475277497
        }
        ListElement {
            xc: 10.0
            yc: 9.3165821539393
        }
        ListElement {
            xc: 11.0
            yc: 8.97641112047178
        }
        ListElement {
            xc: 12.0
            yc: 9.33869115958668
        }

    }

    // workaround https://bugreports.qt-project.org/browse/QTBUG-11403
    Text { text: qsTr(name) }
    ListModel {
        id: statisticsSelectModel
        ListElement { name: QT_TR_NOOP("Total monthly fill") }
        ListElement { name: QT_TR_NOOP("Monthly driving distance") }
        ListElement { name: QT_TR_NOOP("Monthly consumption")}
        ListElement { name: QT_TR_NOOP("Monthly average fuel price")}
    }

    // workaround https://bugreports.qt-project.org/browse/QTBUG-11403
    Text { text: qsTr(name) }
    ListModel {
        id: monthModel
        ListElement {
            name:  QT_TR_NOOP("Jan")
        }
        ListElement {
            name:  QT_TR_NOOP("Feb")
        }
        ListElement {
            name:  QT_TR_NOOP("Mar")
        }
        ListElement {
            name:  QT_TR_NOOP("Apr")
        }
        ListElement {
            name:  QT_TR_NOOP("May")
        }
        ListElement {
            name:  QT_TR_NOOP("Jun")
        }
        ListElement {
            name:  QT_TR_NOOP("Jul")
        }
        ListElement {
            name:  QT_TR_NOOP("Aug")
        }
        ListElement {
            name:  QT_TR_NOOP("Sep")
        }
        ListElement {
            name:  QT_TR_NOOP("Oct")
        }
        ListElement {
            name:  QT_TR_NOOP("Nov")
        }
        ListElement {
            name:  QT_TR_NOOP("Dec")
        }
    }

    FPSelectionDialog {
        id: statisticsSelectionDialog
        titleText: "Choose statistics"
        selectedIndex: stattype
        model: statisticsSelectModel
        onSelectedIndexChanged: changeStatType(selectedIndex)
    }

    property list<FPMenuAction> statisticsMenuModel

    statisticsMenuModel: [
        FPMenuAction {
            text: qsTr("Select statistics")
            onClicked: statisticsSelectionDialog.open()
        }
    ]

    SilicaFlickable {
        id: content

        width: statisticsPage.width
        anchors.top: statisticsHeader.bottom
        anchors.bottom: parent.bottom

        contentHeight: contentColumn.height

        FPMenu {
            id: statisticsMenu
            visualParent: pageStack
            items: statisticsMenuModel
        }

        Column {
            id: contentColumn
            spacing: 10
            width: content.width

            Column {
                id: insideContentColumn
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    top: statisticsHeader.bottom
                }

                FPLabel {
                    id: statisticsLabel
                    text: statisticsSelectModel.get(stattype).name + " " + year
                    platformStyle: MyLabelStyleTitle{}
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                    }
                }

                Plot {
                    id: plot
                    width: parent.width
                    height: isPortrait ? 400 : 250
            //        data: testData
                    data: statisticsModel
                    xLabels: monthModel
                    autoscaleX: false
                    xMin: 1
                    xMax: 12
                }

                FPButtonRow {
                    id: buttonRow
                    exclusive: false
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }
                    FPButton {
                        text: qsTr("Previous")
                        width: parent.width/2
                        onClicked: changeYear(-1)
                    }
                    FPButton {
                        text: qsTr("Next")
                        width: parent.width/2
                        onClicked: changeYear(+1)
                    }
                }

            }

        }

    }

}
