#include "matchdialog.h"
#include "ui_matchdialog.h"
#include <QMessageBox>
#include <QList>
#include <QInputDialog>
#include <QString>

MatchDialog::MatchDialog(MainWindow* parent) :
    QDialog(parent),
    ui(new Ui::MatchDialog),
    players({}),
    parent(parent)
{
    ui->setupUi(this);

    ui->resultComboBox->addItems({"Победа", "Поражение", "Ничья", "Не играли"});
    ui->resultComboBox->setCurrentIndex(3);

    ui->titleMatchLabel->hide();
    ui->label->hide();
    ui->tableView->hide();
    ui->addStatsButton->hide();
    ui->editStatsButton->hide();
    ui->removeStatsButton->hide();


    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->matchCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

MatchDialog::MatchDialog(const Match & match, MainWindow* parent):
    QDialog(parent),
    ui(new Ui::MatchDialog),
    players({}),
    parent(parent)
{
    ui->setupUi(this);

    ui->resultComboBox->addItems({"Победа", "Поражение", "Ничья", "Не играли"});

    ui->opponentLabel->hide();
    ui->opponentLineEdit->hide();
    ui->addStatsButton->hide();
    ui->editStatsButton->hide();
    ui->removeStatsButton->hide();
    ui->matchCancelButton->hide();
    ui->matchOkButton->hide();

    ui->titleMatchLabel->setText(match.opponent);
    ui->DateEdit->setDate(match.date);
    ui->resultComboBox->setCurrentIndex(match.result);
    ui->goalSpinBox->setValue(match.goals);
    ui->missedSpinBox->setValue(match.goalsMissed);

    ui->DateEdit->setEnabled(false);
    ui->resultComboBox->setEnabled(false);
    ui->goalSpinBox->setEnabled(false);
    ui->missedSpinBox->setEnabled(false);

    playersModel = new PlayersStatisticModel(match.id);
    playersModel->setDB(parent->db);
    ui->tableView->setModel(playersModel);
    playersModel->update();

    connect(parent->player_model, SIGNAL(changed()), playersModel, SLOT(update()));
    for(MatchesStatisticModel* model : parent->matches_statistics)
        connect(playersModel, SIGNAL(changed()), model, SLOT(update()));

    parent->players_statistics.insert(playersModel);

    connect(parent->player_model, SIGNAL(changed()), playersModel, SLOT(update()));

}

MatchDialog::MatchDialog(Match & match, const QList<Player>& existsPlayers, MainWindow* parent):
    QDialog(parent),
    ui(new Ui::MatchDialog),
    match(&match),
    players(existsPlayers),
    parent(parent)
{
    ui->setupUi(this);

    ui->resultComboBox->addItems({"Победа", "Поражение", "Ничья", "Не играли"});

    ui->titleMatchLabel->setText(match.opponent);
    ui->opponentLineEdit->setText(match.opponent);
    ui->DateEdit->setDate(match.date);
    ui->resultComboBox->setCurrentIndex(match.result);
    ui->goalSpinBox->setValue(match.goals);
    ui->missedSpinBox->setValue(match.goalsMissed);

    playersModel = new PlayersStatisticModel(match.id);
    playersModel->setDB(parent->db);
    ui->tableView->setModel(playersModel);
    playersModel->update();

    connect(parent->player_model, SIGNAL(changed()), playersModel, SLOT(update()));
    for(MatchesStatisticModel* model : parent->matches_statistics)
        connect(playersModel, SIGNAL(changed()), model, SLOT(update()));

    parent->players_statistics.insert(playersModel);

    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->matchCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void MatchDialog::setModel(MatchModel* m)
{
    this->model = m;
}

MatchDialog::~MatchDialog()
{
    parent->players_statistics.remove(playersModel);
    delete ui;
    delete playersModel;
}

void MatchDialog::on_matchOkButton_clicked()
{
    if(ui->opponentLineEdit->text()==""){
        QMessageBox::warning(this, "Error", "Заполните поле Соперник");
        return;
    }

    switch (ui->resultComboBox->currentIndex()) {
    case Match::Result::WIN:
        if(ui->goalSpinBox->value()<=ui->missedSpinBox->value()){
            QMessageBox::warning(this, "Error", "Счет не соответствует результату");
            return;
        }
        break;
    case Match::Result::LOSE:
        if(ui->goalSpinBox->value()>=ui->missedSpinBox->value()){
            QMessageBox::warning(this, "Error", "Счет не соответствует результату");
            return;
        }
        break;
    case Match::Result::TIE:
        if(ui->goalSpinBox->value()!=ui->missedSpinBox->value()){
            QMessageBox::warning(this, "Error", "Счет не соответствует результату");
            return;
        }
        break;
    }


    try{
        if(match==nullptr)
            model->insert(Match(ui->DateEdit->date(),
                            ui->opponentLineEdit->text(),
                            ui->resultComboBox->currentIndex(),
                            ui->goalSpinBox->value(),
                            ui->missedSpinBox->value()));
        else
            model->updateMatch(Match(ui->DateEdit->date(),
                                     ui->opponentLineEdit->text(),
                                     ui->resultComboBox->currentIndex(),
                                     ui->goalSpinBox->value(),
                                     ui->missedSpinBox->value(),
                                     match->id));

        emit successAdd();
    }catch(std::exception& err){
        std::string str(err.what());
        QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
    }
}

void MatchDialog::on_finish()
{
    emit customFinished(this);
}

void MatchDialog::on_resultComboBox_currentIndexChanged(int index)
{
    if(index<3){
        ui->goalSpinBox->setEnabled(true);
        ui->missedSpinBox->setEnabled(true);
    }
    else{
        ui->goalSpinBox->setEnabled(false);
        ui->goalSpinBox->setValue(0);

        ui->missedSpinBox->setEnabled(false);
        ui->missedSpinBox->setValue(0);
    }
}

void MatchDialog::on_addStatsButton_clicked()
{
    QStringList items;
    QVector<const Player*> positions;
    QSet <int> alreadyInMatch;
    alreadyInMatch = playersModel->getPlayers();

    for(const Player& player: players){
        if(alreadyInMatch.find(player.getId())==alreadyInMatch.end()){
            items.append(QString::number(player.getNumber()).append(", ").append(QString(player.getName().c_str())));
            positions.append(&player);
        }
    }


    QInputDialog* choosePlayerDialog = new QInputDialog(this);
    choosePlayerDialog->setOption(QInputDialog::UseListViewForComboBoxItems);
    choosePlayerDialog->setComboBoxItems(items);
    choosePlayerDialog->setWindowTitle("Выбор игрока");
    choosePlayerDialog->setLabelText("Выберите игрока");

    if(choosePlayerDialog->exec()){
        const Player* choosed = positions.at(items.indexOf(choosePlayerDialog->textValue()));

        QString title = QString(choosed->getName().c_str()).append(
                                "\n").append(match->opponent).append("\n").append(match->date.toString("dd.MM.yyyy"));

        StatisticDialog* dialog = new StatisticDialog(match->id, choosed->getId(), title, this);
        dialog->setModel((PlayersStatisticModel*)ui->tableView->model());
        dialog->exec();
        delete dialog;
    }

    delete choosePlayerDialog;
}

void MatchDialog::on_editStatsButton_clicked()
{
    if(ui->tableView->selectionModel()!=nullptr&&!ui->tableView->selectionModel()->selectedRows().empty()){
         try{
             const Statistic& stat = ((PlayersStatisticModel*)ui->tableView->model())->getPlayer(ui->tableView->selectionModel()->selectedRows().first());
             QString title = QString(stat.playerName).append(
                                     "\n").append(stat.opponentName).append("\n").append(stat.matchDate.toString("dd.MM.yyyy"));

             StatisticDialog* dialog = new StatisticDialog(stat, title,this);
             dialog->setModel((PlayersStatisticModel*)ui->tableView->model());
             dialog->exec();
             delete dialog;
         }catch(std::exception& exc){
             std::string str(exc.what());
             QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
         }
     }
}

void MatchDialog::on_removeStatsButton_clicked()
{
    if(ui->tableView->selectionModel()!=nullptr&&!ui->tableView->selectionModel()->selectedRows().empty()){
         try{
            ((PlayersStatisticModel*)ui->tableView->model())->remove(ui->tableView->selectionModel()->selectedRows().first());
         }catch(std::exception& exc){
             std::string str(exc.what());
             QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
         }
     }
}
