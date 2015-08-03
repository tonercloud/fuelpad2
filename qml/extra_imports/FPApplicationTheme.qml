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
import "FPApplicationTheme.js" as AppTheme

Item {
    property variant fontFamily: AppTheme.fontFamily
    property variant fontFamilyHeading: AppTheme.fontFamilyHeading

    property variant fontSizeExtraSmall: AppTheme.fontSizeExtraSmall
    property variant fontSizeSmall: AppTheme.fontSizeSmall
    property variant fontSizeMedium: AppTheme.fontSizeMedium
    property variant fontSizeLarge: AppTheme.fontSizeLarge
    property variant fontSizeExtraLarge: AppTheme.fontSizeExtraLarge

    property variant primaryColor: AppTheme.primaryColor
    property variant secondaryColor: AppTheme.secondaryColor
    property variant highlightColor: AppTheme.highlightColor
    property variant secondaryHighlightColor: AppTheme.secondaryHighlightColor
    property variant redColor: AppTheme.redColor
    property variant greenColor: AppTheme.greenColor

    property variant itemSizeSmall: AppTheme.itemSizeSmall
    property variant itemSizeMedium: AppTheme.itemSizeMedium
    property variant itemSizeLarge: AppTheme.itemSizeLarge
    property variant itemSizeExtraLarge: AppTheme.itemSizeExtraLarge
    property variant paddingSmall: AppTheme.paddingSmall
    property variant paddingMedium: AppTheme.paddingMedium
    property variant paddingLarge: AppTheme.paddingLarge

    property variant headerHeightPortrait: AppTheme.headerHeightPortrait

    property variant headerColorForeground: AppTheme.headerColorForeground
    property variant headerColorBackground: AppTheme.headerColorBackground

    property variant scrollDecoratorMarginShort: AppTheme.scrollDecoratorMarginShort
    property variant scrollDecoratorMarginLong: AppTheme.scrollDecoratorMarginLong

    property variant buttonSpacing: AppTheme.buttonSpacing
    property variant separatorColor: AppTheme.separatorColor
}
