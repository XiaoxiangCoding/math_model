#include "cityutilities.h"

#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>

CityUtilities::CityUtilities()
{

}

QString CityUtilities::getCityCodeFromName(QString cityName)
{
    if(cityMaps.isEmpty()){
        initMaps();
    }

   auto it= cityMaps.find(cityName);
   if(it==cityMaps.end()){


       cityName.chop(1);

       it=cityMaps.find(cityName);

       if(it!=cityMaps.end()){
           return it.value().cityId;
       }


   }
   else{

       return it.value().cityId;

   }

   return "";




}

QString CityUtilities::getCityProvinceFromName(QString cityName)
{

    if(cityMaps.isEmpty()){
        initMaps();
    }
    auto it= cityMaps.find(cityName);
    if(it==cityMaps.end()){


        cityName.chop(1);

        it=cityMaps.find(cityName);

        if(it!=cityMaps.end()){
            return it.value().cityProvince;
        }
        else{
            return "";
        }


    }
    else{

        return it.value().cityProvince;

    }




}

void CityUtilities::initMaps()
{
    QFile file(":/cityid.json");

    file.open(QIODevice::ReadOnly);

    QByteArray rawData=file.readAll();

    file.close();

    QJsonDocument jsonDoc=QJsonDocument::fromJson(rawData);

    QJsonArray jsonArray=jsonDoc.array();

    for(QJsonValue val:jsonArray){

        if(val.isObject()){

            QString cityName1=val["cityZh"].toString();
            QString cityProvince=val["provinceZh"].toString();
            QString cityCode=val["id"].toString();
            cityMaps.insert(cityName1,City(cityName1,cityProvince,cityCode));

        }


    }




}
