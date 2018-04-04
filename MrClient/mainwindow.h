#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QtGlobal>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QStack>
#include <QTimer>
#include <QSlider>
#include <QMainWindow>
#include <QTcpSocket>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QComboBox>
#include <QMessageBox>
#include <QTime>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timeOfTimerSlot();
    void startSlot();
    void stopSlot();
    void okTermostatButtonSlot();
    void changeSlider(int value);
    void up();
    void slotTimerSoket();
    void upTimeSlot();
    void setPresure();
    void slotReadyRead   ();
    //void slotError       (QAbstractSocket::SocketError);
    void slotConnected   ();
    void slotDisonnected();

private:

    void SendToServer(QString str);
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;
    QStringList oktetIP;
    int IPcounter;

    void setTermostat();

    QStack<QString> serialStack;
    QTimer *timer;
    QTimer *timerSoket;
    QTimer *upTime;

    QTime *time;
    QTimer *timeOfTimer;
    bool termostat;
    double temper;
    double t;
    bool emptySerisl;
    bool Hi;
    bool start;

    int speedStepper;

    QLabel* labelTemperature;
    QLabel* labelPressure;
    QLineEdit *termostatLineEdit;
    QLineEdit *pressureControllerLineEdit;
    QPushButton *okTermostatButton;
    QPushButton *okPressureControllerButton;
    QLabel* timeLabel;
    QLabel* stateLabel;
    QLabel* IPLabel;
    QLineEdit *speed1LineEdit;
    QLineEdit *speed2LineEdit;
    QLineEdit *speed3LineEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QComboBox *syringeComboBox;

};

#endif // MAINWINDOW_H
