#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QIntValidator>
#include <QLineEdit>
#include <QSerialPortInfo>
#include "mainwindow.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

QString filename;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_settings_ui(new Ui::SettingsDialog),
    m_intValidator(new QIntValidator(0, 4000000, this))
{
    m_settings_ui->setupUi(this);

    m_settings_ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(m_settings_ui->applyButton, &QPushButton::clicked,
            this, &SettingsDialog::apply);
    connect(m_settings_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::showPortInfo);
    connect(m_settings_ui->baudRateBox,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomBaudRatePolicy);
    connect(m_settings_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomDevicePathPolicy);
    connect(m_settings_ui->allOff, QPushButton::clicked, this, &SettingsDialog::all_off);
    connect(m_settings_ui->allOn, QPushButton::clicked, this, &SettingsDialog::all_on);
    //connect(m_settings_ui->Save_button, QPushButton::clicked, this, &SettingsDialog::load_rxtx_from_gateway_and_tick);
    connect(m_settings_ui->Save_button, QPushButton::clicked, this, &SettingsDialog::updateSettings);
    connect(m_settings_ui->Load_button, QPushButton::clicked, this, call_table_query);
    connect(m_settings_ui->Save_button, QPushButton::clicked, this, call_send_rx_list);
    connect(m_settings_ui->Reset_button, QPushButton::clicked, this, call_reset);

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();

}

SettingsDialog::~SettingsDialog()
{
    delete m_settings_ui;
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    const QStringList list = m_settings_ui->serialPortInfoListBox->itemData(idx).toStringList();

    m_settings_ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    m_settings_ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    m_settings_ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    m_settings_ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    m_settings_ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    m_settings_ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::all_off()
{

    m_settings_ui->rx1RB->setChecked(false);
    m_settings_ui->rx8RB->setChecked(false);
    m_settings_ui->rx9RB->setChecked(false);
    m_settings_ui->rx10RB->setChecked(false);
    m_settings_ui->rx11RB->setChecked(false);
    m_settings_ui->rx12RB->setChecked(false);
    m_settings_ui->rx13RB->setChecked(false);
    m_settings_ui->rx14RB->setChecked(false);
    m_settings_ui->rx15RB->setChecked(false);
    m_settings_ui->rx16RB->setChecked(false);
    m_settings_ui->rx17RB->setChecked(false);
    m_settings_ui->rx18RB->setChecked(false);
    m_settings_ui->rx19RB->setChecked(false);
    m_settings_ui->rx20RB->setChecked(false);
    m_settings_ui->rx21RB->setChecked(false);
    m_settings_ui->rx22RB->setChecked(false);
    m_settings_ui->rx23RB->setChecked(false);
    m_settings_ui->rx24RB->setChecked(false);
    m_settings_ui->rx25RB->setChecked(false);
    m_settings_ui->rx26RB->setChecked(false);
    m_settings_ui->rx27RB->setChecked(false);
    m_settings_ui->rx28RB->setChecked(false);
    m_settings_ui->rx29RB->setChecked(false);
    m_settings_ui->rx30RB->setChecked(false);
    m_settings_ui->rx31RB->setChecked(false);
    m_settings_ui->rx32RB->setChecked(false);
    m_settings_ui->rx33RB->setChecked(false);
    m_settings_ui->rx34RB->setChecked(false);
    m_settings_ui->rx35RB->setChecked(false);
    m_settings_ui->rx36RB->setChecked(false);
    m_settings_ui->rx37RB->setChecked(false);
    m_settings_ui->rx38RB->setChecked(false);
    m_settings_ui->rx39RB->setChecked(false);
    m_settings_ui->rx40RB->setChecked(false);
    m_settings_ui->rx41RB->setChecked(false);
    m_settings_ui->rx42RB->setChecked(false);
    m_settings_ui->rx43RB->setChecked(false);
    m_settings_ui->rx44RB->setChecked(false);
    m_settings_ui->rx45RB->setChecked(false);
    m_settings_ui->rx46RB->setChecked(false);
    m_settings_ui->rx47RB->setChecked(false);
    m_settings_ui->rx48RB->setChecked(false);
    m_settings_ui->rx49RB->setChecked(false);
    m_settings_ui->rx50RB->setChecked(false);
    m_settings_ui->rx51RB->setChecked(false);
    m_settings_ui->rx52RB->setChecked(false);
    m_settings_ui->rx53RB->setChecked(false);
    m_settings_ui->rx54RB->setChecked(false);
    m_settings_ui->rx55RB->setChecked(false);
    m_settings_ui->rx56RB->setChecked(false);
    m_settings_ui->rx57RB->setChecked(false);
    m_settings_ui->rx58RB->setChecked(false);
    m_settings_ui->rx59RB->setChecked(false);
    m_settings_ui->rx60RB->setChecked(false);
    m_settings_ui->rx61RB->setChecked(false);
    m_settings_ui->rx62RB->setChecked(false);
    m_settings_ui->rx63RB->setChecked(false);
    m_settings_ui->rx64RB->setChecked(false);
    m_settings_ui->rx65RB->setChecked(false);
    m_settings_ui->rx66RB->setChecked(false);
    m_settings_ui->rx67RB->setChecked(false);
    m_settings_ui->rx68RB->setChecked(false);
    m_settings_ui->rx69RB->setChecked(false);
    m_settings_ui->rx70RB->setChecked(false);
    m_settings_ui->rx71RB->setChecked(false);
    m_settings_ui->rx72RB->setChecked(false);
    m_settings_ui->rx73RB->setChecked(false);
    m_settings_ui->rx74RB->setChecked(false);
    m_settings_ui->rx75RB->setChecked(false);
    m_settings_ui->rx76RB->setChecked(false);
    m_settings_ui->rx77RB->setChecked(false);
    m_settings_ui->rx78RB->setChecked(false);
    m_settings_ui->rx79RB->setChecked(false);
    m_settings_ui->rx80RB->setChecked(false);
    m_settings_ui->rx81RB->setChecked(false);
    m_settings_ui->rx82RB->setChecked(false);
    m_settings_ui->rx83RB->setChecked(false);
    m_settings_ui->rx84RB->setChecked(false);
    m_settings_ui->rx85RB->setChecked(false);
    m_settings_ui->rx86RB->setChecked(false);
    m_settings_ui->rx87RB->setChecked(false);
    m_settings_ui->rx88RB->setChecked(false);
    m_settings_ui->rx89RB->setChecked(false);
    m_settings_ui->rx90RB->setChecked(false);
    m_settings_ui->rx91RB->setChecked(false);
    m_settings_ui->rx92RB->setChecked(false);
    m_settings_ui->rx93RB->setChecked(false);
    m_settings_ui->rx94RB->setChecked(false);
    m_settings_ui->rx95RB->setChecked(false);
    m_settings_ui->rx96RB->setChecked(false);
    m_settings_ui->rx97RB->setChecked(false);
    m_settings_ui->rx98RB->setChecked(false);
    m_settings_ui->rx99RB->setChecked(false);
    m_settings_ui->rx100RB->setChecked(false);
    m_settings_ui->rx101RB->setChecked(false);
    m_settings_ui->rx102RB->setChecked(false);
    m_settings_ui->rx103RB->setChecked(false);
    m_settings_ui->rx104RB->setChecked(false);
    m_settings_ui->rx105RB->setChecked(false);
    m_settings_ui->rx106RB->setChecked(false);
    m_settings_ui->rx107RB->setChecked(false);
    m_settings_ui->rx108RB->setChecked(false);
    m_settings_ui->rx109RB->setChecked(false);
    m_settings_ui->rx110RB->setChecked(false);
    m_settings_ui->rx111RB->setChecked(false);
    m_settings_ui->rx112RB->setChecked(false);
    m_settings_ui->rx113RB->setChecked(false);
    m_settings_ui->rx114RB->setChecked(false);
    m_settings_ui->rx115RB->setChecked(false);
    m_settings_ui->rx116RB->setChecked(false);
    m_settings_ui->rx117RB->setChecked(false);
    m_settings_ui->rx118RB->setChecked(false);
    m_settings_ui->rx119RB->setChecked(false);
    m_settings_ui->rx120RB->setChecked(false);
    m_settings_ui->rx121RB->setChecked(false);
    m_settings_ui->rx122RB->setChecked(false);
    m_settings_ui->rx123RB->setChecked(false);
    m_settings_ui->rx124RB->setChecked(false);
    m_settings_ui->rx125RB->setChecked(false);
    m_settings_ui->rx126RB->setChecked(false);
    m_settings_ui->rx127RB->setChecked(false);
    m_settings_ui->rx128RB->setChecked(false);
    m_settings_ui->rx129RB->setChecked(false);
    m_settings_ui->rx130RB->setChecked(false);
    m_settings_ui->rx131RB->setChecked(false);
    m_settings_ui->rx132RB->setChecked(false);
    m_settings_ui->rx133RB->setChecked(false);
    m_settings_ui->rx134RB->setChecked(false);
    m_settings_ui->rx135RB->setChecked(false);
    m_settings_ui->rx136RB->setChecked(false);
    m_settings_ui->rx137RB->setChecked(false);
    m_settings_ui->rx138RB->setChecked(false);
    m_settings_ui->rx139RB->setChecked(false);
    m_settings_ui->rx140RB->setChecked(false);
    m_settings_ui->rx141RB->setChecked(false);
    m_settings_ui->rx142RB->setChecked(false);
    m_settings_ui->rx143RB->setChecked(false);
    m_settings_ui->rx144RB->setChecked(false);
    m_settings_ui->rx145RB->setChecked(false);
    m_settings_ui->rx146RB->setChecked(false);
    m_settings_ui->rx147RB->setChecked(false);
    m_settings_ui->rx148RB->setChecked(false);
    m_settings_ui->rx149RB->setChecked(false);
    m_settings_ui->rx150RB->setChecked(false);
    m_settings_ui->rx151RB->setChecked(false);
    m_settings_ui->rx152RB->setChecked(false);
    m_settings_ui->rx153RB->setChecked(false);
    m_settings_ui->rx154RB->setChecked(false);
    m_settings_ui->rx155RB->setChecked(false);
    m_settings_ui->rx156RB->setChecked(false);
    m_settings_ui->rx157RB->setChecked(false);

    m_settings_ui->tx2RB->setChecked(false);
    m_settings_ui->tx3RB->setChecked(false);
    m_settings_ui->tx6RB->setChecked(false);
    m_settings_ui->tx9RB->setChecked(false);
    m_settings_ui->tx10RB->setChecked(false);
    m_settings_ui->tx11RB->setChecked(false);
    m_settings_ui->tx12RB->setChecked(false);
    m_settings_ui->tx13RB->setChecked(false);
    m_settings_ui->tx14RB->setChecked(false);
    m_settings_ui->tx15RB->setChecked(false);
    m_settings_ui->tx19RB->setChecked(false);
    m_settings_ui->tx20RB->setChecked(false);
    m_settings_ui->tx21RB->setChecked(false);
    m_settings_ui->tx22RB->setChecked(false);
    m_settings_ui->tx23RB->setChecked(false);
    m_settings_ui->tx24RB->setChecked(false);
    m_settings_ui->tx25RB->setChecked(false);
    m_settings_ui->tx26RB->setChecked(false);
    m_settings_ui->tx27RB->setChecked(false);
    m_settings_ui->tx28RB->setChecked(false);
    m_settings_ui->tx29RB->setChecked(false);
    m_settings_ui->tx30RB->setChecked(false);
    m_settings_ui->tx31RB->setChecked(false);
    m_settings_ui->tx32RB->setChecked(false);
    m_settings_ui->tx33RB->setChecked(false);
    m_settings_ui->tx34RB->setChecked(false);
    m_settings_ui->tx35RB->setChecked(false);
    m_settings_ui->tx36RB->setChecked(false);
    m_settings_ui->tx37RB->setChecked(false);
    m_settings_ui->tx38RB->setChecked(false);
    m_settings_ui->tx39RB->setChecked(false);
    m_settings_ui->tx40RB->setChecked(false);
    m_settings_ui->tx41RB->setChecked(false);
    m_settings_ui->tx42RB->setChecked(false);
    m_settings_ui->tx43RB->setChecked(false);
    m_settings_ui->tx44RB->setChecked(false);
    m_settings_ui->tx45RB->setChecked(false);
    m_settings_ui->tx46RB->setChecked(false);
    m_settings_ui->tx47RB->setChecked(false);
    m_settings_ui->tx48RB->setChecked(false);
    m_settings_ui->tx49RB->setChecked(false);
    m_settings_ui->tx50RB->setChecked(false);
    m_settings_ui->tx51RB->setChecked(false);
    m_settings_ui->tx52RB->setChecked(false);
    m_settings_ui->tx53RB->setChecked(false);
    m_settings_ui->tx54RB->setChecked(false);
    m_settings_ui->tx55RB->setChecked(false);
    m_settings_ui->tx56RB->setChecked(false);
    m_settings_ui->tx57RB->setChecked(false);
    m_settings_ui->tx58RB->setChecked(false);
    m_settings_ui->tx59RB->setChecked(false);
    m_settings_ui->tx60RB->setChecked(false);
    m_settings_ui->tx61RB->setChecked(false);
    m_settings_ui->tx62RB->setChecked(false);
    m_settings_ui->tx63RB->setChecked(false);
    m_settings_ui->tx64RB->setChecked(false);
    m_settings_ui->tx65RB->setChecked(false);
    m_settings_ui->tx66RB->setChecked(false);
    m_settings_ui->tx67RB->setChecked(false);
    m_settings_ui->tx68RB->setChecked(false);
    m_settings_ui->tx69RB->setChecked(false);
    m_settings_ui->tx70RB->setChecked(false);
    m_settings_ui->tx71RB->setChecked(false);
    m_settings_ui->tx72RB->setChecked(false);
    m_settings_ui->tx73RB->setChecked(false);
    m_settings_ui->tx74RB->setChecked(false);
    m_settings_ui->tx75RB->setChecked(false);
    m_settings_ui->tx76RB->setChecked(false);
    m_settings_ui->tx77RB->setChecked(false);
    m_settings_ui->tx78RB->setChecked(false);
    m_settings_ui->tx79RB->setChecked(false);
    m_settings_ui->tx80RB->setChecked(false);
    m_settings_ui->tx81RB->setChecked(false);
    m_settings_ui->tx82RB->setChecked(false);
    m_settings_ui->tx83RB->setChecked(false);
    m_settings_ui->tx84RB->setChecked(false);
    m_settings_ui->tx85RB->setChecked(false);
    m_settings_ui->tx86RB->setChecked(false);
    m_settings_ui->tx87RB->setChecked(false);
    m_settings_ui->tx88RB->setChecked(false);
    m_settings_ui->tx89RB->setChecked(false);
    m_settings_ui->tx90RB->setChecked(false);
    m_settings_ui->tx91RB->setChecked(false);
    m_settings_ui->tx92RB->setChecked(false);
    m_settings_ui->tx93RB->setChecked(false);
    m_settings_ui->tx94RB->setChecked(false);
    m_settings_ui->tx95RB->setChecked(false);
    m_settings_ui->tx96RB->setChecked(false);
    m_settings_ui->tx97RB->setChecked(false);
    m_settings_ui->tx98RB->setChecked(false);
    m_settings_ui->tx99RB->setChecked(false);
    m_settings_ui->tx100RB->setChecked(false);
    m_settings_ui->tx101RB->setChecked(false);
    m_settings_ui->tx102RB->setChecked(false);
    m_settings_ui->tx103RB->setChecked(false);
    m_settings_ui->tx104RB->setChecked(false);
    m_settings_ui->tx105RB->setChecked(false);
    m_settings_ui->tx106RB->setChecked(false);
    m_settings_ui->tx107RB->setChecked(false);
    m_settings_ui->tx108RB->setChecked(false);
    m_settings_ui->tx109RB->setChecked(false);
    m_settings_ui->tx110RB->setChecked(false);
    m_settings_ui->tx111RB->setChecked(false);
    m_settings_ui->tx112RB->setChecked(false);
    m_settings_ui->tx113RB->setChecked(false);
    m_settings_ui->tx114RB->setChecked(false);
    m_settings_ui->tx115RB->setChecked(false);
    m_settings_ui->tx116RB->setChecked(false);
    m_settings_ui->tx117RB->setChecked(false);
    m_settings_ui->tx118RB->setChecked(false);
    m_settings_ui->tx119RB->setChecked(false);
    m_settings_ui->tx120RB->setChecked(false);
    m_settings_ui->tx121RB->setChecked(false);
    m_settings_ui->tx122RB->setChecked(false);
    m_settings_ui->tx123RB->setChecked(false);
    m_settings_ui->tx124RB->setChecked(false);
    m_settings_ui->tx125RB->setChecked(false);
    m_settings_ui->tx126RB->setChecked(false);
    m_settings_ui->tx127RB->setChecked(false);
    m_settings_ui->tx128RB->setChecked(false);
    m_settings_ui->tx129RB->setChecked(false);
    m_settings_ui->tx130RB->setChecked(false);
    m_settings_ui->tx131RB->setChecked(false);
    m_settings_ui->tx132RB->setChecked(false);
    m_settings_ui->tx133RB->setChecked(false);
    m_settings_ui->tx134RB->setChecked(false);
    m_settings_ui->tx135RB->setChecked(false);
    m_settings_ui->tx136RB->setChecked(false);
    m_settings_ui->tx137RB->setChecked(false);
    m_settings_ui->tx138RB->setChecked(false);
    m_settings_ui->tx139RB->setChecked(false);
    m_settings_ui->tx140RB->setChecked(false);
    m_settings_ui->tx141RB->setChecked(false);
    m_settings_ui->tx142RB->setChecked(false);
    m_settings_ui->tx143RB->setChecked(false);
    m_settings_ui->tx144RB->setChecked(false);
    m_settings_ui->tx145RB->setChecked(false);
    m_settings_ui->tx146RB->setChecked(false);
    m_settings_ui->tx147RB->setChecked(false);
    m_settings_ui->tx148RB->setChecked(false);
    m_settings_ui->tx149RB->setChecked(false);
    m_settings_ui->tx150RB->setChecked(false);
    m_settings_ui->tx151RB->setChecked(false);
    m_settings_ui->tx152RB->setChecked(false);
    m_settings_ui->tx153RB->setChecked(false);
    m_settings_ui->tx154RB->setChecked(false);
    m_settings_ui->tx155RB->setChecked(false);
    m_settings_ui->tx156RB->setChecked(false);
    m_settings_ui->tx157RB->setChecked(false);

}

void SettingsDialog::all_on()
{

    m_settings_ui->rx1RB->setChecked(true);
    m_settings_ui->rx8RB->setChecked(true);
    m_settings_ui->rx9RB->setChecked(true);
    m_settings_ui->rx10RB->setChecked(true);
    m_settings_ui->rx11RB->setChecked(true);
    m_settings_ui->rx12RB->setChecked(true);
    m_settings_ui->rx13RB->setChecked(true);
    m_settings_ui->rx14RB->setChecked(true);
    m_settings_ui->rx15RB->setChecked(true);
    m_settings_ui->rx16RB->setChecked(true);
    m_settings_ui->rx17RB->setChecked(true);
    m_settings_ui->rx18RB->setChecked(true);
    m_settings_ui->rx19RB->setChecked(true);
    m_settings_ui->rx20RB->setChecked(true);
    m_settings_ui->rx21RB->setChecked(true);
    m_settings_ui->rx22RB->setChecked(true);
    m_settings_ui->rx23RB->setChecked(true);
    m_settings_ui->rx24RB->setChecked(true);
    m_settings_ui->rx25RB->setChecked(true);
    m_settings_ui->rx26RB->setChecked(true);
    m_settings_ui->rx27RB->setChecked(true);
    m_settings_ui->rx28RB->setChecked(true);
    m_settings_ui->rx29RB->setChecked(true);
    m_settings_ui->rx30RB->setChecked(true);
    m_settings_ui->rx31RB->setChecked(true);
    m_settings_ui->rx32RB->setChecked(true);
    m_settings_ui->rx33RB->setChecked(true);
    m_settings_ui->rx34RB->setChecked(true);
    m_settings_ui->rx35RB->setChecked(true);
    m_settings_ui->rx36RB->setChecked(true);
    m_settings_ui->rx37RB->setChecked(true);
    m_settings_ui->rx38RB->setChecked(true);
    m_settings_ui->rx39RB->setChecked(true);
    m_settings_ui->rx40RB->setChecked(true);
    m_settings_ui->rx41RB->setChecked(true);
    m_settings_ui->rx42RB->setChecked(true);
    m_settings_ui->rx43RB->setChecked(true);
    m_settings_ui->rx44RB->setChecked(true);
    m_settings_ui->rx45RB->setChecked(true);
    m_settings_ui->rx46RB->setChecked(true);
    m_settings_ui->rx47RB->setChecked(true);
    m_settings_ui->rx48RB->setChecked(true);
    m_settings_ui->rx49RB->setChecked(true);
    m_settings_ui->rx50RB->setChecked(true);
    m_settings_ui->rx51RB->setChecked(true);
    m_settings_ui->rx52RB->setChecked(true);
    m_settings_ui->rx53RB->setChecked(true);
    m_settings_ui->rx54RB->setChecked(true);
    m_settings_ui->rx55RB->setChecked(true);
    m_settings_ui->rx56RB->setChecked(true);
    m_settings_ui->rx57RB->setChecked(true);
    m_settings_ui->rx58RB->setChecked(true);
    m_settings_ui->rx59RB->setChecked(true);
    m_settings_ui->rx60RB->setChecked(true);
    m_settings_ui->rx61RB->setChecked(true);
    m_settings_ui->rx62RB->setChecked(true);
    m_settings_ui->rx63RB->setChecked(true);
    m_settings_ui->rx64RB->setChecked(true);
    m_settings_ui->rx65RB->setChecked(true);
    m_settings_ui->rx66RB->setChecked(true);
    m_settings_ui->rx67RB->setChecked(true);
    m_settings_ui->rx68RB->setChecked(true);
    m_settings_ui->rx69RB->setChecked(true);
    m_settings_ui->rx70RB->setChecked(true);
    m_settings_ui->rx71RB->setChecked(true);
    m_settings_ui->rx72RB->setChecked(true);
    m_settings_ui->rx73RB->setChecked(true);
    m_settings_ui->rx74RB->setChecked(true);
    m_settings_ui->rx75RB->setChecked(true);
    m_settings_ui->rx76RB->setChecked(true);
    m_settings_ui->rx77RB->setChecked(true);
    m_settings_ui->rx78RB->setChecked(true);
    m_settings_ui->rx79RB->setChecked(true);
    m_settings_ui->rx80RB->setChecked(true);
    m_settings_ui->rx81RB->setChecked(true);
    m_settings_ui->rx82RB->setChecked(true);
    m_settings_ui->rx83RB->setChecked(true);
    m_settings_ui->rx84RB->setChecked(true);
    m_settings_ui->rx85RB->setChecked(true);
    m_settings_ui->rx86RB->setChecked(true);
    m_settings_ui->rx87RB->setChecked(true);
    m_settings_ui->rx88RB->setChecked(true);
    m_settings_ui->rx89RB->setChecked(true);
    m_settings_ui->rx90RB->setChecked(true);
    m_settings_ui->rx91RB->setChecked(true);
    m_settings_ui->rx92RB->setChecked(true);
    m_settings_ui->rx93RB->setChecked(true);
    m_settings_ui->rx94RB->setChecked(true);
    m_settings_ui->rx95RB->setChecked(true);
    m_settings_ui->rx96RB->setChecked(true);
    m_settings_ui->rx97RB->setChecked(true);
    m_settings_ui->rx98RB->setChecked(true);
    m_settings_ui->rx99RB->setChecked(true);
    m_settings_ui->rx100RB->setChecked(true);
    m_settings_ui->rx101RB->setChecked(true);
    m_settings_ui->rx102RB->setChecked(true);
    m_settings_ui->rx103RB->setChecked(true);
    m_settings_ui->rx104RB->setChecked(true);
    m_settings_ui->rx105RB->setChecked(true);
    m_settings_ui->rx106RB->setChecked(true);
    m_settings_ui->rx107RB->setChecked(true);
    m_settings_ui->rx108RB->setChecked(true);
    m_settings_ui->rx109RB->setChecked(true);
    m_settings_ui->rx110RB->setChecked(true);
    m_settings_ui->rx111RB->setChecked(true);
    m_settings_ui->rx112RB->setChecked(true);
    m_settings_ui->rx113RB->setChecked(true);
    m_settings_ui->rx114RB->setChecked(true);
    m_settings_ui->rx115RB->setChecked(true);
    m_settings_ui->rx116RB->setChecked(true);
    m_settings_ui->rx117RB->setChecked(true);
    m_settings_ui->rx118RB->setChecked(true);
    m_settings_ui->rx119RB->setChecked(true);
    m_settings_ui->rx120RB->setChecked(true);
    m_settings_ui->rx121RB->setChecked(true);
    m_settings_ui->rx122RB->setChecked(true);
    m_settings_ui->rx123RB->setChecked(true);
    m_settings_ui->rx124RB->setChecked(true);
    m_settings_ui->rx125RB->setChecked(true);
    m_settings_ui->rx126RB->setChecked(true);
    m_settings_ui->rx127RB->setChecked(true);
    m_settings_ui->rx128RB->setChecked(true);
    m_settings_ui->rx129RB->setChecked(true);
    m_settings_ui->rx130RB->setChecked(true);
    m_settings_ui->rx131RB->setChecked(true);
    m_settings_ui->rx132RB->setChecked(true);
    m_settings_ui->rx133RB->setChecked(true);
    m_settings_ui->rx134RB->setChecked(true);
    m_settings_ui->rx135RB->setChecked(true);
    m_settings_ui->rx136RB->setChecked(true);
    m_settings_ui->rx137RB->setChecked(true);
    m_settings_ui->rx138RB->setChecked(true);
    m_settings_ui->rx139RB->setChecked(true);
    m_settings_ui->rx140RB->setChecked(true);
    m_settings_ui->rx141RB->setChecked(true);
    m_settings_ui->rx142RB->setChecked(true);
    m_settings_ui->rx143RB->setChecked(true);
    m_settings_ui->rx144RB->setChecked(true);
    m_settings_ui->rx145RB->setChecked(true);
    m_settings_ui->rx146RB->setChecked(true);
    m_settings_ui->rx147RB->setChecked(true);
    m_settings_ui->rx148RB->setChecked(true);
    m_settings_ui->rx149RB->setChecked(true);
    m_settings_ui->rx150RB->setChecked(true);
    m_settings_ui->rx151RB->setChecked(true);
    m_settings_ui->rx152RB->setChecked(true);
    m_settings_ui->rx153RB->setChecked(true);
    m_settings_ui->rx154RB->setChecked(true);
    m_settings_ui->rx155RB->setChecked(true);
    m_settings_ui->rx156RB->setChecked(true);
    m_settings_ui->rx157RB->setChecked(true);

    m_settings_ui->tx2RB->setChecked(true);
    m_settings_ui->tx3RB->setChecked(true);
    m_settings_ui->tx6RB->setChecked(true);
    m_settings_ui->tx9RB->setChecked(true);
    m_settings_ui->tx10RB->setChecked(true);
    m_settings_ui->tx11RB->setChecked(true);
    m_settings_ui->tx12RB->setChecked(true);
    m_settings_ui->tx13RB->setChecked(true);
    m_settings_ui->tx14RB->setChecked(true);
    m_settings_ui->tx15RB->setChecked(true);
    m_settings_ui->tx19RB->setChecked(true);
    m_settings_ui->tx20RB->setChecked(true);
    m_settings_ui->tx21RB->setChecked(true);
    m_settings_ui->tx22RB->setChecked(true);
    m_settings_ui->tx23RB->setChecked(true);
    m_settings_ui->tx24RB->setChecked(true);
    m_settings_ui->tx25RB->setChecked(true);
    m_settings_ui->tx26RB->setChecked(true);
    m_settings_ui->tx27RB->setChecked(true);
    m_settings_ui->tx28RB->setChecked(true);
    m_settings_ui->tx29RB->setChecked(true);
    m_settings_ui->tx30RB->setChecked(true);
    m_settings_ui->tx31RB->setChecked(true);
    m_settings_ui->tx32RB->setChecked(true);
    m_settings_ui->tx33RB->setChecked(true);
    m_settings_ui->tx34RB->setChecked(true);
    m_settings_ui->tx35RB->setChecked(true);
    m_settings_ui->tx36RB->setChecked(true);
    m_settings_ui->tx37RB->setChecked(true);
    m_settings_ui->tx38RB->setChecked(true);
    m_settings_ui->tx39RB->setChecked(true);
    m_settings_ui->tx40RB->setChecked(true);
    m_settings_ui->tx41RB->setChecked(true);
    m_settings_ui->tx42RB->setChecked(true);
    m_settings_ui->tx43RB->setChecked(true);
    m_settings_ui->tx44RB->setChecked(true);
    m_settings_ui->tx45RB->setChecked(true);
    m_settings_ui->tx46RB->setChecked(true);
    m_settings_ui->tx47RB->setChecked(true);
    m_settings_ui->tx48RB->setChecked(true);
    m_settings_ui->tx49RB->setChecked(true);
    m_settings_ui->tx50RB->setChecked(true);
    m_settings_ui->tx51RB->setChecked(true);
    m_settings_ui->tx52RB->setChecked(true);
    m_settings_ui->tx53RB->setChecked(true);
    m_settings_ui->tx54RB->setChecked(true);
    m_settings_ui->tx55RB->setChecked(true);
    m_settings_ui->tx56RB->setChecked(true);
    m_settings_ui->tx57RB->setChecked(true);
    m_settings_ui->tx58RB->setChecked(true);
    m_settings_ui->tx59RB->setChecked(true);
    m_settings_ui->tx60RB->setChecked(true);
    m_settings_ui->tx61RB->setChecked(true);
    m_settings_ui->tx62RB->setChecked(true);
    m_settings_ui->tx63RB->setChecked(true);
    m_settings_ui->tx64RB->setChecked(true);
    m_settings_ui->tx65RB->setChecked(true);
    m_settings_ui->tx66RB->setChecked(true);
    m_settings_ui->tx67RB->setChecked(true);
    m_settings_ui->tx68RB->setChecked(true);
    m_settings_ui->tx69RB->setChecked(true);
    m_settings_ui->tx70RB->setChecked(true);
    m_settings_ui->tx71RB->setChecked(true);
    m_settings_ui->tx72RB->setChecked(true);
    m_settings_ui->tx73RB->setChecked(true);
    m_settings_ui->tx74RB->setChecked(true);
    m_settings_ui->tx75RB->setChecked(true);
    m_settings_ui->tx76RB->setChecked(true);
    m_settings_ui->tx77RB->setChecked(true);
    m_settings_ui->tx78RB->setChecked(true);
    m_settings_ui->tx79RB->setChecked(true);
    m_settings_ui->tx80RB->setChecked(true);
    m_settings_ui->tx81RB->setChecked(true);
    m_settings_ui->tx82RB->setChecked(true);
    m_settings_ui->tx83RB->setChecked(true);
    m_settings_ui->tx84RB->setChecked(true);
    m_settings_ui->tx85RB->setChecked(true);
    m_settings_ui->tx86RB->setChecked(true);
    m_settings_ui->tx87RB->setChecked(true);
    m_settings_ui->tx88RB->setChecked(true);
    m_settings_ui->tx89RB->setChecked(true);
    m_settings_ui->tx90RB->setChecked(true);
    m_settings_ui->tx91RB->setChecked(true);
    m_settings_ui->tx92RB->setChecked(true);
    m_settings_ui->tx93RB->setChecked(true);
    m_settings_ui->tx94RB->setChecked(true);
    m_settings_ui->tx95RB->setChecked(true);
    m_settings_ui->tx96RB->setChecked(true);
    m_settings_ui->tx97RB->setChecked(true);
    m_settings_ui->tx98RB->setChecked(true);
    m_settings_ui->tx99RB->setChecked(true);
    m_settings_ui->tx100RB->setChecked(true);
    m_settings_ui->tx101RB->setChecked(true);
    m_settings_ui->tx102RB->setChecked(true);
    m_settings_ui->tx103RB->setChecked(true);
    m_settings_ui->tx104RB->setChecked(true);
    m_settings_ui->tx105RB->setChecked(true);
    m_settings_ui->tx106RB->setChecked(true);
    m_settings_ui->tx107RB->setChecked(true);
    m_settings_ui->tx108RB->setChecked(true);
    m_settings_ui->tx109RB->setChecked(true);
    m_settings_ui->tx110RB->setChecked(true);
    m_settings_ui->tx111RB->setChecked(true);
    m_settings_ui->tx112RB->setChecked(true);
    m_settings_ui->tx113RB->setChecked(true);
    m_settings_ui->tx114RB->setChecked(true);
    m_settings_ui->tx115RB->setChecked(true);
    m_settings_ui->tx116RB->setChecked(true);
    m_settings_ui->tx117RB->setChecked(true);
    m_settings_ui->tx118RB->setChecked(true);
    m_settings_ui->tx119RB->setChecked(true);
    m_settings_ui->tx120RB->setChecked(true);
    m_settings_ui->tx121RB->setChecked(true);
    m_settings_ui->tx122RB->setChecked(true);
    m_settings_ui->tx123RB->setChecked(true);
    m_settings_ui->tx124RB->setChecked(true);
    m_settings_ui->tx125RB->setChecked(true);
    m_settings_ui->tx126RB->setChecked(true);
    m_settings_ui->tx127RB->setChecked(true);
    m_settings_ui->tx128RB->setChecked(true);
    m_settings_ui->tx129RB->setChecked(true);
    m_settings_ui->tx130RB->setChecked(true);
    m_settings_ui->tx131RB->setChecked(true);
    m_settings_ui->tx132RB->setChecked(true);
    m_settings_ui->tx133RB->setChecked(true);
    m_settings_ui->tx134RB->setChecked(true);
    m_settings_ui->tx135RB->setChecked(true);
    m_settings_ui->tx136RB->setChecked(true);
    m_settings_ui->tx137RB->setChecked(true);
    m_settings_ui->tx138RB->setChecked(true);
    m_settings_ui->tx139RB->setChecked(true);
    m_settings_ui->tx140RB->setChecked(true);
    m_settings_ui->tx141RB->setChecked(true);
    m_settings_ui->tx142RB->setChecked(true);
    m_settings_ui->tx143RB->setChecked(true);
    m_settings_ui->tx144RB->setChecked(true);
    m_settings_ui->tx145RB->setChecked(true);
    m_settings_ui->tx146RB->setChecked(true);
    m_settings_ui->tx147RB->setChecked(true);
    m_settings_ui->tx148RB->setChecked(true);
    m_settings_ui->tx149RB->setChecked(true);
    m_settings_ui->tx150RB->setChecked(true);
    m_settings_ui->tx151RB->setChecked(true);
    m_settings_ui->tx152RB->setChecked(true);
    m_settings_ui->tx153RB->setChecked(true);
    m_settings_ui->tx154RB->setChecked(true);
    m_settings_ui->tx155RB->setChecked(true);
    m_settings_ui->tx156RB->setChecked(true);
    m_settings_ui->tx157RB->setChecked(true);


}


void SettingsDialog::apply()
{
    updateSettings();

    hide();
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !m_settings_ui->baudRateBox->itemData(idx).isValid();
    m_settings_ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        m_settings_ui->baudRateBox->clearEditText();
        QLineEdit *edit = m_settings_ui->baudRateBox->lineEdit();
        edit->setValidator(m_intValidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !m_settings_ui->serialPortInfoListBox->itemData(idx).isValid();
    m_settings_ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        m_settings_ui->serialPortInfoListBox->clearEditText();
}

void SettingsDialog::fillPortsParameters()
{/*
    m_settings_ui->rx1Box->addItem(QStringLiteral("0"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("1"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("2"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("3"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("4"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("5"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("6"));
    m_settings_ui->rx1Box->addItem(QStringLiteral("7"));
    m_settings_ui->rx1Box->setCurrentIndex(6);
    m_settings_ui->rx2Box->addItem(QStringLiteral("0"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("1"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("2"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("3"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("4"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("5"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("6"));
    m_settings_ui->rx2Box->addItem(QStringLiteral("7"));
    m_settings_ui->rx2Box->setCurrentIndex(6);*/

    m_settings_ui->baudRateBox->addItem(QStringLiteral("230400"), QSerialPort::Baud115200);
    //m_settings_ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    //m_settings_ui->baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    //m_settings_ui->baudRateBox->addItem(tr("Custom"));

    //m_settings_ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    //m_settings_ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    //m_settings_ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_settings_ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    //m_settings_ui->dataBitsBox->setCurrentIndex(3);

    m_settings_ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    m_settings_ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    m_settings_ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    m_settings_ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_settings_ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    m_settings_ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_settings_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_settings_ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_settings_ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    m_settings_ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    m_settings_ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::fillPortsInfo()
{
    m_settings_ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        m_settings_ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    m_settings_ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SettingsDialog::readReply(QByteArray rx_str, QByteArray tx_str)
{
    all_off();

    QList<QByteArray> rx_pgns = rx_str.split(',');

    rx_pgns.removeFirst();
    rx_pgns.removeFirst();

    foreach ( const QByteArray &rx_pgn, rx_pgns)
    {
        if(rx_pgn == "59392") m_settings_ui->rx1RB->setChecked(true);
        else if(rx_pgn == "126464") m_settings_ui->rx8RB->setChecked(true);
        else if(rx_pgn == "126983") m_settings_ui->rx9RB->setChecked(true);
        else if(rx_pgn == "126984") m_settings_ui->rx10RB->setChecked(true);
        else if(rx_pgn == "126985") m_settings_ui->rx11RB->setChecked(true);
        else if(rx_pgn == "126986") m_settings_ui->rx12RB->setChecked(true);
        else if(rx_pgn == "126987") m_settings_ui->rx13RB->setChecked(true);
        else if(rx_pgn == "126988") m_settings_ui->rx14RB->setChecked(true);
        else if(rx_pgn == "126992") m_settings_ui->rx15RB->setChecked(true);
        else if(rx_pgn == "126993") m_settings_ui->rx16RB->setChecked(true);
        else if(rx_pgn == "126996") m_settings_ui->rx17RB->setChecked(true);
        else if(rx_pgn == "126998") m_settings_ui->rx18RB->setChecked(true);
        else if(rx_pgn == "127233") m_settings_ui->rx19RB->setChecked(true);
        else if(rx_pgn == "127237") m_settings_ui->rx20RB->setChecked(true);
        else if(rx_pgn == "127245") m_settings_ui->rx21RB->setChecked(true);
        else if(rx_pgn == "127250") m_settings_ui->rx22RB->setChecked(true);
        else if(rx_pgn == "127251") m_settings_ui->rx23RB->setChecked(true);
        else if(rx_pgn == "127252") m_settings_ui->rx24RB->setChecked(true);
        else if(rx_pgn == "127257") m_settings_ui->rx25RB->setChecked(true);
        else if(rx_pgn == "127258") m_settings_ui->rx26RB->setChecked(true);
        else if(rx_pgn == "127488") m_settings_ui->rx27RB->setChecked(true);
        else if(rx_pgn == "127489") m_settings_ui->rx28RB->setChecked(true);
        else if(rx_pgn == "127493") m_settings_ui->rx29RB->setChecked(true);
        else if(rx_pgn == "127496") m_settings_ui->rx30RB->setChecked(true);
        else if(rx_pgn == "127497") m_settings_ui->rx31RB->setChecked(true);
        else if(rx_pgn == "127498") m_settings_ui->rx32RB->setChecked(true);
        else if(rx_pgn == "127500") m_settings_ui->rx33RB->setChecked(true);
        else if(rx_pgn == "127501") m_settings_ui->rx34RB->setChecked(true);
        else if(rx_pgn == "127502") m_settings_ui->rx35RB->setChecked(true);
        else if(rx_pgn == "127503") m_settings_ui->rx36RB->setChecked(true);
        else if(rx_pgn == "127504") m_settings_ui->rx37RB->setChecked(true);
        else if(rx_pgn == "127505") m_settings_ui->rx38RB->setChecked(true);
        else if(rx_pgn == "127506") m_settings_ui->rx39RB->setChecked(true);
        else if(rx_pgn == "127507") m_settings_ui->rx40RB->setChecked(true);
        else if(rx_pgn == "127508") m_settings_ui->rx41RB->setChecked(true);
        else if(rx_pgn == "127509") m_settings_ui->rx42RB->setChecked(true);
        else if(rx_pgn == "127510") m_settings_ui->rx43RB->setChecked(true);
        else if(rx_pgn == "127511") m_settings_ui->rx44RB->setChecked(true);
        else if(rx_pgn == "127512") m_settings_ui->rx45RB->setChecked(true);
        else if(rx_pgn == "127513") m_settings_ui->rx46RB->setChecked(true);
        else if(rx_pgn == "127514") m_settings_ui->rx47RB->setChecked(true);
        else if(rx_pgn == "127744") m_settings_ui->rx48RB->setChecked(true);
        else if(rx_pgn == "127745") m_settings_ui->rx49RB->setChecked(true);
        else if(rx_pgn == "127746") m_settings_ui->rx50RB->setChecked(true);
        else if(rx_pgn == "127747") m_settings_ui->rx51RB->setChecked(true);
        else if(rx_pgn == "127748") m_settings_ui->rx52RB->setChecked(true);
        else if(rx_pgn == "127749") m_settings_ui->rx53RB->setChecked(true);
        else if(rx_pgn == "127750") m_settings_ui->rx54RB->setChecked(true);
        else if(rx_pgn == "127751") m_settings_ui->rx55RB->setChecked(true);
        else if(rx_pgn == "128259") m_settings_ui->rx56RB->setChecked(true);
        else if(rx_pgn == "128267") m_settings_ui->rx57RB->setChecked(true);
        else if(rx_pgn == "128275") m_settings_ui->rx58RB->setChecked(true);
        else if(rx_pgn == "128520") m_settings_ui->rx59RB->setChecked(true);
        else if(rx_pgn == "129025") m_settings_ui->rx60RB->setChecked(true);
        else if(rx_pgn == "129026") m_settings_ui->rx61RB->setChecked(true);
        else if(rx_pgn == "129027") m_settings_ui->rx62RB->setChecked(true);
        else if(rx_pgn == "129028") m_settings_ui->rx63RB->setChecked(true);
        else if(rx_pgn == "129029") m_settings_ui->rx64RB->setChecked(true);
        else if(rx_pgn == "129033") m_settings_ui->rx65RB->setChecked(true);
        else if(rx_pgn == "129038") m_settings_ui->rx66RB->setChecked(true);
        else if(rx_pgn == "129039") m_settings_ui->rx67RB->setChecked(true);
        else if(rx_pgn == "129040") m_settings_ui->rx68RB->setChecked(true);
        else if(rx_pgn == "129041") m_settings_ui->rx69RB->setChecked(true);
        else if(rx_pgn == "129044") m_settings_ui->rx70RB->setChecked(true);
        else if(rx_pgn == "129045") m_settings_ui->rx71RB->setChecked(true);
        else if(rx_pgn == "129283") m_settings_ui->rx72RB->setChecked(true);
        else if(rx_pgn == "129284") m_settings_ui->rx73RB->setChecked(true);
        else if(rx_pgn == "129285") m_settings_ui->rx74RB->setChecked(true);
        else if(rx_pgn == "129291") m_settings_ui->rx75RB->setChecked(true);
        else if(rx_pgn == "129301") m_settings_ui->rx76RB->setChecked(true);
        else if(rx_pgn == "129302") m_settings_ui->rx77RB->setChecked(true);
        else if(rx_pgn == "129538") m_settings_ui->rx78RB->setChecked(true);
        else if(rx_pgn == "129539") m_settings_ui->rx79RB->setChecked(true);
        else if(rx_pgn == "129540") m_settings_ui->rx80RB->setChecked(true);
        else if(rx_pgn == "129541") m_settings_ui->rx81RB->setChecked(true);
        else if(rx_pgn == "129542") m_settings_ui->rx82RB->setChecked(true);
        else if(rx_pgn == "129545") m_settings_ui->rx83RB->setChecked(true);
        else if(rx_pgn == "129546") m_settings_ui->rx84RB->setChecked(true);
        else if(rx_pgn == "129547") m_settings_ui->rx85RB->setChecked(true);
        else if(rx_pgn == "129549") m_settings_ui->rx86RB->setChecked(true);
        else if(rx_pgn == "129550") m_settings_ui->rx87RB->setChecked(true);
        else if(rx_pgn == "129551") m_settings_ui->rx88RB->setChecked(true);
        else if(rx_pgn == "129556") m_settings_ui->rx89RB->setChecked(true);
        else if(rx_pgn == "129792") m_settings_ui->rx90RB->setChecked(true);
        else if(rx_pgn == "129793") m_settings_ui->rx91RB->setChecked(true);
        else if(rx_pgn == "129794") m_settings_ui->rx92RB->setChecked(true);
        else if(rx_pgn == "129795") m_settings_ui->rx93RB->setChecked(true);
        else if(rx_pgn == "129796") m_settings_ui->rx94RB->setChecked(true);
        else if(rx_pgn == "129797") m_settings_ui->rx95RB->setChecked(true);
        else if(rx_pgn == "129798") m_settings_ui->rx96RB->setChecked(true);
        else if(rx_pgn == "129799") m_settings_ui->rx97RB->setChecked(true);
        else if(rx_pgn == "129800") m_settings_ui->rx98RB->setChecked(true);
        else if(rx_pgn == "129801") m_settings_ui->rx99RB->setChecked(true);
        else if(rx_pgn == "129802") m_settings_ui->rx100RB->setChecked(true);
        else if(rx_pgn == "129803") m_settings_ui->rx101RB->setChecked(true);
        else if(rx_pgn == "129804") m_settings_ui->rx102RB->setChecked(true);
        else if(rx_pgn == "129805") m_settings_ui->rx103RB->setChecked(true);
        else if(rx_pgn == "129806") m_settings_ui->rx104RB->setChecked(true);
        else if(rx_pgn == "129807") m_settings_ui->rx105RB->setChecked(true);
        else if(rx_pgn == "129808") m_settings_ui->rx106RB->setChecked(true);
        else if(rx_pgn == "129809") m_settings_ui->rx107RB->setChecked(true);
        else if(rx_pgn == "129810") m_settings_ui->rx108RB->setChecked(true);
        else if(rx_pgn == "129811") m_settings_ui->rx109RB->setChecked(true);
        else if(rx_pgn == "129812") m_settings_ui->rx110RB->setChecked(true);
        else if(rx_pgn == "129813") m_settings_ui->rx111RB->setChecked(true);
        else if(rx_pgn == "130052") m_settings_ui->rx112RB->setChecked(true);
        else if(rx_pgn == "130053") m_settings_ui->rx113RB->setChecked(true);
        else if(rx_pgn == "130054") m_settings_ui->rx114RB->setChecked(true);
        else if(rx_pgn == "130060") m_settings_ui->rx115RB->setChecked(true);
        else if(rx_pgn == "130061") m_settings_ui->rx116RB->setChecked(true);
        else if(rx_pgn == "130064") m_settings_ui->rx117RB->setChecked(true);
        else if(rx_pgn == "130065") m_settings_ui->rx118RB->setChecked(true);
        else if(rx_pgn == "130066") m_settings_ui->rx119RB->setChecked(true);
        else if(rx_pgn == "130067") m_settings_ui->rx120RB->setChecked(true);
        else if(rx_pgn == "130068") m_settings_ui->rx121RB->setChecked(true);
        else if(rx_pgn == "130069") m_settings_ui->rx122RB->setChecked(true);
        else if(rx_pgn == "130070") m_settings_ui->rx123RB->setChecked(true);
        else if(rx_pgn == "130071") m_settings_ui->rx124RB->setChecked(true);
        else if(rx_pgn == "130072") m_settings_ui->rx125RB->setChecked(true);
        else if(rx_pgn == "130073") m_settings_ui->rx126RB->setChecked(true);
        else if(rx_pgn == "130074") m_settings_ui->rx127RB->setChecked(true);
        else if(rx_pgn == "130306") m_settings_ui->rx128RB->setChecked(true);
        else if(rx_pgn == "130310") m_settings_ui->rx129RB->setChecked(true);
        else if(rx_pgn == "130311") m_settings_ui->rx130RB->setChecked(true);
        else if(rx_pgn == "130312") m_settings_ui->rx131RB->setChecked(true);
        else if(rx_pgn == "130313") m_settings_ui->rx132RB->setChecked(true);
        else if(rx_pgn == "130314") m_settings_ui->rx133RB->setChecked(true);
        else if(rx_pgn == "130315") m_settings_ui->rx134RB->setChecked(true);
        else if(rx_pgn == "130316") m_settings_ui->rx135RB->setChecked(true);
        else if(rx_pgn == "130320") m_settings_ui->rx136RB->setChecked(true);
        else if(rx_pgn == "130321") m_settings_ui->rx137RB->setChecked(true);
        else if(rx_pgn == "130322") m_settings_ui->rx138RB->setChecked(true);
        else if(rx_pgn == "130323") m_settings_ui->rx139RB->setChecked(true);
        else if(rx_pgn == "130324") m_settings_ui->rx140RB->setChecked(true);
        else if(rx_pgn == "130560") m_settings_ui->rx141RB->setChecked(true);
        else if(rx_pgn == "130567") m_settings_ui->rx142RB->setChecked(true);
        else if(rx_pgn == "130569") m_settings_ui->rx143RB->setChecked(true);
        else if(rx_pgn == "130570") m_settings_ui->rx144RB->setChecked(true);
        else if(rx_pgn == "130571") m_settings_ui->rx145RB->setChecked(true);
        else if(rx_pgn == "130572") m_settings_ui->rx146RB->setChecked(true);
        else if(rx_pgn == "130573") m_settings_ui->rx147RB->setChecked(true);
        else if(rx_pgn == "130574") m_settings_ui->rx148RB->setChecked(true);
        else if(rx_pgn == "130576") m_settings_ui->rx149RB->setChecked(true);
        else if(rx_pgn == "130577") m_settings_ui->rx150RB->setChecked(true);
        else if(rx_pgn == "130578") m_settings_ui->rx151RB->setChecked(true);
        else if(rx_pgn == "130580") m_settings_ui->rx152RB->setChecked(true);
        else if(rx_pgn == "130581") m_settings_ui->rx153RB->setChecked(true);
        else if(rx_pgn == "130582") m_settings_ui->rx154RB->setChecked(true);
        else if(rx_pgn == "130583") m_settings_ui->rx155RB->setChecked(true);
        else if(rx_pgn == "130584") m_settings_ui->rx156RB->setChecked(true);
        else if(rx_pgn == "130585") m_settings_ui->rx157RB->setChecked(true);

    }

    QList<QByteArray> tx_pgns = tx_str.split(',');

    tx_pgns.removeFirst();
    tx_pgns.removeFirst();

    foreach ( const QByteArray &tx_pgn, tx_pgns)
    {

        if(tx_pgn == "59904") m_settings_ui->tx2RB->setChecked(true);
        else if(tx_pgn == "60160") m_settings_ui->tx3RB->setChecked(true);
        else if(tx_pgn == "65240") m_settings_ui->tx6RB->setChecked(true);
        else if(tx_pgn == "126983") m_settings_ui->tx9RB->setChecked(true);
        else if(tx_pgn == "126984") m_settings_ui->tx10RB->setChecked(true);
        else if(tx_pgn == "126985") m_settings_ui->tx11RB->setChecked(true);
        else if(tx_pgn == "126986") m_settings_ui->tx12RB->setChecked(true);
        else if(tx_pgn == "126987") m_settings_ui->tx13RB->setChecked(true);
        else if(tx_pgn == "126988") m_settings_ui->tx14RB->setChecked(true);
        else if(tx_pgn == "126992") m_settings_ui->tx15RB->setChecked(true);
        else if(tx_pgn == "127233") m_settings_ui->tx19RB->setChecked(true);
        else if(tx_pgn == "127237") m_settings_ui->tx20RB->setChecked(true);
        else if(tx_pgn == "127245") m_settings_ui->tx21RB->setChecked(true);
        else if(tx_pgn == "127250") m_settings_ui->tx22RB->setChecked(true);
        else if(tx_pgn == "127251") m_settings_ui->tx23RB->setChecked(true);
        else if(tx_pgn == "127252") m_settings_ui->tx24RB->setChecked(true);
        else if(tx_pgn == "127257") m_settings_ui->tx25RB->setChecked(true);
        else if(tx_pgn == "127258") m_settings_ui->tx26RB->setChecked(true);
        else if(tx_pgn == "127488") m_settings_ui->tx27RB->setChecked(true);
        else if(tx_pgn == "127489") m_settings_ui->tx28RB->setChecked(true);
        else if(tx_pgn == "127493") m_settings_ui->tx29RB->setChecked(true);
        else if(tx_pgn == "127496") m_settings_ui->tx30RB->setChecked(true);
        else if(tx_pgn == "127497") m_settings_ui->tx31RB->setChecked(true);
        else if(tx_pgn == "127498") m_settings_ui->tx32RB->setChecked(true);
        else if(tx_pgn == "127500") m_settings_ui->tx33RB->setChecked(true);
        else if(tx_pgn == "127501") m_settings_ui->tx34RB->setChecked(true);
        else if(tx_pgn == "127502") m_settings_ui->tx35RB->setChecked(true);
        else if(tx_pgn == "127503") m_settings_ui->tx36RB->setChecked(true);
        else if(tx_pgn == "127504") m_settings_ui->tx37RB->setChecked(true);
        else if(tx_pgn == "127505") m_settings_ui->tx38RB->setChecked(true);
        else if(tx_pgn == "127506") m_settings_ui->tx39RB->setChecked(true);
        else if(tx_pgn == "127507") m_settings_ui->tx40RB->setChecked(true);
        else if(tx_pgn == "127508") m_settings_ui->tx41RB->setChecked(true);
        else if(tx_pgn == "127509") m_settings_ui->tx42RB->setChecked(true);
        else if(tx_pgn == "127510") m_settings_ui->tx43RB->setChecked(true);
        else if(tx_pgn == "127511") m_settings_ui->tx44RB->setChecked(true);
        else if(tx_pgn == "127512") m_settings_ui->tx45RB->setChecked(true);
        else if(tx_pgn == "127513") m_settings_ui->tx46RB->setChecked(true);
        else if(tx_pgn == "127514") m_settings_ui->tx47RB->setChecked(true);
        else if(tx_pgn == "127744") m_settings_ui->tx48RB->setChecked(true);
        else if(tx_pgn == "127745") m_settings_ui->tx49RB->setChecked(true);
        else if(tx_pgn == "127746") m_settings_ui->tx50RB->setChecked(true);
        else if(tx_pgn == "127747") m_settings_ui->tx51RB->setChecked(true);
        else if(tx_pgn == "127748") m_settings_ui->tx52RB->setChecked(true);
        else if(tx_pgn == "127749") m_settings_ui->tx53RB->setChecked(true);
        else if(tx_pgn == "127750") m_settings_ui->tx54RB->setChecked(true);
        else if(tx_pgn == "127751") m_settings_ui->tx55RB->setChecked(true);
        else if(tx_pgn == "128259") m_settings_ui->tx56RB->setChecked(true);
        else if(tx_pgn == "128267") m_settings_ui->tx57RB->setChecked(true);
        else if(tx_pgn == "128275") m_settings_ui->tx58RB->setChecked(true);
        else if(tx_pgn == "128520") m_settings_ui->tx59RB->setChecked(true);
        else if(tx_pgn == "129025") m_settings_ui->tx60RB->setChecked(true);
        else if(tx_pgn == "129026") m_settings_ui->tx61RB->setChecked(true);
        else if(tx_pgn == "129027") m_settings_ui->tx62RB->setChecked(true);
        else if(tx_pgn == "129028") m_settings_ui->tx63RB->setChecked(true);
        else if(tx_pgn == "129029") m_settings_ui->tx64RB->setChecked(true);
        else if(tx_pgn == "129033") m_settings_ui->tx65RB->setChecked(true);
        else if(tx_pgn == "129038") m_settings_ui->tx66RB->setChecked(true);
        else if(tx_pgn == "129039") m_settings_ui->tx67RB->setChecked(true);
        else if(tx_pgn == "129040") m_settings_ui->tx68RB->setChecked(true);
        else if(tx_pgn == "129041") m_settings_ui->tx69RB->setChecked(true);
        else if(tx_pgn == "129044") m_settings_ui->tx70RB->setChecked(true);
        else if(tx_pgn == "129045") m_settings_ui->tx71RB->setChecked(true);
        else if(tx_pgn == "129283") m_settings_ui->tx72RB->setChecked(true);
        else if(tx_pgn == "129284") m_settings_ui->tx73RB->setChecked(true);
        else if(tx_pgn == "129285") m_settings_ui->tx74RB->setChecked(true);
        else if(tx_pgn == "129291") m_settings_ui->tx75RB->setChecked(true);
        else if(tx_pgn == "129301") m_settings_ui->tx76RB->setChecked(true);
        else if(tx_pgn == "129302") m_settings_ui->tx77RB->setChecked(true);
        else if(tx_pgn == "129538") m_settings_ui->tx78RB->setChecked(true);
        else if(tx_pgn == "129539") m_settings_ui->tx79RB->setChecked(true);
        else if(tx_pgn == "129540") m_settings_ui->tx80RB->setChecked(true);
        else if(tx_pgn == "129541") m_settings_ui->tx81RB->setChecked(true);
        else if(tx_pgn == "129542") m_settings_ui->tx82RB->setChecked(true);
        else if(tx_pgn == "129545") m_settings_ui->tx83RB->setChecked(true);
        else if(tx_pgn == "129546") m_settings_ui->tx84RB->setChecked(true);
        else if(tx_pgn == "129547") m_settings_ui->tx85RB->setChecked(true);
        else if(tx_pgn == "129549") m_settings_ui->tx86RB->setChecked(true);
        else if(tx_pgn == "129550") m_settings_ui->tx87RB->setChecked(true);
        else if(tx_pgn == "129551") m_settings_ui->tx88RB->setChecked(true);
        else if(tx_pgn == "129556") m_settings_ui->tx89RB->setChecked(true);
        else if(tx_pgn == "129792") m_settings_ui->tx90RB->setChecked(true);
        else if(tx_pgn == "129793") m_settings_ui->tx91RB->setChecked(true);
        else if(tx_pgn == "129794") m_settings_ui->tx92RB->setChecked(true);
        else if(tx_pgn == "129795") m_settings_ui->tx93RB->setChecked(true);
        else if(tx_pgn == "129796") m_settings_ui->tx94RB->setChecked(true);
        else if(tx_pgn == "129797") m_settings_ui->tx95RB->setChecked(true);
        else if(tx_pgn == "129798") m_settings_ui->tx96RB->setChecked(true);
        else if(tx_pgn == "129799") m_settings_ui->tx97RB->setChecked(true);
        else if(tx_pgn == "129800") m_settings_ui->tx98RB->setChecked(true);
        else if(tx_pgn == "129801") m_settings_ui->tx99RB->setChecked(true);
        else if(tx_pgn == "129802") m_settings_ui->tx100RB->setChecked(true);
        else if(tx_pgn == "129803") m_settings_ui->tx101RB->setChecked(true);
        else if(tx_pgn == "129804") m_settings_ui->tx102RB->setChecked(true);
        else if(tx_pgn == "129805") m_settings_ui->tx103RB->setChecked(true);
        else if(tx_pgn == "129806") m_settings_ui->tx104RB->setChecked(true);
        else if(tx_pgn == "129807") m_settings_ui->tx105RB->setChecked(true);
        else if(tx_pgn == "129808") m_settings_ui->tx106RB->setChecked(true);
        else if(tx_pgn == "129809") m_settings_ui->tx107RB->setChecked(true);
        else if(tx_pgn == "129810") m_settings_ui->tx108RB->setChecked(true);
        else if(tx_pgn == "129811") m_settings_ui->tx109RB->setChecked(true);
        else if(tx_pgn == "129812") m_settings_ui->tx110RB->setChecked(true);
        else if(tx_pgn == "129813") m_settings_ui->tx111RB->setChecked(true);
        else if(tx_pgn == "130052") m_settings_ui->tx112RB->setChecked(true);
        else if(tx_pgn == "130053") m_settings_ui->tx113RB->setChecked(true);
        else if(tx_pgn == "130054") m_settings_ui->tx114RB->setChecked(true);
        else if(tx_pgn == "130060") m_settings_ui->tx115RB->setChecked(true);
        else if(tx_pgn == "130061") m_settings_ui->tx116RB->setChecked(true);
        else if(tx_pgn == "130064") m_settings_ui->tx117RB->setChecked(true);
        else if(tx_pgn == "130065") m_settings_ui->tx118RB->setChecked(true);
        else if(tx_pgn == "130066") m_settings_ui->tx119RB->setChecked(true);
        else if(tx_pgn == "130067") m_settings_ui->tx120RB->setChecked(true);
        else if(tx_pgn == "130068") m_settings_ui->tx121RB->setChecked(true);
        else if(tx_pgn == "130069") m_settings_ui->tx122RB->setChecked(true);
        else if(tx_pgn == "130070") m_settings_ui->tx123RB->setChecked(true);
        else if(tx_pgn == "130071") m_settings_ui->tx124RB->setChecked(true);
        else if(tx_pgn == "130072") m_settings_ui->tx125RB->setChecked(true);
        else if(tx_pgn == "130073") m_settings_ui->tx126RB->setChecked(true);
        else if(tx_pgn == "130074") m_settings_ui->tx127RB->setChecked(true);
        else if(tx_pgn == "130306") m_settings_ui->tx128RB->setChecked(true);
        else if(tx_pgn == "130310") m_settings_ui->tx129RB->setChecked(true);
        else if(tx_pgn == "130311") m_settings_ui->tx130RB->setChecked(true);
        else if(tx_pgn == "130312") m_settings_ui->tx131RB->setChecked(true);
        else if(tx_pgn == "130313") m_settings_ui->tx132RB->setChecked(true);
        else if(tx_pgn == "130314") m_settings_ui->tx133RB->setChecked(true);
        else if(tx_pgn == "130315") m_settings_ui->tx134RB->setChecked(true);
        else if(tx_pgn == "130316") m_settings_ui->tx135RB->setChecked(true);
        else if(tx_pgn == "130320") m_settings_ui->tx136RB->setChecked(true);
        else if(tx_pgn == "130321") m_settings_ui->tx137RB->setChecked(true);
        else if(tx_pgn == "130322") m_settings_ui->tx138RB->setChecked(true);
        else if(tx_pgn == "130323") m_settings_ui->tx139RB->setChecked(true);
        else if(tx_pgn == "130324") m_settings_ui->tx140RB->setChecked(true);
        else if(tx_pgn == "130560") m_settings_ui->tx141RB->setChecked(true);
        else if(tx_pgn == "130567") m_settings_ui->tx142RB->setChecked(true);
        else if(tx_pgn == "130569") m_settings_ui->tx143RB->setChecked(true);
        else if(tx_pgn == "130570") m_settings_ui->tx144RB->setChecked(true);
        else if(tx_pgn == "130571") m_settings_ui->tx145RB->setChecked(true);
        else if(tx_pgn == "130572") m_settings_ui->tx146RB->setChecked(true);
        else if(tx_pgn == "130573") m_settings_ui->tx147RB->setChecked(true);
        else if(tx_pgn == "130574") m_settings_ui->tx148RB->setChecked(true);
        else if(tx_pgn == "130576") m_settings_ui->tx149RB->setChecked(true);
        else if(tx_pgn == "130577") m_settings_ui->tx150RB->setChecked(true);
        else if(tx_pgn == "130578") m_settings_ui->tx151RB->setChecked(true);
        else if(tx_pgn == "130580") m_settings_ui->tx152RB->setChecked(true);
        else if(tx_pgn == "130581") m_settings_ui->tx153RB->setChecked(true);
        else if(tx_pgn == "130582") m_settings_ui->tx154RB->setChecked(true);
        else if(tx_pgn == "130583") m_settings_ui->tx155RB->setChecked(true);
        else if(tx_pgn == "130584") m_settings_ui->tx156RB->setChecked(true);
        else if(tx_pgn == "130585") m_settings_ui->tx157RB->setChecked(true);

    }

}

void SettingsDialog::updateSettings()
{

    m_currentSettings.name = m_settings_ui->serialPortInfoListBox->currentText();

    if (m_settings_ui->baudRateBox->currentIndex() == 4) {
        m_currentSettings.baudRate = m_settings_ui->baudRateBox->currentText().toInt();
    } else {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_settings_ui->baudRateBox->itemData(m_settings_ui->baudRateBox->currentIndex()).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                m_settings_ui->dataBitsBox->itemData(m_settings_ui->dataBitsBox->currentIndex()).toInt());
    m_currentSettings.stringDataBits = m_settings_ui->dataBitsBox->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                m_settings_ui->parityBox->itemData(m_settings_ui->parityBox->currentIndex()).toInt());
    m_currentSettings.stringParity = m_settings_ui->parityBox->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                m_settings_ui->stopBitsBox->itemData(m_settings_ui->stopBitsBox->currentIndex()).toInt());
    m_currentSettings.stringStopBits = m_settings_ui->stopBitsBox->currentText();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                m_settings_ui->flowControlBox->itemData(m_settings_ui->flowControlBox->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = m_settings_ui->flowControlBox->currentText();

    m_currentSettings.localEchoEnabled = m_settings_ui->localEchoCheckBox->isChecked();

    m_currentSettings.NormalModeEnabled = m_settings_ui->NormalModeCheckBox->isChecked();

    m_currentSettings.Base64Enabled = m_settings_ui->Base64CheckBox->isChecked();

    m_currentSettings.ShowNSMEnabled = m_settings_ui->ShowNSMCheckBox->isChecked();

    m_currentSettings.rx1en = m_settings_ui->rx1RB->isChecked();
    m_currentSettings.rx2en = m_settings_ui->rx2RB->isChecked();
    m_currentSettings.rx3en = m_settings_ui->rx3RB->isChecked();
    m_currentSettings.rx4en = m_settings_ui->rx4RB->isChecked();
    m_currentSettings.rx5en = m_settings_ui->rx5RB->isChecked();
    m_currentSettings.rx6en = m_settings_ui->rx6RB->isChecked();
    m_currentSettings.rx7en = m_settings_ui->rx7RB->isChecked();
    m_currentSettings.rx8en = m_settings_ui->rx8RB->isChecked();
    m_currentSettings.rx9en = m_settings_ui->rx9RB->isChecked();
    m_currentSettings.rx10en = m_settings_ui->rx10RB->isChecked();
    m_currentSettings.rx11en = m_settings_ui->rx11RB->isChecked();
    m_currentSettings.rx12en = m_settings_ui->rx12RB->isChecked();
    m_currentSettings.rx13en = m_settings_ui->rx13RB->isChecked();
    m_currentSettings.rx14en = m_settings_ui->rx14RB->isChecked();
    m_currentSettings.rx15en = m_settings_ui->rx15RB->isChecked();
    m_currentSettings.rx16en = m_settings_ui->rx16RB->isChecked();
    m_currentSettings.rx17en = m_settings_ui->rx17RB->isChecked();
    m_currentSettings.rx18en = m_settings_ui->rx18RB->isChecked();
    m_currentSettings.rx19en = m_settings_ui->rx19RB->isChecked();
    m_currentSettings.rx20en = m_settings_ui->rx20RB->isChecked();
    m_currentSettings.rx21en = m_settings_ui->rx21RB->isChecked();
    m_currentSettings.rx22en = m_settings_ui->rx22RB->isChecked();
    m_currentSettings.rx23en = m_settings_ui->rx23RB->isChecked();
    m_currentSettings.rx24en = m_settings_ui->rx24RB->isChecked();
    m_currentSettings.rx25en = m_settings_ui->rx25RB->isChecked();
    m_currentSettings.rx26en = m_settings_ui->rx26RB->isChecked();
    m_currentSettings.rx27en = m_settings_ui->rx27RB->isChecked();
    m_currentSettings.rx28en = m_settings_ui->rx28RB->isChecked();
    m_currentSettings.rx29en = m_settings_ui->rx29RB->isChecked();
    m_currentSettings.rx30en = m_settings_ui->rx30RB->isChecked();
    m_currentSettings.rx31en = m_settings_ui->rx31RB->isChecked();
    m_currentSettings.rx32en = m_settings_ui->rx32RB->isChecked();
    m_currentSettings.rx33en = m_settings_ui->rx33RB->isChecked();
    m_currentSettings.rx34en = m_settings_ui->rx34RB->isChecked();
    m_currentSettings.rx35en = m_settings_ui->rx35RB->isChecked();
    m_currentSettings.rx36en = m_settings_ui->rx36RB->isChecked();
    m_currentSettings.rx37en = m_settings_ui->rx37RB->isChecked();
    m_currentSettings.rx38en = m_settings_ui->rx38RB->isChecked();
    m_currentSettings.rx39en = m_settings_ui->rx39RB->isChecked();
    m_currentSettings.rx40en = m_settings_ui->rx40RB->isChecked();
    m_currentSettings.rx41en = m_settings_ui->rx41RB->isChecked();
    m_currentSettings.rx42en = m_settings_ui->rx42RB->isChecked();
    m_currentSettings.rx43en = m_settings_ui->rx43RB->isChecked();
    m_currentSettings.rx44en = m_settings_ui->rx44RB->isChecked();
    m_currentSettings.rx45en = m_settings_ui->rx45RB->isChecked();
    m_currentSettings.rx46en = m_settings_ui->rx46RB->isChecked();
    m_currentSettings.rx47en = m_settings_ui->rx47RB->isChecked();
    m_currentSettings.rx48en = m_settings_ui->rx48RB->isChecked();
    m_currentSettings.rx49en = m_settings_ui->rx49RB->isChecked();
    m_currentSettings.rx50en = m_settings_ui->rx50RB->isChecked();
    m_currentSettings.rx51en = m_settings_ui->rx51RB->isChecked();
    m_currentSettings.rx52en = m_settings_ui->rx52RB->isChecked();
    m_currentSettings.rx53en = m_settings_ui->rx53RB->isChecked();
    m_currentSettings.rx54en = m_settings_ui->rx54RB->isChecked();
    m_currentSettings.rx55en = m_settings_ui->rx55RB->isChecked();
    m_currentSettings.rx56en = m_settings_ui->rx56RB->isChecked();
    m_currentSettings.rx57en = m_settings_ui->rx57RB->isChecked();
    m_currentSettings.rx58en = m_settings_ui->rx58RB->isChecked();
    m_currentSettings.rx59en = m_settings_ui->rx59RB->isChecked();
    m_currentSettings.rx60en = m_settings_ui->rx60RB->isChecked();
    m_currentSettings.rx61en = m_settings_ui->rx61RB->isChecked();
    m_currentSettings.rx62en = m_settings_ui->rx62RB->isChecked();
    m_currentSettings.rx63en = m_settings_ui->rx63RB->isChecked();
    m_currentSettings.rx64en = m_settings_ui->rx64RB->isChecked();
    m_currentSettings.rx65en = m_settings_ui->rx65RB->isChecked();
    m_currentSettings.rx66en = m_settings_ui->rx66RB->isChecked();
    m_currentSettings.rx67en = m_settings_ui->rx67RB->isChecked();
    m_currentSettings.rx68en = m_settings_ui->rx68RB->isChecked();
    m_currentSettings.rx69en = m_settings_ui->rx69RB->isChecked();
    m_currentSettings.rx70en = m_settings_ui->rx70RB->isChecked();
    m_currentSettings.rx71en = m_settings_ui->rx71RB->isChecked();
    m_currentSettings.rx72en = m_settings_ui->rx72RB->isChecked();
    m_currentSettings.rx73en = m_settings_ui->rx73RB->isChecked();
    m_currentSettings.rx74en = m_settings_ui->rx74RB->isChecked();
    m_currentSettings.rx75en = m_settings_ui->rx75RB->isChecked();
    m_currentSettings.rx76en = m_settings_ui->rx76RB->isChecked();
    m_currentSettings.rx77en = m_settings_ui->rx77RB->isChecked();
    m_currentSettings.rx78en = m_settings_ui->rx78RB->isChecked();
    m_currentSettings.rx79en = m_settings_ui->rx79RB->isChecked();
    m_currentSettings.rx80en = m_settings_ui->rx80RB->isChecked();
    m_currentSettings.rx81en = m_settings_ui->rx81RB->isChecked();
    m_currentSettings.rx82en = m_settings_ui->rx82RB->isChecked();
    m_currentSettings.rx83en = m_settings_ui->rx83RB->isChecked();
    m_currentSettings.rx84en = m_settings_ui->rx84RB->isChecked();
    m_currentSettings.rx85en = m_settings_ui->rx85RB->isChecked();
    m_currentSettings.rx86en = m_settings_ui->rx86RB->isChecked();
    m_currentSettings.rx87en = m_settings_ui->rx87RB->isChecked();
    m_currentSettings.rx88en = m_settings_ui->rx88RB->isChecked();
    m_currentSettings.rx89en = m_settings_ui->rx89RB->isChecked();
    m_currentSettings.rx90en = m_settings_ui->rx90RB->isChecked();
    m_currentSettings.rx91en = m_settings_ui->rx91RB->isChecked();
    m_currentSettings.rx92en = m_settings_ui->rx92RB->isChecked();
    m_currentSettings.rx93en = m_settings_ui->rx93RB->isChecked();
    m_currentSettings.rx94en = m_settings_ui->rx94RB->isChecked();
    m_currentSettings.rx95en = m_settings_ui->rx95RB->isChecked();
    m_currentSettings.rx96en = m_settings_ui->rx96RB->isChecked();
    m_currentSettings.rx97en = m_settings_ui->rx97RB->isChecked();
    m_currentSettings.rx98en = m_settings_ui->rx98RB->isChecked();
    m_currentSettings.rx99en = m_settings_ui->rx99RB->isChecked();
    m_currentSettings.rx100en = m_settings_ui->rx100RB->isChecked();
    m_currentSettings.rx101en = m_settings_ui->rx101RB->isChecked();
    m_currentSettings.rx102en = m_settings_ui->rx102RB->isChecked();
    m_currentSettings.rx103en = m_settings_ui->rx103RB->isChecked();
    m_currentSettings.rx104en = m_settings_ui->rx104RB->isChecked();
    m_currentSettings.rx105en = m_settings_ui->rx105RB->isChecked();
    m_currentSettings.rx106en = m_settings_ui->rx106RB->isChecked();
    m_currentSettings.rx107en = m_settings_ui->rx107RB->isChecked();
    m_currentSettings.rx108en = m_settings_ui->rx108RB->isChecked();
    m_currentSettings.rx109en = m_settings_ui->rx109RB->isChecked();
    m_currentSettings.rx110en = m_settings_ui->rx110RB->isChecked();
    m_currentSettings.rx111en = m_settings_ui->rx111RB->isChecked();
    m_currentSettings.rx112en = m_settings_ui->rx112RB->isChecked();
    m_currentSettings.rx113en = m_settings_ui->rx113RB->isChecked();
    m_currentSettings.rx114en = m_settings_ui->rx114RB->isChecked();
    m_currentSettings.rx115en = m_settings_ui->rx115RB->isChecked();
    m_currentSettings.rx116en = m_settings_ui->rx116RB->isChecked();
    m_currentSettings.rx117en = m_settings_ui->rx117RB->isChecked();
    m_currentSettings.rx118en = m_settings_ui->rx118RB->isChecked();
    m_currentSettings.rx119en = m_settings_ui->rx119RB->isChecked();
    m_currentSettings.rx120en = m_settings_ui->rx120RB->isChecked();
    m_currentSettings.rx121en = m_settings_ui->rx121RB->isChecked();
    m_currentSettings.rx122en = m_settings_ui->rx122RB->isChecked();
    m_currentSettings.rx123en = m_settings_ui->rx123RB->isChecked();
    m_currentSettings.rx124en = m_settings_ui->rx124RB->isChecked();
    m_currentSettings.rx125en = m_settings_ui->rx125RB->isChecked();
    m_currentSettings.rx126en = m_settings_ui->rx126RB->isChecked();
    m_currentSettings.rx127en = m_settings_ui->rx127RB->isChecked();
    m_currentSettings.rx128en = m_settings_ui->rx128RB->isChecked();
    m_currentSettings.rx129en = m_settings_ui->rx129RB->isChecked();
    m_currentSettings.rx130en = m_settings_ui->rx130RB->isChecked();
    m_currentSettings.rx131en = m_settings_ui->rx131RB->isChecked();
    m_currentSettings.rx132en = m_settings_ui->rx132RB->isChecked();
    m_currentSettings.rx133en = m_settings_ui->rx133RB->isChecked();
    m_currentSettings.rx134en = m_settings_ui->rx134RB->isChecked();
    m_currentSettings.rx135en = m_settings_ui->rx135RB->isChecked();
    m_currentSettings.rx136en = m_settings_ui->rx136RB->isChecked();
    m_currentSettings.rx137en = m_settings_ui->rx137RB->isChecked();
    m_currentSettings.rx138en = m_settings_ui->rx138RB->isChecked();
    m_currentSettings.rx139en = m_settings_ui->rx139RB->isChecked();
    m_currentSettings.rx140en = m_settings_ui->rx140RB->isChecked();
    m_currentSettings.rx141en = m_settings_ui->rx141RB->isChecked();
    m_currentSettings.rx142en = m_settings_ui->rx142RB->isChecked();
    m_currentSettings.rx143en = m_settings_ui->rx143RB->isChecked();
    m_currentSettings.rx144en = m_settings_ui->rx144RB->isChecked();
    m_currentSettings.rx145en = m_settings_ui->rx145RB->isChecked();
    m_currentSettings.rx146en = m_settings_ui->rx146RB->isChecked();
    m_currentSettings.rx147en = m_settings_ui->rx147RB->isChecked();
    m_currentSettings.rx148en = m_settings_ui->rx148RB->isChecked();
    m_currentSettings.rx149en = m_settings_ui->rx149RB->isChecked();
    m_currentSettings.rx150en = m_settings_ui->rx150RB->isChecked();
    m_currentSettings.rx151en = m_settings_ui->rx151RB->isChecked();
    m_currentSettings.rx152en = m_settings_ui->rx152RB->isChecked();
    m_currentSettings.rx153en = m_settings_ui->rx153RB->isChecked();
    m_currentSettings.rx154en = m_settings_ui->rx154RB->isChecked();
    m_currentSettings.rx155en = m_settings_ui->rx155RB->isChecked();
    m_currentSettings.rx156en = m_settings_ui->rx156RB->isChecked();
    m_currentSettings.rx157en = m_settings_ui->rx157RB->isChecked();


    m_currentSettings.tx1en = m_settings_ui->tx1RB->isChecked();
    m_currentSettings.tx2en = m_settings_ui->tx2RB->isChecked();
    m_currentSettings.tx3en = m_settings_ui->tx3RB->isChecked();
    m_currentSettings.tx4en = m_settings_ui->tx4RB->isChecked();
    m_currentSettings.tx5en = m_settings_ui->tx5RB->isChecked();
    m_currentSettings.tx6en = m_settings_ui->tx6RB->isChecked();
    m_currentSettings.tx7en = m_settings_ui->tx7RB->isChecked();
    m_currentSettings.tx8en = m_settings_ui->tx8RB->isChecked();
    m_currentSettings.tx9en = m_settings_ui->tx9RB->isChecked();
    m_currentSettings.tx10en = m_settings_ui->tx10RB->isChecked();
    m_currentSettings.tx11en = m_settings_ui->tx11RB->isChecked();
    m_currentSettings.tx12en = m_settings_ui->tx12RB->isChecked();
    m_currentSettings.tx13en = m_settings_ui->tx13RB->isChecked();
    m_currentSettings.tx14en = m_settings_ui->tx14RB->isChecked();
    m_currentSettings.tx15en = m_settings_ui->tx15RB->isChecked();
    m_currentSettings.tx16en = m_settings_ui->tx16RB->isChecked();
    m_currentSettings.tx17en = m_settings_ui->tx17RB->isChecked();
    m_currentSettings.tx18en = m_settings_ui->tx18RB->isChecked();
    m_currentSettings.tx19en = m_settings_ui->tx19RB->isChecked();
    m_currentSettings.tx20en = m_settings_ui->tx20RB->isChecked();
    m_currentSettings.tx21en = m_settings_ui->tx21RB->isChecked();
    m_currentSettings.tx22en = m_settings_ui->tx22RB->isChecked();
    m_currentSettings.tx23en = m_settings_ui->tx23RB->isChecked();
    m_currentSettings.tx24en = m_settings_ui->tx24RB->isChecked();
    m_currentSettings.tx25en = m_settings_ui->tx25RB->isChecked();
    m_currentSettings.tx26en = m_settings_ui->tx26RB->isChecked();
    m_currentSettings.tx27en = m_settings_ui->tx27RB->isChecked();
    m_currentSettings.tx28en = m_settings_ui->tx28RB->isChecked();
    m_currentSettings.tx29en = m_settings_ui->tx29RB->isChecked();
    m_currentSettings.tx30en = m_settings_ui->tx30RB->isChecked();
    m_currentSettings.tx31en = m_settings_ui->tx31RB->isChecked();
    m_currentSettings.tx32en = m_settings_ui->tx32RB->isChecked();
    m_currentSettings.tx33en = m_settings_ui->tx33RB->isChecked();
    m_currentSettings.tx34en = m_settings_ui->tx34RB->isChecked();
    m_currentSettings.tx35en = m_settings_ui->tx35RB->isChecked();
    m_currentSettings.tx36en = m_settings_ui->tx36RB->isChecked();
    m_currentSettings.tx37en = m_settings_ui->tx37RB->isChecked();
    m_currentSettings.tx38en = m_settings_ui->tx38RB->isChecked();
    m_currentSettings.tx39en = m_settings_ui->tx39RB->isChecked();
    m_currentSettings.tx40en = m_settings_ui->tx40RB->isChecked();
    m_currentSettings.tx41en = m_settings_ui->tx41RB->isChecked();
    m_currentSettings.tx42en = m_settings_ui->tx42RB->isChecked();
    m_currentSettings.tx43en = m_settings_ui->tx43RB->isChecked();
    m_currentSettings.tx44en = m_settings_ui->tx44RB->isChecked();
    m_currentSettings.tx45en = m_settings_ui->tx45RB->isChecked();
    m_currentSettings.tx46en = m_settings_ui->tx46RB->isChecked();
    m_currentSettings.tx47en = m_settings_ui->tx47RB->isChecked();
    m_currentSettings.tx48en = m_settings_ui->tx48RB->isChecked();
    m_currentSettings.tx49en = m_settings_ui->tx49RB->isChecked();
    m_currentSettings.tx50en = m_settings_ui->tx50RB->isChecked();
    m_currentSettings.tx51en = m_settings_ui->tx51RB->isChecked();
    m_currentSettings.tx52en = m_settings_ui->tx52RB->isChecked();
    m_currentSettings.tx53en = m_settings_ui->tx53RB->isChecked();
    m_currentSettings.tx54en = m_settings_ui->tx54RB->isChecked();
    m_currentSettings.tx55en = m_settings_ui->tx55RB->isChecked();
    m_currentSettings.tx56en = m_settings_ui->tx56RB->isChecked();
    m_currentSettings.tx57en = m_settings_ui->tx57RB->isChecked();
    m_currentSettings.tx58en = m_settings_ui->tx58RB->isChecked();
    m_currentSettings.tx59en = m_settings_ui->tx59RB->isChecked();
    m_currentSettings.tx60en = m_settings_ui->tx60RB->isChecked();
    m_currentSettings.tx61en = m_settings_ui->tx61RB->isChecked();
    m_currentSettings.tx62en = m_settings_ui->tx62RB->isChecked();
    m_currentSettings.tx63en = m_settings_ui->tx63RB->isChecked();
    m_currentSettings.tx64en = m_settings_ui->tx64RB->isChecked();
    m_currentSettings.tx65en = m_settings_ui->tx65RB->isChecked();
    m_currentSettings.tx66en = m_settings_ui->tx66RB->isChecked();
    m_currentSettings.tx67en = m_settings_ui->tx67RB->isChecked();
    m_currentSettings.tx68en = m_settings_ui->tx68RB->isChecked();
    m_currentSettings.tx69en = m_settings_ui->tx69RB->isChecked();
    m_currentSettings.tx70en = m_settings_ui->tx70RB->isChecked();
    m_currentSettings.tx71en = m_settings_ui->tx71RB->isChecked();
    m_currentSettings.tx72en = m_settings_ui->tx72RB->isChecked();
    m_currentSettings.tx73en = m_settings_ui->tx73RB->isChecked();
    m_currentSettings.tx74en = m_settings_ui->tx74RB->isChecked();
    m_currentSettings.tx75en = m_settings_ui->tx75RB->isChecked();
    m_currentSettings.tx76en = m_settings_ui->tx76RB->isChecked();
    m_currentSettings.tx77en = m_settings_ui->tx77RB->isChecked();
    m_currentSettings.tx78en = m_settings_ui->tx78RB->isChecked();
    m_currentSettings.tx79en = m_settings_ui->tx79RB->isChecked();
    m_currentSettings.tx80en = m_settings_ui->tx80RB->isChecked();
    m_currentSettings.tx81en = m_settings_ui->tx81RB->isChecked();
    m_currentSettings.tx82en = m_settings_ui->tx82RB->isChecked();
    m_currentSettings.tx83en = m_settings_ui->tx83RB->isChecked();
    m_currentSettings.tx84en = m_settings_ui->tx84RB->isChecked();
    m_currentSettings.tx85en = m_settings_ui->tx85RB->isChecked();
    m_currentSettings.tx86en = m_settings_ui->tx86RB->isChecked();
    m_currentSettings.tx87en = m_settings_ui->tx87RB->isChecked();
    m_currentSettings.tx88en = m_settings_ui->tx88RB->isChecked();
    m_currentSettings.tx89en = m_settings_ui->tx89RB->isChecked();
    m_currentSettings.tx90en = m_settings_ui->tx90RB->isChecked();
    m_currentSettings.tx91en = m_settings_ui->tx91RB->isChecked();
    m_currentSettings.tx92en = m_settings_ui->tx92RB->isChecked();
    m_currentSettings.tx93en = m_settings_ui->tx93RB->isChecked();
    m_currentSettings.tx94en = m_settings_ui->tx94RB->isChecked();
    m_currentSettings.tx95en = m_settings_ui->tx95RB->isChecked();
    m_currentSettings.tx96en = m_settings_ui->tx96RB->isChecked();
    m_currentSettings.tx97en = m_settings_ui->tx97RB->isChecked();
    m_currentSettings.tx98en = m_settings_ui->tx98RB->isChecked();
    m_currentSettings.tx99en = m_settings_ui->tx99RB->isChecked();
    m_currentSettings.tx100en = m_settings_ui->tx100RB->isChecked();
    m_currentSettings.tx101en = m_settings_ui->tx101RB->isChecked();
    m_currentSettings.tx102en = m_settings_ui->tx102RB->isChecked();
    m_currentSettings.tx103en = m_settings_ui->tx103RB->isChecked();
    m_currentSettings.tx104en = m_settings_ui->tx104RB->isChecked();
    m_currentSettings.tx105en = m_settings_ui->tx105RB->isChecked();
    m_currentSettings.tx106en = m_settings_ui->tx106RB->isChecked();
    m_currentSettings.tx107en = m_settings_ui->tx107RB->isChecked();
    m_currentSettings.tx108en = m_settings_ui->tx108RB->isChecked();
    m_currentSettings.tx109en = m_settings_ui->tx109RB->isChecked();
    m_currentSettings.tx110en = m_settings_ui->tx110RB->isChecked();
    m_currentSettings.tx111en = m_settings_ui->tx111RB->isChecked();
    m_currentSettings.tx112en = m_settings_ui->tx112RB->isChecked();
    m_currentSettings.tx113en = m_settings_ui->tx113RB->isChecked();
    m_currentSettings.tx114en = m_settings_ui->tx114RB->isChecked();
    m_currentSettings.tx115en = m_settings_ui->tx115RB->isChecked();
    m_currentSettings.tx116en = m_settings_ui->tx116RB->isChecked();
    m_currentSettings.tx117en = m_settings_ui->tx117RB->isChecked();
    m_currentSettings.tx118en = m_settings_ui->tx118RB->isChecked();
    m_currentSettings.tx119en = m_settings_ui->tx119RB->isChecked();
    m_currentSettings.tx120en = m_settings_ui->tx120RB->isChecked();
    m_currentSettings.tx121en = m_settings_ui->tx121RB->isChecked();
    m_currentSettings.tx122en = m_settings_ui->tx122RB->isChecked();
    m_currentSettings.tx123en = m_settings_ui->tx123RB->isChecked();
    m_currentSettings.tx124en = m_settings_ui->tx124RB->isChecked();
    m_currentSettings.tx125en = m_settings_ui->tx125RB->isChecked();
    m_currentSettings.tx126en = m_settings_ui->tx126RB->isChecked();
    m_currentSettings.tx127en = m_settings_ui->tx127RB->isChecked();
    m_currentSettings.tx128en = m_settings_ui->tx128RB->isChecked();
    m_currentSettings.tx129en = m_settings_ui->tx129RB->isChecked();
    m_currentSettings.tx130en = m_settings_ui->tx130RB->isChecked();
    m_currentSettings.tx131en = m_settings_ui->tx131RB->isChecked();
    m_currentSettings.tx132en = m_settings_ui->tx132RB->isChecked();
    m_currentSettings.tx133en = m_settings_ui->tx133RB->isChecked();
    m_currentSettings.tx134en = m_settings_ui->tx134RB->isChecked();
    m_currentSettings.tx135en = m_settings_ui->tx135RB->isChecked();
    m_currentSettings.tx136en = m_settings_ui->tx136RB->isChecked();
    m_currentSettings.tx137en = m_settings_ui->tx137RB->isChecked();
    m_currentSettings.tx138en = m_settings_ui->tx138RB->isChecked();
    m_currentSettings.tx139en = m_settings_ui->tx139RB->isChecked();
    m_currentSettings.tx140en = m_settings_ui->tx140RB->isChecked();
    m_currentSettings.tx141en = m_settings_ui->tx141RB->isChecked();
    m_currentSettings.tx142en = m_settings_ui->tx142RB->isChecked();
    m_currentSettings.tx143en = m_settings_ui->tx143RB->isChecked();
    m_currentSettings.tx144en = m_settings_ui->tx144RB->isChecked();
    m_currentSettings.tx145en = m_settings_ui->tx145RB->isChecked();
    m_currentSettings.tx146en = m_settings_ui->tx146RB->isChecked();
    m_currentSettings.tx147en = m_settings_ui->tx147RB->isChecked();
    m_currentSettings.tx148en = m_settings_ui->tx148RB->isChecked();
    m_currentSettings.tx149en = m_settings_ui->tx149RB->isChecked();
    m_currentSettings.tx150en = m_settings_ui->tx150RB->isChecked();
    m_currentSettings.tx151en = m_settings_ui->tx151RB->isChecked();
    m_currentSettings.tx152en = m_settings_ui->tx152RB->isChecked();
    m_currentSettings.tx153en = m_settings_ui->tx153RB->isChecked();
    m_currentSettings.tx154en = m_settings_ui->tx154RB->isChecked();
    m_currentSettings.tx155en = m_settings_ui->tx155RB->isChecked();
    m_currentSettings.tx156en = m_settings_ui->tx156RB->isChecked();
    m_currentSettings.tx157en = m_settings_ui->tx157RB->isChecked();

    m_settings_ui->tx1RB->setChecked(true);
    m_settings_ui->tx4RB->setChecked(true);
    m_settings_ui->tx5RB->setChecked(true);
    m_settings_ui->tx7RB->setChecked(true);
    m_settings_ui->tx8RB->setChecked(true);
    m_settings_ui->tx16RB->setChecked(true);
    m_settings_ui->tx17RB->setChecked(true);
    m_settings_ui->tx18RB->setChecked(true);

    m_settings_ui->rx2RB->setChecked(true);
    m_settings_ui->rx3RB->setChecked(true);
    m_settings_ui->rx4RB->setChecked(true);
    m_settings_ui->rx5RB->setChecked(true);
    m_settings_ui->rx6RB->setChecked(true);
    m_settings_ui->rx7RB->setChecked(true);

    m_settings_ui->tx1RB->setEnabled(false);
    m_settings_ui->tx4RB->setEnabled(false);
    m_settings_ui->tx5RB->setEnabled(false);
    m_settings_ui->tx7RB->setEnabled(false);
    m_settings_ui->tx8RB->setEnabled(false);
    m_settings_ui->tx16RB->setEnabled(false);
    m_settings_ui->tx17RB->setEnabled(false);
    m_settings_ui->tx18RB->setEnabled(false);

    m_settings_ui->rx2RB->setEnabled(false);
    m_settings_ui->rx3RB->setEnabled(false);
    m_settings_ui->rx4RB->setEnabled(false);
    m_settings_ui->rx5RB->setEnabled(false);
    m_settings_ui->rx6RB->setEnabled(false);
    m_settings_ui->rx7RB->setEnabled(false);

    m_settings_ui->rx1RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx4RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx2RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx3RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx5RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx6RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx7RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx8RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx16RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx17RB->setStyleSheet("QCheckBox { color : black; }");
    m_settings_ui->rx18RB->setStyleSheet("QCheckBox { color : black; }");

    m_currentSettings.CSV = m_settings_ui->filename_label->text();

    m_currentSettings.repeat = m_settings_ui->repeat_spin->value();

}



void SettingsDialog::on_browseCSV_pressed()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open CSV file"),"/path/to/file/",tr("CSV Files (*.csv)"));
    m_settings_ui->filename_label->setText(filename);
}
