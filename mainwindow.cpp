#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"
#include "playerdialog.h"
#include "match.h"
#include "matchdialog.h"
#include "aggregatesqlquerymodel.h"
#include "aboutdialog.h"
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Создаем в корне директории программы папку, где будут храниться базы данных
    if(!QDir(DB_DIR).exists()){
        if(!QDir().mkdir(DB_DIR))
            QMessageBox::warning(this,
                                 "Warning!",
                                 "Не удается создать папку для хранения базы данных! "
                                 "Для корректной работы в корневой папке программы в ручную создайте паку " + DB_DIR);
    }

    db = new DataBaseUtil();

    player_model = new PlayerModel();
    player_model->setDB(db);
    ui->tableView->setModel(player_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    match_model = new MatchModel();
    match_model->setDB(db);
    ui->tableView_2->setModel(match_model);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    aggregateSqlQueryModel* playersStats = new aggregateSqlQueryModel("SELECT name as Имя, number as Номер, SUM(goals) as Голы,"
                                                                      " SUM(assists) as ГП, SUM(yellow) as ЖК, SUM(red) as КК,"
                                                                      "SUM(minutes) as Минуты "
                                                                      "from players join statistic on "
                                                                      "players.id = statistic.player_id GROUP BY players.id;",
                                                               this);
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(playersStats);

    ui->tableView_3->setModel(proxyModel);
    ui->tableView_3->setSortingEnabled(true);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(player_model, SIGNAL(changed()), playersStats, SLOT(update()));
    connect(match_model, SIGNAL(changed()), playersStats, SLOT(update()));

    connect(match_model, SIGNAL(changed()), this, SLOT(aggregationUpdate()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete player_model;
    delete match_model;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionCreate_triggered()
{
    QString db_name = "";
    QString db_dir = "";
    bool ok;

    db_dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"), DB_DIR);

    if(db_dir=="")
        return;

    db_name = QInputDialog::getText(0,
                                    "Ввод",
                                    "Введите название базы данных",
                                    QLineEdit::Normal,
                                    "",
                                    &ok);



    if(ok){

        if(QFile(db_dir+QDir::separator()+db_name+DB_EXTENSION).exists()){
            QMessageBox::warning(this, "Error", "В выбранной директории уже существует файл с данным именем");
            return;
        }

        try{
            db->connectToDB(db_dir+QDir::separator()+db_name+DB_EXTENSION);
            db->initDB();
            player_model->update();
            match_model->update();
            this->setWindowTitle("FootballManager - "+db_name.append(DB_EXTENSION));
        }catch(std::exception& err){
            std::string str(err.what());
            QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
            this->setWindowTitle("FootballManager");
        }
    }

}


void MainWindow::on_actionOpen_triggered()
{
    QString db_name = QFileDialog::getOpenFileName(0, "Open", DB_DIR, "*"+DB_EXTENSION);

    if (db_name!=""){
        try{
            db->connectToDB(db_name);
            player_model->update();
            match_model->update();
            this->setWindowTitle("FootballManager - "+QUrl(db_name).fileName());

        }catch(std::exception& err){
            std::string str(err.what());
            QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
            this->setWindowTitle("FootballManager");
        }
    }
}


void MainWindow::on_actionDelete_triggered()
{
    QUrl db_url = QFileDialog::getOpenFileUrl(0, "Open", DB_DIR, "*"+DB_EXTENSION);

    if(db_url.path()=="")
        return;

    QFile file(DB_DIR+QDir::separator()+db_url.fileName());


    if (!(file.exists()&&file.remove())){
        if(!QFile(db_url.path()+db_url.fileName()).remove())
            QMessageBox::warning(this, "Error", "Ошибка удаления файла");
    }

}


void MainWindow::on_addPlayerButton_clicked()
{
    PlayerDialog* dialog = new PlayerDialog(this);
    dialog->setModel(player_model);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_editPlayerButton_clicked()
{
   if(ui->tableView->selectionModel()!=nullptr&&!ui->tableView->selectionModel()->selectedRows().empty()){
        try{
            Player& player = player_model->getPlayer(ui->tableView->selectionModel()->selectedRows().first());
            PlayerDialog* dialog = new PlayerDialog(player, this);
            dialog->setModel(player_model);
            dialog->exec();
            delete dialog;
        }catch(std::exception& exc){
            std::string str(exc.what());
            QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
        }
    }
}

void MainWindow::on_dropPlayerButton_clicked()
{
    if(ui->tableView->selectionModel()!=nullptr&&!ui->tableView->selectionModel()->selectedRows().empty())
        player_model->remove(ui->tableView->selectionModel()->selectedRows().first());
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    try{
        const Player& player = player_model->getPlayer(index);
        PlayerDialog* dialog = new PlayerDialog(player, this);
        connect(dialog, SIGNAL(customFinished(QDialog*)), this, SLOT(deleteWindow(QDialog*)));
        dialog->show();
    }catch(std::exception& exc){
        std::string str(exc.what());
        QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
    }
}

void MainWindow::on_addMatchButton_clicked()
{
    MatchDialog* dialog = new MatchDialog(this);
    dialog->setModel(match_model);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_matchDeleteButton_clicked()
{
    if(ui->tableView_2->selectionModel()!=nullptr&&!ui->tableView_2->selectionModel()->selectedRows().empty())
        match_model->remove(ui->tableView_2->selectionModel()->selectedRows().first());
}

void MainWindow::deleteWindow(QDialog* dialog)
{
    delete dialog;
    dialog = nullptr;
}

void MainWindow::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    try{
        const Match& match = match_model->getMatch(index);
        MatchDialog* dialog = new MatchDialog(match, this);
        connect(dialog, SIGNAL(customFinished(QDialog*)), this, SLOT(deleteWindow(QDialog*)));
        dialog->show();
    }catch(std::exception& exc){
        std::string str(exc.what());
        QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
    }
}

void MainWindow::on_editMatchButton_clicked()
{
    if(ui->tableView_2->selectionModel()!=nullptr&&!ui->tableView_2->selectionModel()->selectedRows().empty()){
         try{
             Match& match = match_model->getMatch(ui->tableView_2->selectionModel()->selectedRows().first());
             MatchDialog* dialog = new MatchDialog(match, player_model->getPlayers() ,this);
             dialog->setModel(match_model);
             dialog->exec();
             delete dialog;
         }catch(std::exception& exc){
             std::string str(exc.what());
             QMessageBox::warning(this, "Error", QString::fromUtf8(str.c_str()));
         }
     }
}

void MainWindow::aggregationUpdate()
{
    ui->winsLabel->setText(QString::number(match_model->getWins()));
    ui->tiesLabel->setText(QString::number(match_model->getTies()));
    ui->loosesLabel->setText(QString::number(match_model->getLoses()));
}

void MainWindow::on_actionDev_triggered()
{
      QMessageBox::information(this, "Разработчик", "Студент СПбГЭТУ\nГрасс Максим\nhttps:/github.com/mvgrass");
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog* dialog = new AboutDialog(this);
    dialog->exec();
    delete dialog;
}
