#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>
#include<ctime>
namespace Ui {
class Task;
}

class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(const QString& name,const QString& time,QWidget *parent = nullptr);

    ~Task();
    void setName(const QString& name);
    void setTime(const QString& time);
    QString name() const;
    QString time() const;
    bool isCompleted() const;
    void setCompleted(bool completed);
        public slots:
           void rename();
signals :
           void removed(Task* task);
           void statusChanged(Task* task);
private slots:
           void checked(bool checked);
private:
    Ui::Task *ui;
};

#endif // TASK_H
