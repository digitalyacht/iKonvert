/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        QString CSV;
        int repeat;
        bool localEchoEnabled;
        bool NormalModeEnabled;
        bool Base64Enabled;
        bool ShowNSMEnabled;
        bool rx1en,
        rx2en,
        rx3en,
        rx4en,
        rx5en,
        rx6en,
        rx7en,
        rx8en,
        rx9en,
        rx10en,
        rx11en,
        rx12en,
        rx13en,
        rx14en,
        rx15en,
        rx16en,
        rx17en,
        rx18en,
        rx19en,
        rx20en,
        rx21en,
        rx22en,
        rx23en,
        rx24en,
        rx25en,
        rx26en,
        rx27en,
        rx28en,
        rx29en,
        rx30en,
        rx31en,
        rx32en,
        rx33en,
        rx34en,
        rx35en,
        rx36en,
        rx37en,
        rx38en,
        rx39en,
        rx40en,
        rx41en,
        rx42en,
        rx43en,
        rx44en,
        rx45en,
        rx46en,
        rx47en,
        rx48en,
        rx49en,
        rx50en,
        rx51en,
        rx52en,
        rx53en,
        rx54en,
        rx55en,
        rx56en,
        rx57en,
        rx58en,
        rx59en,
        rx60en,
        rx61en,
        rx62en,
        rx63en,
        rx64en,
        rx65en,
        rx66en,
        rx67en,
        rx68en,
        rx69en,
        rx70en,
        rx71en,
        rx72en,
        rx73en,
        rx74en,
        rx75en,
        rx76en,
        rx77en,
        rx78en,
        rx79en,
        rx80en,
        rx81en,
        rx82en,
        rx83en,
        rx84en,
        rx85en,
        rx86en,
        rx87en,
        rx88en,
        rx89en,
        rx90en,
        rx91en,
        rx92en,
        rx93en,
        rx94en,
        rx95en,
        rx96en,
        rx97en,
        rx98en,
        rx99en,
        rx100en,
        rx101en,
        rx102en,
        rx103en,
        rx104en,
        rx105en,
        rx106en,
        rx107en,
        rx108en,
        rx109en,
        rx110en,
        rx111en,
        rx112en,
        rx113en,
        rx114en,
        rx115en,
        rx116en,
        rx117en,
        rx118en,
        rx119en,
        rx120en,
        rx121en,
        rx122en,
        rx123en,
        rx124en,
        rx125en,
        rx126en,
        rx127en,
        rx128en,
        rx129en,
        rx130en,
        rx131en,
        rx132en,
        rx133en,
        rx134en,
        rx135en,
        rx136en,
        rx137en,
        rx138en,
        rx139en,
        rx140en,
        rx141en,
        rx142en,
        rx143en,
        rx144en,
        rx145en,
        rx146en,
        rx147en,
        rx148en,
        rx149en,
        rx150en,
        rx151en,
        rx152en,
        rx153en,
        rx154en,
        rx155en,
        rx156en,
        rx157en,
        tx1en,
        tx2en,
        tx3en,
        tx4en,
        tx5en,
        tx6en,
        tx7en,
        tx8en,
        tx9en,
        tx10en,
        tx11en,
        tx12en,
        tx13en,
        tx14en,
        tx15en,
        tx16en,
        tx17en,
        tx18en,
        tx19en,
        tx20en,
        tx21en,
        tx22en,
        tx23en,
        tx24en,
        tx25en,
        tx26en,
        tx27en,
        tx28en,
        tx29en,
        tx30en,
        tx31en,
        tx32en,
        tx33en,
        tx34en,
        tx35en,
        tx36en,
        tx37en,
        tx38en,
        tx39en,
        tx40en,
        tx41en,
        tx42en,
        tx43en,
        tx44en,
        tx45en,
        tx46en,
        tx47en,
        tx48en,
        tx49en,
        tx50en,
        tx51en,
        tx52en,
        tx53en,
        tx54en,
        tx55en,
        tx56en,
        tx57en,
        tx58en,
        tx59en,
        tx60en,
        tx61en,
        tx62en,
        tx63en,
        tx64en,
        tx65en,
        tx66en,
        tx67en,
        tx68en,
        tx69en,
        tx70en,
        tx71en,
        tx72en,
        tx73en,
        tx74en,
        tx75en,
        tx76en,
        tx77en,
        tx78en,
        tx79en,
        tx80en,
        tx81en,
        tx82en,
        tx83en,
        tx84en,
        tx85en,
        tx86en,
        tx87en,
        tx88en,
        tx89en,
        tx90en,
        tx91en,
        tx92en,
        tx93en,
        tx94en,
        tx95en,
        tx96en,
        tx97en,
        tx98en,
        tx99en,
        tx100en,
        tx101en,
        tx102en,
        tx103en,
        tx104en,
        tx105en,
        tx106en,
        tx107en,
        tx108en,
        tx109en,
        tx110en,
        tx111en,
        tx112en,
        tx113en,
        tx114en,
        tx115en,
        tx116en,
        tx117en,
        tx118en,
        tx119en,
        tx120en,
        tx121en,
        tx122en,
        tx123en,
        tx124en,
        tx125en,
        tx126en,
        tx127en,
        tx128en,
        tx129en,
        tx130en,
        tx131en,
        tx132en,
        tx133en,
        tx134en,
        tx135en,
        tx136en,
        tx137en,
        tx138en,
        tx139en,
        tx140en,
        tx141en,
        tx142en,
        tx143en,
        tx144en,
        tx145en,
        tx146en,
        tx147en,
        tx148en,
        tx149en,
        tx150en,
        tx151en,
        tx152en,
        tx153en,
        tx154en,
        tx155en,
        tx156en,
        tx157en;

    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    Settings settings();

public slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    void on_browseCSV_pressed();
    void all_on();
    void all_off();
    void updateSettings();
    void readReply(QByteArray rx_str, QByteArray tx_str);

signals:

    void call_table_query();
    void call_send_rx_list();
    void call_reset();
    void call_config_serial();

private:
    void fillPortsParameters();
    void fillPortsInfo();

public:
    Ui::SettingsDialog *m_settings_ui = nullptr;
    Settings m_currentSettings;
    QIntValidator *m_intValidator = nullptr;
};

#endif // SETTINGSDIALOG_H
