#include "debugger.h"
#include "mainwindow.h"
#include <QGridLayout>

debugger::debugger(QWidget *parent):QDialog (parent)
{
    this->setMinimumSize(600,400);
    run_button = new QPushButton("Run");
    continue_button = new QPushButton("Continue");
    next_button = new QPushButton("Next");
    step_button = new QPushButton("Step");
    clear_allbreak_button = new QPushButton("Clear All Breakpoints");
    show_var_button = new QPushButton("Show Variables");
    send_toGDB_button = new QPushButton("GDB Debugging");
    show_var_edit = new QLineEdit();
    send_toGDB_edit = new QLineEdit();
    debug_brower = new QTextBrowser();

    QGridLayout *grid1 = new QGridLayout();
    grid1->addWidget(run_button,0,0);
    grid1->addWidget(continue_button,0,1);
    grid1->addWidget(next_button,1,0);
    grid1->addWidget(step_button,1,1);
    grid1->addWidget(clear_allbreak_button,2,0);

    QGridLayout *grid2 = new QGridLayout();
    grid2->addWidget(show_var_button,0,0);
    grid2->addWidget(show_var_edit,0,1);
    grid2->addWidget(send_toGDB_button,1,0);
    grid2->addWidget(send_toGDB_edit,1,1);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addWidget(debug_brower);
    main_layout->addLayout(grid1);
    main_layout->addLayout(grid2);

    this->setLayout(main_layout);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    connect(clear_allbreak_button,SIGNAL(clicked()),this,SLOT(on_clearAllMarker_button_clicked()));
    connect(run_button,SIGNAL(clicked()),this,SLOT(on_run_button_clicked()));
    connect(continue_button,SIGNAL(clicked()),this,SLOT(on_continue_button_clicked()));
    connect(next_button,SIGNAL(clicked()),this,SLOT(on_next_button_clicked()));
    connect(step_button,SIGNAL(clicked()),this,SLOT(on_step_button_clicked()));
    connect(show_var_button,SIGNAL(clicked()),this,SLOT(on_show_var_button_clicked()));
    connect(send_toGDB_button,SIGNAL(clicked()),this,SLOT(on_sendtoGDB_button_clicked()));

}

debugger::~debugger()
{
    this->close();
}

void debugger::on_clearAllMarker_button_clicked()
{
    emit SendClearMarkerToMain();
}

void debugger::on_run_button_clicked()
{
    QString str = "run\r\n";
    emit SendDebugCmmandToMain(str);
}

void debugger::on_continue_button_clicked()
{
    QString str = "continue\r\n";
    emit SendDebugCmmandToMain(str);
}

void debugger::on_next_button_clicked()
{
    QString str = "next\r\n";
    emit SendDebugCmmandToMain(str);
}

void debugger::on_step_button_clicked()
{
    debug_brower->append("Line Number");
    QString str = "step\r\n";
    emit SendDebugCmmandToMain(str);
}

void debugger::on_show_var_button_clicked()
{
    debug_brower->append("Value and Data Type of Variable: "+show_var_edit->text());
    QString str = "print "+this->show_var_edit->text()+"\r\n";
    str = str+"whatis "+show_var_edit->text()+"\r\n";
    emit SendDebugCmmandToMain(str);
}

void debugger::on_sendtoGDB_button_clicked()
{
    QString str = this->send_toGDB_edit->text()+"\r\n";
    emit SendDebugCmmandToMain(str);
}
