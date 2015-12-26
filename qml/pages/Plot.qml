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

import CustomComponents 1.0
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id: plot

    property bool autoscaleX: true
    property bool autoscaleY: true
    property double xMin
    property double xMax
    property double yMin
    property double yMax
    property double originX: 0.2*width
    property double originY: 0.8*height
    property double plotWidth: 0.6*width
    property double plotHeight: 0.6*height
    property color penColor: "blue"
    property alias data: dataView.model
    property alias xLabels: xlabelView.model
    property int datachange: 0

    opacity: 0.5

    // Coordinate axes
    // y-axis
    Line {
        x1: originX
        y1: originY-plotHeight
        x2: originX
        y2: originY
        penWidth: 3
        color: plot.penColor
    }
    // x-axis
    Line {
        x1: originX
        y1: originY
        x2: originX+plotWidth
        y2: originY
        penWidth: 3
        color: plot.penColor
    }

    ListView {
        id: dataView
        x: originX
        y: originY-plotHeight
        width: plotWidth
        height: plotHeight
//        model: data
        delegate: barGraph
    }

    ListView {
        id: xlabelView
        x: originX
        y: originY-plotHeight
        width: plotWidth
        height: plotHeight
        delegate: xlabelGraph
    }

    Connections {
        target: dataView.model
        onDataChanged: {
            datachange = (datachange == 1) ? 0 : 1
        }
    }

    function maxX() {
        var maxVal=0;
        var i;
        var val;
        for (i=0;i<data.count;i++) {
            val = data.get(i).xc;
            if (Number(val) > maxVal) {
                maxVal = Number(val);
            }
        }
        console.debug("maxX = " + maxVal);
        return maxVal;
    }

    function minX() {
        var minVal=0;
        var i;
        var val;
        for (i=0;i<data.count;i++) {
            val = data.get(i).xc;
            if (Number(val) < minVal) {
                minVal = Number(val);
            }
        }
        return minVal;
    }

    function maxY() {
        var maxVal=0;
        var i;
        var val;
        for (i=0;i<data.count;i++) {
            val = data.get(i).yc;
            if (Number(val) > maxVal) {
                maxVal = Number(val);
            }
        }
        return maxVal;
    }

    function minY() {
        var minVal=0;
        var i;
        var val;
        for (i=0;i<data.count;i++) {
            val = data.get(i).yc;
            if (Number(val) < minVal) {
                minVal = Number(val);
            }
        }
        return minVal;
    }

    function xcoord(xval) {
        var xscale;
        if (autoscaleX) {
            xscale = maxX()-minX();
        }
        else {
            xscale = xMax-xMin;
        }
        return xval/xscale;
    }

    function ycoord(yval) {
        var yscale;
        var retVal;
        if (autoscaleY) {
            yscale = maxY()-minY();
        }
        else {
            yscale = yMax-yMin;
        }
        if (yscale == 0) {
            retVal = 0;
        }
        else {
            retVal = yval/yscale;
        }

        return retVal;
    }

    Component {
        id: barGraph
        Item {
            x: (datachange || !datachange) ? xcoord(dataView.model.get(index).xc)*plotWidth*0.9 : 0
            y: 0
            Text {
                text: (datachange || !datachange) ? Number(dataView.model.get(index).yc).toFixed(1) : 0
                font.pixelSize: 16
                y: bar.y-1.1*font.pixelSize
            }

            Rectangle {
                id: bar
                width: 20
                height: (datachange || !datachange) ? ycoord(dataView.model.get(index).yc)*plotHeight : 0
                y: (datachange || !datachange) ? plotHeight-ycoord(dataView.model.get(index).yc)*plotHeight : 0
                color: plot.penColor
            }
        }
    }

    Component {
        id: xlabelGraph
        Item {
            x: (index+1)/(xlabelView.model.count-1)*plotWidth*0.9
            y: (datachange || !datachange) ? plotHeight : 0

            Text {
                text: name
                font.pixelSize: 16
                rotation: -90
            }

            // Tick marks
//            Rectangle {
//                width: 5
//                height: 10
//                y: -5
//                color: plot.penColor
//            }
        }
    }

}

