#ifndef CITY_H
#define CITY_H

#include <QString>



class City
{
public:
    City();
    City(QString name,QString province,QString id);

    QString cityName;
    QString cityProvince;
    QString cityId;
};

#endif // CITY_H
