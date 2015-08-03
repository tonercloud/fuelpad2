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

Dialog {
    id: addDialog

    property bool editMode: false
    property string oldId
    property string oldFullname
    property string oldNickname

    width: parent.width

    title: editMode ? qsTr("Edit driver") : qsTr("Add a new driver")

    function addDialogAccepted() {
        if (editMode) {
            applicationData.updateDriver(oldId, fullnameField.text, nicknameField.text)
        } else {
            applicationData.addDriver(fullnameField.text, nicknameField.text)
        }
    }

    Flickable {
        id: addDialogData
        anchors {
            fill: parent
            leftMargin: appTheme.paddingLarge
            rightMargin: appTheme.paddingLarge
        }
        contentWidth: addDialogGrid.width
        contentHeight: addDialogGrid.height
        Grid {
            id: addDialogGrid
            columns: 1
            spacing: appTheme.paddingMedium
            Text {
                text: qsTr("Full name")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: fullnameField
                width: addDialog.width-2*appTheme.paddingLarge
                placeholderText: qsTr("Full name")
                maximumLength: 40
                validator: RegExpValidator{}
                text: editMode ? oldFullname : ""
            }
            Text {
                text: qsTr("Nick name")
                font.pixelSize: appTheme.fontSizeMedium
            }
            FPTextField {
                id: nicknameField
                width: addDialog.width-2*appTheme.paddingLarge
                placeholderText: qsTr("Nick name")
                maximumLength: 40
                validator: RegExpValidator{}
                text: editMode ? oldNickname : ""
            }
        }

    }

    buttons: FPButtonRow {
        anchors.horizontalCenter: parent.horizontalCenter
        FPButton {
            text: editMode ? qsTr("Apply") : qsTr("Add");
            onClicked: addDialog.accept()
        }
        FPButton {
            text: qsTr("Cancel");
            onClicked: addDialog.cancel()
        }
      }

    onAccepted: addDialogAccepted()

}
