#include "task.h"
#include "ui_task.h"
#include <QInputDialog>
Task::Task(const QString& name,const QString& time,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Task)
{
    ui->setupUi(this);
    setName(name);
    setTime(time);
    connect(ui->editButton,&QPushButton::clicked,this,&Task::rename);
    connect(ui->removeButton,&QPushButton::clicked,[this]{
        emit removed(this);
    });
    connect(ui->checkBox,&QCheckBox::toggled,this,&Task::checked);
}

Task::~Task()
{
    delete ui;
}

void Task::setName(const QString& name){
    ui->checkBox->setText(name);
}
void Task::setTime(const QString& time){
    ui->timeAdded->setText(time);
}
QString Task::name() const{
    return ui->checkBox->text();
}
QString Task::time() const {
    return ui->timeAdded->text();
}
bool Task::isCompleted() const{
    return ui->checkBox->isChecked();
}
void Task::rename(){
    bool ok;
    QString value = QInputDialog::getText(this,
                                         tr("Edit Task"),
                                         tr("Task name"),
                                          QLineEdit::Normal,
                                          this->name(),&ok                                          );
    if(ok&&!value.isEmpty()){
        setName(value);
    }
}
void Task::checked(bool checked){
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    ui->timeAdded->setFont(font);

    emit statusChanged(this);
}
void Task::setCompleted(bool completed ){
    ui->checkBox->setChecked(completed);
}
