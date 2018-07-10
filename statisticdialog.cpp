#include "statisticdialog.h"
#include "ui_statisticdialog.h"
#include <QMessageBox>

StatisticDialog::StatisticDialog(int match_id, int player_id, const QString title, QWidget *parent):
    QDialog(parent),
    ui(new Ui::StatisticDialog),
    player_id(player_id),
    match_id(match_id)
{
    ui->setupUi(this);

    ui->titleLabel->setText(title);

    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

StatisticDialog::StatisticDialog(const Statistic &stats, const QString title, QWidget *parent):
    QDialog(parent),
    ui(new Ui::StatisticDialog),
    player_id(stats.player_id),
    match_id(stats.match_id)
{

    ui->setupUi(this);

    ui->titleLabel->setText(title);

    ui->goalSpinBox->setValue(stats.goals);
    ui->assistsSpinBox->setValue(stats.assists);
    ui->minutesSpinBox->setValue(stats.minutes);
    ui->yellowSpinBox->setValue(stats.yellows);
    ui->redSpinBox->setValue(stats.reds);

    isNew = false;

    connect(this, SIGNAL(successAdd()), this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void StatisticDialog::setModel(PlayersStatisticModel *m)
{
    this->model = m;
}

StatisticDialog::~StatisticDialog()
{
    delete ui;
}

void StatisticDialog::on_okButton_clicked()
{
    try{
        if(isNew){

            this->model->insert(Statistic(match_id,
                                          player_id,
                                          ui->goalSpinBox->value(),
                                          ui->assistsSpinBox->value(),
                                          ui->minutesSpinBox->value(),
                                          ui->yellowSpinBox->value(),
                                          ui->redSpinBox->value(),
                                          "",
                                          QDate(),
                                          0,
                                          ""));
        }else{
            this->model->updateStatistic(Statistic(match_id,
                                                   player_id,
                                                   ui->goalSpinBox->value(),
                                                   ui->assistsSpinBox->value(),
                                                   ui->minutesSpinBox->value(),
                                                   ui->yellowSpinBox->value(),
                                                   ui->redSpinBox->value(),
                                                   "",
                                                   QDate(),
                                                   0,
                                                   ""));
        }

        emit successAdd();
    }catch(std::exception& err){
        std::string str(err.what());
        QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
    }
}
