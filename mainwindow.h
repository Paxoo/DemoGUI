#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QParallelAnimationGroup>
#include <GUI\Data\match.h>

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
    void on_actionOpen_Demo_triggered();

    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_actionStats_2_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;
    Match *pMatch = nullptr;

    QParallelAnimationGroup *group = nullptr;
    bool gDemoParsed = false;
};
#endif // MAINWINDOW_H
