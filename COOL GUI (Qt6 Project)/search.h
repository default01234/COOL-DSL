#ifndef SEARCH_H
#define SEARCH_H


#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>

class search:public QDialog
{
    Q_OBJECT
public:
    explicit search(QWidget *parent = 0);
    QLineEdit *search_word;
    QLineEdit *replace_word;
    QLabel *search_label;
    QLabel *opt_label;
    QLabel *direction_label;
    QLabel *begin_label;
    QPushButton *search_button;
    QPushButton *cancel_button;
    QPushButton *replace_button;
    QPushButton *replace_all_button;
    QCheckBox *opt_1;
    QCheckBox *opt_2;
    QRadioButton *search_forward;
    QRadioButton *search_backward;
    QRadioButton *begin_here_button;
    QRadioButton *begin_head_button;

    ~search();
signals:
    void  sendSearchDataToMain(QString,int,int);
    void  sendReplaceDataToMain(QString);
    void  sendReplaceAllDataToMain(QString);
private slots:
    void  on_search_button_clicked();
    void  on_replace_button_clicked();
    void  on_replace_all_button_clicked();
    void  on_cancel_button_clicked();

};

#endif // SEARCH_H
