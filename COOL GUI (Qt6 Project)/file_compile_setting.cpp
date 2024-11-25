#include "file_compile_setting.h"
#include "ui_file_compile_setting.h"
#include <QDebug>
#include <QFile>
#include <QString>

file_compile_setting::file_compile_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_compile_setting)
{
    ui->setupUi(this);
}

file_compile_setting::~file_compile_setting()
{
    delete ui;
}

void file_compile_setting::on_ok_button_clicked()
{
    emit receive_compile_setting();

}

/**
 * @brief file_compile_setting::receive_setting_path this function receive coolang_project_path and generate a .setting file in it.
 * @param coolang_project_path selfevident.
 */
using namespace Qt;
void file_compile_setting::receive_setting_path(QString coolang_project_path)
{

    QString setting_content_line;
    QString setting_file_path;
    QString setting_file_name;
    setting_file_name = coolang_project_path.section("/",-1,-1) + ".setting";
    setting_file_path = coolang_project_path + "/" + setting_file_name;

    //create setting file
    QFile f(setting_file_path);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("Failed to open file");
    }
    else {
        qDebug() << ("File created successfully");
    }
    QTextStream txtOutput(&f);

    //IDE版本
    setting_content_line = "version:COOLANG2022";
    txtOutput<<setting_content_line<<endl;
    //代码类型类型
    if(ui->codetype_combobox->currentIndex() == 0)
    {
        setting_content_line = "codeForm:1";
        txtOutput<<setting_content_line<<endl;
    }
    else if (ui->codetype_combobox->currentIndex() == 1) {
        setting_content_line = "codeForm:2";
        txtOutput<<setting_content_line<<endl;
    }
    else if (ui->codetype_combobox->currentIndex() == 2) {
        setting_content_line = "codeForm:3";
        txtOutput<<setting_content_line<<endl;
    }

    //是否生成多文件,默认单文件
    if(ui->MultiFile_yes->isChecked())
    {
        setting_content_line = "multiFile:1";
        txtOutput<<setting_content_line<<endl;
    }
    else {
        setting_content_line = "multiFile:0";
        txtOutput<<setting_content_line<<endl;
    }

    //推理状态最大数目
    setting_content_line = "matchStateSiloMaxCapacity:"+QString::number(ui->ZhuangTai_max_num_spinBox->value());
    txtOutput<<setting_content_line<<endl;
    //推理用代码段最大数目
    setting_content_line = "codeTableSiloMaxCapacity:"+QString::number(ui->DaiMa_max_num_spinBox->value());
    txtOutput<<setting_content_line<<endl;
    //推理轮数上限
    setting_content_line = "maxSwitchTurn:"+QString::number(ui->LunShu_max_num_spinBox->value());
    txtOutput << setting_content_line << endl;

    //MaxBackStep
    setting_content_line = "maxBackStep:"+QString::number(ui->huidiao_max_num_spinBox->value());
    txtOutput << setting_content_line << endl;

    //生成调试版代码,默认relese
    if(ui->output_type_code->currentIndex()==1)
    {
        setting_content_line = "debugVersion:1";
        txtOutput<<setting_content_line<<endl;
    }
    else {
        setting_content_line = "debugVersion:0";
        txtOutput<<setting_content_line<<endl;
    }

    //生成后是否执行代码（默认执行
    if(ui->code_run_No_radioButton->isChecked())
    {
        setting_content_line = "execute:0";
        txtOutput<<setting_content_line<<endl;
    }
    else {
        setting_content_line = "execute:1";
        txtOutput<<setting_content_line<<endl;
    }
    //执行时是否应用调试模式
    if(ui->Debug_mode_combobox->currentIndex()==1)
    {
        setting_content_line = "debugMode:1";
        txtOutput<<setting_content_line<<endl;
    }
    else {
        setting_content_line = "debugMode:0";
        txtOutput<<setting_content_line<<endl;
    }


    // 是否需要AI辅助
    if(ui->ai_enable_no_btn->isChecked()){

        setting_content_line = "neuralNetworkEnabled:0";
        txtOutput<<setting_content_line<<endl;

    }else {
        setting_content_line = "neuralNetworkEnabled:1";
        txtOutput<<setting_content_line<<endl;

    }


    // 是否导出训练数据
    if(ui->export_yes_btn->isChecked()){

        setting_content_line = "exportModelingData:1";
        txtOutput<<setting_content_line<<endl;

    }else {
        setting_content_line = "exportModelingData:0";
        txtOutput<<setting_content_line<<endl;

    }

    //执行结束后立即训练
    if(ui->train_yes_btn->isChecked()){

        setting_content_line = "train:1";
        txtOutput<<setting_content_line<<endl;

    }else {
        setting_content_line = "train:0";
        txtOutput<<setting_content_line<<endl;

    }

    txtOutput <<endl <<endl;


    //fifo调试参数
    setting_content_line = "debugInfoOutputPath:debugInfoOutputPath.pipe";
    txtOutput<<setting_content_line<<endl;
    setting_content_line = "debugOrderInputPath:debugOrderInputPath.pipe";
    txtOutput<<setting_content_line<<endl;

    txtOutput<<"baseAlgorithm:2\n"
            "userPromptEnabled:1\n"
            "chainEnabled:1\n"
            "userPromptAlgorithm:1\n"
            "progressRewardFactor:0.2\n"
            "neuralNetworkAlgorithm:1\n"


            "nnPenaltyEnabled:1\n"
            "nnRewardEnabled:1\n"

            "exportSuccessTrainingData:1\n"
            "exportFailureTrainingData:1\n"
            "exportStepSkippingTrainingData:0\n"
            "exportDuplicatedTrainingData:0\n"
            "exportPerformance:1\n"
            "cosineSimilarityThreshold:0.8\n"
            "sampleProportion:0.5\n"

            "CollectionCycleSerialNumber:1\n"
              <<endl;
    f.close();
}
