#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timer = new QTimer;
    timeOfTimer = new QTimer;
    Hi = false;
    start = false;

   speedStepper = 0;


   labelTemperature = new QLabel("Temperature:");
   labelTemperature->setMaximumHeight(40);

   labelPressure = new QLabel("Pressure:");
   labelPressure->setMaximumHeight(40);

   QLabel* termostatLabel = new QLabel("Termostat");
   termostatLineEdit = new QLineEdit;
   termostatLineEdit->setMaximumWidth(60);
   okTermostatButton = new QPushButton("OK");

   QLabel* presureControllerLabel = new QLabel("Pressure controller");
   pressureControllerLineEdit = new QLineEdit;
   pressureControllerLineEdit->setMaximumWidth(60);
   okPressureControllerButton = new QPushButton("OK");

   timeLabel = new QLabel;
   IPLabel = new QLabel;
   time = new QTime(0,0);


   timeLabel->setText(time->toString("hh:mm:ss"));


   startButton = new QPushButton;
   startButton->setIcon(QIcon(":/start.png"));
   startButton->setMaximumSize(40,40);
   startButton->setMinimumSize(40,40);
   startButton->setIconSize(QSize(startButton->width()-5, startButton->height()-5));


   stopButton = new QPushButton;
   stopButton->setIcon(QIcon(":/stop.png"));
   stopButton->setMaximumSize(40,40);
   stopButton->setMinimumSize(40,40);
   stopButton->setIconSize(QSize(stopButton->width()-5, stopButton->height()-5));


   QLabel* syringeLabel = new QLabel("Choose a syringe:");
   syringeComboBox = new QComboBox;
   syringeComboBox->addItem("Syringe 20 ml");
   syringeComboBox->addItem("Peristaltic");

   QLabel* speed1Label = new QLabel("Flow speed A:");
   QLabel* speed1mklLabel = new QLabel("mkl/min");
   speed1LineEdit = new QLineEdit;
   speed1LineEdit->setValidator(new QIntValidator(0,6000));

   QLabel* speed2Label = new QLabel("Flow speed two:");
   QLabel* speed2mklLabel = new QLabel("mkl/min");
   speed2LineEdit = new QLineEdit;
   speed2LineEdit->setValidator(new QIntValidator(0,6000));

   QLabel* speed3Label = new QLabel("Flow speed B:");
   QLabel* speed3mklLabel = new QLabel("mkl/min");
   speed3LineEdit = new QLineEdit;
   speed3LineEdit->setValidator(new QIntValidator(0,6000));

   QLabel* connectionStsteLabel = new QLabel("Connection state:");
   stateLabel = new QLabel;
   stateLabel->setPixmap(QPixmap(":/linkdown.png"));


   QVBoxLayout* v = new QVBoxLayout;
   QHBoxLayout* h1 = new QHBoxLayout;
   QHBoxLayout* h2 = new QHBoxLayout;
   QHBoxLayout* h3 = new QHBoxLayout;
   QHBoxLayout* h4 = new QHBoxLayout;
   QHBoxLayout* h5 = new QHBoxLayout;
   QHBoxLayout* h6 = new QHBoxLayout;
   QHBoxLayout* h7 = new QHBoxLayout;
   QHBoxLayout* h8 = new QHBoxLayout;
   QHBoxLayout* h9 = new QHBoxLayout;

   h1->addWidget(timeLabel);
   h1->addWidget(startButton);
   h1->addWidget(stopButton);
   h2->addWidget(syringeLabel);
   h2->addWidget(syringeComboBox);
   h3->addWidget(speed1Label);
   h3->addWidget(speed1LineEdit);
   h3->addWidget(speed1mklLabel);
   h4->addWidget(speed2Label);
   h4->addWidget(speed2LineEdit);
   h4->addWidget(speed2mklLabel);
   h5->addWidget(speed3Label);
   h5->addWidget(speed3LineEdit);
   h5->addWidget(speed3mklLabel);
   h6->addWidget(termostatLabel);
   h6->addWidget(termostatLineEdit);
   h6->addWidget(okTermostatButton);
   h7->addWidget(presureControllerLabel);
   h7->addWidget(pressureControllerLineEdit);
   h7->addWidget(okPressureControllerButton);
   h8->addWidget(labelTemperature);
   h8->addWidget(labelPressure);
   h9->addWidget(connectionStsteLabel);
   h9->addWidget(stateLabel);
   h9->addWidget(IPLabel);


   v->addLayout(h1);
   v->addLayout(h2);
   v->addLayout(h3);
   //v->addLayout(h4);
   v->addLayout(h5);
   v->addLayout(h6);
   v->addLayout(h7);
   v->addLayout(h8);
   v->addLayout(h9);


   QWidget* w = new QWidget;
   w->setLayout(v);
   this->setCentralWidget(w);

   m_nNextBlockSize = 0;
   m_pTcpSocket = new QTcpSocket(this);

   QString localIP;
   QList<QNetworkInterface> list =QNetworkInterface::allInterfaces();
//   for(int i = 0; i<list.at(0).allAddresses().size();i++)
//   {
//       qDebug() << list.at(0).allAddresses().at(i).toString();
//       localIP = list.at(0).allAddresses().at(i).toString();
//       QStringList stringList = localIP.split(QRegExp("\\."));
//       qDebug() << stringList.at(0);
//       if(QString(stringList.at(0)).toInt() == 192) break;
//   }

       oktetIP = localIP.split(QRegExp("\\."));
       IPcounter =1;

       connect(timer,SIGNAL(timeout()),this, SLOT(up()));
       connect(okTermostatButton, &QPushButton::clicked,this,&MainWindow::okTermostatButtonSlot);
       connect(okPressureControllerButton,&QPushButton::clicked,this,&MainWindow::setPresure);
       connect(startButton, &QPushButton::clicked,this,&MainWindow::startSlot);
       connect(stopButton, &QPushButton::clicked,this,&MainWindow::stopSlot);
       connect(timeOfTimer,SIGNAL(timeout()),this, SLOT(timeOfTimerSlot()));
       timerSoket = new QTimer;
       upTime = new QTimer;

       connect(timerSoket,SIGNAL(timeout()),this, SLOT(slotTimerSoket()));
       timerSoket->start(5000);

       connect(upTime,SIGNAL(timeout()),this, SLOT(upTimeSlot()));
               m_pTcpSocket = new QTcpSocket(this);
               connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
               connect(m_pTcpSocket, SIGNAL(disconnected()), SLOT(slotDisonnected()));
               connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
               m_pTcpSocket->connectToHost(QString("192.168.0.177"), 23);
               SendToServer("Hi, , ");
}

void MainWindow::slotTimerSoket()
{
    if(!Hi)
    {
    delete m_pTcpSocket;
        m_pTcpSocket = new QTcpSocket(this);
        connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
        connect(m_pTcpSocket, SIGNAL(disconnected()), SLOT(slotDisonnected()));
        connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
//    IPcounter++;
//    if(IPcounter>256) IPcounter=1;
//    m_pTcpSocket->connectToHost(QString("%1.%2.%3.%4").arg(oktetIP.at(0)).arg(oktetIP.at(1)).arg(oktetIP.at(2)).arg(IPcounter), 23);
//    SendToServer("Hi, , ");
//    qDebug() << QString("%1.%2.%3.%4").arg(oktetIP.at(0)).arg(oktetIP.at(1)).arg(oktetIP.at(2)).arg(IPcounter);
//    IPLabel->setText(QString("%1.%2.%3.%4").arg(oktetIP.at(0)).arg(oktetIP.at(1)).arg(oktetIP.at(2)).arg(IPcounter));
            m_pTcpSocket->connectToHost(QString("192.168.0.177"), 23);
            SendToServer("Hi, , ");
    } else timerSoket->stop();
}
void MainWindow::startSlot()
{
    if(!start)
    {

    if(Hi)
     {
        if(speed1LineEdit->text()!="" && speed3LineEdit->text()!="")
        {
        timeOfTimer->start(1000);
        start = true;
        qDebug() << "test";

        float s1 = speed1LineEdit->text().toInt();
        float s2 = speed2LineEdit->text().toInt();
        float s3 = speed3LineEdit->text().toInt();
        //serialStack.push(QString("Stepper,speed,%1").arg(100/(s1 + s2)));

        if(syringeComboBox->currentIndex()==1)
        {
          if(s1==0)  serialStack.push(QString("Stepper,ratio1,%1").arg(0));
         else serialStack.push(QString("Stepper,ratio1,%1").arg(312/s1*0.2646*1000));
        if(s2==0) serialStack.push(QString("Stepper,ratio2,%1").arg(0));
        else serialStack.push(QString("Stepper,ratio2,%1").arg(312/s2*0.2733*1000));
        }
        if(syringeComboBox->currentIndex()==0)
        {
          if(s1==0)  serialStack.push(QString("Stepper,ratio1,%1").arg(0));
         else serialStack.push(QString("Stepper,ratio1,%1").arg(s1));
        if(s2==0) serialStack.push(QString("Stepper,ratio2,%1").arg(0));
        else serialStack.push(QString("Stepper,ratio2,%1").arg(s2));
        if(s3==0) serialStack.push(QString("Stepper,ratio3,%1").arg(0));
        else serialStack.push(QString("Stepper,ratio3,%1").arg(s3));
        }
        SendToServer(QString(serialStack.pop()));
        } else QMessageBox::information(this,"Empty fields","Specify the speed of the first and second flow, the fields should not remain empty.");
     } else QMessageBox::information(this,"Not connection","There is no connection to the equipment on the local network, check the connection.");
    }
     else
      {
        start = false;
        timeOfTimer->stop();
        serialStack.push(QString("Stepper,speed,0"));
        serialStack.push(QString("Stepper,ratio1,%1").arg(0));
        serialStack.push(QString("Stepper,ratio2,%1").arg(0));
        serialStack.push(QString("Stepper,ratio3,%1").arg(0));
        SendToServer(QString(serialStack.pop()));
      }

}

void MainWindow::stopSlot()
{
    if(Hi)
    {
        start = false;
        timeOfTimer->stop();
        delete time;
        time = new QTime(0,0,0);
        timeLabel->setText(time->toString());
        serialStack.push(QString("Stepper,ratio1,%1").arg(0));
        serialStack.push(QString("Stepper,ratio2,%1").arg(0));
        serialStack.push(QString("Stepper,ratio3,%1").arg(0));
        SendToServer(QString(serialStack.pop()));
    }

}

void MainWindow::upTimeSlot()
{
    upTime->stop();
        timerSoket->start(100);
        Hi = false;
}

void MainWindow::timeOfTimerSlot()
{
    *time = time->addSecs(1);
    timeLabel->setText(time->toString("hh:mm:ss"));
    qDebug() << time->toString();
//    QTime *tempTimer = new QTime(0,1,0);
//    if(tempTimer->toString()==time->toString())
//    {
//        stopSlot();
//    }
}

void MainWindow::slotConnected()
{
    qDebug() << "Connected";
    stateLabel->setPixmap(QPixmap(":/link.png"));
}

void MainWindow::slotDisonnected()
{
    stateLabel->setPixmap(QPixmap(":/linkdown.png"));
//    m_pTcpSocket->connectToHost(QString("192.168.0.177"), 23);
//    SendToServer("Hi, , ");
}

void MainWindow::slotReadyRead()
{
    QByteArray arr =m_pTcpSocket->readAll();
{
            QString str(arr);


            QStringList list;
            qDebug() << str;
            list = QString(str).split(QRegExp("\\,"));
            if (!list.isEmpty() && list.at(0) == "ok")
            {
                if(list.at(1) == "Hi")
                {
                   Hi = true;
                }
                if(list.at(1) == "Thermometer")
                {
                   temper = QString(list.at(2)).toDouble();
                   labelTemperature->setText("Temperature:   " + QString(list.at(2)) + " C");
                   setTermostat();
                }
                if(list.at(1) == "Pressure")
                {
                   labelPressure->setText("   Pressure:   " + QString(list.at(2)) + "KPa");
                }
                if(list.at(1) == "Heating")
                {
                    if(list.at(2) == "heat")
                    {
                        termostat = true;
                    }
                    if(list.at(2) == "cold")
                    {
                        termostat = false;
                    }
                }
                if(list.at(1) == "Stepper" )
                {

                }

                if(list.at(1) == "Ratio1" )
                {

                }
                if(list.at(1) == "Ratio2" )
                {

                }
                if(list.at(1) == "Ratio3" )
                {

                }
                emptySerisl = true;
                upTime->stop();
            }

            list.clear();
        }
        if (!serialStack.isEmpty())
        {
            if(emptySerisl)
            {
            SendToServer(QString(serialStack.pop()));
            emptySerisl = false;
            }
        }

}

void MainWindow::SendToServer(QString str)
{
        QByteArray  arrBlock;

        m_pTcpSocket->write(arrBlock.append(str));
        upTime->start(1000);
       // qDebug() << "sent";
}

void MainWindow::setTermostat()
{
    if(temper<t && !termostat) serialStack.push(QString("Heating,heat, "));
    if(temper>=t && termostat) serialStack.push(QString("Heating,cold, "));
}

void MainWindow::setPresure()
{
    serialStack.push(QString("Pressure,position," + pressureControllerLineEdit->text()));
}

void MainWindow::okTermostatButtonSlot()
{
    t = termostatLineEdit->text().toDouble();
    timer->start(2000);
}
void MainWindow::up()
{
    serialStack.push(QString("Thermometer,Measure, "));
    serialStack.push(QString("Pressure,Measure, "));
    if(emptySerisl)
    {
    SendToServer(QString(serialStack.pop()));
    emptySerisl = false;
    }
}

void MainWindow::changeSlider(int value)
{
    serialStack.push(QString("Stepper,ratio,%1").arg(value));
    if(emptySerisl)
    {
    SendToServer(QString(serialStack.pop()));
    emptySerisl = false;
    }
}


MainWindow::~MainWindow()
{

}
