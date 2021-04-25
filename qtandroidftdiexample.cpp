#include "qtandroidftdiexample.h"
#include "ui_qtandroidftdiexample.h"
#include <QtAndroid>
#include <QSettings>
qtandroidftdiexample::qtandroidftdiexample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::qtandroidftdiexample)
{
    ui->setupUi(this);
    qDebug()<<"houyawei_qtandroidftdiexample"<<endl;
    this->adnrActivity = QtAndroid::androidActivity();

    startJar();
}

qtandroidftdiexample::~qtandroidftdiexample()
{
    delete ui;
}
void qtandroidftdiexample::startJar()
{
    qDebug()<<"houyawei_startJar()"<<endl;
    qtFtDev2xxManager =  QAndroidJniObject(QByteArray("org/qtproject/qt5/QtFtDev").constData());

//    qDebug() << "context is valid = " <<adnrActivity.isValid() << adnrActivity.object() <<  qtFtDev2xxManager.isValid() << qtFtDev2xxManager.object() << QAndroidJniObject::isClassAvailable(QByteArray("org/qtproject/qt5/QtFtDev").constData());

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "houyawei_1 env =================================================== ";
        env->ExceptionClear();
    }
    qDebug() << "houyawei_getInstance = " << qtFtDev2xxManager.callMethod<jint>(QByteArray("getInstance").constData(), QByteArray("(Landroid/content/Context;)I").constData(), adnrActivity.object());
//                                                                           QByteArray("(I)Ljava/lang/Object;").constData(), adnrActivity.object());

    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "houyawei_2 env =================================================== ";
        env->ExceptionClear();
    }

//    QTimer::singleShot(200, this, SLOT(createDeviceInfoList()));
    loadSettings(0);

//    historyModel->clearAllData();
//    for(int i = 0, iMax = listHistoryCommand.size(); i < iMax; i++)
//        historyModel->addCustomDevice(QList<QVariant>() << listHistoryCommand.at(i));

}
void qtandroidftdiexample::createDeviceInfoList(bool onlyUpdate)
{
qDebug()<<"houyawei_createDeviceInfoList(bool onlyUpdate)"<<endl;

    jint devCount = qtFtDev2xxManager.callMethod<jint>(QByteArray("createDeviceList").constData());
    qDebug() << devCount;


    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    for(int i = 0; i < devCount; i++)
        ui->devicename->setText(QString("find useful ttyusb num:%1").arg(i));

    if(devCount > 0){
         ui->devicename->setText(QString("ttyUSB%1").arg(devCount - 1));
        emit setCurrntPortIndx(devCount - 1);
    }else
        emit setCurrntPortIndx(-1);
//    if(devCount > 0 && !onlyUpdate)
//         connectToOnePort();
}
void qtandroidftdiexample::connectToOnePort()
{

    qDebug()<<"houyawei_connectToOnePort()"<<endl;
    QList<QVariant> list = loadSettings(1);
    while(list.size() < 4)
        list.append(0);

    if(list.size() < 5)
        list.append(3);

    connectToOnePort(0, list.at(4).toInt(), list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt(), list.at(3).toInt());
}
void qtandroidftdiexample::connectToOnePort(int indx, int baudIndx, int dataIndx, int stopIndx, int parityIndx, int flowIndx)
{

    QList<QVariant> listVar;
    listVar.append(dataIndx);
    listVar.append(stopIndx);
    listVar.append(parityIndx);
    listVar.append(flowIndx);
    listVar.append(baudIndx);


    jint baudRate ;

    switch (baudIndx) {
    case 0: baudRate = 1200; break;
    case 1: baudRate = 2400; break;
    case 2: baudRate = 4800; break;
    case 3: baudRate = 9600; break;
    case 4: baudRate = 19200; break;
    case 5: baudRate = 38400; break;
    case 6: baudRate = 57600; break;
    case 7: baudRate = 115200; break;

    default: baudRate = 9600; break;
    }

    jint devIndx = indx;
    jint devDataIndx = dataIndx;
    jint devStopIndx = stopIndx ;
    jint devParityIndx = parityIndx;
    jint devFlowIndx = flowIndx;

    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("connToOneDevice").constData(),
                                                     QByteArray("(IIIIII)I").constData(), devIndx,
                                                     baudRate,
                                                     devDataIndx,
                                                     devStopIndx,
                                                     devParityIndx,
                                                     devFlowIndx  );

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    QString openStts("");
    switch (retVal) {
    case 0: openStts = tr("Opened ttyUSB%1!").arg(indx) ;  emit onFtdiStateChanged(true); emit startReadTmr(); break;
    case 1: openStts = tr("Already opened ttyUSB%1!").arg(indx);  emit onFtdiStateChanged(true); emit startReadTmr(); break;
    case 2: openStts = tr("Device not valid ttyUSB%1!").arg(indx); break;
    case 3: openStts = tr("Can't open ttyUSB%1!").arg(indx); break;

    default: openStts = "unkonown error";   break;
    }


    ui->devicename->setText(openStts);
//    emit addLineToLog(openStts );
}
QList<QVariant> qtandroidftdiexample::loadSettings(int key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");
    QList<QVariant> list;
    switch(key){
    case 0: { //load all settings
        settings.beginGroup("uart");
        emit onConnectPageSettt(settings.value("dataBits", 0).toInt(),
                                settings.value("stopBits", 0).toInt(),
                                settings.value("parity", 0).toInt(),
                                settings.value("flowCtrl", 0).toInt(),
                                settings.value("baudRate", 3).toInt() );

        settings.endGroup();

        settings.beginGroup("terminal");
        emit onTerminalPageSettt(settings.value("endSymb", 0).toInt(), settings.value("hexOut", false).toBool());
        settings.endGroup();

        settings.beginGroup("sett");
        emit onSettPage(settings.value("fontSize", 0).toReal());
        settings.endGroup();

        settings.beginGroup("app");
        listHistoryCommand = settings.value("listHistoryCommand").toStringList();
        if(listHistoryCommand.isEmpty())
            listHistoryCommand = QString("+++ ATAD ATBD ATCH ATID ATNI ATSL ATSH ATVR ATHV ATAP ATSM ATND ATBD").split(' ');
        settings.endGroup();

        return list; }

    case 1:{
        settings.beginGroup("uart");
        list.append(settings.value("dataBits", 0));
        list.append(settings.value("stopBits", 0));
        list.append(settings.value("parity", 0));
        list.append(settings.value("flowCtrl", 0));
        list.append(settings.value("baudRate", 3));

        settings.endGroup();
        return list;}
    }
    return list;
}
void qtandroidftdiexample::sendDataToPort(QString line, bool hexOut, int endLineIndx)
{
    QString tmpStr(line);
    emit startReadTmr();

    if(hexOut){
        switch(endLineIndx){
        case 0:  line.append("0D0A"); break;
        case 1:  line.append("0D"); break;
        case 2:  line.append("0A"); break;
        default: break;
        }
    }else{

        switch(endLineIndx){
        case 0:  line.append("\r\n"); break;
        case 1:  line.append("\r"); break;
        case 2:  line.append("\n"); break;
        default: break;
        }
    }


    QAndroidJniEnvironment env;
    if(!hexOut)
        line = line.toLocal8Bit().toHex();

    if((line.size() % 2))
        line.prepend("0");

    QAndroidJniObject string2 = QAndroidJniObject::fromString(line);

    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("sendData").constData(),
                                                     QByteArray("(Ljava/lang/String;)I").constData(),
                                                     string2.object<jstring>()/*, arr*/);
    if (env->ExceptionCheck()) {
        // Handle exception here.
        emit addLineToLog("write exeption");
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

//    if(!listHistoryCommand.contains(tmpStr.simplified().trimmed(), Qt::CaseInsensitive)){
//        listHistoryCommand.prepend(tmpStr.simplified().trimmed());
//        historyModel->prependCustomDevice(QList<QVariant>() << listHistoryCommand.first());
//        saveSettings(QList<QVariant>(), 4); //save list last command
//    }

    if(retVal >= 0){
        emit addLineToLog(tmpStr);
        ui->devicename->setText(tmpStr);
    }else{
        emit addLineToLog("line error write\r\n" + QString::number(retVal));
//        disconnectFromPort();
    }
}

bool qtandroidftdiexample::readData()
{
    jint bytesAvailable = qtFtDev2xxManager.callMethod<jint>(QByteArray("bytesAvailable").constData());
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        emit addLineToLog("read errror");

        env->ExceptionClear();
        return false;
    }

    if(bytesAvailable < 0){
        return false;
    }

    if(bytesAvailable < 1)
        return true;

    QAndroidJniObject stringArray = qtFtDev2xxManager.callObjectMethod<jstring>(QByteArray("readData").constData());
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        emit addLineToLog("read errror");

        env->ExceptionClear();
//        disconnectFromPort();
        return false;
    }
    QString readStr = stringArray.toString();

    if(readStr.isEmpty())
        return true;

    if((readStr.length()%2))
        readStr.prepend("0");

    readStr = QByteArray::fromHex(readStr.toLocal8Bit());


    if(!readStr.isEmpty())
        emit addLineToLog(readStr);
    ui->devicename->setText(readStr);

    return true;
}
void qtandroidftdiexample::on_refreshButton_clicked()
{
    createDeviceInfoList(true);
}


void qtandroidftdiexample::on_openButton_clicked()
{
//    connectToOnePort();
     connectToOnePort(0,7,0,0,0,0);
}


void qtandroidftdiexample::on_sendButton_clicked()
{
   QString text = ui->sendlineEdit->text();
    sendDataToPort(text,false,1);
//    serialWrite->write(line.toUtf8().constData());
//    serialWrite->write("\n");

}


void qtandroidftdiexample::on_receive_clicked()
{
    readData();
//    QByteArray dataSerial_temp =serialRead->readAll();
//    QByteArray dataSerial = dataSerial_temp.toHex();
}

