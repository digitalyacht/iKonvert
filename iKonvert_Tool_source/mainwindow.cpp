#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QtWidgets>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QTextOption>
#include <QTextEdit>
#include <QThread>
#include <QStringRef>
#include "countworker.h"
#include "infinitecountworker.h"
#include "portablesleep.h"

const char* stringtx;

char* tx;

QStringList lines;
QStringList lines_recorded;

bool record = false;
bool normal_mode = true;
bool base64 = true;
bool showNSM = false;
bool initialised = false;

int rep = 100;
int line_count = 0;
int line_count_all = 0;

QString CSV_file = "test.csv";
QString all_mode = "$PDGY,N2NET_INIT,ALL,\r\n";
QString status_bar = "";
QByteArray RXTable;
QByteArray TXTable;

QDate dateSince(1970, 01, 01);
QTime timeSince(0, 0, 0, 0);

static void process_line(const QByteArray &)
{
}

//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_console(new Console),
    m_settings(new SettingsDialog),
    //loaded(new Loaded_pgns),
//! [1]
    m_serial(new QSerialPort(this)),

    infiniteCountRunning(false)

//! [1]
{
//! [0]
    m_ui->setupUi(this);

    m_console->setEnabled(true);

    QFont font("Monospace");
    font.setStyleHint(QFont::Monospace);
    m_console->setFont(font);

    setCentralWidget(m_console);

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionQuit->setEnabled(true);
    m_ui->actionConfigure->setEnabled(true);
    m_ui->actionRun->setEnabled(false);
    m_ui->actionSend->setEnabled(false);
    m_ui->actionSend_126992->setEnabled(false);
    m_ui->actionStop->setEnabled(false);
    m_ui->actionStop_Record->setEnabled(false);
    m_ui->actionRec->setEnabled(false);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnections();

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

//! [2]
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
//! [2]
    connect(m_console, &Console::getData, this, &MainWindow::writeData);

    openSerialPort();

    //QTimer *timer = new QTimer(this);
    //QObject::connect(timer, SIGNAL(timeout()), this, SLOT(send_126992()));
    //timer->start(1000); //time specified in ms

//! [3]
}
//! [3]

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete m_settings;
    delete m_ui;

}

SettingsDialog::Settings SettingsDialog::settings()
{
    return m_currentSettings;
}

//! [4]
void MainWindow::openSerialPort()
{

    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(230400);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);

    if (m_serial->open(QIODevice::ReadWrite)) {
        m_console->setEnabled(true);
        m_console->setWordWrapMode(QTextOption::WrapAnywhere);
        m_console->setLocalEchoEnabled(p.localEchoEnabled);
        //m_console->setTextInteractionFlags(Qt::TextSelectableByMouse);
        m_ui->actionConnect->setEnabled(true);
        m_ui->actionDisconnect->setEnabled(false);
        m_ui->actionConfigure->setEnabled(true);

        QString temp_put = QString("Connected to %1 : 230400\r\n").arg(p.name);

        m_console->putData(temp_put.toUtf8()); // show COM port number in terminal window

    } else {

        QMessageBox::critical(this, tr("Error opening serial port"), m_serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

//! [4]
void MainWindow::process_line(QByteArray buf)
{

      if(buf[0]=='$'){

          if(buf.mid(0, 13) == "$PDGY,000000,"){ // Network Status Sentence received

              if(showNSM) m_console->putData(buf); //only show Network Status Message in terminal if Show N.S.M. ticked

                          QString status = QString::fromUtf8(buf);
                          QStringList status_list = status.split(","); // split by comma

                          showStatusMessage(tr("Bus Load: %1, Devices Online: %2, Uptime: %3 sec, Address: %4, Rejected TX PGN`s: %5")
                                            .arg(status_list[2]).arg(status_list[4]).arg(status_list[5]).arg(status_list[6]).arg(status_list[7])); // show in status bar

            }else{

              m_console->putData(buf); // write out a data line from serial to the console

              if(buf.mid(0, 10) == "$PDGY,ACK,"){

                if(buf[10]=='R'&&buf[11]=='X'){ //RX PGN lists sent to the gateway succesfully

                    send_tx_list();

                }else if(buf[10]=='T'&&buf[11]=='X'){ // TX PGN lists sent to the gateway succesfully

                    m_console->putData("RX and TX Lists successfully saved to the Gateway\r\n");

                }else if(buf[16]=='I'&&buf[17]=='N'){ //init ack received

                    m_ui->actionRec->setEnabled(true);
                    m_ui->actionRun->setEnabled(true);
                    m_ui->actionSend->setEnabled(true);
                    m_ui->actionSend_126992->setEnabled(true);

                    initialised = true;

                    m_console->putData("iKonvert Gateway initialised\r\n");

                }

            }else if(buf[11]=='X'&&buf[12]=='T'){

                if(buf[10]=='R'){ // RX table arrived

                    RXTable = buf;

                }else if(buf[10]=='T'){ // TX table arrived

                    TXTable = buf;

                    call_readReply(RXTable, TXTable);

                }

              }

            }


            }else if(buf[0] = '!'){ // data message received, start with !

                QString status2 = QString::fromUtf8(buf);
                QStringList status2_list = status2.split(","); // split by comma

                if(status2_list.count() > 6){ // more then 6 commas in the sentence

                    QByteArray bazeg3 = QByteArray::fromBase64(status2_list[6].toUtf8()); //convert data to hex

                    QString encoded = QString(status2_list[0]+","
                            + status2_list[1]+","
                            + status2_list[2]+","
                            + status2_list[3]+","
                            + status2_list[4]+","
                            + status2_list[5]+","
                            + bazeg3.toHex() + "\r\n"); // assemble the sentence again, with hex data

                    if(!base64){ // if base64 mode disabled

                        m_console->putData(encoded.toUtf8()); // write out decoded line from serial to the console

                    }else{

                        m_console->putData(buf); // write out a data line from serial to the console

                    }

                    if(record){

                       lines_recorded.append(encoded);

                    }

                }

             }
}

void MainWindow::table_query()
{
    m_console->putData("Reading RX and TX Lists from the Gateway\r\n");
    m_console->putData("$PDGY,SHOW_LISTS,\r\n");
    m_serial->write("$PDGY,SHOW_LISTS,\r\n");

}

void MainWindow::reset()
{
    m_console->putData("Sending Factory Reset command to the Gateway\r\n");
    m_console->putData("$PDGY,N2NET_RESET,\r\n");
    m_serial->write("$PDGY,N2NET_RESET,\r\n");

}

void MainWindow::closeSerialPort()
{
    while(!lines.isEmpty())
    lines.clear();

    line_count_all = 0;    

    if (m_serial->isOpen()) m_serial->close();

}

//! [5]
void MainWindow::init_Gateway(){

    if (m_serial->isOpen()) m_serial->close();

    openSerialPort();

    const SettingsDialog::Settings p = m_settings->settings();

    rep = p.repeat;
    CSV_file = p.CSV;
    normal_mode = p.NormalModeEnabled;
    base64 = p.Base64Enabled;
    showNSM = p.ShowNSMEnabled;

        m_console->setEnabled(true);
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);
        //showStatusMessage(tr("Connected to %1 : 230400, PGN TX Interval(ms): %2, TX Filename: %3")
                         // .arg(p.name).arg(rep).arg(CSV_file));

        QFile inputFile(CSV_file);
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            QTextStream textStream(&inputFile);
            while (!textStream.atEnd())
            {
                QString line = textStream.readLine();
                if (line.isNull())
                    break;
                else{
                    if(line.length() > 24){

                        lines.append(line);
                        line_count_all++;

                        }
                    }
            }

            inputFile.close();

        }else{

           QMessageBox::critical(this, "Error", "Unable to open CSV file.");

        }

        if(normal_mode){

            send_init_command();

        }else{

            send_init_command_allmode();

        }


}

void MainWindow::de_init_Gateway(){ // de-initialise

    initialised = false;

    m_serial->write("$PDGY,N2NET_OFFLINE,\r\n"); //restart the gateway to go offline

    m_serial->waitForBytesWritten(500);

    while(!lines.isEmpty())
    lines.clear();

    line_count_all = 0;

    m_console->putData("De-init: iKonvert Gateway is not on the NMEA2000 bus\r\n");
    showStatusMessage(tr("Disconnected"));

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionConfigure->setEnabled(true);
    m_ui->actionRun->setEnabled(false);
    m_ui->actionStop->setEnabled(false);
    m_ui->actionRec->setEnabled(false);
    m_ui->actionSend->setEnabled(false);
    m_ui->actionSend_126992->setEnabled(false);
    m_ui->actionStop_Record->setEnabled(false);

}

void MainWindow::send_rx_list()
{
    const SettingsDialog::Settings p = m_settings->settings();

    m_console->putData("Sending RX PGN list to the iKonvert Gateway\r\n");

    QString rx_list_for_sending = QString("$PDGY,RX_LIST,");

    if(p.rx1en)rx_list_for_sending = QString(rx_list_for_sending)+("59392,");
    if(p.rx8en)rx_list_for_sending = QString(rx_list_for_sending)+("126464,");
    if(p.rx9en)rx_list_for_sending = QString(rx_list_for_sending)+("126983,");
    if(p.rx10en)rx_list_for_sending = QString(rx_list_for_sending)+("126984,");
    if(p.rx11en)rx_list_for_sending = QString(rx_list_for_sending)+("126985,");
    if(p.rx12en)rx_list_for_sending = QString(rx_list_for_sending)+("126986,");
    if(p.rx13en)rx_list_for_sending = QString(rx_list_for_sending)+("126987,");
    if(p.rx14en)rx_list_for_sending = QString(rx_list_for_sending)+("126988,");
    if(p.rx15en)rx_list_for_sending = QString(rx_list_for_sending)+("126992,");
    if(p.rx16en)rx_list_for_sending = QString(rx_list_for_sending)+("126993,");
    if(p.rx17en)rx_list_for_sending = QString(rx_list_for_sending)+("126996,");
    if(p.rx18en)rx_list_for_sending = QString(rx_list_for_sending)+("126998,");
    if(p.rx19en)rx_list_for_sending = QString(rx_list_for_sending)+("127233,");
    if(p.rx20en)rx_list_for_sending = QString(rx_list_for_sending)+("127237,");
    if(p.rx21en)rx_list_for_sending = QString(rx_list_for_sending)+("127245,");
    if(p.rx22en)rx_list_for_sending = QString(rx_list_for_sending)+("127250,");
    if(p.rx23en)rx_list_for_sending = QString(rx_list_for_sending)+("127251,");
    if(p.rx24en)rx_list_for_sending = QString(rx_list_for_sending)+("127252,");
    if(p.rx25en)rx_list_for_sending = QString(rx_list_for_sending)+("127257,");
    if(p.rx26en)rx_list_for_sending = QString(rx_list_for_sending)+("127258,");
    if(p.rx27en)rx_list_for_sending = QString(rx_list_for_sending)+("127488,");
    if(p.rx28en)rx_list_for_sending = QString(rx_list_for_sending)+("127489,");
    if(p.rx29en)rx_list_for_sending = QString(rx_list_for_sending)+("127493,");
    if(p.rx30en)rx_list_for_sending = QString(rx_list_for_sending)+("127496,");
    if(p.rx31en)rx_list_for_sending = QString(rx_list_for_sending)+("127497,");
    if(p.rx32en)rx_list_for_sending = QString(rx_list_for_sending)+("127498,");
    if(p.rx33en)rx_list_for_sending = QString(rx_list_for_sending)+("127500,");
    if(p.rx34en)rx_list_for_sending = QString(rx_list_for_sending)+("127501,");
    if(p.rx35en)rx_list_for_sending = QString(rx_list_for_sending)+("127502,");
    if(p.rx36en)rx_list_for_sending = QString(rx_list_for_sending)+("127503,");
    if(p.rx37en)rx_list_for_sending = QString(rx_list_for_sending)+("127504,");
    if(p.rx38en)rx_list_for_sending = QString(rx_list_for_sending)+("127505,");
    if(p.rx39en)rx_list_for_sending = QString(rx_list_for_sending)+("127506,");
    if(p.rx40en)rx_list_for_sending = QString(rx_list_for_sending)+("127507,");
    if(p.rx41en)rx_list_for_sending = QString(rx_list_for_sending)+("127508,");
    if(p.rx42en)rx_list_for_sending = QString(rx_list_for_sending)+("127509,");
    if(p.rx43en)rx_list_for_sending = QString(rx_list_for_sending)+("127510,");
    if(p.rx44en)rx_list_for_sending = QString(rx_list_for_sending)+("127511,");
    if(p.rx45en)rx_list_for_sending = QString(rx_list_for_sending)+("127512,");
    if(p.rx46en)rx_list_for_sending = QString(rx_list_for_sending)+("127513,");
    if(p.rx47en)rx_list_for_sending = QString(rx_list_for_sending)+("127514,");
    if(p.rx48en)rx_list_for_sending = QString(rx_list_for_sending)+("127744,");
    if(p.rx49en)rx_list_for_sending = QString(rx_list_for_sending)+("127745,");
    if(p.rx50en)rx_list_for_sending = QString(rx_list_for_sending)+("127746,");
    if(p.rx51en)rx_list_for_sending = QString(rx_list_for_sending)+("127747,");
    if(p.rx52en)rx_list_for_sending = QString(rx_list_for_sending)+("127748,");
    if(p.rx53en)rx_list_for_sending = QString(rx_list_for_sending)+("127749,");
    if(p.rx54en)rx_list_for_sending = QString(rx_list_for_sending)+("127750,");
    if(p.rx55en)rx_list_for_sending = QString(rx_list_for_sending)+("127751,");
    if(p.rx56en)rx_list_for_sending = QString(rx_list_for_sending)+("128259,");
    if(p.rx57en)rx_list_for_sending = QString(rx_list_for_sending)+("128267,");
    if(p.rx58en)rx_list_for_sending = QString(rx_list_for_sending)+("128275,");
    if(p.rx59en)rx_list_for_sending = QString(rx_list_for_sending)+("128520,");
    if(p.rx60en)rx_list_for_sending = QString(rx_list_for_sending)+("129025,");
    if(p.rx61en)rx_list_for_sending = QString(rx_list_for_sending)+("129026,");
    if(p.rx62en)rx_list_for_sending = QString(rx_list_for_sending)+("129027,");
    if(p.rx63en)rx_list_for_sending = QString(rx_list_for_sending)+("129028,");
    if(p.rx64en)rx_list_for_sending = QString(rx_list_for_sending)+("129029,");
    if(p.rx65en)rx_list_for_sending = QString(rx_list_for_sending)+("129033,");
    if(p.rx66en)rx_list_for_sending = QString(rx_list_for_sending)+("129038,");
    if(p.rx67en)rx_list_for_sending = QString(rx_list_for_sending)+("129039,");
    if(p.rx68en)rx_list_for_sending = QString(rx_list_for_sending)+("129040,");
    if(p.rx69en)rx_list_for_sending = QString(rx_list_for_sending)+("129041,");
    if(p.rx70en)rx_list_for_sending = QString(rx_list_for_sending)+("129044,");
    if(p.rx71en)rx_list_for_sending = QString(rx_list_for_sending)+("129045,");
    if(p.rx72en)rx_list_for_sending = QString(rx_list_for_sending)+("129283,");
    if(p.rx73en)rx_list_for_sending = QString(rx_list_for_sending)+("129284,");
    if(p.rx74en)rx_list_for_sending = QString(rx_list_for_sending)+("129285,");
    if(p.rx75en)rx_list_for_sending = QString(rx_list_for_sending)+("129291,");
    if(p.rx76en)rx_list_for_sending = QString(rx_list_for_sending)+("129301,");
    if(p.rx77en)rx_list_for_sending = QString(rx_list_for_sending)+("129302,");
    if(p.rx78en)rx_list_for_sending = QString(rx_list_for_sending)+("129538,");
    if(p.rx79en)rx_list_for_sending = QString(rx_list_for_sending)+("129539,");
    if(p.rx80en)rx_list_for_sending = QString(rx_list_for_sending)+("129540,");
    if(p.rx81en)rx_list_for_sending = QString(rx_list_for_sending)+("129541,");
    if(p.rx82en)rx_list_for_sending = QString(rx_list_for_sending)+("129542,");
    if(p.rx83en)rx_list_for_sending = QString(rx_list_for_sending)+("129545,");
    if(p.rx84en)rx_list_for_sending = QString(rx_list_for_sending)+("129546,");
    if(p.rx85en)rx_list_for_sending = QString(rx_list_for_sending)+("129547,");
    if(p.rx86en)rx_list_for_sending = QString(rx_list_for_sending)+("129549,");
    if(p.rx87en)rx_list_for_sending = QString(rx_list_for_sending)+("129550,");
    if(p.rx88en)rx_list_for_sending = QString(rx_list_for_sending)+("129551,");
    if(p.rx89en)rx_list_for_sending = QString(rx_list_for_sending)+("129556,");
    if(p.rx90en)rx_list_for_sending = QString(rx_list_for_sending)+("129792,");
    if(p.rx91en)rx_list_for_sending = QString(rx_list_for_sending)+("129793,");
    if(p.rx92en)rx_list_for_sending = QString(rx_list_for_sending)+("129794,");
    if(p.rx93en)rx_list_for_sending = QString(rx_list_for_sending)+("129795,");
    if(p.rx94en)rx_list_for_sending = QString(rx_list_for_sending)+("129796,");
    if(p.rx95en)rx_list_for_sending = QString(rx_list_for_sending)+("129797,");
    if(p.rx96en)rx_list_for_sending = QString(rx_list_for_sending)+("129798,");
    if(p.rx97en)rx_list_for_sending = QString(rx_list_for_sending)+("129799,");
    if(p.rx98en)rx_list_for_sending = QString(rx_list_for_sending)+("129800,");
    if(p.rx99en)rx_list_for_sending = QString(rx_list_for_sending)+("129801,");
    if(p.rx100en)rx_list_for_sending = QString(rx_list_for_sending)+("129802,");
    if(p.rx101en)rx_list_for_sending = QString(rx_list_for_sending)+("129803,");
    if(p.rx102en)rx_list_for_sending = QString(rx_list_for_sending)+("129804,");
    if(p.rx103en)rx_list_for_sending = QString(rx_list_for_sending)+("129805,");
    if(p.rx104en)rx_list_for_sending = QString(rx_list_for_sending)+("129806,");
    if(p.rx105en)rx_list_for_sending = QString(rx_list_for_sending)+("129807,");
    if(p.rx106en)rx_list_for_sending = QString(rx_list_for_sending)+("129808,");
    if(p.rx107en)rx_list_for_sending = QString(rx_list_for_sending)+("129809,");
    if(p.rx108en)rx_list_for_sending = QString(rx_list_for_sending)+("129810,");
    if(p.rx109en)rx_list_for_sending = QString(rx_list_for_sending)+("129811,");
    if(p.rx110en)rx_list_for_sending = QString(rx_list_for_sending)+("129812,");
    if(p.rx111en)rx_list_for_sending = QString(rx_list_for_sending)+("129813,");
    if(p.rx112en)rx_list_for_sending = QString(rx_list_for_sending)+("130052,");
    if(p.rx113en)rx_list_for_sending = QString(rx_list_for_sending)+("130053,");
    if(p.rx114en)rx_list_for_sending = QString(rx_list_for_sending)+("130054,");
    if(p.rx115en)rx_list_for_sending = QString(rx_list_for_sending)+("130060,");
    if(p.rx116en)rx_list_for_sending = QString(rx_list_for_sending)+("130061,");
    if(p.rx117en)rx_list_for_sending = QString(rx_list_for_sending)+("130064,");
    if(p.rx118en)rx_list_for_sending = QString(rx_list_for_sending)+("130065,");
    if(p.rx119en)rx_list_for_sending = QString(rx_list_for_sending)+("130066,");
    if(p.rx120en)rx_list_for_sending = QString(rx_list_for_sending)+("130067,");
    if(p.rx121en)rx_list_for_sending = QString(rx_list_for_sending)+("130068,");
    if(p.rx122en)rx_list_for_sending = QString(rx_list_for_sending)+("130069,");
    if(p.rx123en)rx_list_for_sending = QString(rx_list_for_sending)+("130070,");
    if(p.rx124en)rx_list_for_sending = QString(rx_list_for_sending)+("130071,");
    if(p.rx125en)rx_list_for_sending = QString(rx_list_for_sending)+("130072,");
    if(p.rx126en)rx_list_for_sending = QString(rx_list_for_sending)+("130073,");
    if(p.rx127en)rx_list_for_sending = QString(rx_list_for_sending)+("130074,");
    if(p.rx128en)rx_list_for_sending = QString(rx_list_for_sending)+("130306,");
    if(p.rx129en)rx_list_for_sending = QString(rx_list_for_sending)+("130310,");
    if(p.rx130en)rx_list_for_sending = QString(rx_list_for_sending)+("130311,");
    if(p.rx131en)rx_list_for_sending = QString(rx_list_for_sending)+("130312,");
    if(p.rx132en)rx_list_for_sending = QString(rx_list_for_sending)+("130313,");
    if(p.rx133en)rx_list_for_sending = QString(rx_list_for_sending)+("130314,");
    if(p.rx134en)rx_list_for_sending = QString(rx_list_for_sending)+("130315,");
    if(p.rx135en)rx_list_for_sending = QString(rx_list_for_sending)+("130316,");
    if(p.rx136en)rx_list_for_sending = QString(rx_list_for_sending)+("130320,");
    if(p.rx137en)rx_list_for_sending = QString(rx_list_for_sending)+("130321,");
    if(p.rx138en)rx_list_for_sending = QString(rx_list_for_sending)+("130322,");
    if(p.rx139en)rx_list_for_sending = QString(rx_list_for_sending)+("130323,");
    if(p.rx140en)rx_list_for_sending = QString(rx_list_for_sending)+("130324,");
    if(p.rx141en)rx_list_for_sending = QString(rx_list_for_sending)+("130560,");
    if(p.rx142en)rx_list_for_sending = QString(rx_list_for_sending)+("130567,");
    if(p.rx143en)rx_list_for_sending = QString(rx_list_for_sending)+("130569,");
    if(p.rx144en)rx_list_for_sending = QString(rx_list_for_sending)+("130570,");
    if(p.rx145en)rx_list_for_sending = QString(rx_list_for_sending)+("130571,");
    if(p.rx146en)rx_list_for_sending = QString(rx_list_for_sending)+("130572,");
    if(p.rx147en)rx_list_for_sending = QString(rx_list_for_sending)+("130573,");
    if(p.rx148en)rx_list_for_sending = QString(rx_list_for_sending)+("130574,");
    if(p.rx149en)rx_list_for_sending = QString(rx_list_for_sending)+("130576,");
    if(p.rx150en)rx_list_for_sending = QString(rx_list_for_sending)+("130577,");
    if(p.rx151en)rx_list_for_sending = QString(rx_list_for_sending)+("130578,");
    if(p.rx152en)rx_list_for_sending = QString(rx_list_for_sending)+("130580,");
    if(p.rx153en)rx_list_for_sending = QString(rx_list_for_sending)+("130581,");
    if(p.rx154en)rx_list_for_sending = QString(rx_list_for_sending)+("130582,");
    if(p.rx155en)rx_list_for_sending = QString(rx_list_for_sending)+("130583,");
    if(p.rx156en)rx_list_for_sending = QString(rx_list_for_sending)+("130584,");
    if(p.rx157en)rx_list_for_sending = QString(rx_list_for_sending)+("130585,");

    rx_list_for_sending = QString(rx_list_for_sending)+("\r\n");

    m_console->putData(rx_list_for_sending.toUtf8());

    writeData(rx_list_for_sending.toUtf8());

    m_serial->waitForBytesWritten(1500);

    readData();

}

void MainWindow::send_tx_list()
{
    const SettingsDialog::Settings p = m_settings->settings();

    m_console->putData("Sending TX PGN list to the iKonvert Gateway\r\n");

    QString tx_list_for_sending = QString("$PDGY,TX_LIST,");

    if(p.tx2en)tx_list_for_sending = QString(tx_list_for_sending)+("59904,");
    if(p.tx3en)tx_list_for_sending = QString(tx_list_for_sending)+("60160,");
    if(p.tx6en)tx_list_for_sending = QString(tx_list_for_sending)+("65240,");
    if(p.tx9en)tx_list_for_sending = QString(tx_list_for_sending)+("126983,");
    if(p.tx10en)tx_list_for_sending = QString(tx_list_for_sending)+("126984,");
    if(p.tx11en)tx_list_for_sending = QString(tx_list_for_sending)+("126985,");
    if(p.tx12en)tx_list_for_sending = QString(tx_list_for_sending)+("126986,");
    if(p.tx13en)tx_list_for_sending = QString(tx_list_for_sending)+("126987,");
    if(p.tx14en)tx_list_for_sending = QString(tx_list_for_sending)+("126988,");
    if(p.tx15en)tx_list_for_sending = QString(tx_list_for_sending)+("126992,");
    if(p.tx19en)tx_list_for_sending = QString(tx_list_for_sending)+("127233,");
    if(p.tx20en)tx_list_for_sending = QString(tx_list_for_sending)+("127237,");
    if(p.tx21en)tx_list_for_sending = QString(tx_list_for_sending)+("127245,");
    if(p.tx22en)tx_list_for_sending = QString(tx_list_for_sending)+("127250,");
    if(p.tx23en)tx_list_for_sending = QString(tx_list_for_sending)+("127251,");
    if(p.tx24en)tx_list_for_sending = QString(tx_list_for_sending)+("127252,");
    if(p.tx25en)tx_list_for_sending = QString(tx_list_for_sending)+("127257,");
    if(p.tx26en)tx_list_for_sending = QString(tx_list_for_sending)+("127258,");
    if(p.tx27en)tx_list_for_sending = QString(tx_list_for_sending)+("127488,");
    if(p.tx28en)tx_list_for_sending = QString(tx_list_for_sending)+("127489,");
    if(p.tx29en)tx_list_for_sending = QString(tx_list_for_sending)+("127493,");
    if(p.tx30en)tx_list_for_sending = QString(tx_list_for_sending)+("127496,");
    if(p.tx31en)tx_list_for_sending = QString(tx_list_for_sending)+("127497,");
    if(p.tx32en)tx_list_for_sending = QString(tx_list_for_sending)+("127498,");
    if(p.tx33en)tx_list_for_sending = QString(tx_list_for_sending)+("127500,");
    if(p.tx34en)tx_list_for_sending = QString(tx_list_for_sending)+("127501,");
    if(p.tx35en)tx_list_for_sending = QString(tx_list_for_sending)+("127502,");
    if(p.tx36en)tx_list_for_sending = QString(tx_list_for_sending)+("127503,");
    if(p.tx37en)tx_list_for_sending = QString(tx_list_for_sending)+("127504,");
    if(p.tx38en)tx_list_for_sending = QString(tx_list_for_sending)+("127505,");
    if(p.tx39en)tx_list_for_sending = QString(tx_list_for_sending)+("127506,");
    if(p.tx40en)tx_list_for_sending = QString(tx_list_for_sending)+("127507,");
    if(p.tx41en)tx_list_for_sending = QString(tx_list_for_sending)+("127508,");
    if(p.tx42en)tx_list_for_sending = QString(tx_list_for_sending)+("127509,");
    if(p.tx43en)tx_list_for_sending = QString(tx_list_for_sending)+("127510,");
    if(p.tx44en)tx_list_for_sending = QString(tx_list_for_sending)+("127511,");
    if(p.tx45en)tx_list_for_sending = QString(tx_list_for_sending)+("127512,");
    if(p.tx46en)tx_list_for_sending = QString(tx_list_for_sending)+("127513,");
    if(p.tx47en)tx_list_for_sending = QString(tx_list_for_sending)+("127514,");
    if(p.tx48en)tx_list_for_sending = QString(tx_list_for_sending)+("127744,");
    if(p.tx49en)tx_list_for_sending = QString(tx_list_for_sending)+("127745,");
    if(p.tx50en)tx_list_for_sending = QString(tx_list_for_sending)+("127746,");
    if(p.tx51en)tx_list_for_sending = QString(tx_list_for_sending)+("127747,");
    if(p.tx52en)tx_list_for_sending = QString(tx_list_for_sending)+("127748,");
    if(p.tx53en)tx_list_for_sending = QString(tx_list_for_sending)+("127749,");
    if(p.tx54en)tx_list_for_sending = QString(tx_list_for_sending)+("127750,");
    if(p.tx55en)tx_list_for_sending = QString(tx_list_for_sending)+("127751,");
    if(p.tx56en)tx_list_for_sending = QString(tx_list_for_sending)+("128259,");
    if(p.tx57en)tx_list_for_sending = QString(tx_list_for_sending)+("128267,");
    if(p.tx58en)tx_list_for_sending = QString(tx_list_for_sending)+("128275,");
    if(p.tx59en)tx_list_for_sending = QString(tx_list_for_sending)+("128520,");
    if(p.tx60en)tx_list_for_sending = QString(tx_list_for_sending)+("129025,");
    if(p.tx61en)tx_list_for_sending = QString(tx_list_for_sending)+("129026,");
    if(p.tx62en)tx_list_for_sending = QString(tx_list_for_sending)+("129027,");
    if(p.tx63en)tx_list_for_sending = QString(tx_list_for_sending)+("129028,");
    if(p.tx64en)tx_list_for_sending = QString(tx_list_for_sending)+("129029,");
    if(p.tx65en)tx_list_for_sending = QString(tx_list_for_sending)+("129033,");
    if(p.tx66en)tx_list_for_sending = QString(tx_list_for_sending)+("129038,");
    if(p.tx67en)tx_list_for_sending = QString(tx_list_for_sending)+("129039,");
    if(p.tx68en)tx_list_for_sending = QString(tx_list_for_sending)+("129040,");
    if(p.tx69en)tx_list_for_sending = QString(tx_list_for_sending)+("129041,");
    if(p.tx70en)tx_list_for_sending = QString(tx_list_for_sending)+("129044,");
    if(p.tx71en)tx_list_for_sending = QString(tx_list_for_sending)+("129045,");
    if(p.tx72en)tx_list_for_sending = QString(tx_list_for_sending)+("129283,");
    if(p.tx73en)tx_list_for_sending = QString(tx_list_for_sending)+("129284,");
    if(p.tx74en)tx_list_for_sending = QString(tx_list_for_sending)+("129285,");
    if(p.tx75en)tx_list_for_sending = QString(tx_list_for_sending)+("129291,");
    if(p.tx76en)tx_list_for_sending = QString(tx_list_for_sending)+("129301,");
    if(p.tx77en)tx_list_for_sending = QString(tx_list_for_sending)+("129302,");
    if(p.tx78en)tx_list_for_sending = QString(tx_list_for_sending)+("129538,");
    if(p.tx79en)tx_list_for_sending = QString(tx_list_for_sending)+("129539,");
    if(p.tx80en)tx_list_for_sending = QString(tx_list_for_sending)+("129540,");
    if(p.tx81en)tx_list_for_sending = QString(tx_list_for_sending)+("129541,");
    if(p.tx82en)tx_list_for_sending = QString(tx_list_for_sending)+("129542,");
    if(p.tx83en)tx_list_for_sending = QString(tx_list_for_sending)+("129545,");
    if(p.tx84en)tx_list_for_sending = QString(tx_list_for_sending)+("129546,");
    if(p.tx85en)tx_list_for_sending = QString(tx_list_for_sending)+("129547,");
    if(p.tx86en)tx_list_for_sending = QString(tx_list_for_sending)+("129549,");
    if(p.tx87en)tx_list_for_sending = QString(tx_list_for_sending)+("129550,");
    if(p.tx88en)tx_list_for_sending = QString(tx_list_for_sending)+("129551,");
    if(p.tx89en)tx_list_for_sending = QString(tx_list_for_sending)+("129556,");
    if(p.tx90en)tx_list_for_sending = QString(tx_list_for_sending)+("129792,");
    if(p.tx91en)tx_list_for_sending = QString(tx_list_for_sending)+("129793,");
    if(p.tx92en)tx_list_for_sending = QString(tx_list_for_sending)+("129794,");
    if(p.tx93en)tx_list_for_sending = QString(tx_list_for_sending)+("129795,");
    if(p.tx94en)tx_list_for_sending = QString(tx_list_for_sending)+("129796,");
    if(p.tx95en)tx_list_for_sending = QString(tx_list_for_sending)+("129797,");
    if(p.tx96en)tx_list_for_sending = QString(tx_list_for_sending)+("129798,");
    if(p.tx97en)tx_list_for_sending = QString(tx_list_for_sending)+("129799,");
    if(p.tx98en)tx_list_for_sending = QString(tx_list_for_sending)+("129800,");
    if(p.tx99en)tx_list_for_sending = QString(tx_list_for_sending)+("129801,");
    if(p.tx100en)tx_list_for_sending = QString(tx_list_for_sending)+("129802,");
    if(p.tx101en)tx_list_for_sending = QString(tx_list_for_sending)+("129803,");
    if(p.tx102en)tx_list_for_sending = QString(tx_list_for_sending)+("129804,");
    if(p.tx103en)tx_list_for_sending = QString(tx_list_for_sending)+("129805,");
    if(p.tx104en)tx_list_for_sending = QString(tx_list_for_sending)+("129806,");
    if(p.tx105en)tx_list_for_sending = QString(tx_list_for_sending)+("129807,");
    if(p.tx106en)tx_list_for_sending = QString(tx_list_for_sending)+("129808,");
    if(p.tx107en)tx_list_for_sending = QString(tx_list_for_sending)+("129809,");
    if(p.tx108en)tx_list_for_sending = QString(tx_list_for_sending)+("129810,");
    if(p.tx109en)tx_list_for_sending = QString(tx_list_for_sending)+("129811,");
    if(p.tx110en)tx_list_for_sending = QString(tx_list_for_sending)+("129812,");
    if(p.tx111en)tx_list_for_sending = QString(tx_list_for_sending)+("129813,");
    if(p.tx112en)tx_list_for_sending = QString(tx_list_for_sending)+("130052,");
    if(p.tx113en)tx_list_for_sending = QString(tx_list_for_sending)+("130053,");
    if(p.tx114en)tx_list_for_sending = QString(tx_list_for_sending)+("130054,");
    if(p.tx115en)tx_list_for_sending = QString(tx_list_for_sending)+("130060,");
    if(p.tx116en)tx_list_for_sending = QString(tx_list_for_sending)+("130061,");
    if(p.tx117en)tx_list_for_sending = QString(tx_list_for_sending)+("130064,");
    if(p.tx118en)tx_list_for_sending = QString(tx_list_for_sending)+("130065,");
    if(p.tx119en)tx_list_for_sending = QString(tx_list_for_sending)+("130066,");
    if(p.tx120en)tx_list_for_sending = QString(tx_list_for_sending)+("130067,");
    if(p.tx121en)tx_list_for_sending = QString(tx_list_for_sending)+("130068,");
    if(p.tx122en)tx_list_for_sending = QString(tx_list_for_sending)+("130069,");
    if(p.tx123en)tx_list_for_sending = QString(tx_list_for_sending)+("130070,");
    if(p.tx124en)tx_list_for_sending = QString(tx_list_for_sending)+("130071,");
    if(p.tx125en)tx_list_for_sending = QString(tx_list_for_sending)+("130072,");
    if(p.tx126en)tx_list_for_sending = QString(tx_list_for_sending)+("130073,");
    if(p.tx127en)tx_list_for_sending = QString(tx_list_for_sending)+("130074,");
    if(p.tx128en)tx_list_for_sending = QString(tx_list_for_sending)+("130306,");
    if(p.tx129en)tx_list_for_sending = QString(tx_list_for_sending)+("130310,");
    if(p.tx130en)tx_list_for_sending = QString(tx_list_for_sending)+("130311,");
    if(p.tx131en)tx_list_for_sending = QString(tx_list_for_sending)+("130312,");
    if(p.tx132en)tx_list_for_sending = QString(tx_list_for_sending)+("130313,");
    if(p.tx133en)tx_list_for_sending = QString(tx_list_for_sending)+("130314,");
    if(p.tx134en)tx_list_for_sending = QString(tx_list_for_sending)+("130315,");
    if(p.tx135en)tx_list_for_sending = QString(tx_list_for_sending)+("130316,");
    if(p.tx136en)tx_list_for_sending = QString(tx_list_for_sending)+("130320,");
    if(p.tx137en)tx_list_for_sending = QString(tx_list_for_sending)+("130321,");
    if(p.tx138en)tx_list_for_sending = QString(tx_list_for_sending)+("130322,");
    if(p.tx139en)tx_list_for_sending = QString(tx_list_for_sending)+("130323,");
    if(p.tx140en)tx_list_for_sending = QString(tx_list_for_sending)+("130324,");
    if(p.tx141en)tx_list_for_sending = QString(tx_list_for_sending)+("130560,");
    if(p.tx142en)tx_list_for_sending = QString(tx_list_for_sending)+("130567,");
    if(p.tx143en)tx_list_for_sending = QString(tx_list_for_sending)+("130569,");
    if(p.tx144en)tx_list_for_sending = QString(tx_list_for_sending)+("130570,");
    if(p.tx145en)tx_list_for_sending = QString(tx_list_for_sending)+("130571,");
    if(p.tx146en)tx_list_for_sending = QString(tx_list_for_sending)+("130572,");
    if(p.tx147en)tx_list_for_sending = QString(tx_list_for_sending)+("130573,");
    if(p.tx148en)tx_list_for_sending = QString(tx_list_for_sending)+("130574,");
    if(p.tx149en)tx_list_for_sending = QString(tx_list_for_sending)+("130576,");
    if(p.tx150en)tx_list_for_sending = QString(tx_list_for_sending)+("130577,");
    if(p.tx151en)tx_list_for_sending = QString(tx_list_for_sending)+("130578,");
    if(p.tx152en)tx_list_for_sending = QString(tx_list_for_sending)+("130580,");
    if(p.tx153en)tx_list_for_sending = QString(tx_list_for_sending)+("130581,");
    if(p.tx154en)tx_list_for_sending = QString(tx_list_for_sending)+("130582,");
    if(p.tx155en)tx_list_for_sending = QString(tx_list_for_sending)+("130583,");
    if(p.tx156en)tx_list_for_sending = QString(tx_list_for_sending)+("130584,");
    if(p.tx157en)tx_list_for_sending = QString(tx_list_for_sending)+("130585,");

    tx_list_for_sending = QString(tx_list_for_sending)+("\r\n");

    m_console->putData(tx_list_for_sending.toUtf8());

    writeData(tx_list_for_sending.toUtf8());

    m_serial->waitForBytesWritten(1500);

}

void MainWindow::send_init_command()
{

    m_serial->write("$PDGY,N2NET_INIT,NORMAL\r\n");

}

void MainWindow::send_init_command_allmode()
{

    m_serial->write("$PDGY,N2NET_INIT,ALL,\r\n");

}

void MainWindow::send_126992() // sending system time PGN command to the gateway
{
    if(initialised){

        QDate date = QDate::currentDate();

        qint64 dayCount = dateSince.daysTo(date);

        QString hexDays = QString::number(dayCount,16);

        QString hexDate = hexDays.mid(2,2);

        hexDate.append(hexDays.mid(0,2));

        QTime time = QTime::currentTime();

        qint64 msecCount = 10*timeSince.msecsTo(time);

        QString hexmsecs = QString::number(msecCount,16);

        QString hexTime = hexmsecs.mid(6,2);
                hexTime.append(hexmsecs.mid(4,2));
                hexTime.append(hexmsecs.mid(2,2));
                hexTime.append(hexmsecs.mid(0,2));

        QString system_time_PGN = QString("!PDGY,126992,255,00F5"+hexDate.toUtf8()+hexTime.toUtf8()+"\r\n");

        m_console->putData(system_time_PGN.toUtf8());

        m_serial->write(system_time_PGN.toUtf8());

    }

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About iKonvert Gateway test by Digital Yacht"),
                       tr("<b>iKonvert Gateway test by Akos Kelemen<b>"));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    m_serial->write(data);
}
//! [6]
void MainWindow::generate()
{

    if (line_count == line_count_all)line_count = 0;

    QString temp_line = lines[line_count];

    QStringList temp_list = temp_line.split(",");

    if(temp_list.count() > 6 && temp_list[0] == "!PDGY"){

        QString temp = QString("%1,%2,%3,%4\r\n").arg(temp_list[0]).arg(temp_list[1]).arg(temp_list[4]).arg(temp_list[6]);

        if (m_serial->isOpen()) writeData(temp.toUtf8());

        QString temp2 = QString("%1%2").arg("Sending: ").arg(temp);

        m_console->putData(temp2.toUtf8());

    }else{

        //QString temp3 = QString("Unrecognised line in CSV file\r\n");

        //m_console->putData(temp3.toUtf8());

    }

    line_count++;

}

void MainWindow::stop_generate()
{

    infiniteCountFinished();

}

void MainWindow::start_generate()
{
        line_count = 0;

        startInfiniteCount();

}

void MainWindow::send_clipboard()
{
    QString clipboard = QApplication::clipboard()->text();

    if(clipboard.mid(0, 6)=="!PDGY," || clipboard.mid(0, 6)=="$PDGY,"){

        clipboard = QString(clipboard + "\r\n");

        m_console->putData(clipboard.toUtf8());

        m_serial->write(clipboard.toUtf8());

    }

}

void MainWindow::start_record()
{

    m_ui->actionRun->setEnabled(false);
    m_ui->actionRec->setEnabled(false);
    m_ui->actionStop->setEnabled(false);
    m_ui->actionStop_Record->setEnabled(true);

    record = true;

    QString show_recording_started = QString("Recording started...\r\n");
    m_console->putData(show_recording_started.toUtf8());

}

void MainWindow::stop_record()
{

    m_ui->actionRun->setEnabled(true);
    m_ui->actionRec->setEnabled(true);
    m_ui->actionStop->setEnabled(false);
    m_ui->actionStop_Record->setEnabled(false);

    record = false;

    QString show_recording_stopped = QString("Recording stopped...\r\n");
    m_console->putData(show_recording_stopped.toUtf8());

    QString filename = QFileDialog::getSaveFileName(this, "Save CSV file", "test.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0); // getting the filename (full path)
    QFile recorded_file(filename);
    if(recorded_file.open(QFile::WriteOnly |QFile::Truncate))
    {
        QTextStream output(&recorded_file);

        for (int i = 0; i < lines_recorded.size(); ++i){
          output << lines_recorded.at(i) << '\n';
        }

        recorded_file.close();
    }

    while(!lines_recorded.isEmpty())
    lines_recorded.clear();

}

//! [7]
void MainWindow::readData()
{

    while (m_serial->canReadLine()) MainWindow::process_line(m_serial->readLine());

}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionConnect, &QAction::triggered, this, &MainWindow::init_Gateway);
    connect(m_ui->actionDisconnect, &QAction::triggered, this, &MainWindow::de_init_Gateway);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    //connect(m_ui->actionConfigure, &QAction::triggered, this, &MainWindow::table_query);
    connect(m_ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::show);
    connect(m_ui->actionClear, &QAction::triggered, m_console, &Console::clear);
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(m_ui->actionRun, &QAction::triggered, this, &MainWindow::start_generate);
    connect(m_ui->actionSend, &QAction::triggered, this, &MainWindow::send_clipboard);
    connect(m_ui->actionRec, &QAction::triggered, this, &MainWindow::start_record);
    connect(m_ui->actionStop_Record, &QAction::triggered, this, &MainWindow::stop_record);
    connect(m_ui->actionSend_126992, &QAction::triggered, this, &MainWindow::send_126992);
    connect(m_settings, SIGNAL(call_send_rx_list()), this, SLOT(send_rx_list()));
    connect(m_settings, SIGNAL(call_table_query()), this, SLOT(table_query()));
    connect(m_settings, SIGNAL(call_reset()), this, SLOT(reset()));
    connect(m_settings, SIGNAL(call_config_serial()), this, SLOT(config_serial()));
    connect(this, SIGNAL(call_readReply(QByteArray, QByteArray)), m_settings, SLOT(readReply(QByteArray, QByteArray)));

    //connect(m_ui->actionStop, &QAction::triggered, this, &MainWindow::stop_generate);

}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}


void MainWindow::updateInfiniteCount(int cnt)
{
    //m_status->setText(QString::number(cnt));

    generate();

}


void MainWindow::startInfiniteCount()
{
    QThread             *workerThread;
    InfiniteCountWorker *worker;

    if (infiniteCountRunning) {
        QMessageBox::critical(this, "Error", "Infinite count is already running!");
        return;
    }

    workerThread = new QThread;
    worker       = new InfiniteCountWorker;
    worker->moveToThread(workerThread);
    connect(workerThread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), workerThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this, SLOT(infiniteCountFinished()));
    connect(worker, SIGNAL(updateCount(int)), this, SLOT(updateInfiniteCount(int)));
    connect(this, SIGNAL(repeatSec(int)), worker, SLOT(repeat_sec(int)));
    connect(m_ui->actionStop, SIGNAL(triggered(bool)), worker, SLOT(stopWork()));
    workerThread->start();

    emit repeatSec(rep);

    infiniteCountRunning = true;

    m_ui->actionRun->setEnabled(false);
    m_ui->actionRec->setEnabled(false);
    m_ui->actionStop->setEnabled(true);
    m_ui->actionStop_Record->setEnabled(false);

    QString show_message = QString("Filename: %1\r\nNumber of lines: %2\r\nTX interval(ms): %3\r\nSending started...\r\n")
            .arg(CSV_file).arg(line_count_all).arg(rep);

    m_console->putData(show_message.toUtf8());

}


void MainWindow::infiniteCountFinished()
{
    infiniteCountRunning = false;

    m_ui->actionRun->setEnabled(true);
    m_ui->actionRec->setEnabled(true);
    m_ui->actionStop->setEnabled(false);
    m_ui->actionStop_Record->setEnabled(false);

}
