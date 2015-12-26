# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-fuelpad2

CONFIG += sailfishapp

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT += sql

include(src/engine/engine.pri)

RESOURCES += resources.qrc

HEADERS += src/line.h \
    src/engine/uiengine.h

SOURCES += src/harbour-fuelpad2.cpp \
    src/line.cpp

OTHER_FILES += qml/harbour-fuelpad2.qml \
    qml/cover/CoverPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/AboutDialog.qml \
    qml/pages/AddAlarmEventDialog.qml \
    qml/pages/AddAlarmTypeDialog.qml \
    qml/pages/AddCarDialog.qml \
    qml/pages/AddDriverDialog.qml \
    qml/pages/AddFuelEntryDialog.qml \
    qml/pages/AlarmEventPage.qml \
    qml/pages/AlarmTypePage.qml \
    qml/pages/DeleteCarDialog.qml \
    qml/pages/DeleteDriverDialog.qml \
    qml/pages/DeleteFuelEntryDialog.qml \
    qml/pages/DrivingLogPage.qml \
    qml/pages/ElementText.qml \
    qml/pages/FuelViewPage.qml \
    qml/pages/HelpDialog.qml \
    qml/pages/LabelText.qml \
    qml/pages/MainPage.qml \
    qml/pages/ManageCarsPage.qml \
    qml/pages/ManageDriversPage.qml \
    qml/pages/MyDialog.qml \
    qml/pages/MyLabelStyleTitle.qml \
    qml/pages/PageHeader.qml \
    qml/pages/Plot.qml \
    qml/pages/RemindersPage.qml \
    qml/pages/ReportPage.qml \
    qml/pages/SelectLocationPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/StatisticsPage.qml \
    qml/pages/TouchSelector.qml \
    qml/extra_imports/CommonFuncs.js \
    qml/extra_imports/CommonUnits.js \
    qml/extra_imports/DialogStatus.js \
    qml/extra_imports/UIConstants.js \
    qml/extra_imports/FPApplicationTheme.qml \
    qml/extra_imports/FPButtonRow.qml \
    qml/extra_imports/FPButtonStyle.qml \
    qml/extra_imports/FPDatePickerDialog.qml \
    qml/extra_imports/FPDialog.qml \
    qml/extra_imports/FPLabel.qml \
    qml/extra_imports/FPLabelStyle.qml \
    qml/extra_imports/FPListButton.qml \
    qml/extra_imports/FPMenuAction.qml \
    qml/extra_imports/FPMenuItem.qml \
    qml/extra_imports/FPMenuLayout.qml \
    qml/extra_imports/FPMenu.qml \
    qml/extra_imports/FPPageStackWindow.qml \
    qml/extra_imports/FPQueryDialog.qml \
    qml/extra_imports/FPScrollDecorator.qml \
    qml/extra_imports/FPSelectionDialog.qml \
    qml/extra_imports/FPSheet.qml \
    qml/extra_imports/FPSimpleDialog.qml \
    qml/extra_imports/FPSwitch.qml \
    qml/extra_imports/FPTextArea.qml \
    qml/extra_imports/FPTextField.qml \
    qml/extra_imports/FPToolBarLayout.qml \
    qml/extra_imports/FPToolIcon.qml \
    qml/extra_imports/FPTouchSelector.qml \
    rpm/julius_fuelpad.changes.in \
    rpm/julius_fuelpad.spec \
    rpm/julius_fuelpad.yaml \
    translations/*.ts \
    julius_fuelpad.desktop \
    qml/pages/MainPage.qml \
    qml/pages/UnitModel.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-fuelpad2-de.ts

