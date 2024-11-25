#ifndef COMPILE_SETTING_H
#define COMPILE_SETTING_H
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QTextBrowser>
#include <QComboBox>
#include <QSpinBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>


class compile_setting:public QDialog
{
    Q_OBJECT
public:
    compile_setting();
    QPushButton *ok_button;
    QPushButton *cancel_button;

    QRadioButton *codeForm_cos;
    QRadioButton *codeForm_coc;
    QRadioButton *codeForm_cob;

    QRadioButton *multiFile_0;
    QRadioButton *multiFile_1;

    QSpinBox *matchStateSiloMaxCapacity_num;
    QSpinBox *codeTableSiloMaxCapacity_num;
    QSpinBox *maxSwitchTurn_num;

    QRadioButton *debugVersion_0;
    QRadioButton *debugVersion_1;

    QRadioButton *execute_0;
    QRadioButton *execute_1;

    QRadioButton *debugMode_0;
    QRadioButton *debugMode_1;

    QRadioButton *debugInfoOutputPath_0;
    QRadioButton *debugOrderInputPath_0;

    QListWidget *compile_list;

    QVBoxLayout *option0_layout;
    QVBoxLayout *option1_layout;
    QVBoxLayout *option2_layout;
    QVBoxLayout *option3_layout;
    QVBoxLayout *option4_layout;
    QVBoxLayout *option5_layout;
    QVBoxLayout *option6_layout;
    QVBoxLayout *option7_layout;
    QVBoxLayout *option8_layout;

    QGridLayout *G_layout;

    QLabel *num_tips;

private slots:
    void on_cancel_button_clicked();
    void on_list_cilcked(QListWidgetItem *item); //选项被点击后

};

#endif // COMPILE_SETTING_H
