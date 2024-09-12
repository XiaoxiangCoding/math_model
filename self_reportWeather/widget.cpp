#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFont(QFont("Arial",15));
    this->setFixedSize(605,870);
    setWindowFlag(Qt::FramelessWindowHint);

    setMouseEvent();


    url="http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=69798948&appsecret=88w2P0j6";
    manager = new QNetworkAccessManager(this);
    QNetworkRequest res(url);
    reply= manager->get(res);


    initWeatherTypeMaps();
    initDayQlist();
    connect(manager,&QNetworkAccessManager::finished,this,&Widget::readHttp);

    ui->widgetWendu1->installEventFilter(this);
    ui->widgetWendu2->installEventFilter(this);
}

void Widget:: initDayQlist(){



    mDateList<<ui->labelDate0<<ui->labelDate1<<ui->labelDate2 <<ui->labelDate3
            <<ui->labelDate4<<ui->labelDate5;
    mWeekList<<ui->labelDay0<<ui->labelDay1<<ui->labelDay2<<ui->labelDay3<<
               ui->labelDay4<<ui->labelDay5;

    mIconList<<ui->labelDayWeatherIcon<<ui->labelDayWeatherIcon_2<<ui->labelDayWeatherIcon_3
            <<ui->labelDayWeatherIcon_4<<ui->labelDayWeatherIcon_5<<ui->labelDayWeatherIcon_6;

    mWeaTypeList<<ui->labelDayWeather0<<ui->labelDayWeather0_2<<ui->labelDayWeather0_3
               <<ui->labelDayWeather0_4<<ui->labelDayWeather0_5<<ui->labelDayWeather0_6;

    mFxList<<ui->labelFx0<<ui->labelFx0_2<<ui->labelFx0_3<<ui->labelFx0_4
          <<ui->labelFx0_5<<ui->labelFx0_6;
    mFlList<<ui->labelFXValue0<<ui->labelFXValue0_2<<ui->labelFXValue0_3
          <<ui->labelFXValue0_4<<ui->labelFXValue0_5<<ui->labelFXValue0_6;
    mAirqList<<ui->labelDayAir0<<ui->labelDayAir1<<ui->labelDayAir2
            <<ui->labelDayAir3<<ui->labelDayAir4<<ui->labelDayAir5;


}

Widget::~Widget()
{
    delete ui;
}

void Widget::initWeatherTypeMaps()
{
    mTypeMaps.insert("晴",":/weathercondition/qing2.png");

    mTypeMaps.insert("晴",":/weathercondition/qing2.png");
    mTypeMaps.insert("多云",":/weathercondition/duoyun (1).png");
    mTypeMaps.insert("小雨",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("中雨",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("中雨转阴",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("大雨",":/weathercondition/tedabaoyu.png");
    mTypeMaps.insert("小雨转大雨",":/weathercondition/tedabaoyu.png");
    mTypeMaps.insert("阴",":/weathercondition/yin.png");
    mTypeMaps.insert("阴转晴",":/weathercondition/yin.png");
    mTypeMaps.insert("雷阵雨转多云",":/weathercondition/leizhenyuzhuanduoyun.png");
    mTypeMaps.insert("多云转雷阵雨",":/weathercondition/leizhenyuzhuanduoyun.png");
    mTypeMaps.insert("雷阵雨转晴",":/weathercondition/leizhenyuzhuanqing.png");
    mTypeMaps.insert("多云转晴",":/weathercondition/qingzhuanduoyun.png");
    mTypeMaps.insert("晴转多云",":/weathercondition/qingzhuanduoyun.png");
    mTypeMaps.insert("阵雨转小雨",":/weathercondition/leizhenyuzhuanduoyun.png");
    mTypeMaps.insert("小雨转阵雨",":/weathercondition/leizhenyuzhuanduoyun.png");
    mTypeMaps.insert("阵雨转多云",":/weathercondition/leizhenyuzhuanqing.png");
    mTypeMaps.insert("阴转小雨",":/weathercondition/leizhenyuzhuanqing.png");
    mTypeMaps.insert("阴转多云",":/weathercondition/yinzhuanduoyun.png");
    mTypeMaps.insert("小雨转晴",":/weathercondition/xiayu.png");
    mTypeMaps.insert("小到中雨",":/weathercondition/xiayu.png");
    mTypeMaps.insert("多云转阴",":/weathercondition/duoyun.png");
    mTypeMaps.insert("多云转小雨",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("小雨转阴",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("晴转阴",":/weathercondition/yin.png");
    mTypeMaps.insert("阵雨",":/weathercondition/zhenyuzhuanxiaoyu.png");
    mTypeMaps.insert("多云转阵雨",":/weathercondition/zhenyuzhuanxiaoyu.png");
    mTypeMaps.insert("雷阵雨",":/weathercondition/leizhenyuzhuanduoyun.png");
    mTypeMaps.insert("小雨转多云",":/weathercondition/xiaoyu.png");
    mTypeMaps.insert("小到中雨",":/weathercondition/zhongyu.png");

    mTypeMaps.insert("特大暴雨",":/weathercondition/zhongyu.png");




}




void Widget:: setMouseEvent(){
    menuQuit=new QMenu(this);
    menuQuit->setStyleSheet("QMenu::item{color:white}");
    QAction *openAct=new QAction(QIcon(":/weathercondition/close.png"),tr("退出"),this);
    menuQuit->addAction(openAct);
    connect(openAct,&QAction::triggered,[=](){
        this->close();

    });
    QAction *minAct =new QAction(tr("最小化"),this);
    menuQuit->addAction(minAct);

    connect(minAct,&QAction::triggered,[=](){
        this->showMinimized();
    });

}


void Widget::readHttp(QNetworkReply *reply){

    int resCode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(resCode==200){


        QByteArray data=reply->readAll();
        QString str = QString::fromUtf8(data);
        qDebug()<<str;
        parseWeatherJsonData(data);
    }

    else{

        QMessageBox msg;

        msg.setWindowTitle("错误");
        msg.setText("网络请求错误");
        msg.setStyleSheet("QPushButton {color:red}");

        msg.setStandardButtons(QMessageBox::Ok);

        msg.exec();



    }


}

void Widget::parseWeatherJsonData(QByteArray rawdata){

    QJsonDocument rootDoc=QJsonDocument::fromJson(rawdata);

    if(!rootDoc.isNull()&&rootDoc.isObject()){

        QJsonObject objRoot=rootDoc.object();

        days[0].mCity=objRoot["city"].toString();
        days[0].mPm25=objRoot["aqi"].toObject()["pm25"].toString();

        //ToDo 省份需要填写。
        days[0].mProvince=cityUtils.getCityProvinceFromName(days[0].mCity);



        if(objRoot.contains("data") &&objRoot["data"].isArray()){

            QJsonArray weaArray=objRoot["data"].toArray();


            for(int i=0;i<weaArray.size();i++){
                QJsonObject obj=weaArray[i].toObject();

                days[i].mDate=obj["date"].toString();
                days[i].mFx=obj["win"].toArray()[0].toString();
                days[i].mFl=obj["win_speed"].toString();
                days[i].mTempNow=obj["tem"].toString();
                days[i].mTempLow=obj["tem2"].toString();
                days[i].mTempHigh=obj["tem1"].toString();
                days[i].mWeatherType=obj["wea"].toString();
                days[i].mHu=obj["humidity"].toString();
                days[i].mWeek=obj["week"].toString();
                days[i].mAirq=obj["air_level"].toString();
                days[i].mTips=obj["air_tips"].toString();


            }



        }


    }

    updateUI();



}



void Widget:: updateUI(){


    ui->labelcurrentDate->setText(days[0].mDate+"  "+days[0].mWeek);
    ui->labelTemNow->setText(days[0].mTempNow+"℃");
    ui->labelWeatherRange->setText(days[0].mTempLow+"℃ - "+days[0].mTempHigh+"℃");
    ui->labelCity->setText(days[0].mCity);
    if(days[0].mProvince=="北京"||days[0].mProvince=="天津"||days[0].mProvince=="重庆" ||days[0].mProvince=="香港" ||days[0].mProvince=="澳门" )
        ui->labelProvince->setText(days[0].mProvince+"市");
    else
        ui->labelProvince->setText(days[0].mProvince+"省");
    ui->labelWeather->setText(days[0].mWeatherType);
    ui->weatherIcon->setPixmap(mTypeMaps[days[0].mWeatherType]);

    ui->weatherTips->setText(days[0].mTips);
    ui->labelFx->setText(days[0].mFx);
    ui->labelFxValue->setText(days[0].mFl);


    for(int i=0;i<=5;i++){

        QStringList dateTemp=days[i].mDate.split("-");
        days[i].mDate=dateTemp[1]+"-"+dateTemp[2];
        mDateList[i]->setText(days[i].mDate);

        mWeekList[i]->setText(days[i].mWeek);


        mWeaTypeList[i]->setText(days[i].mWeatherType);

        if(days[i].mWeatherType.indexOf("到")!=-1){
                days[i].mWeatherType="中雨";
        }


        QString iconDir=mTypeMaps[days[i].mWeatherType];
        if(iconDir==""){
            int iconTempIndex=days[i].mWeatherType.indexOf("转");
            if(iconTempIndex!=-1){
                iconDir=mTypeMaps[days[i].mWeatherType.left(iconTempIndex)] ;
            }
        }

        mIconList[i]->setPixmap(QPixmap(iconDir));

        mFxList[i]->setText(days[i].mFx);

        int index=   days[i].mFl.indexOf("转");
        if(index!=-1){
            mFlList[i]->setText(days[i].mFl.left(index));
        }

        else{
            mFxList[i]->setText(days[i].mFl);
        }


        mFlList[i]->setText(days[i].mFl);
        mAirqList[i]->setText(days[i].mAirq);
        QString airQ=days[i].mAirq;

        if(airQ=="优"){

            mAirqList[i]->setStyleSheet("color: rgb(238, 238, 238);background-color:rgb(150,213,32);border-radius:7px");

        }


        else if(airQ=="良"){

            mAirqList[i]->setStyleSheet("color:rgb(0,0,0);background-color:rgb(21,224,103);border-radius:7px");
        }

        else if(airQ=="轻度污染"){

            mAirqList[i]->setStyleSheet("color: rgb(0, 0, 0);background-color:rgb(255,199,199);border-radius:7px");
        }

        else if(airQ=="中度污染"){

            mAirqList[i]->setStyleSheet("color:rgb(0,0,0); background-color:rgb(255,17,17);border-radius:7px");
        }

        else if(airQ=="重度污染"){

            mAirqList[i]->setStyleSheet("color:rgb(0,0,0);background-color:rgb(153,0,0);border-radius:7px");
        }

    }

    update();







}


void Widget:: mouseMoveEvent(QMouseEvent *event) {

    //将窗口的左上角移动到新位置，新位置在哪里？ 在当前位置减去偏移值。
    this->move(event->globalPos()-moffset);

}
void Widget:: mousePressEvent(QMouseEvent *event){

    if(event->button()==Qt::RightButton){
        menuQuit->exec(QCursor::pos());
    }

    if(event->button()==Qt::LeftButton){
//        qDebug()<<event->globalPos()<<this->pos();

        //鼠标表当前位置 相对于整个桌面。  //this->pos 是窗口左上角顶点相对于整个桌面的位置。
        moffset=event->globalPos()-this->pos();
    }

}


//获取新城市的信息 使用现有的url+城市的id.所以需要新建一个城市的类。 这个城市的类

void Widget::on_pushButton_clicked()
{
    QString cityName=ui->lineEdit_search->text();
    QString cityId= cityUtils.getCityCodeFromName(cityName);

    if(cityId!=""){


        url+="&cityid="+cityId;
        manager->get(QNetworkRequest(QUrl(url)));


    }
    else{

        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("输入的城市错误，请输入正确的城市名称");
        msgBox.setStyleSheet("QPushButton {color:red}");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();




    }

}


bool Widget::eventFilter(QObject *watched, QEvent *event)
{
        if(watched==ui->widgetWendu1&&event->type()==QEvent::Paint){

            drawTemLineHeight();
            return true;
        }

        if(watched==ui->widgetWendu2&&event->type()==QEvent::Paint){
            drawTemLineLow();
            return true;
        }

        return QWidget::eventFilter(watched,event);



}

void Widget::drawTemLineHeight(){

    QPainter painter(ui->widgetWendu1);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::yellow);
    QPoint points[6];
    int offSet=0;
    int avg;
    int sum=0;
    int mid=ui->widgetWendu1->height()/2;
    for(int i=0;i<6;i++){
        sum+=days[i].mTempHigh.toInt();
    }
    avg=sum/6;


    for(int i=0;i<6;i++){


        points[i].setX(mAirqList[i]->x()+mAirqList[i]->width()/2);
//        qDebug()<<mAirqList[i]->x();
        offSet=(days[i].mTempHigh.toInt()-avg)*3;
        points[i].setY(mid-offSet);
//        qDebug()<<points[i].x();

        painter.drawEllipse(QPoint(points[i]),3,3);

        //绘制温度
        painter.drawText(points[i].x()-12,points[i].y()-10,days[i].mTempHigh+"℃");


    }
    for(int i=0;i<5;i++){

        painter.drawLine(points[i],points[i+1]);

    }


}

void Widget::drawTemLineLow(){

    QPainter painter(ui->widgetWendu2);

    painter.setPen(Qt::gray);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0,0,0));
    QPoint points[6];
    int offSet=0;
    int avg=0;
    int sum=0;
    int mid=ui->widgetWendu2->height()/2;
    for(int i=0;i<6;i++){
        sum+=days[i].mTempLow.toInt();
    }

    avg=sum/6;

    for(int i=0;i<6;i++){


        points[i].setX(mAirqList[i]->x()+mAirqList[i]->width()/2);
        offSet=(days[i].mTempLow.toInt()-avg)*3;
        points[i].setY(mid-offSet);

        painter.drawEllipse(QPoint(points[i]),3,3);

        //绘制温度
        painter.drawText(points[i].x()-12,points[i].y()-10,days[i].mTempLow+"℃");


    }
    for(int i=0;i<5;i++){

        painter.drawLine(points[i],points[i+1]);

    }


}


void Widget::on_lineEdit_search_returnPressed()
{
    on_pushButton_clicked();
}
