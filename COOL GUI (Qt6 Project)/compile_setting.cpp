#include "compile_setting.h"
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLayoutItem>

compile_setting::compile_setting()
{
    //this->setMinimumSize(600,400);
    QLabel *codeForm = new QLabel;
    codeForm->setText("Generated Program File Type");
    QLabel *multiFile = new QLabel;
    multiFile->setText("Generate Multiple Files");
    QLabel *matchStateSiloMaxCapacity = new QLabel;
    matchStateSiloMaxCapacity->setText("Maximum Number of Inference States/Transformation Pairs");
    QLabel *codeTableSiloMaxCapacity = new QLabel;
    codeTableSiloMaxCapacity->setText("Maximum Number of Code Segments for Inference");
    QLabel *maxSwitchTurn = new QLabel;
    maxSwitchTurn->setText("Maximum Length of Inference Path/Synthesis Path");
    QLabel *debugVersion = new QLabel;
    debugVersion->setText("Generate Debug Version Program");
    QLabel *execute = new QLabel;
    execute->setText("Execute Code Immediately After Generation");
    QLabel *debugMode = new QLabel;
    debugMode->setText("Apply Debug Mode During Execution");
    QLabel *debugInfoOutputPath = new QLabel;
    debugInfoOutputPath->setText("Name of Debug Information Output Pipe");
    QLabel *debugOrderInputPath = new QLabel;
    debugOrderInputPath->setText("Name of Debug Command Input Pipe");

    codeForm_cos = new QRadioButton("Generate COS Code");
    codeForm_coc = new QRadioButton("Generate COC Code");
    codeForm_cob = new QRadioButton("Generate COB Code");

    multiFile_0 = new QRadioButton("Generate a Single File");
    multiFile_1 = new QRadioButton("Generate Multiple Files");

    debugVersion_0 = new QRadioButton("Generate Release Version Code (Not Debuggable)");
    debugVersion_1 = new QRadioButton("Generate Debug Version Code (Debuggable)");

    execute_0 = new QRadioButton("Do Not Execute Generated Code");
    execute_1 = new QRadioButton("Execute Generated Code");

    debugMode_0 = new QRadioButton("Do Not Use Debug Mode During Execution");
    debugMode_1 = new QRadioButton("Use Debug Mode During Execution");

    debugInfoOutputPath_0 = new QRadioButton("Path for Debug Information Output Pipe");
    debugOrderInputPath_0 = new QRadioButton("Path for Debug Command Input Pipe");

    matchStateSiloMaxCapacity_num = new QSpinBox;
    matchStateSiloMaxCapacity_num->setValue(5);
    matchStateSiloMaxCapacity_num->setSingleStep(1);
    matchStateSiloMaxCapacity_num->setRange(1,50);

    codeTableSiloMaxCapacity_num = new QSpinBox;
    codeTableSiloMaxCapacity_num->setValue(5);
    codeTableSiloMaxCapacity_num->setSingleStep(1);
    codeTableSiloMaxCapacity_num->setRange(1,50);

    maxSwitchTurn_num = new QSpinBox;
    maxSwitchTurn_num->setValue(5);
    maxSwitchTurn_num->setSingleStep(1);
    maxSwitchTurn_num->setRange(1,50);

    ok_button = new QPushButton("OK");
    cancel_button = new QPushButton("Cancel");
    compile_list = new QListWidget;
    compile_list->addItem("Generated Program File Type");
    compile_list->addItem("Generate Multiple Files");
    compile_list->addItem("Maximum Number of Inference States/Transformation Pairs");
    compile_list->addItem("Maximum Number of Code Segments for Inference");
    compile_list->addItem("Maximum Length of Inference Path/Synthesis Path");
    compile_list->addItem("Generate Debug Version Program");
    compile_list->addItem("Execute Program After Generation");
    compile_list->addItem("Apply Debug Mode During Execution");
    compile_list->addItem("Name of Debug Information Output Pipe");
    compile_list->addItem("Name of Debug Command Input Pipe");
    compile_list->setMaximumHeight(200);
    compile_list->setMidLineWidth(20);

//    QGridLayout *grid1 = new QGridLayout();
//    QVBoxLayout *v_layout = new QVBoxLayout();

    G_layout = new QGridLayout();
    option0_layout = new QVBoxLayout();
    option1_layout = new QVBoxLayout();
    option2_layout = new QVBoxLayout();
    option3_layout = new QVBoxLayout();
    option4_layout = new QVBoxLayout();
    option5_layout = new QVBoxLayout();
    option6_layout = new QVBoxLayout();
    option7_layout = new QVBoxLayout();
    option8_layout = new QVBoxLayout();

    option0_layout->addWidget(codeForm_cos);
    option0_layout->addWidget(codeForm_coc);
    option0_layout->addWidget(codeForm_cob);
    option0_layout->addWidget(multiFile_0);
    option0_layout->addWidget(multiFile_1);
    option2_layout->addWidget(matchStateSiloMaxCapacity_num);
    option3_layout->addWidget(codeTableSiloMaxCapacity_num);
    option4_layout->addWidget(maxSwitchTurn_num);
    option5_layout->addWidget(debugVersion_0);
    option5_layout->addWidget(debugVersion_1);
    option6_layout->addWidget(execute_0);
    option6_layout->addWidget(execute_1);
    option7_layout->addWidget(debugMode_0);
    option7_layout->addWidget(debugMode_1);
    option8_layout->addWidget(debugInfoOutputPath_0);
    option8_layout->addWidget(debugOrderInputPath_0);




    G_layout->addWidget(compile_list,0,0);
    G_layout->addLayout(option0_layout,0,1);
    G_layout->addWidget(ok_button,1,0);
    G_layout->addWidget(cancel_button,1,1);

    this->setLayout(G_layout);

    connect(cancel_button,SIGNAL(clicked()),this,SLOT(on_cancel_button_clicked()));
    connect(compile_list,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(on_list_cilcked(QListWidgetItem *)));


}

void compile_setting::on_cancel_button_clicked()
{
    this->close();
}

void compile_setting::on_list_cilcked(QListWidgetItem *item)
{
    QLayoutItem *child;
    switch (compile_list->currentRow())
    {

    case 0:
    {
        while((child = G_layout->itemAtPosition(0,1)->layout()->takeAt(0)) != 0)
        {
            child->widget()->hide();
        }
        codeForm_cos->show();
        codeForm_coc->show();
        codeForm_cob->show();
        break;}
    case 1:
        while((child = G_layout->itemAtPosition(0,1)->layout()->takeAt(0)) != 0)
        {
            child->widget()->hide();
        }
        multiFile_0->show();
        multiFile_1->show();
        break;
    case 2:
        G_layout->itemAtPosition(0,1)->layout()->setParent(0);
        G_layout = new QGridLayout();
        G_layout->addWidget(compile_list,0,0);
        G_layout->addLayout(option2_layout,0,1);
        G_layout->addWidget(ok_button,1,0);
        G_layout->addWidget(cancel_button,1,1);
        this->layout()->update();
        break;
    }

}
