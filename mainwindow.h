#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databaseutil.h"
#include "playermodel.h"
#include "matchmodel.h"
#include "matchesstatisticmodel.h"
#include "playersstatisticmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionCreate_triggered();

    void on_addPlayerButton_clicked();

    void on_actionOpen_triggered();

    void on_actionDelete_triggered();

    void on_editPlayerButton_clicked();

    void on_dropPlayerButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_addMatchButton_clicked();

    void on_matchDeleteButton_clicked();

    void deleteWindow(QDialog*);

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_editMatchButton_clicked();

    void aggregationUpdate();

    void on_actionDev_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    PlayerModel* player_model = nullptr;
    MatchModel* match_model = nullptr;
    const QString DB_DIR = "Databases";
    const QString DB_EXTENSION = ".mdb";
    DataBaseUtil* db = nullptr;
    QSet<PlayersStatisticModel*> players_statistics;
    QSet<MatchesStatisticModel*> matches_statistics;

friend class PlayerDialog;
friend class MatchDialog;
};

#endif // MAINWINDOW_H
