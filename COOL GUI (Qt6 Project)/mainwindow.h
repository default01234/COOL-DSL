#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QLayout>
#include <QList>
#include <QMap>
#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include <qscilexer.h>
#include <QTextEdit>
#include <QSplitter>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QTextCodec>
#else
#include <QtCore5Compat/QTextCodec>
#endif
#include <QProcess>
#include <QTime>
#include <qsciapis.h>
#include "codeeditor.h"
#include "search.h"
#include "search.h"
#include "debugger.h"
#include "compile_setting.h"
#include "file_compile_setting.h"

#if __WIN32__
#include <windows.h>
#endif

#define C_FILE 1
#define JAVA_FILE 2
#define PYTHON_FILE 3
#define DOC_FILE 10
#define GUI_TYPE_FILE 4
#define COS_FILE 5

class QAction;
class QMenu;
class QsciScintilla;

class NewBulidFile : public QDialog{
    Q_OBJECT
public:
    NewBulidFile(QWidget *parent=0);
    QLineEdit *fileNameEdit;
    QComboBox *fileNameTypeBox;
    QPushButton *okButton;
    QPushButton *cancalButton;
};

// 创建文件夹时弹出的窗体
class NewBuildfolder : public QDialog
{
    Q_OBJECT
public:
    NewBuildfolder(QWidget *parent=0);
    QLineEdit *fileNameEdit;
    QPushButton *okButton;
    QPushButton *cancalButton;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    //========文件树===================
    QTreeWidget *treemenu;
    void CreateTopItem(QString path);
    void FindFile(QDir *path,QTreeWidgetItem *parent);
    void buttonCollapseItem();
    QTreeWidgetItem *nowItem; //当前光标所在Item
    QMap<QString,QString> fileNameToPath; //项目列表中名字和目录的对应关系
    QMap<QString,QString> projectNameToPath; //项目名和项目路径的对应关系
    QMap<QString,QStringList> projectToChildren; //项目名和属于它的子文件们的路径的对应关系
    void terminatesubProcesses();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void newproject();
    void OpenProject();
    void open();
    bool save();
    bool saveAs();
    void about();
    void about_IDE();
    void run();
    void compile_setting_cilcked();
    void compile_clicked();
    void start_nn_clicked();
    void start_nn_silently_clicked();
    void stop_nn_clicked();
    void stop_py_clicked();
    void cmd_process_finished(int,QProcess::ExitStatus);
    void py_cmd_process_finished(int,QProcess::ExitStatus);
    void stop_cilcked();
    void documentWasModified();
    void debug_act_clicked();
    void search_act_clicked();
    void receiveSearchDataForMain(QString,int,int);
    void receiveReplaceDataForMain(QString);
    void receiveReplaceAllDataForMain(QString);
    void receiveDebugCommand(QString);   //接收debug的调试命令
    void receiveCompileSetting();
    void debug_cmdOutPut();
    void debug_cmdError();
    void MarginMarker(int m, int n, Qt::KeyboardModifiers);
    void ClearAllMarker();
    void ShowTime();
//======界面风格===============================
    void Ubuntu_style_clicked();
    void BrightWhite_style_clicked();
    void ElegantDark_style_clicked();

//==============文件树信号===============================================

    void DoubleClickpath(QTreeWidgetItem *item, int column);
    void itemPressedSlot(QTreeWidgetItem * pressedItem, int column);
    void tempActionInformation(QAction *act);
    void bulidNewFileSlot(bool flag);
    void closeBuildFileSlot(bool flag);
    void buildNewDirSlot(bool flag);
    void cancelDirSlot(bool flag);
    void dir_change(const QString &path);
    void open_filetree_file(QString file_path);
    void rename_filetree_file(QTreeWidgetItem*,int colom);
    void rename_start(const QTreeWidgetItem*);
    void refresh_tree(const QString &path);

//============新建项目=========================================
    void new_project_choose_project_dir();
    void new_project_ok();
    void new_project_cancel();

signals:
    void showTextSignal(QString path);
    void collapseItemSignal(const QTreeWidgetItem *item);
    void renameSignal(const QTreeWidgetItem *nowItem);
    void project_build_down();//新建项目完成信号
    void debug_break_information(QString str);
//=====================================================================
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void loadstyle(const QString &style);
    bool copyfile(QString srcPath, QString dstPath, bool coverFileIfExist);//复制文件
    void copydir(const QString& sourcePath,const QString& targetPath);//复制文件夹

    void terminateAllCodeCmdProcesses();
    void terminateBatchProcess(QProcess* process);
    void terminateAllPyCodeCmdProcesses();

    QString cur_style;
    QString curdir;

    QString IDE_Setting_path;
    QFile *IDE_Setting_File;
    QString *IDE_Setting_txt;
    QsciScintilla *textEdit;
    QsciLexer *Lexer;
    QsciAPIs *apis;
    QString curFile;
    QLabel *time_label;
    QTextEdit *textbottom;
    QTextEdit *text1;
    QSplitter *splitterMain;
    search *search_dialog;
    debugger *debug_dialog;
    compile_setting *Compile_Seetting_Dialog;
    file_compile_setting *Compile_Setting;
    QProcess *debug_cmd;

    //=======文件树=============
    QTreeWidgetItem *root; //树的根
    QMenu *dirMenu;  //文件夹菜单栏
    QMenu *treefileMenu;  //文件菜单栏
    QStringList childrenNameList;
    int nowCol; //当前所在列
    NewBulidFile *newFileWidget;
    NewBuildfolder *newDirWidget;
    QFileSystemModel *file_modle;
    QTreeView *file_tree;
    QTreeWidget *file_tree_menu;
    QFileSystemWatcher *dir_watcher;
    //=================菜单设置=========================
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *styleMenu;
    QMenu *WindowStyle;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *NewProject;
    QAction *OpenProjectAction;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *firstUseAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *aboutIDEAct;
    QAction *search_act;
    QAction *debug_act;
    QAction *compile_setting_act;
    QAction *compile;
    QAction *compile_stop;
    QAction *launch_AI;
    QAction *terminate_AI;
    QAction *terminate_py;
    QAction *launch_AI_silently;
    QComboBox *code_run_type;
    QLabel *code_run_type_lable;
    QAction *code_run;

    //======运行进程======
    QProcess* code_run_in_cmd;
    QProcess* py_code_run_in_cmd;

    QList<QProcess*> code_run_in_cmd_container;
    QList<QProcess*> py_code_run_in_cmd_container;


    //======风格==========
    QAction *dark_style;
    QAction *AMOLED_style;
    QAction *Aqua_style;
    QAction *Console_style;
    QAction *ElegantDark_style;
    QAction *MacOS_style;
    QAction *ManjaroMix_style;
    QAction *MaterialDark_style;
    QAction *NeonButtons_style;
    QAction *Ubuntu_style;

    int CUR_FILE_TYPE;


    //============================新建项目=======================================
    QLineEdit *project_name_edit;
    QLineEdit *project_path_lable;

    QPushButton *newproject_okButton;
    QPushButton *newproject_cancalButton;
    QPushButton *newproject_chooseDir;
    QString new_project_name_;
    QString new_project_file_name;
    QString new_project_dir_;

    QString excute_file_dir_;
    QDialog *newproject_dialog;

    //=============================编译器相关运行变量=======================================
    QString coolang_home;
    QString coolang_project_path;
    QString pythonExecutable;
    const QString coolang_vm_name = "coolang_vm.exe";
    //============================中文路径支持==========================================
    typedef std::wstring wstring;
    typedef std::string string;
    typedef std::wstring pathStr;
    /**
     * @brief convert char * to wstring
     * @param charp
     * @param codePage
     * @param existUnknownCharacters if there are unknown character in charp, set to 1.
     * @return
     */
    std::wstring charpToWString(const char *charp, const int codePage,
            int *existUnknownCharacters);
    std::wstring StringToWString(const std::string &str);
    std::string WStringToString(const wstring &wstr, const int codePage,
            int *existUnknownCharacters);
    std::string WStringToString(const wstring &wstr);




};
void terminateProcessByName(const QString& processName);
#endif
