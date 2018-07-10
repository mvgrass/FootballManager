#include "playerdialog.h"
#include "ui_playerdialog.h"
#include <QMessageBox>
#include <QPushButton>

PlayerDialog::PlayerDialog(MainWindow* parent) :
    QDialog(parent),
    ui(new Ui::PlayerDialog),
    parent(parent)
{
    ui->setupUi(this);
    ui->TitleNameLabel->hide();
    ui->tableView->hide();
    ui->matchesLabel->hide();
    ui->goalsLabel->hide();
    ui->assistsLabel->hide();
    ui->minutesLabel->hide();
    ui->yellowLabel->hide();
    ui->redLabel->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();


    ui->positionComboBox->addItems({"GK", "DEF", "MID", "FWR"});

    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

PlayerDialog::PlayerDialog(const Player & player, MainWindow* parent):
    QDialog(parent),
    ui(new Ui::PlayerDialog),
    parent(parent)
{
    ui->setupUi(this);

    ui->positionComboBox->addItems({"GK", "DEF", "MID", "FWR"});

    ui->namLabel->hide();
    ui->nameEdit->hide();
    ui->okButton->hide();
    ui->cancelButton->hide();
    ui->nameEdit->setEnabled(false);
    ui->birthSpinBox->setEnabled(false);
    ui->heightSpinBox->setEnabled(false);
    ui->weightSpinBox->setEnabled(false);
    ui->positionComboBox->setEnabled(false);
    ui->numberSpinBox->setEnabled(false);

    ui->TitleNameLabel->setText(QString(player.getName().c_str()));
    ui->numberSpinBox->setValue(player.getNumber());
    ui->birthSpinBox->setValue(player.getBirth());
    ui->heightSpinBox->setValue(player.getHeight());
    ui->weightSpinBox->setValue(player.getWeight());
    ui->positionComboBox->setCurrentIndex(player.getPos());

    matchesModel = new MatchesStatisticModel(player.getId(), this);
    matchesModel->setDB(parent->db);
    connect(matchesModel, SIGNAL(changed()), this, SLOT(update()));

    ui->tableView->setModel(matchesModel);
    matchesModel->update();

    connect(parent->match_model, SIGNAL(changed()), matchesModel, SLOT(update()));
    for(PlayersStatisticModel* model : parent->players_statistics)
        connect(matchesModel, SIGNAL(changed()), model, SLOT(update()));

    parent->matches_statistics.insert(matchesModel);

    connect(this, SIGNAL(finished(int)), this, SLOT(on_finished()));
}

PlayerDialog::PlayerDialog(Player & player, MainWindow* parent):
    QDialog(parent),
    ui(new Ui::PlayerDialog),
    player(&player),
    parent(parent)
{
    ui->setupUi(this);

    ui->positionComboBox->addItems({"GK", "DEF", "MID", "FWR"});

    ui->TitleNameLabel->setText(QString(player.getName().c_str()));
    ui->nameEdit->setText(ui->TitleNameLabel->text());
    ui->numberSpinBox->setValue(player.getNumber());
    ui->birthSpinBox->setValue(player.getBirth());
    ui->heightSpinBox->setValue(player.getHeight());
    ui->weightSpinBox->setValue(player.getWeight());
    ui->positionComboBox->setCurrentIndex(player.getPos());

    matchesModel = new MatchesStatisticModel(player.getId(), this);
    matchesModel->setDB(parent->db);
    connect(matchesModel, SIGNAL(changed()), this, SLOT(update()));

    ui->tableView->setModel(matchesModel);
    matchesModel->update();

    connect(parent->match_model, SIGNAL(changed()), matchesModel, SLOT(update()));
    for(PlayersStatisticModel* model : parent->players_statistics)
        connect(matchesModel, SIGNAL(changed()), model, SLOT(update()));

    parent->matches_statistics.insert(matchesModel);

    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void PlayerDialog::setModel(PlayerModel *m)
{
    this->model = m;
}


PlayerDialog::~PlayerDialog()
{
    parent->matches_statistics.remove(matchesModel);
    delete ui;
    delete matchesModel;
}

void PlayerDialog::on_okButton_clicked()
{
    if(ui->nameEdit->text()==""){
        QMessageBox::warning(this, "Error", "Заполните поле имя");
        return;
    }

    try{
        if(player == nullptr){
            model->insert(Player(ui->numberSpinBox->value(),
                             ui->nameEdit->text().toUtf8().constData(),
                             ui->birthSpinBox->value(),
                             ui->weightSpinBox->value(),
                             ui->heightSpinBox->value(),
                             ui->positionComboBox->currentIndex()));
        }else{
            model->updatePlayer(Player(ui->numberSpinBox->value(),
                                 ui->nameEdit->text().toUtf8().constData(),
                                 ui->birthSpinBox->value(),
                                 ui->weightSpinBox->value(),
                                 ui->heightSpinBox->value(),
                                 ui->positionComboBox->currentIndex(),
                                 player->getId()));
        }

        emit successAdd();
    }catch(std::exception& err){
        std::string str(err.what());
        QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
    }
}

void PlayerDialog::on_finished()
{
    emit customFinished(this);
}

void PlayerDialog::update()
{
    ui->goalsLabel->setText(QString::number(matchesModel->getGoals()));
    ui->assistsLabel->setText(QString::number(matchesModel->getAssists()));
    ui->minutesLabel->setText(QString::number(matchesModel->getMinutes()));
    ui->yellowLabel->setText(QString::number(matchesModel->getYellows()));
    ui->redLabel->setText(QString::number(matchesModel->getReds()));
}
