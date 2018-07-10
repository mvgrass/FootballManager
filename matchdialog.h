#ifndef MATCHDIALOG_H
#define MATCHDIALOG_H
#include <QDialog>
#include "mainwindow.h"
#include "matchmodel.h"
#include "player.h"
#include "playersstatisticmodel.h"
#include "statisticdialog.h"

namespace Ui {
class MatchDialog;
}

class MatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatchDialog(MainWindow* parent);

    MatchDialog(const Match&, MainWindow* parent);

    MatchDialog(Match&, const QList<Player>&, MainWindow* parent);

    void setModel(MatchModel*);

    ~MatchDialog();

signals:
    void successAdd();
    void customFinished(QDialog*);

private slots:
    void on_matchOkButton_clicked();
    void on_finish();

    void on_resultComboBox_currentIndexChanged(int index);

    void on_addStatsButton_clicked();

    void on_editStatsButton_clicked();

    void on_removeStatsButton_clicked();

private:
    Ui::MatchDialog *ui;
    MatchModel* model;
    Match* match = nullptr;
    PlayersStatisticModel* playersModel = nullptr;
    const QList<Player>& players;
    MainWindow* parent;
};

#endif // MATCHDIALOG_H
