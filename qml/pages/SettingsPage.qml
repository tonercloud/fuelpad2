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
import "../extra_imports/CommonUnits.js" as Units

Page {
    id: settingsPage

    function setMainUnit(index) {
        var check = applicationData.getIndividualUnit()
        applicationData.setMainUnit(index, check)
    }

    function setIndividualUnit(checked) {
        var mainUnit = mainUnitSelector.selectedIndex
        applicationData.setMainUnit(mainUnit, checked)
    }

    ListView {
        model: UnitModel {}
        delegate: Text {
            text: name
        }
    }

    // Text { text: qsTr(name) }
    ListModel {
        id: unitModel
        ListElement { name: QT_TR_NOOP("SI") }
        ListElement { name: QT_TR_NOOP("US") }
        ListElement { name: QT_TR_NOOP("Imperial") }
    }

    // SilicaFlickable {
    SilicaListView {
        id: settingsFlickable
        model: unitModel
        anchors.fill: parent
        contentHeight: settingsColumn.height + Theme.paddingLarge
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: settingsColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            PageHeader {
                title: "Settings"
            }

            SectionHeader {
                text: qsTr("Length Unit")
            }

            ComboBox {
                id: unitLengthMethod
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("Select unit system")
                currentIndex: applicationData.getMainUnit()

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("SI")
                    }
                    MenuItem {
                        text: qsTr("US")
                    }
                    MenuItem {
                        text: qsTr("Imperial")
                    }
                }
            }

            SectionHeader {
                text: qsTr("Volume Unit")
            }

            ComboBox {
                id: unitVolumeMethod
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("Select unit system")
                currentIndex: applicationData.getMainUnit()

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("SI")
                    }
                    MenuItem {
                        text: qsTr("US")
                    }
                    MenuItem {
                        text: qsTr("Imperial")
                    }
                }
            }

            SectionHeader {
                text: qsTr("Mass Unit")
            }

            ComboBox {
                id: unitMassMethod
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("Select unit system")
                currentIndex: applicationData.getMainUnit()

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("SI")
                    }
                    MenuItem {
                        text: qsTr("US")
                    }
                    MenuItem {
                        text: qsTr("Imperial")
                    }
                }
            }

            SectionHeader {
                text: qsTr("Consumption Unit")
            }

            ComboBox {
                id: unitConsumptionMethod
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("Select unit system")
                currentIndex: applicationData.getMainUnit()

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("SI")
                    }
                    MenuItem {
                        text: qsTr("US")
                    }
                    MenuItem {
                        text: qsTr("Imperial")
                    }
                }
            }
        }
    }
}
