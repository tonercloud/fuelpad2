/*
 * This file is part of Fuelpad.
 *
 * Copyright (C) 2007-2012 Julius Luukko <julle.luukko@quicknet.inet.fi>
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

import QtQuick 1.1
import org.fuelpad.qmlui 1.0

FPSheet {
    id: dialog

    property alias titleText: titleText.text

    title: Column {
            visible: false
            spacing: appTheme.paddingMedium
            Text {
                id: titleText
                font.pixelSize: appTheme.fontSizeLarge
                font.weight: Font.Bold
//                    anchors.centerIn: parent
                color: "white"
                text: titleText
            }
            Rectangle {
                id: titleField
                height: 2
                width: dialog.width
                color: "red"
            }
    }


    function cancel() {
        close();
        rejected();
    }

    function reject() {
        console.log("Dialog: overridden reject here")
    }

}
