#ifndef DEBUGGER_H
#define DEBUGGER_H
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

class debugger:public QDialog
{
    Q_OBJECT
public:
    explicit debugger(QWidget *parent = 0);
    QPushButton *run_button;
    QPushButton *continue_button;
    QPushButton *next_button;
    QPushButton *step_button;
    QPushButton *clear_allbreak_button;
    QPushButton *show_var_button;
    QPushButton *send_toGDB_button;
    QLineEdit *show_var_edit;
    QLineEdit *send_toGDB_edit;
    QTextBrowser *debug_brower;
    ~debugger();

signals:
    void SendClearMarkerToMain();
    void SendDebugCmmandToMain(QString);

private slots:
    void on_clearAllMarker_button_clicked();
    void on_run_button_clicked();
    void on_continue_button_clicked();
    void on_next_button_clicked();
    void on_step_button_clicked();
    void on_show_var_button_clicked();
    void on_sendtoGDB_button_clicked();


};

#endif // DEBUGGER_H
