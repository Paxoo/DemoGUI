#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>
#include <GUI\Data\match.h>
#include <QPushButton>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    float mapRatio = 1.285; // 1024 / 800 = 1.28

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void create_roundButtons(int rounds);
    void color_roundButtons(QPushButton* button, QString team);
    void set_staticText(int round);

    void on_actionOpen_Demo_triggered();

    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_actionStats_2_triggered();

    void on_roundButton_5_clicked();

    void on_roundButton_clicked();

    void on_roundButton_2_clicked();

    void on_roundButton_3_clicked();

    void on_roundButton_4_clicked();

    void on_roundButton_6_clicked();

    void on_roundButton_7_clicked();

    void on_roundButton_8_clicked();

    void on_roundButton_9_clicked();

    void on_roundButton_10_clicked();

    void on_roundButton_11_clicked();

    void on_roundButton_12_clicked();

    void on_roundButton_13_clicked();

    void on_roundButton_14_clicked();

    void on_roundButton_15_clicked();

    void on_roundButton_16_clicked();

    void on_roundButton_17_clicked();

    void on_roundButton_18_clicked();

    void on_roundButton_19_clicked();

    void on_roundButton_20_clicked();

    void on_roundButton_21_clicked();

    void on_roundButton_22_clicked();

    void on_roundButton_23_clicked();

    void on_roundButton_24_clicked();

    void on_roundButton_25_clicked();

    void on_roundButton_26_clicked();

    void on_roundButton_27_clicked();

    void on_roundButton_28_clicked();

    void on_roundButton_29_clicked();

    void on_roundButton_30_clicked();

    void on_roundButton_31_clicked();

    void on_roundButton_32_clicked();

    void on_roundButton_33_clicked();

    void on_roundButton_34_clicked();

    void on_roundButton_35_clicked();

    void on_roundButton_36_clicked();

    void on_roundButton_37_clicked();

    void on_roundButton_38_clicked();

    void on_roundButton_39_clicked();

    void on_roundButton_40_clicked();

    void on_roundButton_41_clicked();

    void on_roundButton_42_clicked();

    void on_roundButton_43_clicked();

    void on_roundButton_44_clicked();

    void on_roundButton_45_clicked();

    void on_roundButton_46_clicked();

    void on_roundButton_47_clicked();

    void on_roundButton_48_clicked();

    void on_actionFreezetimeEndEquipValue_triggered();

    void on_actionGithub_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;
    Match *pMatch = nullptr;

    QParallelAnimationGroup *group = nullptr;
    bool gDemoParsed = false;
    QList<QPushButton*> listRoundButton;
};
#endif // MAINWINDOW_H
