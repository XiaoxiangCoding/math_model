#ifndef CITYUTILITIES_H
#define CITYUTILITIES_H

#include <QMap>
#include <QString>
#include"city.h"


class CityUtilities
{
public:
    CityUtilities();
    QMap<QString,City> cityMaps;
    QString getCityCodeFromName(QString cityName);
    QString getCityProvinceFromName(QString cityName);
    void initMaps();

};

#endif // CITYUTILITIES_H
