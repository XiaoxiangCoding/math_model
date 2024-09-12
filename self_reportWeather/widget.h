#ifndef WIDGET_H
#define WIDGET_H

#include <QMenu>
#include <QNetworkReply>
#include <QWidget>
#include <qevent.h>
#include "day.h"
#include <QLabel>
#include<QList>
#include "cityutilities.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:


    QList<QLabel *> mDateList;
    QList<QLabel *> mWeekList;
    QList<QLabel *> mIconList;
    QList<QLabel *> mWeaTypeList;
    QList<QLabel *> mAirqList;
    QList<QLabel *> mFxList;
    QList<QLabel *> mFlList;


    QMenu *menuQuit;
    QPoint moffset;
    QNetworkReply *reply;
    QString url;
    Day days[7];
    QNetworkAccessManager *manager;
    CityUtilities cityUtils;
    QMap<QString,QString> mTypeMaps;
    void initWeatherTypeMaps();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_clicked();
    bool eventFilter(QObject *watched,QEvent *event) override;

    void on_lineEdit_search_returnPressed();

private:
    Ui::Widget *ui;
    void setMouseEvent();
    void readHttp(QNetworkReply *reply);
    void parseWeatherJsonData(QByteArray data);
    void updateUI();
    void initDayQlist();
    void drawTemLineHeight();
    void drawTemLineLow();
};
#endif // WIDGET_H
