#ifndef PLAYERDIALOG_H
#define PLAYERDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "mainwindow.h"
#include "player.h"
#include "playermodel.h"
#include "matchesstatisticmodel.h"
#include "databaseutil.h"

namespace Ui {
class PlayerDialog;
}

class PlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerDialog(MainWindow* parent);

    PlayerDialog(const Player&, MainWindow* parent);

    PlayerDialog(Player&, MainWindow* parent);

    void setModel(PlayerModel* m);

    ~PlayerDialog();

signals:
    void successAdd();
    void customFinished(QDialog*);

private slots:
    void on_okButton_clicked();
    void on_finished();
    void update();

private:
    Ui::PlayerDialog *ui;
    PlayerModel* model;
    Player* player = nullptr;
    DataBaseUtil* db = nullptr;
    MatchesStatisticModel* matchesModel = nullptr;
    MainWindow* parent;
};

#endif // PLAYERDIALOG_H
