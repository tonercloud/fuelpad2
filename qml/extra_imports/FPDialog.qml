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
import "FPApplicationTheme.js" as AppTheme

Page {
    id: pageRoot

    default property alias children: content.children
    property alias buttons: buttons.children
    property alias title: header.title

    signal accepted()
    signal done()
    signal rejected()

    function accept() {
        done()
        pageStack.pop()
        accepted()
    }

    function cancel() {
        done()
        pageStack.pop()
        rejected()
    }

    Item {
        id: backGround
        anchors.fill: parent
    }

    PageHeader {
        id: header
    }

    Item {
        id:buttons
        width: parent.width
        height: AppTheme.headerHeightPortrait
        anchors.top: header.bottom
    }

    Item {
        id: content
        anchors.top: buttons.bottom
        anchors.bottom: backGround.bottom
        width: isPortrait ? Screen.width : Screen.height
        height: isPortrait ? Screen.height : Screen.width
    }

}
