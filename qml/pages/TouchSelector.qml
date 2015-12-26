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

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import org.fuelpad.qmlui 1.0

Item {
    id: touchSelector
    property alias buttonText: button.text
    property alias titleText: selectionDialog.titleText
    property alias selectedIndex: selectionDialog.selectedIndex
    property alias model: selectionDialog.model

    signal selected(int selectedIndex)

    FPApplicationTheme {
        id: appTheme
    }

    height: content.height
    width: parent.width

    Rectangle {
        id: background
        anchors.fill: touchSelector
        anchors.margins: -appTheme.paddingLarge
        visible: mouseArea.pressed
//        color: UIConstants.COLOR_SELECT
        color: appTheme.highlightColor
    }

    FPSelectionDialog {
        id: selectionDialog
        titleText: titleText
        selectedIndex: selectedIndex
        model: model
        onSelectedIndexChanged: touchSelector.selected(selectedIndex)
    }

    Column {
        id: content
        width: parent.width

        FPLabel {
            id: button
            text: buttonText
            font.bold: true
        }

        FPLabel {
            id: value
            text: selectionDialog.model.get(selectedIndex).name
        }
    }


    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: selectionDialog.open()
    }
}
