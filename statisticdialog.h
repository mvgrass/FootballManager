#ifndef STATISTICDIALOG_H
#define STATISTICDIALOG_H

#include <QDialog>
#include "playersstatisticmodel.h"
#include "player.h"
#include "match.h"
#include "statistic.h"

namespace Ui {
class StatisticDialog;
}

class StatisticDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticDialog(int match_id, int player_id, const QString title, QWidget *parent);

    StatisticDialog(const Statistic& stats, const QString title, QWidget *parent = 0);

    void setModel(PlayersStatisticModel* m);

    ~StatisticDialog();

private slots:
    void on_okButton_clicked();

signals:
    successAdd();

private:
    Ui::StatisticDialog *ui;
    PlayersStatisticModel* model;
    int match_id;
    int player_id;
    bool isNew = true;
};

#endif // STATISTICDIALOG_H
