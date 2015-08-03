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
    id: settingsPage

    property variant tools

    tools: commonTools

    function setMainUnit(index) {
        var check = applicationData.getIndividualUnit()
        applicationData.setMainUnit(index, check)
    }

    function setIndividualUnit(checked) {
        var mainUnit = mainUnitSelector.selectedIndex
        applicationData.setMainUnit(mainUnit, checked)
    }

    // FPApplicationTheme {
        // id: appTheme
    // }

    ListView {
        model: UnitModel {}
        delegate: Text {
            text: name
        }
    }

    PageHeader {
        id: applicationHeader
        title: qsTr("Settings")
    }

    Column {
        anchors.top: applicationHeader.bottom
        anchors.topMargin: appTheme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: Theme.paddingMedium

        FPTouchSelector {
            id: mainUnitSelector
            buttonText: qsTr("Unit system")
            titleText: qsTr("Select unit system")
            selectedIndex: applicationData.getMainUnit()
            model: UnitModel {}
            onSelected: setMainUnit(selectedIndex)
        }

        Row {

            Label {
                text: qsTr("Select units individually")
            }

            Switch {
                id: individualUnit
                checked: applicationData.getIndividualUnit()
                onCheckedChanged: setIndividualUnit(checked)
            }
        }

        FPTouchSelector {
            id: lengthSelector
            visible: individualUnit.checked
            buttonText: qsTr("Unit system for length")
            titleText: qsTr("Select unit system")
            selectedIndex: applicationData.getLengthUnit()
            model: UnitModel {}
            onSelected: applicationData.setLengthUnit(selectedIndex)
        }

        FPTouchSelector {
            visible: individualUnit.checked
            buttonText: qsTr("Unit system for volume")
            titleText: qsTr("Select unit system")
            selectedIndex: applicationData.getVolumeUnit()
            model: UnitModel {}
            onSelected: applicationData.setVolumeUnit(selectedIndex)
        }

        FPTouchSelector {
            visible: individualUnit.checked
            buttonText: qsTr("Unit system for mass")
            titleText: qsTr("Select unit system")
            selectedIndex: applicationData.getMassUnit()
            model: UnitModel {}
            onSelected: applicationData.setMassUnit(selectedIndex)
        }

        FPTouchSelector {
            visible: individualUnit.checked
            buttonText: qsTr("Unit system for consumption")
            titleText: qsTr("Select unit system")
            selectedIndex: applicationData.getConsumeUnit()
            model: UnitModel {}
            onSelected: applicationData.setConsumeUnit(selectedIndex)
        }
    }
}
