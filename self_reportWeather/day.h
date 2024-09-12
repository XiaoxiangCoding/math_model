#ifndef DAY_H
#define DAY_H

#include <QString>



class Day
{
public:
    Day();

    QString mDate;
    QString mWeek;
    QString mCity;
    QString mProvince;
    QString mWeatherType;
    QString mTempLow;
    QString mTempHigh;
    QString mTempNow;

    QString mTips;
    QString mFx;
    QString mFl;
    QString mPm25;
    QString mHu;
    QString mAirq;

};

#endif // DAY_H
