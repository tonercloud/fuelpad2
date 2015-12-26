import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow {
    property bool showStatusBar
    property bool showToolBar
    property variant tools
    property alias mainMenuItems: menuBarItems.children

    Item {
        id: menuBarItems
    }
}
