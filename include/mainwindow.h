#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <task.h>
#include <cstdlib>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateStatus();
public slots:
    void addTask();
    void removeTask(Task* task);
    void taskStatusChanged(Task* task);
    void loadTasks();
    void backUp();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
    Task *ts;
    QVector<Task*> mTasks;
};
#endif // MAINWINDOW_H
