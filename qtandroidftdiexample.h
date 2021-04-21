#ifndef QTANDROIDFTDIEXAMPLE_H
#define QTANDROIDFTDIEXAMPLE_H

#include <QMainWindow>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtDebug>
#include <QAndroidJniObject>
QT_BEGIN_NAMESPACE
namespace Ui { class qtandroidftdiexample; }
QT_END_NAMESPACE

class qtandroidftdiexample : public QMainWindow
{
    Q_OBJECT

public:
    qtandroidftdiexample(QWidget *parent = nullptr);
    ~qtandroidftdiexample();

private:
    Ui::qtandroidftdiexample *ui;
    QAndroidJniObject qtFtDev2xxManager;
    QList<QVariant> loadSettings(int key);
    QStringList listHistoryCommand;


    QAndroidJniObject adnrActivity;
    bool readData();
signals:
    void setCurrntPortIndx(int);
    void onFtdiStateChanged(bool);
    void startReadTmr();
    void addLineToLog(QString);
    void onConnectPageSettt(int, int,int ,int,int);
    void onTerminalPageSettt(int,bool);
    void onSettPage(qreal);

public slots:
    void createDeviceInfoList(bool onlyUpdate = false);
    void connectToOnePort();
    void connectToOnePort(int indx , int baudIndx, int dataIndx, int stopIndx, int parityIndx, int flowIndx);

    void startJar();
private slots:
    void on_refreshButton_clicked();
    void on_openButton_clicked();
    void on_sendButton_clicked();
    void sendDataToPort(QString line, bool hexOut, int endLineIndx);
};
#endif // QTANDROIDFTDIEXAMPLE_H
