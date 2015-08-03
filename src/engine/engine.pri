SOURCES += \
    $$PWD/database.cpp \
    $$PWD/databasesqlite.cpp \
    $$PWD/fuelrecord.cpp \
    $$PWD/datafield.cpp \
    $$PWD/unitsystem.cpp \
    $$PWD/cardata.cpp \
    $$PWD/userconfig.cpp \
    $$PWD/driverdata.cpp \
    $$PWD/roleitemmodel.cpp \
    $$PWD/uiwrapper.cpp \
    $$PWD/mysortfilterproxymodel.cpp \
    $$PWD/alarmtypedata.cpp \
    $$PWD/alarmeventdata.cpp \
    $$PWD/geocode.cpp \
    $$PWD/geocodenominatim.cpp \
    $$PWD/plotdatamodel.cpp \
    $$PWD/uiengine.cpp \
    $$PWD/carstatistics.cpp
HEADERS += \
    $$PWD/database.h \
    $$PWD/databasesqlite.h \
    $$PWD/fuelrecord.h \
    $$PWD/datafield.h \
    $$PWD/unitsystem.h \
    $$PWD/cardata.h \
    $$PWD/userconfig.h \
    $$PWD/driverdata.h \
    $$PWD/roleitemmodel.h \
    $$PWD/uiwrapper.h \
    $$PWD/mysortfilterproxymodel.h \
    $$PWD/alarmtypedata.h \
    $$PWD/alarmeventdata.h \
    $$PWD/geocode.h \
    $$PWD/geocodenominatim.h \
    $$PWD/plotdatamodel.h \
    $$PWD/uiengine.h \
    $$PWD/carstatistics.h
QT += \
    sql \
    network \
    xml

INCLUDEPATH += $$PWD
