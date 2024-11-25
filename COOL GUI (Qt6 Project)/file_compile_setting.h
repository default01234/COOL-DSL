#ifndef FILE_COMPILE_SETTING_H
#define FILE_COMPILE_SETTING_H

#include <QDialog>
#include <QString>

namespace Ui {
class file_compile_setting;
}

class file_compile_setting : public QDialog
{
    Q_OBJECT

public:
    explicit file_compile_setting(QWidget *parent = nullptr);
    Ui::file_compile_setting *ui;
    void receive_setting_path(QString path);
    ~file_compile_setting();

private slots:
    void on_ok_button_clicked();

signals:
    void receive_compile_setting();
};

#endif // FILE_COMPILE_SETTING_H
