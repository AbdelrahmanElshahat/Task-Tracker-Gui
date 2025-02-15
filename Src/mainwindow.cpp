#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QInputDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mTasks()

{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    loadTasks();
    updateStatus();

}
void MainWindow::closeEvent(QCloseEvent *event)
{
    backUp();
    event->accept();
}


void MainWindow::addTask()
{
    bool ok;
    QString time;
    QString name = QInputDialog::getText(this,
            tr("Add Task"),
            tr("Task name"),
            QLineEdit::Normal,
            tr("Untitled task"), &ok);
            if (ok && !name.isEmpty()) {
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);
            char buffer[80];
            std::strftime(buffer, 80, "Added at  %H:%M", now);
            Task* task = new Task(name, QString(buffer));
            connect(task,&Task::removed,this,&MainWindow::removeTask);
            connect(task,&Task::statusChanged,this,&MainWindow::taskStatusChanged);
            mTasks.append(task);
            ui->taskLayout->addWidget(task);
            updateStatus();
            }
}
void MainWindow::removeTask(Task* task ){

    mTasks.removeOne(task);
    ui->taskLayout->removeWidget(task);
    task->setParent(0);
    delete task;
    updateStatus();

}
void MainWindow::taskStatusChanged(Task*  ){

    updateStatus();
}
void MainWindow::updateStatus(){
    int completedCount = 0;
    for(auto t:mTasks){
        if(t->isCompleted()){
            completedCount++;
        }
    }
    int todoCount = mTasks.size()-completedCount;
    ui->statusLabel->setText(
        QString("Tasks Status :\n     %1 To Do \n     %2  Completed")
        .arg(todoCount)
        .arg(completedCount));
}
void MainWindow::loadTasks(){
    QFile file("tasks.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Failed to open file for reading";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    if(doc.isNull()){
        qDebug()<<"failed to parse";
        return;
    }
    QJsonArray tasksArray = doc.array();
    QVBoxLayout *taskLayout = ui->taskLayout;
    QLayoutItem* item;
    while ((item = taskLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    for (const QJsonValue &taskValue : tasksArray){
        QJsonObject taskobj = taskValue.toObject();
        QString name = taskobj["Name"].toString();
        QString time = taskobj["Time"].toString();
        bool status = taskobj["Status"].toBool();

        // Create Task object using your custom Task class
        Task* task = new Task(name, time);
        task->setCompleted(status);

        connect(task, &Task::removed, this, &MainWindow::removeTask);
        connect(task, &Task::statusChanged, this, &MainWindow::taskStatusChanged);

        mTasks.append(task);
        taskLayout->addWidget(task);
    }
    file.close();
    updateStatus();
}
void MainWindow::backUp(){
    QJsonArray tasksArray;
    QVBoxLayout* tasklayout = ui->taskLayout;

    for (int i = 0; i < tasklayout->count(); ++i) {
        QWidget *widget = tasklayout->itemAt(i)->widget();
        Task* task = qobject_cast<Task*>(widget);

        if (task) {
            QJsonObject taskObj;
            taskObj["Name"] = task->name();
            taskObj["Status"] = task->isCompleted();
            taskObj["Time"] = task->time();
            tasksArray.append(taskObj);
        }
    }

    QJsonDocument doc(tasksArray);
    QFile file("tasks.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
