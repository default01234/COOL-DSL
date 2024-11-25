#include "search.h"
#include <QLayout>
#include <QVBoxLayout>
search::search(QWidget *parent):QDialog (parent)
{
    //this->setFixedSize(800,600);//设置新建文件对话窗口大小
    search_word = new QLineEdit();
    replace_word = new QLineEdit();
    search_label = new QLabel("Search");
    QLabel *replace_label = new QLabel("Replace");
    opt_label = new QLabel("Options");

    direction_label = new QLabel("Direction");
    begin_label = new QLabel("From");
    search_button = new QPushButton("Search");
    cancel_button = new QPushButton("Cancel");
    replace_button = new QPushButton("Replace");
    replace_all_button = new QPushButton("Replace All");
    opt_1 = new QCheckBox("Case Sensitive");
    opt_2 = new QCheckBox("Whole Word");
    search_forward = new QRadioButton("Previous");
    search_backward = new QRadioButton("Next");
    begin_head_button = new QRadioButton("Beginning");
    begin_here_button = new QRadioButton("Cursor");

    QHBoxLayout *Hbox1 = new QHBoxLayout();
    Hbox1->addWidget(search_label);
    Hbox1->addWidget(search_word);

    QHBoxLayout *Hbox4 = new QHBoxLayout();
    Hbox4->addWidget(replace_label);
    Hbox4->addWidget(replace_word);

    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(opt_1);
    vbox1->addWidget(opt_2);
    QGroupBox *groupbox1 = new QGroupBox;
    groupbox1->setLayout(vbox1);
    groupbox1->setTitle("Options");

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(search_forward);
    vbox2->addWidget(search_backward);
    QGroupBox *groupbox2 = new QGroupBox;
    groupbox2->setLayout(vbox2);
    groupbox2->setTitle("Direction");

    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(begin_head_button);
    vbox3->addWidget(begin_here_button);
    QGroupBox *groupbox3 = new QGroupBox;
    groupbox3->setLayout(vbox3);
    groupbox3->setTitle("From");

    QHBoxLayout *Hbox2 = new QHBoxLayout();
    Hbox2->addWidget(groupbox1);
    Hbox2->addWidget(groupbox2);
    Hbox2->addWidget(groupbox3);

    QHBoxLayout *Hbox3 = new QHBoxLayout();
    Hbox3->addWidget(search_button);
    Hbox3->addWidget(replace_button);
    Hbox3->addWidget(replace_all_button);
    Hbox3->addWidget(cancel_button);

    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
    MainVlayout->addLayout(Hbox1);
    MainVlayout->addLayout(Hbox4);
    MainVlayout->addLayout(Hbox2);
    MainVlayout->addLayout(Hbox3);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    connect(this->search_button,SIGNAL(clicked()),this,SLOT(on_search_button_clicked()));
    connect(this->cancel_button,SIGNAL(clicked()),this,SLOT(on_cancel_button_clicked()));
    connect(this->replace_button,SIGNAL(clicked()),this,SLOT(on_replace_button_clicked()));
    connect(this->replace_all_button,SIGNAL(clicked()),this,SLOT(on_replace_all_button_clicked()));
}

search::~search()
{
    this->close();
}

void search::on_search_button_clicked()
{
    QString to_search_str = this->search_word->text();
    int state = 0, begin = 0;
    if(this->opt_1->isChecked()){//全字匹配
        state += 1;
    }
    if(this->opt_2->isChecked()){//区分大小写
        state += 2;
    }
    if(this->search_backward->isChecked()){
        state -= 4;
    }
    if(this->begin_here_button->isChecked()){
        begin = 1;
    }
    emit sendSearchDataToMain(to_search_str,state,begin);
}

void search::on_replace_button_clicked()
{
    QString to_replace_str = this->replace_word->text();
    emit sendReplaceDataToMain(to_replace_str);
}

void search::on_replace_all_button_clicked()
{
    QString to_replace_str = this->replace_word->text();
    emit sendReplaceAllDataToMain(to_replace_str);
}

void search::on_cancel_button_clicked()
{
    search_word->clear();
    replace_word->clear();
    this->close();
}
