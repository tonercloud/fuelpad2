#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <QSettings>

class UserConfig
{
public:
    UserConfig();
    QSettings settings;
};

#endif // USERCONFIG_H
