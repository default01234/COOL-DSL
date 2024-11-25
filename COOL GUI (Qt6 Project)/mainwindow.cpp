#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QLayout>
#include <QDesktopServices>
#include <QMap>
#include<QPushButton>
#include <QLabel>
#include <QLayout>
#include <QApplication>
#include <QMenu>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QScrollBar>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QKeyEvent>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QtDebug>
#include <Qsci/qsciscintilla.h>
#include <qscilexercpp.h>
#include <qscilexerpython.h>
#include <qscilexerjava.h>
#include <qscilexertex.h>
#include <qsciapis.h>
#include <qscistyle.h>

#include <windows.h>
#include <tlhelp32.h>
#include <QString>


#include "mainwindow.h"

using namespace Qt;
MainWindow::MainWindow()
{
    //initial env-variable
    coolang_home = qEnvironmentVariable("COOLANG_HOME");
    if(coolang_home == ""){
        QMessageBox::warning(this, tr("Warning"),
                             tr("The environment variable COOLANG_HOME is not set correctly!\n"
                                "If this is your first installation, please restart the system and run the program again.\n"
                                "If this is not your first installation, manually add the path of the COOLANG_HOME folder "
                                "in the root directory to the COOLANG_HOME environment variable."));

    }

    CUR_FILE_TYPE = /*DO*/C_FILE;
    code_run_in_cmd = NULL;
    py_code_run_in_cmd = NULL;

    //文本高亮设置====================================================================================
    textEdit = new QsciScintilla;
    Lexer = new QsciLexerCPP;
//    lexercpp->setColor(QColor(Qt:: yellow),QsciLexerCPP::CommentLine);//c++的注释设置为黄色
    textEdit->setLexer(Lexer);
    //行号显示区域设置==================================================================================
    textEdit->setMarginType(0, QsciScintilla::NumberMargin);
    textEdit->setMarginLineNumbers(0,true);
    textEdit->setMarginWidth(0,50);
    //断点设置区域
    textEdit->setMarginType(1,QsciScintilla::SymbolMargin); //设置1号页边显示符号
    textEdit->SendScintilla(QsciScintilla::SCI_SETMARGINMASKN,1,0x02); //1号页边显示2号标记
    textEdit->SendScintilla(textEdit->SCI_SETMARGINSENSITIVEN,1,true);
    textEdit->SendScintilla(QsciScintilla::SCI_MARKERSETFORE, 1,QColor(Qt::black)); //置标记前景和背景标记
    textEdit->SendScintilla(QsciScintilla::SCI_MARKERSETBACK, 1,QColor(Qt::red));
    connect(textEdit, SIGNAL(marginClicked(int,int, Qt::KeyboardModifiers)),this,SLOT(MarginMarker(int, int, Qt::KeyboardModifiers)));
    textEdit->markerDefine(QsciScintilla::Circle, 1);
    //单步执行显示区域
    textEdit->setMarginType(2, QsciScintilla::SymbolMargin);
    textEdit->setMarginLineNumbers(2, false);
    textEdit->setMarginWidth(2, 20);
    textEdit->setMarginSensitivity(2, false);
    textEdit->setMarginMarkerMask(2, 0x04);
    textEdit->markerDefine(QsciScintilla::RightArrow, 2);
    //自动折叠区域
    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle);//折叠样式
    textEdit->setFoldMarginColors(Qt::gray,Qt::lightGray);//折叠栏颜色
    textEdit->setMarginType(3, QsciScintilla::SymbolMargin);
    textEdit->setMarginLineNumbers(3, false);
    textEdit->setMarginWidth(3, 15);
    textEdit->setMarginSensitivity(3, true);


    //换行设置==========================================================================
    textEdit->setWrapMode(QsciScintilla::WrapWord);//换行以单词为界限
    textEdit->setWrapVisualFlags(QsciScintilla::WrapFlagByBorder,QsciScintilla::WrapFlagInMargin,QsciScintilla::WrapIndentFixed);//设置换行符号在边界上
    //设置换行的模式，换行符，换行缩进
    textEdit->setIndentationGuides(true);//用tab键缩进时，在缩进位置上显示一个竖点线，缩进有效，在字符串后加空格不显示
    bool autoIndent = true;
    textEdit->setAutoIndent(autoIndent);//设置自动缩进，换行后光标位置和上一行一样
    textEdit->setCaretForegroundColor(QColor("#ff0000f"));//设置光标颜色
    bool enable = true;
    QColor caretground = "#87ceeb";//天蓝色
    textEdit->setCaretLineBackgroundColor(caretground);
    textEdit->setCaretLineVisible(enable);//设置光标所在行背景颜色可见
    textEdit->setCaretWidth(4);//设置光标宽度

    //自动补全设置=======================================================================
    apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
    if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
    {
        QMessageBox::warning(this,QString("提示"),QString("Failed to read the file"));
    }
    else{
        qDebug()<<QString(":/auto-cool.txt");
        apis->prepare();
    }
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
    textEdit->setAutoCompletionThreshold(2);//设置补全阈值，打2个字符出现补全
    textEdit->setAutoCompletionReplaceWord(true);//直接替换
    textEdit->setAutoCompletionCaseSensitivity(false);//不区分大小写
    textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);//括号匹配

    //字体设置


    //hotpot-style

    textEdit->indicatorDefine(QsciScintilla::PlainIndicator,0);
    textEdit->SendScintilla(QsciScintilla::SCI_SETINDICATORCURRENT,0);
    textEdit->SendScintilla(QsciScintilla::SCI_SETINDICATORVALUE, 0);





    //========================================文件树制作=================================================
    file_tree_menu = new QTreeWidget;
//    this->setObjectName("TreeMenu");
//    file_tree_menu->verticalScrollBar()->setObjectName("TreeMenuVerticl");
//    file_tree_menu->horizontalScrollBar()->setObjectName("TreeMenuHorizontal");



    file_tree_menu->setColumnCount(1);
    file_tree_menu->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    file_tree_menu->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    file_tree_menu->setAutoScroll(true);
    file_tree_menu->header()->setVisible(false);   //隐藏表头

    file_tree_menu->setFrameStyle(QFrame::Sunken);
    file_tree_menu->setAnimated(true); //设置折叠动画


    file_tree_menu->setAnimated(true);
    file_tree_menu->setIndentation(20);
    file_tree_menu->setSortingEnabled(true);
    file_tree_menu->setColumnWidth(0,this->width()/3);
    file_tree_menu->setHeaderHidden(true);
    file_tree_menu->setColumnHidden(1,true);
    file_tree_menu->setColumnHidden(2,true);
    file_tree_menu->setColumnHidden(3,true);
    QFont file_tree_font("ZYSong18030",10);
    this->setFont(file_tree_font);

    newFileWidget = new NewBulidFile();
    connect(newFileWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(bulidNewFileSlot(bool)));
    connect(newFileWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(closeBuildFileSlot(bool)));

    newDirWidget = new NewBuildfolder();
    connect(newDirWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(buildNewDirSlot(bool)));
    connect(newDirWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(cancelDirSlot(bool)));

    // 创建右击文件夹的菜单栏
    dirMenu = new QMenu();
    dirMenu->addAction("New File");
    dirMenu->addAction("Rename");
    dirMenu->addSeparator();
    dirMenu->addAction("New Folder");
    dirMenu->addAction("Delete Folder");
    dirMenu->addAction("Search in Folder");
    dirMenu->addAction("Close Project");
    // 创建右击文件的菜单栏
    treefileMenu = new QMenu();
    treefileMenu->addAction("Rename");
    treefileMenu->addAction("Delete");
    treefileMenu->addAction("Show in Explorer");
    connect(dirMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));
    connect(treefileMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));

    connect(file_tree_menu,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(itemPressedSlot(QTreeWidgetItem*,int)));
    connect(file_tree_menu,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(DoubleClickpath(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(collapseItemSignal(const QTreeWidgetItem*)),file_tree_menu,SLOT(collapseItem(const QTreeWidgetItem*)));
//    connect(this,SIGNAL(dir_change(const QString &path)),this,SLOT(refresh_tree(const QString &path)));

    //将软件打开的各项设置存储为一个setting文件，每次运行都读取使用
    QString setting_path = QDir::currentPath()+"/"+"IDE.setting";
    IDE_Setting_path = setting_path;
    IDE_Setting_File = new QFile(IDE_Setting_path);
    if(!IDE_Setting_File->exists()) //如果setting文件不存在
    {
        IDE_Setting_File->open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream ide_Setting(IDE_Setting_File);//txtoutput为读取的文件文本
        QString str = QDir::currentPath()+"/workspace/example/welcome";//初始时先定位到程序当前文件夹下的"/workspace/example/welcome"
        ide_Setting<<str<<endl;
        str = "curstyle:0";  //初始界面风格为系统默认，后面风格改变会记录
        ide_Setting<<str<<endl;
        IDE_Setting_File->close();
    }


    IDE_Setting_File->open(QIODevice::ReadOnly | QIODevice::Text);
    IDE_Setting_txt = new QString(IDE_Setting_File->readAll());//为读取的文件文本
    QStringList settinglist = IDE_Setting_txt->split("\n");

    //第一行setting为上次文件位置(异或上次项目所在地址)
    QString line1 = settinglist.at(0);
    coolang_project_path = settinglist.at(0);
    qDebug()<<settinglist.at(0);
    IDE_Setting_File->close();
    CreateTopItem(line1);//文件树定位到上次文件处
    curdir = line1;
//    CreateTopItem(QDir::currentPath());//初始时先定位到程序当前文件夹下
//    curdir = QDir::currentPath();


    qDebug()<<"这是当前软件运行的文件夹"<<QDir::currentPath();

    dir_watcher = new QFileSystemWatcher;
    dir_watcher->addPath(curdir);
    //connect(dir_watcher,SIGNAL(directoryChanged(const QString)),this,SLOT(dir_change(const QString)));
    //将文件监视功能关闭，以防止文件刷新太快使得程序崩溃
    file_tree_menu->setAnimated(true);
    file_tree_menu->setIndentation(20);
    file_tree_menu->setSortingEnabled(true);
    file_tree_menu->setColumnWidth(0,file_tree_menu->width()/3);
    file_tree_menu->setHeaderHidden(true);
    file_tree_menu->setColumnHidden(1,true);
    file_tree_menu->setColumnHidden(2,true);
    file_tree_menu->setColumnHidden(3,true);
    file_tree_menu->setFont(file_tree_font);

    file_modle = new QFileSystemModel;
    file_modle->setRootPath(QDir::currentPath());
    file_tree = new QTreeView;
    file_tree->setModel(file_modle);
    file_tree->setRootIndex(file_modle->index(QDir::currentPath()));


    //实现双击文件打开
    connect(this,SIGNAL(showTextSignal(QString)),this,SLOT(open_filetree_file(QString)));

    //文件树重命名操作,受到文件对象被改变的信号，传入该对象
    connect(this,SIGNAL(renameSignal(const QTreeWidgetItem*)),this,SLOT(rename_start(const QTreeWidgetItem*)));


    //========================================================================================================================
    splitterMain =new QSplitter(Qt::Horizontal,0);
    QSplitter *splitterRight =new QSplitter(Qt::Vertical,splitterMain);
    splitterRight->setOpaqueResize(true);//设定中间的分裂条是否实时更新
    splitterRight->setMinimumHeight(100);
    splitterRight->setFrameStyle(0);
    splitterRight->setHandleWidth(0);//设置中间分裂条大小
    textbottom =new QTextEdit();
    QFont f_debug("consolo",10);
    textbottom->setFont(f_debug);
    textbottom->append("Debug Window: ");

    text1 = new QTextEdit;
    splitterMain->addWidget(file_tree_menu);
    splitterRight->addWidget(textEdit);
    splitterRight->addWidget(textbottom);
    splitterRight->setStretchFactor(0,2);
    splitterRight->setStretchFactor(1,1);
    splitterMain->addWidget(splitterRight);
    splitterMain->setHandleWidth(0);
    //右部分割窗口

    splitterMain->setStretchFactor(1,1);
    splitterMain->setWindowTitle(QObject::tr("Splitter"));

    setCentralWidget(splitterMain);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit, SIGNAL(textChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");

    //新建项目=======================================================
    //new_project类实例化,新建文件操作
//    newproject_dialog = new QDialog;

    //设置项目文件夹，每次打开，之后运行的文件路径根据此设定
//    new_project_dir_ = QDir::currentPath();
//    new_project_name_ = "/workspace/compiler_lib";//设定默认文件夹,方便打开其他文件时能在默认文件夹成功运行


    new_project_dir_ = QString(line1.section("/",0,-2));
    new_project_name_ = line1.section("/",-1,-1);
    qDebug()<<"Dir"<<new_project_dir_;

    newproject_chooseDir = new QPushButton;
    newproject_okButton = new QPushButton;
    newproject_cancalButton = new QPushButton;
    connect(newproject_chooseDir,SIGNAL(clicked()),this,SLOT(new_project_choose_project_dir()));
    connect(newproject_okButton,SIGNAL(clicked()),this,SLOT(new_project_ok()));
    connect(newproject_cancalButton,SIGNAL(clicked()),this,SLOT(new_project_cancel()));

    //搜索替换功能====================================
    search_dialog = new search();
    connect(search_dialog,SIGNAL(sendSearchDataToMain(QString,int,int)),this,SLOT(receiveSearchDataForMain(QString,int,int)));
    connect(search_dialog,SIGNAL(sendReplaceDataToMain(QString)),this,SLOT(receiveReplaceDataForMain(QString)));
    connect(search_dialog,SIGNAL(sendReplaceAllDataToMain(QString)),this,SLOT(receiveReplaceAllDataForMain(QString)));

    //调试功能初始化
    debug_dialog = new debugger();
    debug_cmd = new QProcess(this);
    connect(debug_dialog,SIGNAL(SendClearMarkerToMain()),this,SLOT(ClearAllMarker()));
    connect(debug_dialog,SIGNAL(SendDebugCmmandToMain(QString)),this,SLOT(receiveDebugCommand(QString)));
    connect(this,SIGNAL(debug_break_information(QString)),this,SLOT(receiveDebugCommand(QString)));

    //编译选项初始化
    Compile_Seetting_Dialog = new compile_setting();
    Compile_Setting = new file_compile_setting();
    connect(Compile_Setting,SIGNAL(receive_compile_setting()),this,SLOT(receiveCompileSetting()));


    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ShowTime()));
    timer->start(1000);
    ShowTime();


    //=======界面风格设置===============================================

    //加载上次的界面风格
    if(settinglist.size()>=2){
        QString ide_style = settinglist.at(1).section(":",-1,-1);
        cur_style =ide_style;
        loadstyle(cur_style);
        qDebug()<<"Current interface style:"<<ide_style;
    }
    //加载代码页面
    QString codefile = coolang_project_path + '/' + coolang_project_path.section('/',-1,-1)+".cos";
    open_filetree_file(codefile);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Main window is closing. Terminating all processes...";
    this->terminateAllCodeCmdProcesses();
    this->terminateAllPyCodeCmdProcesses();

    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::OpenProject()
{
    fileNameToPath.clear();
    projectNameToPath.clear();
    QString projectPath = coolang_project_path; //current project path
    QString title = "Open Project"; //对话框标题
    QString projectfilePath=QFileDialog::getExistingDirectory(this,title,projectPath);
    if(projectfilePath == ""){
        return;//cancel operation
    }
    new_project_dir_ = projectfilePath;
    qDebug()<<"current project dir:"<<new_project_dir_<<endl;
    new_project_name_ = projectfilePath.section("/",-1,-1);
    qDebug()<<"current project name:"<<new_project_name_<<endl;
    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");
    rootname = rootname.right(rootname.size()-l-1);

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    dir_watcher->addPath(curdir);

//    qDebug()<<"加载项目，当前的目录为"<<curdir;
    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);

    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();

    //将软件setting文件更新
    IDE_Setting_File->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ide_setting(IDE_Setting_File);
    QString str = projectfilePath;
    ide_setting<<str<<endl;
    IDE_Setting_File->close();

    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);

    //update coolang project path
    coolang_project_path = new_project_dir_;
    //qDebug()<<fileNameToPath<<endl;

    //打开项目主程序文件
    QString codefile = coolang_project_path + '/' + coolang_project_path.section('/',-1,-1)+".cos";
    open_filetree_file(codefile);
    return;
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);

        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}


//====================新建项目==============================================
void MainWindow::newproject()
{
    newproject_dialog = new QDialog;
//
    newproject_dialog->setFixedSize(400,250);//设置新建文件对话窗口大小

    QLabel *project_NameLabel = new QLabel("Project Name");
    QLabel *project_PathLabel = new QLabel("Project Path");

    project_name_edit = new QLineEdit();
    project_path_lable = new QLineEdit();
//    project_path_lable->setText("选择项目文件夹路径");
    newproject_okButton->setText("OK");
    newproject_cancalButton->setText("Cancel");
    newproject_chooseDir->setText("Choose Folder");

    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(project_NameLabel);
    Hlayout1->addWidget(project_name_edit);

    QHBoxLayout *Hlayout3 = new QHBoxLayout();
    Hlayout3->addWidget(project_PathLabel);
    Hlayout3->addWidget(project_path_lable);
    Hlayout3->addWidget(newproject_chooseDir);


    QHBoxLayout *Hlayout4 = new QHBoxLayout();
    Hlayout4->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding));
    Hlayout4->addWidget(newproject_okButton);
    Hlayout4->addSpacerItem(new QSpacerItem(100, 20, QSizePolicy::Expanding));
    Hlayout4->addWidget(newproject_cancalButton);
    Hlayout4->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding));


    QVBoxLayout *MainVlayout = new QVBoxLayout(newproject_dialog); //布局使用嵌套布局
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout3);
    MainVlayout->addLayout(Hlayout4);
//    newproject_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    newproject_dialog->setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);
    newproject_dialog->exec();
}

void MainWindow::new_project_choose_project_dir()
{
    QFileDialog *file_dialog = new QFileDialog(newproject_dialog);
    // get the screen size
    const QRect mainWindowRect = this->geometry();;
    // move the dialog to the center of the screen
    file_dialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, file_dialog->size(), mainWindowRect));

    file_dialog->setOption(QFileDialog::ShowDirsOnly);
    file_dialog->setFileMode((QFileDialog::Directory));
    file_dialog->setViewMode(QFileDialog::List);
    file_dialog->setFixedSize(800,600);
    if(file_dialog->exec() == QFileDialog::Accepted)
    {
        new_project_dir_ = file_dialog->selectedFiles().at(0);
        project_path_lable->setText(new_project_dir_);
        return;
    }


    else
    {
        return;
    }
}

void MainWindow::new_project_ok()
{
    QString file_name;//获取文件名称，在项目名文件夹下新建一个文件
    QString file_path;
    QString project_name;//获取项目名称，在目标文件夹下再创建一个项目名称的文件夹
    QString project_path;
    QStringList compile_file_path;//编译器路径
    QStringList compile_file_name;//
    new_project_dir_ = project_path_lable->text() + "/" + project_name_edit->text();
    project_path = new_project_dir_;//项目文件夹路径
    new_project_name_ = project_name_edit->text();
    new_project_file_name = project_name_edit->text() + ".cos";
    QString exe_path = QDir::currentPath();
    qDebug()<<exe_path;
    qDebug()<<QDir::currentPath();
//    compile_file_path<<exe_path+"/workspace/compiler_lib/libstdc++-6.dll"
//                     <<exe_path+"/workspace/compiler_lib/libgcc_s_sjlj-1.dll"
//                     <<exe_path+"/workspace/compiler_lib/precompiler_convert_non_ascii_character.exe"
//                     <<exe_path+"/workspace/compiler_lib/precompiler_delete_redundancy.exe"
//                     <<exe_path+"/workspace/compiler_lib/precompiler_function_name_deformation.exe"
//                     <<exe_path+"/workspace/compiler_lib/HAN_VM.exe"
//                     <<exe_path+"/workspace/compiler_lib/compiler.exe"
//                     <<exe_path+"/workspace/compiler_lib/compiler_file_combination.exe"
//                     <<exe_path+"/workspace/compiler_lib/CompileFiles"
//                     <<exe_path+"/workspace/compiler_lib/COOLANG_HOME"
//                     <<exe_path+"/workspace/compiler_lib/COOLANG_PROJECT";

//    compile_file_name<<"/libstdc++-6.dll"
//                     <<"/libgcc_s_sjlj-1.dll"
//                     <<"/precompiler_convert_non_ascii_character.exe"
//                     <<"/precompiler_delete_redundancy.exe"
//                     <<"/precompiler_function_name_deformation.exe"
//                     <<"/HAN_VM.exe"
//                     <<"/compiler.exe"
//                     <<"/compiler_file_combination.exe"
//                     <<"/CompileFiles"
//                     <<"/COOLANG_HOME"
//                     <<"/COOLANG_PROJECT";

    project_name = project_name_edit->text();
    file_name = project_name_edit->text();
    if(file_name == "cos" || file_name.section(".",-1,-1) == "cos"){
          file_path = project_path + "/"+file_name+".cos";
    }
    //创建项目文件夹
    QDir *project_dir = new QDir(new_project_dir_);
    if(new_project_dir_ == "" || project_name == ""){
        QMessageBox::warning(this, tr("Application"),
                             tr("Folder name cannot be empty!"));
        return;
    }
    if(project_dir->exists()){
        QMessageBox::warning(this,tr("Folder Already Exists"),tr("Do not create duplicates!"));
        return;
    }
//    else {
//        project_dir->mkdir(project_path+"/"+project_name);
//    }

//    //将所需的编译器和库文件都复制到目标项目文件夹下
//    for(int i = 0;i<compile_file_path.length();i++)
//    {
//        QFileInfo file_or_dir(compile_file_path.at(i));
//        if(file_or_dir.isDir())
//        {
//            this->copydir(compile_file_path.at(i),project_path+"/"+project_name+compile_file_name.at(i));
//        }
//        else if (file_or_dir.isFile()) {
//            qDebug()<<copyfile(compile_file_path.at(i),project_path+"/"+project_name+compile_file_name.at(i),true);
//        }
//    }

//    //创建txt文件
//    QFile *project_file = new QFile;
//    if(file_path == "")
//    {
//        QMessageBox::warning(this,tr("Application"),tr("文件名不能为空"));
//        return;
//    }
//    if(project_file->exists(file_path))
//    {
//        QMessageBox::warning(this,tr("创建文件夹"),tr("文件已经存在！"));
//        return;
//    }
//    project_file->setFileName(file_path);
//    project_file->open(QIODevice::ReadWrite | QIODevice::Text);
//    project_file->close();

    //创建与项目同stem的cos文件
    file_path = new_project_dir_ + "/" + new_project_name_ + ".cos";
    if(file_path == "")
    {
        QMessageBox::warning(this,tr("Application"),tr("文件名不能为空"));
        return;
    }
//    if(QDir(file_path).exists(file_path))
//    {
//        QMessageBox::warning(this,tr("创建文件夹"),tr("文件已经存在！"));
//        return;
//    }

    QDir().mkpath(file_path.section("/",0,-2));
    QFile *project_file = new QFile;
    project_file->setFileName(file_path);
    project_file->open(QIODevice::WriteOnly|QIODevice::Truncate);
    project_file->close();


//    emit project_build_down();
//    emit compile_setting_cilcked();

//    //======刷新文件树=================================
//    QString projectfilePath= new_project_dir_;

//    QString rootname=projectfilePath;
//    int l = rootname.lastIndexOf("/");

//    dir_watcher->removePath(curdir);
//    curdir = projectfilePath;
//    rootname = rootname.right(rootname.size()-l-1);
//    curdir = rootname;
//    dir_watcher->addPath(curdir);

//    projectNameToPath.insert(rootname,projectfilePath);
//    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
//    root->setIcon(0, QIcon(":/folder.png"));
//    root->setCheckState(1, Qt::Checked);
//    root->setToolTip(0,projectfilePath);



//    QDir *rootdir = new QDir(projectfilePath);
//    root = new QTreeWidgetItem();
//    file_tree_menu->clear();
//    file_tree_menu->addTopLevelItem(root);
//    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
//    //将文件树最上方的根设置为当前目录

//    root->setText(0,rootdir->dirName());
//    FindFile(rootdir,root);
//    projectToChildren.insert(root->text(0),childrenNameList);
//    childrenNameList.clear();
////    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
//    QApplication::processEvents(); //防止界面假死
//    root->setExpanded(true);


    //create project folder and project main code file
    newproject_dialog->close();
    file_path = new_project_dir_ + "/" + new_project_name_ + ".cos";
    setCurrentFile(file_path);
    CUR_FILE_TYPE = COS_FILE;
    open_filetree_file(file_path);

    //create setting file
    IDE_Setting_File->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ide_setting(IDE_Setting_File);
    QString str = new_project_dir_+"/"+file_name;
    ide_setting<<str<<endl;
    IDE_Setting_File->close();



    //update coolang_project_path
    coolang_project_path = new_project_dir_;

    //refresh file tree
    QString projectfilePath= new_project_dir_;

    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    rootname = rootname.right(rootname.size()-l-1);
    curdir = rootname;
    dir_watcher->addPath(curdir);

    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);
    root->setToolTip(0,projectfilePath);



    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
    //将文件树最上方的根设置为当前目录

    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);


    //项目设置
    Compile_Setting->exec();

}

//复制文件夹操作
void MainWindow::copydir(const QString& sourcePath,const QString& targetPath)
{
    QDir sourceDir(sourcePath);
        //判断源文件夹是否为空，空则返回
        if(!sourceDir.exists() || sourceDir.isEmpty()){
            return;
        }
        QDir targetDir(targetPath);
        //判断是否有目标文件夹，没有则创建一个
        if(!targetDir.exists()){
            if(!targetDir.mkpath(targetPath)){
                qDebug()<<"创建文件夹失败";
                return;
            }
        }
        //去掉.和..文件，..代表上一层，.代表当前，一般每个文件夹都会有这两个隐藏文件
        sourceDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
        //获取该文件夹内所有文件除.和..以外
        QStringList sourceFileList = sourceDir.entryList();//获取文件夹下所有文件的名字

        //遍历源文件夹列表
        for(int i = 0;i<sourceFileList.size();i++)
        {
            //获取每个文件的信息
            QFileInfo *FileInfo = new QFileInfo(sourceDir.absolutePath()+"/"+sourceFileList[i]);
            if(FileInfo->isFile())
            {
                QFile::copy(FileInfo->path()+"/"+sourceFileList[i],targetPath+"/"+sourceFileList[i]);
            }
            if(FileInfo->isDir())
            {
                this->copydir(sourcePath+"/"+sourceFileList[i],targetPath+"/"+sourceFileList[i]);
            }
        }
//        for(auto& i : sourceFileList){
//            QFileInfo fileInfo(sourcePath + i);
//            //判断是否是文件
//            if(fileInfo.isFile()){
//                QFile file(targetPath + i);
//                //如果文件重名则不执行拷贝
//                if(!file.exists()){
//                    //没有则执行拷贝
//                    if(!QFile::copy(sourcePath + i,targetPath + i)){
//                        qDebug()<<"拷贝失败";
//                        return;
//                    }
//                }
//            }
//            //判断是否是文件夹
//            if(fileInfo.isDir()){
//                //如果文件夹重名则不执行拷贝
//                if(!targetDir.exists(targetDir.absolutePath() + "/" + i)){
//                    //如果不重复则执行拷贝文件
//                    if(!targetDir.mkdir(i)){
//                        qDebug()<<"文件夹拷贝失败";
//                        return;
//                    }
//                }
//                //回调
//                this->copydir(sourcePath + i + "/",targetPath + i + "/");
//            }
//        }
}

//复制文件操作
bool MainWindow::copyfile(QString srcPath, QString dstPath, bool coverFileIfExist)
{
    srcPath.replace("\\", "/");
        dstPath.replace("\\", "/");
        if (srcPath == dstPath) {
            return true;
        }

        if (!QFile::exists(srcPath)) {  //源文件不存在
            return false;
        }

        if (QFile::exists(dstPath)) {
            if (coverFileIfExist) {
                QFile::remove(dstPath);
            }
        }

        if (!QFile::copy(srcPath, dstPath)){
            return false;
        }
        return true;
}

void MainWindow::new_project_cancel()
{
    newproject_dialog->close();//关闭当前对话框
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"save",coolang_project_path,"COOLang files (*.cos *.coc *.setting);;Text files (*.txt);;Other files (*.*)");
    if (fileName.isEmpty())
        return false;

    if( saveFile(fileName)){
    //refresh file tree
    QString projectfilePath = coolang_project_path;

    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    rootname = rootname.right(rootname.size()-l-1);
    curdir = rootname;
    dir_watcher->addPath(curdir);

    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);
    root->setToolTip(0,projectfilePath);



    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
    //将文件树最上方的根设置为当前目录

    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);
      return true;
    }else {
        return false;
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About"),
            tr("<b>COOLang</b> is an advanced Domain-Specific Language programming system."));
}

void MainWindow::about_IDE()
{


    // QString file3= QDir::currentPath()+QString("/IDE_Guide_Book.pdf");
    // qDebug()<<file3;
    // QFile file(file3);

    // if (file.exists())

    // {

    // QDesktopServices::openUrl(QUrl::fromLocalFile(file3));
    // }
    QDesktopServices::openUrl(QUrl("https://coolang.org"));

}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->isModified());
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/icon/file.png"), tr("New File"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));


    //新建项目
    NewProject = new QAction(QIcon(":/images/icon/NewProject.png"), tr("New Project"), this);
    NewProject->setStatusTip(tr("Create a new project"));
    connect(NewProject, SIGNAL(triggered()), this, SLOT(newproject()));


    openAct = new QAction(QIcon(":/images/icon/file-open.png"), tr("Open File"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    //打开项目
    OpenProjectAction = new QAction(QIcon(":/images/icon/project.png"), tr("Open Project"), this);
    OpenProjectAction->setStatusTip(tr("Open an existing project"));
    connect(OpenProjectAction, SIGNAL(triggered()), this, SLOT(OpenProject()));

    saveAct = new QAction(QIcon(":/images/icon/save.png"), tr("Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/icon/order.png"),tr("Save As"), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/icon/cut.png"), tr("Cut"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/icon/copy.png"), tr("Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/icon/paste.png"), tr("Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));


    firstUseAct = new QAction(tr("First Use"), this);
    firstUseAct->setStatusTip(tr("Show the application's first use box"));
    connect(firstUseAct, &QAction::triggered, this, [=]() {
        QMessageBox::information(this, tr("First Use"),
                                 tr("To apply AI enhancement, please ensure the Python environment satisfies the following requirements:\n\n"
                                    "1. Python Version >= 3.11.5\n"
                                    "2. Required Python Packages:\n"
                                    "   - PyTorch (with CUDA support, if available)\n"
                                    "   - PyTorch Geometric\n"
                                    "   - Matplotlib\n"
                                    "   - NetworkX\n"
                                    "   - Numpy\n"
                                    "   - PyWin32\n\n"
                                    "After that, please set up the Python environment using \"neuralNetworkWizard.bat\" located in the \"software root folder/COOLANG_HOME/AI\"."));
    });


    aboutAct = new QAction(QIcon(":/images/icon/international.png"),tr("About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//    aboutQtAct = new QAction(tr("About &Qt"), this);
//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutIDEAct = new QAction(tr("Online Tutorials"), this);
    aboutIDEAct->setStatusTip(tr("Instructions for using the software"));
    connect(aboutIDEAct, SIGNAL(triggered()),this, SLOT(about_IDE()));

    //search action===========================================================
    search_act = new QAction(QIcon(":/images/icon/search.png"),tr("Search"), this);
    search_act->setStatusTip(tr("search word in the text"));
    search_act->setShortcut(tr("Ctrl+F"));
    connect(search_act, SIGNAL(triggered()),this, SLOT(search_act_clicked()));

    //debug action============================================================
    debug_act = new QAction(QIcon(":/images/icon/debug.png"),tr("Debug"), this);
    debug_act->setStatusTip(tr("Open debug interface"));
    connect(debug_act, SIGNAL(triggered()),this, SLOT(debug_act_clicked()));
    //compile
    compile = new QAction(QIcon(":/images/icon/compile box-line.png"),tr("Compile"),this);
    compile->setStatusTip(tr("Compile"));
    connect(compile,SIGNAL(triggered()),this,SLOT(compile_clicked()));

    //compile_setting
    compile_setting_act = new QAction(tr("Project Settings"),this);
    compile_setting_act->setStatusTip(tr("Configure project settings"));
    connect(compile_setting_act,SIGNAL(triggered()),this,SLOT(compile_setting_cilcked()));


    //compile_stop
    compile_stop = new QAction(QIcon(":/images/icon/stop.png"),tr("Stop"),this);
    compile_stop->setStatusTip(tr("Stop execution"));
    connect(compile_stop,SIGNAL(triggered()),this,SLOT(stop_cilcked()));

    //start python nn thread
    launch_AI = new QAction(QIcon(":/images/icon/nn_start.png"),tr("Launch AI"),this);
    launch_AI->setStatusTip(tr("Launch AI"));
    connect(launch_AI,SIGNAL(triggered()),this,SLOT(start_nn_clicked()));

    //start python nn thread
    launch_AI_silently = new QAction(QIcon(":/images/icon/nn_silent.png"),tr("Launch AI (silent mode)"),this);
    launch_AI_silently->setStatusTip(tr("Launch AI (silent mode)"));
    connect(launch_AI_silently,SIGNAL(triggered()),this,SLOT(start_nn_silently_clicked()));

    //stop python nn thread
    terminate_AI = new QAction(QIcon(":/images/icon/nn_stop.png"),tr("Terminate AI"),this);
    terminate_AI->setStatusTip(tr("Terminate AI"));
    connect(terminate_AI,SIGNAL(triggered()),this,SLOT(stop_nn_clicked()));

    //stop all python processes
    terminate_py = new QAction(QIcon(":/images/icon/terminate_py.png"),tr("Terminate all python processes"),this);
    terminate_py->setStatusTip(tr("Terminate all python processes"));
    connect(terminate_py,SIGNAL(triggered()),this,SLOT(stop_py_clicked()));

    //运行代码选择
    code_run_type_lable = new QLabel();
    code_run_type_lable->setText("Execution Program Type:");
    code_run_type = new QComboBox;
    code_run_type->addItem("cos");
    code_run_type->addItem("coc");
    code_run_type->addItem("cob");

    //code_run
    code_run = new QAction(QIcon(":/images/icon/play.png"),tr("Run (without settings)"),this);
    code_run->setStatusTip(tr("Run (without settings)"));
    code_run->setShortcut(tr("Ctrl+R"));
    connect(code_run,SIGNAL(triggered()),this,SLOT(run()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));

    //=========界面风格设置================
    dark_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("Dark"),this);
    dark_style->setStatusTip(tr("dark_style"));
    connect(dark_style,SIGNAL(triggered()),this,SLOT(ElegantDark_style_clicked()));


    AMOLED_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("AMOLED"),this);
    AMOLED_style->setStatusTip(tr("AMOLED_style"));

    Aqua_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("Aqua"),this);
    Aqua_style->setStatusTip(tr("Aqua_style"));

    Console_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("BrightWhite"),this);
    Console_style->setStatusTip(tr("Console_style"));
    connect(Console_style,SIGNAL(triggered()),this,SLOT(BrightWhite_style_clicked()));

    ElegantDark_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("ElegantDark"),this);
    ElegantDark_style->setStatusTip(tr("ElegantDark_style"));

    MacOS_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("MacOS"),this);
    MacOS_style->setStatusTip(tr("MacOS_style"));

    ManjaroMix_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("ManjaroMix"),this);
    ManjaroMix_style->setStatusTip(tr("ManjaroMix_style"));

    MaterialDark_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("MaterialDark"),this);
    MaterialDark_style->setStatusTip(tr("MaterialDark_style"));

    NeonButtons_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("NeonButtons"),this);
    NeonButtons_style->setStatusTip(tr("NeonButtons_style"));

    Ubuntu_style = new QAction(QIcon(":/images/icon/style_choose.png"),tr("Ubuntu"),this);
    Ubuntu_style->setStatusTip(tr("Ubuntu_style"));
    connect(Ubuntu_style,SIGNAL(triggered()),this,SLOT(Ubuntu_style_clicked()));
}

void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->setMinimumWidth(150);//指定菜单的宽度为150
    fileMenu->addAction(newAct);
    fileMenu->addAction(NewProject);
    fileMenu->addAction(OpenProjectAction);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(compile_setting_act);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("Tools"));
    editMenu->setMinimumWidth(150);//指定菜单的宽度为150
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(compile);
    editMenu->addAction(compile_stop);
    editMenu->addAction(code_run);
    editMenu->addAction(search_act);
    editMenu->addAction(debug_act);
    editMenu->addAction(compile_setting_act);

    editMenu->addAction(launch_AI);
    editMenu->addAction(launch_AI_silently);
    editMenu->addAction(terminate_AI);
    editMenu->addAction(terminate_py);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->setMinimumWidth(150);//指定菜单的宽度为150
    helpMenu->addAction(firstUseAct);
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(aboutIDEAct);

    styleMenu = menuBar()->addMenu(tr("Appearance"));
    styleMenu->setMinimumWidth(150);//指定菜单的宽度为150

    WindowStyle = new QMenu(tr("Interface Style"));
    WindowStyle->setMinimumWidth(150);//指定菜单的宽度为150
    WindowStyle->setIcon(QIcon(":/images/icon/style.png"));
    WindowStyle->addAction(dark_style);
    WindowStyle->addAction(AMOLED_style);
    WindowStyle->addAction(Aqua_style);
    WindowStyle->addAction(Console_style);
    WindowStyle->addAction(ElegantDark_style);
    WindowStyle->addAction(MacOS_style);
    WindowStyle->addAction(ManjaroMix_style);
    WindowStyle->addAction(MaterialDark_style);
    WindowStyle->addAction(NeonButtons_style);
    WindowStyle->addAction(Ubuntu_style);
    styleMenu->addMenu(WindowStyle);
    styleMenu->setMinimumWidth(150);//指定菜单的宽度为150
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addSeparator();
    editToolBar->addAction(compile_setting_act);
    editToolBar->addSeparator();
    editToolBar->addWidget(code_run_type_lable);
    editToolBar->addWidget(code_run_type);
    editToolBar->addSeparator();
    editToolBar->addAction(compile);
    editToolBar->addAction(compile_stop);

    editToolBar->addAction(code_run);

    editToolBar = addToolBar(tr("AI"));
    editToolBar->addAction(launch_AI);
    editToolBar->addAction(launch_AI_silently);
    editToolBar->addAction(terminate_AI);
    editToolBar->addAction(terminate_py);
}

void MainWindow::createStatusBar()
{
    time_label = new QLabel;
    statusBar()->addWidget(time_label);
}

void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::search_act_clicked()
{
    search_dialog->setModal(false);
    search_dialog->setWindowFlag(Qt::WindowStaysOnTopHint);
    search_dialog->show();
}

void MainWindow::debug_act_clicked()
{
    debug_cmd = new QProcess();
    debug_dialog->setModal(false);
    debug_cmd->start("cmd.exe");
    debug_cmd->write(QString("del debugger.exe\r\n").toStdString().c_str());
    QString str = "gcc -g "+curFile+" -o debugger\r\n";
    debug_cmd->write(str.toStdString().c_str());
    str = "gdb -q debugger\r\n";
    debug_cmd->write(str.toStdString().c_str());
    debug_cmd->waitForStarted();
    connect(debug_cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(debug_cmdOutPut()));
    connect(debug_cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(debug_cmdError()));
    debug_dialog->show();
}

/**
 * @brief MainWindow::compile_clicked this function handles the compiling process.
 */
void MainWindow::compile_clicked()
{

    if(code_run_in_cmd != nullptr){

        code_run_in_cmd->kill();
    }

    textbottom->clear();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString projectName = coolang_project_path.section("/",-1,-1);
    QString project_main_file = projectName + ".cos";
    QString setting_file_path = coolang_project_path+"/"+projectName + ".setting";
    QFileInfo file_exist(setting_file_path);
    QString compile_combination_path = coolang_home + "/compilers/compiler_file_combination.exe";
    QString resultInfo;
    qDebug()<<"setting_file_path:";
    qDebug()<<setting_file_path;


    if(QFile::exists(setting_file_path))
    {


        qDebug()<<"setting file exists.";
        QStringList arguments;//参数，程序入口文件和setting文件的路径
        arguments<<project_main_file<<setting_file_path;
        qDebug()<<compile_combination_path<<project_main_file<<setting_file_path;
        code_run_in_cmd = new QProcess();
        //设置项目主目录为工作路径
        code_run_in_cmd->setWorkingDirectory(coolang_project_path);


        code_run_in_cmd->start(compile_combination_path,arguments);
        this->code_run_in_cmd_container.append(code_run_in_cmd);

        // Capture the standard output in real-time and write it to the file
        QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
            QByteArray output = code_run_in_cmd->readAllStandardOutput();
            QString outputString = QString::fromUtf8(output);
            textbottom->append(outputString); // Append to the QTextEdit
        });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        // connect(code_run_in_cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));

        QObject::connect(code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================Process Finished=====================");
            } else {
                textbottom->append("=====================Process Finished=====================");
            }
        });

    }
    else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Settings file does not exist. Continue with default settings."));

        qDebug()<<"Settings file does not exist.";
        QStringList arguments;//参数，编译文件的路径
        arguments<<project_main_file;
        qDebug()<<compile_combination_path<<project_main_file<<setting_file_path;
        code_run_in_cmd = new QProcess();
        //设置当前目录为工作路径
        code_run_in_cmd->setWorkingDirectory(coolang_project_path);


        code_run_in_cmd->start(compile_combination_path,arguments);
        this->code_run_in_cmd_container.append(code_run_in_cmd);

        // Capture the standard output in real-time and write it to the file
        QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
            QByteArray output = code_run_in_cmd->readAllStandardOutput();
            QString outputString = QString::fromUtf8(output);
            textbottom->append(outputString); // Append to the QTextEdit
        });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        QObject::connect(code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================Process Finished=====================");
            } else {
                textbottom->append("=====================Process Finished=====================");
            }
        });

        // connect(code_run_in_cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));


//        code_run_in_cmd->waitForReadyRead();
//        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
//        code_run_in_cmd->close();
//        textbottom->append(resultInfo);
//        textbottom->append("=====================编译完成=====================");
//        QScrollBar *scrollbar = textbottom->verticalScrollBar();
//        if(scrollbar){
//            scrollbar->setSliderPosition(scrollbar->maximum());
//        }
//        return;
    }


//    QString projectfilePath = coolang_project_path;
//    QString rootname=projectfilePath;
//    int l = rootname.lastIndexOf("/");

//    dir_watcher->removePath(curdir);
//    curdir = projectfilePath;
//    rootname = rootname.right(rootname.size()-l-1);
//    curdir = rootname;
//    dir_watcher->addPath(curdir);

//    projectNameToPath.insert(rootname,projectfilePath);
//    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
//    root->setIcon(0, QIcon(":/folder.png"));
//    root->setCheckState(1, Qt::Checked);
//    root->setToolTip(0,projectfilePath);



//    QDir *rootdir = new QDir(projectfilePath);
//    root = new QTreeWidgetItem();
//    file_tree_menu->clear();
//    file_tree_menu->addTopLevelItem(root);
//    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
//    //将文件树最上方的根设置为当前目录

//    root->setText(0,rootdir->dirName());
//    FindFile(rootdir,root);
//    projectToChildren.insert(root->text(0),childrenNameList);
//    childrenNameList.clear();
////    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
//    QApplication::processEvents(); //防止界面假死
//    root->setExpanded(true);

//    //删除当前cmd子进程
//    code_run_in_cmd = NULL;


}

void MainWindow::start_nn_clicked()
{


    if(py_code_run_in_cmd != nullptr){

        this->terminateBatchProcess(py_code_run_in_cmd);
    }
    textbottom->clear();

    // //find python

    // QProcess findPythonProcess;
    // QString pythonPath;

    // // Run the 'where' command to find the Python executable
    // findPythonProcess.start("cmd", QStringList() << "/c" << "where python");
    // findPythonProcess.waitForFinished();

    // // Capture the output
    // QString output(findPythonProcess.readAllStandardOutput());

    // // Check if there is any valid output
    // if (!output.isEmpty()) {
    //     QStringList paths = output.split("\r\n", SkipEmptyParts);

    //     // Filter out paths that contain "WindowsApps"
    //     for (const QString& path : paths) {
    //         if (!path.contains("WindowsApps", Qt::CaseInsensitive)) {
    //             pythonPath = path;
    //             break;  // Use the first valid Python path that is not in "WindowsApps"
    //         }
    //     }

    //     if (pythonPath.isEmpty()) {
    //         textbottom->append("No valid Python executable found outside WindowsApps folder.");
    //         return;
    //     } else {
    //         qDebug() << "Python path found:" << pythonPath;
    //     }
    // } else {
    //     textbottom->append("Unable to find Python executable. Ensure Python is installed and added to PATH.");
    //     return;
    // }




    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString projectName = coolang_project_path.section("/",-1,-1);
    QString project_main_file = projectName + ".cos";
    QString setting_file_path = coolang_project_path+"/"+projectName + ".setting";
    QFileInfo file_exist(setting_file_path);
    QString AI_wizard_path = coolang_home + "/AI/neuralNetworkWizard.bat";
    QString resultInfo;
    qDebug()<<"setting_file_path:";
    qDebug()<<setting_file_path;

    textbottom->append("=====================AI Process Start=====================");

    if(QFile::exists(setting_file_path))
    {


        qDebug()<<"setting file exists.";
        QStringList arguments;//参数，程序入口文件和setting文件的路径
        // arguments<<AI_wizard_path<< "--setting_file_path"<<setting_file_path;
        // arguments<<AI_wizard_path;
        // qDebug() << arguments <<endl;
        py_code_run_in_cmd = new QProcess();
        //设置工作路径
        py_code_run_in_cmd->setWorkingDirectory(coolang_home + "/AI");

        py_code_run_in_cmd->start(AI_wizard_path);

        this->py_code_run_in_cmd_container.append(py_code_run_in_cmd);
        // py_code_run_in_cmd->start(pythonExecutable,arguments);

        // pythonExecutable = pythonPath;


        // py_code_run_in_cmd->start(pythonExecutable,arguments);

        // Handle errors if the process fails to start
        QObject::connect(py_code_run_in_cmd, &QProcess::errorOccurred, [=](QProcess::ProcessError error) mutable {
            switch (error) {
            case QProcess::FailedToStart:
                textbottom->append("Failed to start Python program. Please check the script path or Python installation.");
                break;
            case QProcess::Crashed:
                textbottom->append("Python program crashed.");
                break;
            default:
                textbottom->append("An unknown error occurred.");
                break;
            }
        });


        // Capture the standard output in real-time and write it to the file
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
            QByteArray output = py_code_run_in_cmd->readAllStandardOutput();
            QString outputString = QString::fromUtf8(output);
            textbottom->append(outputString); // Append to the QTextEdit
        });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = py_code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        QObject::connect(py_code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================AI Process Finished=====================");
            } else {
                textbottom->append("=====================AI Process Finished=====================");
            }
        });



    }
    else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Settings file does not exist. Continue with default settings."));

        qDebug()<<"Settings file does not exist.";
        QStringList arguments;//参数，编译文件的路径
        arguments<<AI_wizard_path<< "--setting_file_path"<<setting_file_path;

        py_code_run_in_cmd = new QProcess();
        //设置当前目录为工作路径
        py_code_run_in_cmd->setWorkingDirectory(coolang_project_path);


        py_code_run_in_cmd->start(pythonExecutable,arguments);

        this->py_code_run_in_cmd_container.append(py_code_run_in_cmd);

        // Capture the standard output in real-time and write it to the file
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
            QByteArray output = py_code_run_in_cmd->readAllStandardOutput();
            QString outputString = QString::fromUtf8(output);
            textbottom->append(outputString); // Append to the QTextEdit
        });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = py_code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        QObject::connect(py_code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================AI Process Finished=====================");
            } else {
                textbottom->append("=====================AI Process Finished=====================");
            }
        });



    }


}

void MainWindow::start_nn_silently_clicked()
{

    // this->py_code_run_in_cmd_container.append(py_code_run_in_cmd);
    if(py_code_run_in_cmd != nullptr){

        this->terminateBatchProcess(py_code_run_in_cmd);
    }
    textbottom->clear();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString projectName = coolang_project_path.section("/",-1,-1);
    QString project_main_file = projectName + ".cos";
    QString setting_file_path = coolang_project_path+"/"+projectName + ".setting";
    QFileInfo file_exist(setting_file_path);
    QString AI_wizard_path = coolang_home + "/AI/neuralNetworkWizard.bat";
    QString resultInfo;
    qDebug()<<"setting_file_path:";
    qDebug()<<setting_file_path;

    textbottom->append("=====================AI Process Start=====================");

    if(QFile::exists(setting_file_path))
    {


        qDebug()<<"setting file exists.";
        QStringList arguments;//参数，程序入口文件和setting文件的路径
        // arguments<<AI_wizard_path<< "--setting_file_path"<<setting_file_path;
        // arguments<<AI_wizard_path;
        // qDebug() << arguments <<endl;
        py_code_run_in_cmd = new QProcess();
        //设置工作路径
        py_code_run_in_cmd->setWorkingDirectory(coolang_home + "/AI");

        py_code_run_in_cmd->start(AI_wizard_path);

        this->py_code_run_in_cmd_container.append(py_code_run_in_cmd);
        // py_code_run_in_cmd->start(pythonExecutable,arguments);

        // pythonExecutable = pythonPath;


        // py_code_run_in_cmd->start(pythonExecutable,arguments);

        // Handle errors if the process fails to start
        QObject::connect(py_code_run_in_cmd, &QProcess::errorOccurred, [=](QProcess::ProcessError error) mutable {
            switch (error) {
            case QProcess::FailedToStart:
                textbottom->append("Failed to start Python program. Please check the script path or Python installation.");
                break;
            case QProcess::Crashed:
                textbottom->append("Python program crashed.");
                break;
            default:
                textbottom->append("An unknown error occurred.");
                break;
            }
        });


        // // Capture the standard output in real-time and write it to the file
        // QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
        //     QByteArray output = py_code_run_in_cmd->readAllStandardOutput();
        //     QString outputString = QString::fromUtf8(output);
        //     textbottom->append(outputString); // Append to the QTextEdit
        // });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = py_code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        QObject::connect(py_code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================AI Process Finished=====================");
            } else {
                textbottom->append("=====================AI Process Finished=====================");
            }
        });



    }
    else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Settings file does not exist. Continue with default settings."));

        qDebug()<<"Settings file does not exist.";
        QStringList arguments;//参数，编译文件的路径
        arguments<<AI_wizard_path<< "--setting_file_path"<<setting_file_path;

        py_code_run_in_cmd = new QProcess();
        //设置当前目录为工作路径
        py_code_run_in_cmd->setWorkingDirectory(coolang_project_path);


        py_code_run_in_cmd->start(pythonExecutable,arguments);

        this->py_code_run_in_cmd_container.append(py_code_run_in_cmd);

        // // Capture the standard output in real-time and write it to the file
        // QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
        //     QByteArray output = py_code_run_in_cmd->readAllStandardOutput();
        //     QString outputString = QString::fromUtf8(output);
        //     textbottom->append(outputString); // Append to the QTextEdit
        // });

        // Capture the standard error in real-time and also append it to the QTextEdit
        QObject::connect(py_code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
            QByteArray errorOutput = py_code_run_in_cmd->readAllStandardError();
            QString errorString = QString::fromUtf8(errorOutput);
            textbottom->append("Error: " + errorString); // Append error to the QTextEdit
        });

        QObject::connect(py_code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
            if (exitStatus == QProcess::NormalExit) {
                textbottom->append("=====================AI Process Finished=====================");
            } else {
                textbottom->append("=====================AI Process Finished=====================");
            }
        });



    }


}

void MainWindow::terminateAllCodeCmdProcesses(){
    auto it = code_run_in_cmd_container.begin();
    while (it != code_run_in_cmd_container.end()) {
        QProcess* process = *it;
        if (process && process->state() != QProcess::NotRunning) {
            process->terminate();
            if (!process->waitForFinished(100)) { // Wait for 0.1 seconds, then force kill if not finished
                process->kill();
            }
        }
        if (process && process->state() == QProcess::NotRunning) {
            delete process; // Clean up the process object
            it = code_run_in_cmd_container.erase(it); // Remove from container and get the next iterator
        } else {
            ++it; // Move to the next process
        }
    }

}

void MainWindow::terminateBatchProcess(QProcess* process) {
    if (process) {
        QString pid = QString::number(process->processId());

        if (process->state() != QProcess::NotRunning) {
            // First, try to terminate gracefully
            process->terminate();
            if (!process->waitForFinished(100)) { // Wait for 0.1 seconds
                // Use taskkill to force terminate all child processes
                QStringList arguments;
                arguments << "/F" << "/T" << "/PID" << pid;

                QProcess killerProcess;
                killerProcess.start("taskkill", arguments);
                killerProcess.waitForFinished();

                process->kill(); // Finally, kill the batch process itself if still running
            }
        }

        // Even if the batch process has exited, try to find and terminate child processes
        QString wmicCommand = QString("wmic process where (ParentProcessId=%1) get ProcessId").arg(pid);
        QProcess wmicProcess;
        wmicProcess.start("cmd.exe", QStringList() << "/C" << wmicCommand);
        wmicProcess.waitForFinished();

        // Parse the output and kill child processes
        QString output = wmicProcess.readAllStandardOutput();
        QStringList lines = output.split("\n", SkipEmptyParts);
        for (const QString& line : lines) {
            bool ok;
            int childPid = line.trimmed().toInt(&ok);
            if (ok) {
                QStringList killArguments;
                killArguments << "/F" << "/PID" << QString::number(childPid);
                QProcess killChildProcess;
                killChildProcess.start("taskkill", killArguments);
                killChildProcess.waitForFinished();
            }
        }
    }
}

void MainWindow::terminateAllPyCodeCmdProcesses(){
    auto it = this->py_code_run_in_cmd_container.begin();
    while (it != py_code_run_in_cmd_container.end()) {
        QProcess* process = *it;
        terminateBatchProcess(process);

        if (process && process->state() == QProcess::NotRunning) {
            delete process; // Clean up the process object
            it = py_code_run_in_cmd_container.erase(it); // Remove from container
        } else {
            ++it; // Move to the next process
        }
    }
}

void MainWindow::terminatesubProcesses(){
    this->terminateAllCodeCmdProcesses();
    this->terminateAllPyCodeCmdProcesses();

}
void MainWindow::cmd_process_finished(int,QProcess::ExitStatus){
//    code_run_in_cmd->waitForReadyRead();
    if(code_run_in_cmd == nullptr){
        return;
    }
    auto resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
    code_run_in_cmd->close();
    textbottom->append(resultInfo);
    textbottom->append("=====================Process Finished=====================");
    QScrollBar *scrollbar = textbottom->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
    QString projectfilePath = coolang_project_path;
    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    rootname = rootname.right(rootname.size()-l-1);
    curdir = rootname;
    dir_watcher->addPath(curdir);

    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);
    root->setToolTip(0,projectfilePath);



    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
    //将文件树最上方的根设置为当前目录

    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
//    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);

    //删除当前cmd子进程
    code_run_in_cmd = NULL;



}

void MainWindow::py_cmd_process_finished(int,QProcess::ExitStatus){

    if(py_code_run_in_cmd == nullptr){
        return;
    }
    auto resultInfo = QString::fromLocal8Bit(py_code_run_in_cmd->readAllStandardOutput());
    py_code_run_in_cmd->close();
    textbottom->append(resultInfo);
    textbottom->append("=====================AI Process Finished=====================");
    QScrollBar *scrollbar = textbottom->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
    QString projectfilePath = coolang_project_path;
    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    rootname = rootname.right(rootname.size()-l-1);
    curdir = rootname;
    dir_watcher->addPath(curdir);

    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);
    root->setToolTip(0,projectfilePath);



    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
    //将文件树最上方的根设置为当前目录

    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    //    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);

    //删除当前cmd子进程
    py_code_run_in_cmd = NULL;



}

void MainWindow::compile_setting_cilcked()
{
    Compile_Setting->show();
    //Compile_Seetting_Dialog->show();
}
void MainWindow::stop_cilcked()
{
    textbottom->append("=====================Force Exit=====================");
    qDebug()<<"force terminate clicked!!";
    // if(code_run_in_cmd&&(code_run_in_cmd->atEnd()==false||code_run_in_cmd->processId()>0)){
    //     code_run_in_cmd->kill();
    // }
    this->terminateAllCodeCmdProcesses();
    terminateProcessByName(coolang_vm_name);
//    code_run_in_cmd = NULL;
}

void MainWindow::stop_nn_clicked()
{
    textbottom->append("=====================Force Exit=====================");
    qDebug()<<"force terminate clicked!!";
    // if(py_code_run_in_cmd&&(py_code_run_in_cmd->atEnd()==false||py_code_run_in_cmd->processId()>0)){
    //     py_code_run_in_cmd->kill();
    // }
    this->terminateAllPyCodeCmdProcesses();
}

void MainWindow::stop_py_clicked()
{
    textbottom->append("=====================Terminate Python Processes=====================");
    // List of traditional Python executable names to terminate
    QStringList pythonExecutables = {"python.exe", "python3.exe", "pythonw.exe"};

    // Iterate through the list and terminate each Python executable
    for (const QString &exeName : pythonExecutables) {
        terminateProcessByName(exeName);
    }
}
void MainWindow::debug_cmdOutPut()
{
    QByteArray  strBytes = debug_cmd->readAllStandardOutput();
    QTextCodec *qtc=QTextCodec::codecForName("GBK");
    QString str = qtc->toUnicode(strBytes);
    textbottom->append(str);
    debug_dialog->debug_brower->append(str);
}

void MainWindow::debug_cmdError()
{
    QByteArray  strBytes = debug_cmd->readAllStandardError().data();
    QTextCodec *qtc=QTextCodec::codecForName("GBK");
    QString str = qtc->toUnicode(strBytes);
    textbottom->append(str);
    debug_dialog->debug_brower->append(str);
}

void MainWindow::receiveSearchDataForMain(QString data,int state,int begin)
{
    QString real_search_str = data;
    QByteArray ba=real_search_str.toLatin1();
    char *c=ba.data();
    qDebug("%s\n",c);
    qDebug("%d\n",state);
    if(real_search_str.length()>0){
//        bool found = false;
        switch (state) {
        case 0:
            textEdit->findFirst(real_search_str,false,false,false,true);
            //向前搜索、不区分大小写、不全字匹配
            break;
        case 2:
            textEdit->findFirst(real_search_str,false,false,true,true);
            //向前搜索、不区分大小写、全字匹配
            break;
        case 1:
            //向前搜索、区分大小写、不全字匹配
            textEdit->findFirst(real_search_str,false,true,false,true);
            break;
        case 3:
            //向前搜索、区分大小写、全字匹配
            textEdit->findFirst(real_search_str,false,true,true,true);
            break;
        case -4:
            //向后搜索、不区分大小写、不全字匹配
            textEdit->findFirst(real_search_str,false,false,false,true,false);
            break;
        case -3:
            //向后搜索、区分大小写、不全字匹配
            textEdit->findFirst(real_search_str,false,true,false,true,false);
            break;
        case -2:
            //向后搜索、不区分大小写、全字匹配
            textEdit->findFirst(real_search_str,false,false,true,true,false);
            break;
        case -1:
            //向后搜索、区分大小写、全字匹配
            textEdit->findFirst(real_search_str,false,true,true,true,false);
            break;
        }
        if(textEdit->findNext())
        {
            qDebug("found");
        }
        else {
            QMessageBox::warning(this,QString("Search"),QString("No matches found"));
        }
//        if(real_search_str.contains(textEdit->selectedText(),Qt::CaseInsensitive)){
//            qDebug("found");
//        }
//        else {
//            QMessageBox::warning(this,QString("查找"),QString("无匹配项"));
//        }
    }

}

void MainWindow::receiveReplaceDataForMain(QString data)
{
    QString replace_word = data;
    textEdit->replace(replace_word);
    qDebug()<<textEdit->findNext();
}

void MainWindow::receiveReplaceAllDataForMain(QString data)
{
    QString replace_word = data;
    while (textEdit->findNext()) {
        textEdit->replace(replace_word);
    }
}

//接收debug窗口的调试命令
void MainWindow::receiveDebugCommand(QString str)
{
    debug_cmd->write(str.toStdString().c_str());
    textbottom->append(str);
}

void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (textEdit->isModified()) {
        int ret = QMessageBox::warning(this, tr("Warning"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    QString file_name;
    QString file_type;
    file_name = fileName.section('/',-1,-1);//截取文件路径名最后一段为文件名
    file_type = fileName.section('.',-1,-1);//截取文件名最后一段为文件类型
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //textEdit->setText(QString::fromLocal8Bit(in.readAll()));
    QString inread = in.readAll();
    textEdit->setText(inread);
    QApplication::restoreOverrideCursor();
  //设置自动补全和文本高亮===================================================
    QString cFormat_1 = ".cpp";
    QString cFormat_2 = ".h";
    QString cFormat_3 = ".c";
    QString pythonFormat = ".py";
    QString javaFormat = ".java";
    QString my_code_language = ".cos";
    QString txt_file = ".txt";
    if(fileName.indexOf(cFormat_1)!=-1)
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-cool.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = C_FILE;
    }

    else if(fileName.indexOf(cFormat_2)!=-1)
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        {
            QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));
        }
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = C_FILE;
    }
    else if(fileName.indexOf(cFormat_3)!=-1)
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = C_FILE;
    }
    else if(fileName.indexOf(pythonFormat)!=-1)
    {
        Lexer = new QsciLexerPython;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-python.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-python.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = PYTHON_FILE;
    }
    else if(fileName.indexOf(javaFormat)!=-1)
    {
        Lexer = new QsciLexerJava;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-java.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-java.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = JAVA_FILE;
    }
    else if(fileName.indexOf(my_code_language)!=-1||fileName.indexOf(txt_file)!=-1)
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
        { QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-cool.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<"规文件加载";
        CUR_FILE_TYPE = GUI_TYPE_FILE;
    }
    else
    {
        Lexer = new QsciLexerTeX;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-cool.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = DOC_FILE;
    }
 //=======================================================================================
    if(fileNameToPath.contains(file_name))
    {
//        QMessageBox::warning(this, tr("Application"),
//                             tr("文件已打开")
//                             .arg(fileName)
//                             .arg(file.errorString()));
        return;
    }
    else
    {
        QTreeWidgetItem *child = new QTreeWidgetItem(file_tree_menu);
        child->setText(0,file_name);//设置文件树该子节点的text
        child->setToolTip(0,fileName);//设置文件树该子节点的tooltip，让双击等操作有参数

        if(file_type.indexOf("py")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/py.png")); //设置Item的图标，python
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("java")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/JAVA.png")); //设置Item的图标，java
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("txt")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/txt.png")); //设置Item的图标，txt
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("exe")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/exe.png")); //设置Item的图标，exe
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("规")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/gui.png")); //设置Item的图标，规语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("cos")!=-1||file_type.indexOf("COS")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/cos.png")); //设置Item的图标，coolang语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("coc")!=-1||file_type.indexOf("COC")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/coc.png")); //设置Item的图标，coolang语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("cob")!=-1||file_type.indexOf("COB")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/cob.png")); //设置Item的图标，coolang语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("cos1")!=-1||file_type.indexOf("cos2")!=-1||file_type.indexOf("cos3")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/tmp.png")); //设置Item的图标，coolang语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("setting")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/filetree_file.png")); //设置Item的图标，coolang语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("cpp")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("c")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
            fileNameToPath.insert(file_name,fileName);
        }
        else if(file_type.indexOf("h")!=-1)
        {
            child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
            fileNameToPath.insert(file_name,fileName);
        }
        else
        {
            child->setIcon(0,QIcon(":/images/icon/unknown.png")); //设置Item的图标，其他文件
            fileNameToPath.insert(file_name,fileName);
        }
        //这里根据新建文件类型设置图标
    }


    setCurrentFile(fileName);
    loadstyle(cur_style);
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm:ss");
    textbottom->append(text+":   "+fileName+" loaded\n");
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


//保存当前文件编译设置
void MainWindow::receiveCompileSetting()
{
    Compile_Setting->receive_setting_path(coolang_project_path);

    //refresh file tree
    QString projectfilePath= coolang_project_path;

    QString rootname=projectfilePath;
    int l = rootname.lastIndexOf("/");

    dir_watcher->removePath(curdir);
    curdir = projectfilePath;
    rootname = rootname.right(rootname.size()-l-1);
    curdir = rootname;
    dir_watcher->addPath(curdir);

    projectNameToPath.insert(rootname,projectfilePath);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);
    root->setToolTip(0,projectfilePath);



    QDir *rootdir = new QDir(projectfilePath);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
    //将文件树最上方的根设置为当前目录

    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);

}
//==================运行=========================================
void MainWindow::run()
{


    if(code_run_in_cmd != nullptr){

        code_run_in_cmd->kill();
    }

    textbottom->clear();
    code_run_in_cmd = new QProcess();
    QTextCodec*  pathcodec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(pathcodec);
    QStringList coderun_list;
    QString resultInfo;
    qDebug()<<"当前文件类型为："<<CUR_FILE_TYPE;
    switch (/*CUR_FILE_TYPE*/COS_FILE)
    {
    case C_FILE:// gcc
        {
//        code_run_in_cmd = new QProcess();
//        code_run_in_cmd->start("cmd.exe");
//        code_run_in_cmd->write(QString("del tmp.exe\r\n").toStdString().c_str());
//        QString str = "gcc -o tmp"+curFile+"\r\n";
//        code_run_in_cmd->write(str.toStdString().c_str());

//        code_run_in_cmd->start("cmd.exe");
        QProcess::execute("rm tmp");
//        code_run_in_cmd->write(QString("del tmp.exe\r\n").toStdString().c_str());
        QProcess::execute("g++ -o tmp "+curFile);//直接调用QProcess编译文件生成名为tmp的可执行文件
//        code_run_in_cmd->start("rm tmp");    // 简单处理之前编译结果
//        code_run_in_cmd->start("gcc -o tmp "+curFile);
        code_run_in_cmd->start("./tmp.exe");//在用新建的Qprocess对象执行文件，不传入任何参数

        this->code_run_in_cmd_container.append(code_run_in_cmd);
//        code_run_in_cmd->start("cmd", QStringList()<<"/c"<<"rm tmp\n"<<"gcc -o tmp "+curFile+"\n"<<"./tmp.exe");
//        code_run_in_cmd->start("cmd", QStringList()<<"/c"<<"gcc -o tmp "+curFile);
//        code_run_in_cmd->start("cmd", QStringList()<<"/c"<<"./tmp.exe");
//        code_run_in_cmd->start(":/../build-my_gui-Desktop_Qt_5_13_0_MinGW_64_bit-Debug/tmp.exe");
        // code_run_in_cmd->waitForReadyRead();//等待程序运行结束
        // resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());//读出运行结果输出

        // textbottom->append(resultInfo);//将结果附在调试窗口那

        // textbottom->append("==================Program Execution Ended====================\n");
        break;
        }
    case PYTHON_FILE:// python
        {
//        code_run_in_cmd->open(QIODevice::ReadOnly);
//        code_run_in_cmd->execute("python "+curFile);
        code_run_in_cmd->start("cmd", QStringList()<<"/c"<<"python "+curFile);
        //QProcess::execute("python"+curFile);
        // code_run_in_cmd->waitForReadyRead();
        // resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
        // textbottom->append(resultInfo);
        // textbottom->append("==================Program Execution Ended====================\n");
        break;
        }
    case JAVA_FILE:// javac,java
        {
//        code_run_in_cmd->start("cmd", QStringList()<<"/c"<<"javac "+curFile);
        QProcess::execute("javac "+curFile);
        QString javaFileSuffix = ".java";
        int sufPos = curFile.lastIndexOf(javaFileSuffix)-5;
        int start = curFile.lastIndexOf("/");
        QString curJavaPath = curFile.left(start+1);
        QProcess::execute("java -classpath "
                          + curJavaPath
                          + " " + curFile.mid(start+1,(sufPos-start)+4));
        break;
        }

    case GUI_TYPE_FILE:// 我们的语言文件
        {
        QString power_shell_path = "C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe";
        qDebug()<<curFile;

        QString precompile_exe_path = this->new_project_dir_+"/"+this->new_project_name_+ "./precompiler_delete_redundancy.exe";
        QString precompile2_exe_path = this->new_project_dir_+"/"+this->new_project_name_+ "./precompiler_convert_non_ascii_character.exe";
        QString precompile3_exe_path = this->new_project_dir_+"/"+this->new_project_name_+ "./precompiler_function_name_deformation.exe";
        QString compile_exe_path = this->new_project_dir_+"/"+this->new_project_name_+ "./compiler.exe";
        QString virtual_machine_path = this->new_project_dir_+"/"+this->new_project_name_+ "./HAN_VM.exe";
        QString precompile1 = curFile;//原来的文件
        QString precompile2 =this->new_project_dir_+"/"+this->new_project_name_+ "./codeTest1out.txt"; //第一次预编译产生的文件
        QString precompile3 =this->new_project_dir_+"/"+this->new_project_name_+ "./codeTest2out.txt"; //第二次预编译产生的文件
        QString precompile4 =this->new_project_dir_+"/"+this->new_project_name_+ "./codeTest3out.txt"; //第三次预编译产生的文件
        QString compile5 =this->new_project_dir_+"/"+this->new_project_name_+ "./codeTest4out.txt"; //编译产生的文件

        code_run_in_cmd->start(precompile_exe_path,QStringList()<<precompile1<<precompile2);
        this->code_run_in_cmd_container.append(code_run_in_cmd);
        code_run_in_cmd->waitForFinished();
        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
        textbottom->append(resultInfo);
        code_run_in_cmd->start(precompile2_exe_path,QStringList()<<precompile2<<precompile3);
        code_run_in_cmd->waitForFinished();
        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
        textbottom->append(resultInfo);
        code_run_in_cmd->start(precompile3_exe_path,QStringList()<<precompile3<<precompile4);
        code_run_in_cmd->waitForFinished();
        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
        textbottom->append(resultInfo);
        code_run_in_cmd->start(compile_exe_path,QStringList()<<precompile4<<compile5);
        code_run_in_cmd->waitForFinished();
        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
        textbottom->append(resultInfo);
        code_run_in_cmd->start(virtual_machine_path,QStringList()<<compile5<<"10"<<"10");
        code_run_in_cmd->waitForFinished();
        resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());

        textbottom->append(resultInfo);
        textbottom->append("==================Program Execution Ended====================\n");
        break;
        }
    case COS_FILE:
        {
//        QString power_shell_path = "C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe";

        QString compile_combination_path = coolang_home+"/compilers/compiler_file_combination.exe";
        QString project_name = coolang_project_path.section("/",-1,-1);
//        QString fileName = curFile.section(".",0,-2);
//        filename = filename.section("/",-1,-1);

        //定义coc和cob代码的路径

        QString cos_file_path = coolang_project_path + "/" + project_name +".cos";

        QString coc_file_path = coolang_project_path + "/CompiledFiles"+"/"+project_name+".coc";
        QString cob_file_path = coolang_project_path + "/CompiledFiles"+"/"+project_name+".cob";

//        qDebug()<<"当前主代码文件为"<<curFile<<"\n"<<"编译器路径为"<<compile_combination_path;
        QString code_type;
        code_type = code_run_type->currentText();
        //设置cmd调用的目录
        code_run_in_cmd->setWorkingDirectory(coolang_project_path);
        if(code_type == "cos")
        {
            QFileInfo file_exist(cos_file_path);
            if(file_exist.isFile())
            {
//                std::string cos_file_path_stdstr = cos_file_path.toStdString();
//                wstring cos_file_path_p = StringToWString(cos_file_path_stdstr);
//                cos_file_path_stdstr = WStringToString(cos_file_path_p);


                qDebug()<<"运行代码为cos"<<code_type;


                code_run_in_cmd->start(compile_combination_path,(QStringList()<<cos_file_path));

                this->code_run_in_cmd_container.append(code_run_in_cmd);
                // connect(code_run_in_cmd,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));
//                code_run_in_cmd->waitForFinished();
//                auto cmdres = code_run_in_cmd->readAllStandardOutput();
//                //resultInfo = QTextDecoder::toUnicode(cmdres);
//                resultInfo = QString::fromLocal8Bit(cmdres);
//                textbottom->append(resultInfo);

                // Capture the standard output in real-time and write it to the file
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
                    QByteArray output = code_run_in_cmd->readAllStandardOutput();
                    QString outputString = QString::fromUtf8(output);
                    textbottom->append(outputString); // Append to the QTextEdit
                });

                // Capture the standard error in real-time and also append it to the QTextEdit
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
                    QByteArray errorOutput = code_run_in_cmd->readAllStandardError();
                    QString errorString = QString::fromUtf8(errorOutput);
                    textbottom->append("Error: " + errorString); // Append error to the QTextEdit
                });

                // connect(code_run_in_cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));

                QObject::connect(code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
                    if (exitStatus == QProcess::NormalExit) {
                        textbottom->append("=====================Process Finished=====================");
                    } else {
                        textbottom->append("=====================Process Finished=====================");
                    }
                });
            }
            else {
                QMessageBox::warning(this, tr("Warning"),
                                     "\""+cos_file_path+"\" "+tr("File not found"));
                qDebug()<<"File needed: "<<cos_file_path;
                return;
            }

        }
        else if (code_type == "coc") {
            QFileInfo file_exist(coc_file_path);
            if(file_exist.isFile())
            {
                qDebug()<<"run as coc code:"<<code_type;
                code_run_in_cmd->start(compile_combination_path,QStringList()<<coc_file_path);

                this->code_run_in_cmd_container.append(code_run_in_cmd);
                // connect(code_run_in_cmd,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));
//                code_run_in_cmd->waitForFinished();
//                resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
//                textbottom->append(resultInfo);

                // Capture the standard output in real-time and write it to the file
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
                    QByteArray output = code_run_in_cmd->readAllStandardOutput();
                    QString outputString = QString::fromUtf8(output);
                    textbottom->append(outputString); // Append to the QTextEdit
                });

                // Capture the standard error in real-time and also append it to the QTextEdit
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
                    QByteArray errorOutput = code_run_in_cmd->readAllStandardError();
                    QString errorString = QString::fromUtf8(errorOutput);
                    textbottom->append("Error: " + errorString); // Append error to the QTextEdit
                });

                // connect(code_run_in_cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));

                QObject::connect(code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
                    if (exitStatus == QProcess::NormalExit) {
                        textbottom->append("=====================Process Finished=====================");
                    } else {
                        textbottom->append("=====================Process Finished=====================");
                    }
                });
            }
            else {
                QMessageBox::warning(this, tr("Warning"),
                                     "File not found: "+coc_file_path);
                qDebug()<<"File needed: "<<coc_file_path;
                return;
            }
        }
        else if (code_type == "cob") {
            QFileInfo file_exist(cob_file_path);
            if(file_exist.isFile())
            {
                qDebug()<<"Run as cob: "<<code_type;
                code_run_in_cmd->start(compile_combination_path,QStringList()<<cob_file_path);
                    this->code_run_in_cmd_container.append(code_run_in_cmd);
                // connect(code_run_in_cmd,SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));

//                code_run_in_cmd->waitForFinished();
//                resultInfo = QString::fromLocal8Bit(code_run_in_cmd->readAllStandardOutput());
//                textbottom->append(resultInfo);

                // Capture the standard output in real-time and write it to the file
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardOutput, [=]() mutable {
                    QByteArray output = code_run_in_cmd->readAllStandardOutput();
                    QString outputString = QString::fromUtf8(output);
                    textbottom->append(outputString); // Append to the QTextEdit
                });

                // Capture the standard error in real-time and also append it to the QTextEdit
                QObject::connect(code_run_in_cmd, &QProcess::readyReadStandardError, [=]() mutable {
                    QByteArray errorOutput = code_run_in_cmd->readAllStandardError();
                    QString errorString = QString::fromUtf8(errorOutput);
                    textbottom->append("Error: " + errorString); // Append error to the QTextEdit
                });

                // connect(code_run_in_cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmd_process_finished(int,QProcess::ExitStatus)));

                QObject::connect(code_run_in_cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) mutable {
                    if (exitStatus == QProcess::NormalExit) {
                        textbottom->append("=====================Process Finished=====================");
                    } else {
                        textbottom->append("=====================Process Finished=====================");
                    }
                });
            }
            else {
//                QString warn_info = "\""+cob_file_path+"\" "+"does not exist.";
                QMessageBox::warning(this, tr("Warning"),
                                     "File not found: "+cob_file_path);
                qDebug()<<"File needed: "<<cob_file_path;
                return;
            }
        }
        else {
            qDebug()<<"Run as cos"<<code_type;
        }

//        textbottom->append(resultInfo);
//        textbottom->append("程序运行完成");
//        textbottom->append("===================Program Execution Ended===================\n");
//        QScrollBar *scrollbar = textbottom->verticalScrollBar();
//        if(scrollbar){
//            scrollbar->setSliderPosition(scrollbar->maximum());
//        }


        }
    }
//    //======刷新文件树=================================


//    QString projectfilePath = coolang_project_path;
//    QString rootname=projectfilePath;
//    int l = rootname.lastIndexOf("/");

//    dir_watcher->removePath(curdir);
//    curdir = projectfilePath;
//    rootname = rootname.right(rootname.size()-l-1);
//    curdir = rootname;
//    dir_watcher->addPath(curdir);

//    projectNameToPath.insert(rootname,projectfilePath);
//    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
//    root->setIcon(0, QIcon(":/folder.png"));
//    root->setCheckState(1, Qt::Checked);
//    root->setToolTip(0,projectfilePath);



//    QDir *rootdir = new QDir(projectfilePath);
//    root = new QTreeWidgetItem();
//    file_tree_menu->clear();
//    file_tree_menu->addTopLevelItem(root);
//    file_tree_menu->topLevelItem(0)->setToolTip(0,projectfilePath);
//    //将文件树最上方的根设置为当前目录

//    root->setText(0,rootdir->dirName());
//    FindFile(rootdir,root);
//    projectToChildren.insert(root->text(0),childrenNameList);
//    childrenNameList.clear();
////    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
//    QApplication::processEvents(); //防止界面假死
//    root->setExpanded(true);

//    //删除当前cmd子进程
//    code_run_in_cmd = NULL;

}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::MarginMarker(int m, int n, Qt::KeyboardModifiers)
{
    //m为被点击的页边的编号（列），n为行
       int maskn=textEdit->SendScintilla(QsciScintilla::SCI_MARKERGET,n);
       //qDebug()<<n<<endl;
       if(maskn == 0)
       {
           textEdit->SendScintilla(QsciScintilla::SCI_MARKERADD,n,m);
           QString str = "break "+QString::number(n+1)+"\r\n";
           qDebug()<<str;
           emit debug_break_information(str);
       }
       else
       {
            textEdit->SendScintilla(QsciScintilla::SCI_MARKERDELETE,n,m);
            QString str = "clear "+QString::number(n+1)+"\r\n";
            emit debug_break_information(str);
       }

}

//==========================文件树函数=====================================================
NewBulidFile::NewBulidFile(QWidget *parent):QDialog(parent){
    QLabel *fileNameLabel = new QLabel("File Name");
    QLabel *fileNameTypeLabel = new QLabel("File Type");
    fileNameEdit = new QLineEdit();
    fileNameTypeBox = new QComboBox();
    fileNameTypeBox->addItem(".cos");
    fileNameTypeBox->addItem(".txt"); //这里可以多加几个Item,方法类似，用于新建文件时指定文件的类型
    fileNameTypeBox->addItem(".c");
    fileNameTypeBox->addItem(".cpp");
    fileNameTypeBox->addItem(".py");
    okButton = new QPushButton("OK");
    cancalButton = new QPushButton("Cancel");

    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(fileNameLabel);
    Hlayout1->addWidget(fileNameEdit);
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(fileNameTypeLabel);
    Hlayout2->addWidget(fileNameTypeBox);
    QHBoxLayout *Hlayout3 = new QHBoxLayout();
    Hlayout3->addWidget(okButton);
    Hlayout3->addWidget(cancalButton);
    QVBoxLayout *MainVlayout = new QVBoxLayout(this); //布局使用嵌套布局
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout2);
    MainVlayout->addLayout(Hlayout3);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}


NewBuildfolder::NewBuildfolder(QWidget *parent):QDialog(parent){
    QLabel *fileNameLabel = new QLabel("New Folder");
    fileNameEdit = new QLineEdit();
    okButton = new QPushButton("OK");
    cancalButton = new QPushButton("Cancel");
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    Hlayout1->addWidget(fileNameLabel);
    Hlayout1->addWidget(fileNameEdit);
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    Hlayout2->addWidget(okButton);
    Hlayout2->addWidget(cancalButton);
    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
    MainVlayout->addLayout(Hlayout1);
    MainVlayout->addLayout(Hlayout2);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

}


// 创建文件树的根
void MainWindow::CreateTopItem(QString path){
    QDir *rootdir = new QDir(path);
    root = new QTreeWidgetItem();
    root->setText(0,rootdir->dirName());
    root->setToolTip(0,path);
    file_tree_menu->addTopLevelItem(root);
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);
}

// 创建文件树
void MainWindow::FindFile(QDir *path,QTreeWidgetItem *parent){
    QApplication::processEvents(); //防止界面假死
    QFileInfoList file_list = path->entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks,
                                                     QDir::Size | QDir::Reversed);   //获取文件信息列表
    QStringList AllDirName = path->entryList();  //返回所有文件名
    QString PathContent = path->absolutePath();
    for(int i=2;i<AllDirName.size();i++) //从该目录开始获取文件和文件夹，i如果等于1则会直接把上一层目录遍历一遍，会卡死
    {
        QFileInfo *FileInfo = new QFileInfo(PathContent+"/"+AllDirName[i]);
        if(FileInfo->isFile())
        {
            QTreeWidgetItem *child = new QTreeWidgetItem(parent); //创建对象并添加所属关系
            QString File_tree_file = AllDirName[i];
//            qDebug()<<File_tree_file;

            if(File_tree_file.indexOf(".py")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/py.png")); //设置Item的图标，python
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".java")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/JAVA.png")); //设置Item的图标，java
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".txt")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/txt.png")); //设置Item的图标，txt
            }
            else if(File_tree_file.indexOf(".exe")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/exe.png")); //设置Item的图标，txt
            }
            else if(File_tree_file.indexOf(".规")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/gui.png")); //设置Item的图标，规语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".cos")!=-1||File_tree_file.indexOf(".COS")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/cos.png")); //设置Item的图标，coolang语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".coc")!=-1||File_tree_file.indexOf(".COC")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/coc.png")); //设置Item的图标，coolang语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".cob")!=-1||File_tree_file.indexOf(".COB")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/cob.png")); //设置Item的图标，coolang语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".cos1")!=-1||File_tree_file.indexOf(".cos2")!=-1||File_tree_file.indexOf(".cos3")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/tmp.png")); //设置Item的图标，coolang语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".setting")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/filetree_file.png")); //设置Item的图标，coolang语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".cpp")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".c")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else if(File_tree_file.indexOf(".h")!=-1)
            {
                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
                childrenNameList.append(PathContent+"/"+AllDirName[i]);//将所有目录下的编程文件放到childrenlist中
            }
            else
            {
                child->setIcon(0,QIcon(":/images/icon/unknown.png")); //设置Item的图标，其他文件
            }
            child->setText(0,AllDirName[i]); //设置Item的名字及所扫描到的文件名。
            child->setToolTip(0,PathContent+"/"+AllDirName[i]);
            fileNameToPath.insert(AllDirName[i],PathContent+"/"+AllDirName[i]);
//            qDebug()<<AllDirName[i]<<"     "<<PathContent+"/"+AllDirName[i];
            //projectToChildren.insert(rootname,childrenNameList);
        }
        else if(FileInfo->isDir())
        {
            QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]); //返回包含文件名的绝对路径。
            QTreeWidgetItem *child = new QTreeWidgetItem(parent);  //创建对象并添加所属关系。
//            qDebug()<<child;
            child->setIcon(0,QIcon(":/images/icon/filetree_dir.png")); //设置Item的图标
            child->setText(0,AllDirName[i]);  //设置Item的名字及所扫描到的文件名。
            child->setToolTip(0,PathContent+"/"+AllDirName[i]); //鼠标移动到Item上所显示的内容，这里为绝对路径。
            FindFile(NextDir,child);
        }
    }
}

// 双击槽函数检测是文件还是目录
void MainWindow::DoubleClickpath(QTreeWidgetItem *item, int /*column*/){
    QString absolute_Path = item->toolTip(0);
    QFileInfo *FileInfo = new QFileInfo(absolute_Path);
    file_tree_menu->resizeColumnToContents(0);

    if(FileInfo->isFile())
    {
        // 发送信号量打开文件
        //emit showTextSignal(absolute_Path,FileInfo->baseName(),item);
        emit showTextSignal(absolute_Path);
        qDebug()<<absolute_Path<<" "<<FileInfo->baseName();
    }
    else{
        // 由于不是文件所以除了执行展开或折叠文件外不再需要执行其他操作，因此此处不需要执行别的操作。
    }
}


// 鼠标右键单击文件或文件夹时所执行的操作
void MainWindow::itemPressedSlot(QTreeWidgetItem * pressedItem, int column){
    file_tree_menu->resizeColumnToContents(0);
    nowItem = pressedItem;
    nowCol = column;
    if(qApp->mouseButtons() == Qt::RightButton)   //判断是否为右键
    {
        QString absolute_Path = pressedItem->toolTip(0);
        QFileInfo *FileInfo = new QFileInfo(absolute_Path);
        if(FileInfo->isFile()){
            treefileMenu->exec(QCursor::pos()); //显示右键菜单栏
        }
        else{
            dirMenu->exec(QCursor::pos());  //显示右键菜单栏
        }
    }
}

void MainWindow::tempActionInformation(QAction *act){
    qDebug() << "Item " <<act->text();
    if(act->text()=="Close Project"){
        QTreeWidgetItem *item = file_tree_menu->currentItem();
        QTreeWidgetItem* parItem=item->parent();
        if(parItem==0)
        {
            delete item;
        }
        else {
            parItem->removeChild(item) ;
            delete item;
        }
    }
    if(act->text() == "Delete"){
        if (QMessageBox::Yes == QMessageBox::question(this,
                      tr("Delete"), tr("Are you sure to perform this action?"),
                      QMessageBox::Yes | QMessageBox::No,
                      QMessageBox::Yes))
        {
            QTreeWidgetItem *item = file_tree_menu->currentItem();

            //获取当前所选文件路径
            QString clicked_file_path = nowItem->toolTip(0);
            qDebug()<<nowItem->toolTip(0);
            QFile::remove(clicked_file_path);


            delete item;
        }


    }
    if(act->text() == "Delete Folder"){
        if (QMessageBox::Yes == QMessageBox::question(this,
                      tr("Delete Folder"), tr("Are you sure to perform this action?"),
                      QMessageBox::Yes | QMessageBox::No,
                      QMessageBox::Yes))
        {
            QTreeWidgetItem *item = file_tree_menu->currentItem();
            QString clicked_file_path = nowItem->toolTip(0);
            QDir clicked_dir(clicked_file_path);
            clicked_dir.removeRecursively();
            qDebug()<<clicked_file_path;
            delete item;
        }

    }
    if(act->text() == "Rename"){
        QString file_old_name = nowItem->toolTip(0);
        QString file_new_name;
        QTreeWidgetItem *t = file_tree_menu->currentItem();

        nowItem->setFlags(Qt::ItemIsEditable |Qt::ItemIsEnabled);
        file_tree_menu->editItem(nowItem,0);
        file_new_name = t->text(0);
        nowItem->setFlags(Qt::ItemIsEnabled);
        QString clicked_file_path = nowItem->toolTip(0);
        emit MainWindow::renameSignal(t);
    }
    if(act->text() == "Show in Explorer"){
        // QString path = nowItem->toolTip(0);
        // int index=0;
        // for(int i=0;i<path.length();i++){
        //     if(path[i]=="/")
        //         index=i;
        // }
        // QString filePath = path.mid(0,index).replace("/","\\");
        // QProcess::startDetached("explorer "+filePath);//打开上面获取的目录
        QString path = nowItem->toolTip(0);
        int index = 0;

        for (int i = 0; i < path.length(); i++) {
            if (path[i] == "/") {
                index = i;
            }
        }

        QString filePath = path.mid(0, index).replace("/", "\\");

        // Wrap file path in quotes to handle spaces in path names
        QString command = QString("explorer \"%1\"").arg(filePath);

        // Launch explorer using QProcess
        if (!QProcess::startDetached(command)) {
            qWarning() << "Failed to launch explorer for path:" << filePath;
        }

    }
    if(act->text() == "New File"){
        newFileWidget->setModal(true);
        newFileWidget->show();
    }
    if(act->text()=="New Folder"){
        newDirWidget->setModal(true);
        newDirWidget->exec();
    }
}

// 文件夹折叠的实现主要为顶部按钮提供方法
void MainWindow::buttonCollapseItem(){
    QTreeWidgetItem *t = file_tree_menu->currentItem();
    if(t==0)
        return;
    if(t->childCount()==0){
        QTreeWidgetItem *p = t->parent();
        if(p==0){
            return;
        }
        else{
            emit collapseItemSignal(p); //折叠文件夹
        }
    }
    else{
        emit collapseItemSignal(t);
    }
}

// 创建文件的具体实现,新建文件文件树下新建文件
void MainWindow::bulidNewFileSlot(bool /*flag*/){
    QString fileName = newFileWidget->fileNameEdit->text();
    QString filetype = newFileWidget->fileNameTypeBox->currentText();
    if(fileName==""){
        QMessageBox::warning(this, tr("Warning"),
                             tr("File name cannot be empty."));
        return;
    }
    QString tempName = fileName+filetype;
    QString path = nowItem->toolTip(0)+"/"+tempName;
    qDebug()<<path;
    QFile file(path);
    if(file.exists()){
        QMessageBox::warning(this, tr("Warning"),
                             tr("File already exists"));
        return;
    }
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.close();
    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setText(0,tempName);

    if(filetype.indexOf(".py")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/py.png")); //设置Item的图标，python
    }
    else if(filetype.indexOf(".java")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/JAVA.png")); //设置Item的图标，java
    }
    else if(filetype.indexOf(".txt")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/txt.png")); //设置Item的图标，txt
    }
    else if(filetype.indexOf(".exe")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/exe.png")); //设置Item的图标，txt
    }
    else if(filetype.indexOf(".规")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/gui.png")); //设置Item的图标，规语言
    }
    else if(filetype.indexOf(".cos")!=-1||filetype.indexOf(".COS")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/cos.png")); //设置Item的图标，coolang语言
    }
    else if(filetype.indexOf(".coc")!=-1||filetype.indexOf(".COC")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/coc.png")); //设置Item的图标，coolang语言
    }
    else if(filetype.indexOf(".cob")!=-1||filetype.indexOf(".COB")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/cob.png")); //设置Item的图标，coolang语言
    }
    else if(filetype.indexOf(".tmp")!=-1||filetype.indexOf(".cos1")!=-1||filetype.indexOf(".cos2")!=-1||filetype.indexOf(".cos3")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/tmp.png")); //设置Item的图标，coolang语言
    }
    else if(filetype.indexOf(".setting")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/filetree_file.png")); //设置Item的图标，coolang语言
    }
    else if(filetype.indexOf(".cpp")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
    }
    else if(filetype.indexOf(".c")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
    }
    else if(filetype.indexOf(".h")!=-1)
    {
        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
    }
    else
    {
        child->setIcon(0,QIcon(":/images/icon/unknown.png")); //设置Item的图标，其他文件
    }
    //这里根据新建文件类型设置图标
    child->setToolTip(0,path);
    newFileWidget->close();
}

//取消按钮的具体实现
void MainWindow::closeBuildFileSlot(bool /*flag*/){
    newFileWidget->close();
}


// 创建文件夹的具体实现
void MainWindow::buildNewDirSlot(bool /*flag*/){
    QString dirName = newDirWidget->fileNameEdit->text();
    if(dirName==""){
        QMessageBox::warning(this, tr("Warning"),
                             tr("File name cannot be empty"));
        return;
    }
    QString path = nowItem->toolTip(0)+"/"+dirName;
    QDir *temp = new QDir;
    bool exist = temp->exists(path);
    if(exist){
        QMessageBox::warning(this,tr("New Folder"),tr("File already exists"));
        return;
    }
    temp->mkdir(path);
    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setText(0,dirName);
    child->setIcon(0,QIcon(":/images/icon/filetree_dir.png"));
    child->setToolTip(0,path);
    newDirWidget->close();
}

// 取消按钮的具体实现
void MainWindow::cancelDirSlot(bool /*flag*/){
    newDirWidget->close();
}

void MainWindow::dir_change(const QString &path)
{
    qDebug()<<curdir;
    qDebug()<<dir_watcher->directories();
    qDebug()<<"dir_change"<<path;

    QString rootname=path;
    int l = rootname.lastIndexOf("/");
    rootname = rootname.right(rootname.size()-l-1);
    projectNameToPath.insert(rootname,path);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);

    QDir *rootdir = new QDir(path);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
//    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);
    return;
}


//文件数双击打开文件
void MainWindow::open_filetree_file(QString file_path)
{// 载入文件
    if(!maybeSave()){
        return;
    }
    QFile file(file_path);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Open File"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file_path), file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());

    QString cFormat_1 = ".cpp";
    QString cFormat_2 = ".h";
    QString cFormat_3 = ".c";
    QString pythonFormat = ".py";
    QString javaFormat = ".java";
    QString my_code_language = ".规";
    QString txt_file = ".txt";
    QString cos_file = ".cos";
    QString file_type;
    file_type = file_path.section('.',-1,-1);//截取文件名最后一段为文件类型
    if(file_type == "c")
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<QString("load C file");
        CUR_FILE_TYPE = C_FILE;
    }

    else if(file_type == "cpp")
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<QString("load C file");
        CUR_FILE_TYPE = C_FILE;
    }
    else if(file_type == "h")
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<QString("load c file");
        CUR_FILE_TYPE = C_FILE;
    }
    else if(file_type == "py")
    {
        Lexer = new QsciLexerPython;
        textEdit->setLexer(Lexer);
        apis->clear();
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-python.txt")))//给api添加词源
            QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));
        else
            qDebug()<<QString(":/auto-python.txt");
        apis->prepare();
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = PYTHON_FILE;
        qDebug()<<QString("load python file");
    }
    else if(file_path.indexOf(javaFormat)!=-1)
    {
        Lexer = new QsciLexerJava;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-java.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-java.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = JAVA_FILE;
    }
    else if(file_type == "规"||file_type == ".规")
    {
        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/auto-cool.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<"规文件加载";
        CUR_FILE_TYPE = GUI_TYPE_FILE;
    }
    else if (file_type == "cos") {

        Lexer = new QsciLexerCPP;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/auto-cool.txt")))//给api添加词源
        {QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString("load cos file");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        qDebug()<<"cos file loaded";
        CUR_FILE_TYPE = COS_FILE;
    }
    else
    {
        Lexer = new QsciLexerTeX;
        textEdit->setLexer(Lexer);
        apis = new QsciAPIs(Lexer); //实例化一个api，将其插入到lexer父对象
        if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        { QMessageBox::warning(this,QString("Warning"),QString("Failed to read lexer file"));}
        else
        {
            qDebug()<<QString(":/autocpp.txt");
            apis->prepare();
        }
        textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);//设置补全词源为当前文件和添加的api词源
        CUR_FILE_TYPE = DOC_FILE;
    }
    QApplication::restoreOverrideCursor();

    setCurrentFile(file_path);
    statusBar()->showMessage(tr("File loaded"), 2000);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    loadstyle(cur_style);
    //底部窗口输出信息
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm:ss");
    textbottom->append(text+":   "+file_path+" loaded\n");
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::rename_filetree_file(QTreeWidgetItem *now_tree_item,int a)
{
    QString old_path = now_tree_item->toolTip(0);
    QString new_path;
    QFile change_file(old_path);
    QString file_type = old_path.section('.',-1,-1);
    new_path = old_path.section('/',0,-2);
    new_path.append("/"+now_tree_item->text(0));//得到新命名文件路径
    if(change_file.rename(old_path,new_path) == true)
    {
        qDebug()<<new_path;
        now_tree_item->setToolTip(0,new_path);//更新文件树信息
        QTime time=QTime::currentTime();
        QString text=time.toString("hh:mm:ss");
        textbottom->append(text+":   "+now_tree_item->text(0)+"重命名成功");
        statusBar()->showMessage(tr("rename successful"), 2000);
    }
    else
    {
//        textbottom->append(old_path+"重命名失败");
        now_tree_item->setToolTip(0,old_path);
    }
    return;


}

void MainWindow::refresh_tree(const QString &path)
{
    QString rootname=path;
    int l = rootname.lastIndexOf("/");
    rootname = rootname.right(rootname.size()-l-1);
    projectNameToPath.insert(rootname,path);
    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList()<<rootname);
    root->setIcon(0, QIcon(":/folder.png"));
    root->setCheckState(1, Qt::Checked);

    QDir *rootdir = new QDir(path);
    root = new QTreeWidgetItem();
    file_tree_menu->clear();
    file_tree_menu->addTopLevelItem(root);
    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    projectToChildren.insert(root->text(0),childrenNameList);
    childrenNameList.clear();
    qDebug()<<projectToChildren.find("build-application-Desktop_Qt_5_13_0_MinGW_32_bit-Debug").value();
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);
    return;
}

void MainWindow::rename_start(const QTreeWidgetItem *nowitem)
{
    connect(file_tree_menu,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(rename_filetree_file(QTreeWidgetItem*,int)));
    return;
}

void MainWindow::ShowTime()
{

    QTime time=QTime::currentTime();//Qtime是时间函数
    QString text=time.toString("hh:mm:ss");
    time_label->setText(text);

}

void MainWindow::ClearAllMarker()
{
    textEdit->SendScintilla(QsciScintilla::SCI_MARKERDELETEALL,1,0x02);
    QString str = "delete\r\n";
    emit debug_break_information(str);
    debug_cmd->waitForReadyRead();
    str = "y\r\n";
    emit debug_break_information(str);
}

//=======================window style===========================

void MainWindow::Ubuntu_style_clicked()
{
    //读取编译setting文件,将 curstyle 一栏修改
    QFile f(IDE_Setting_path);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString setting_txt(f.readAll());
    QStringList settinglist = setting_txt.split("\n");
    settinglist.replace(1,"curstyle:ubantu");
    setting_txt = settinglist.join("\n");

    f.close();
    f.open(QIODevice::Truncate | QIODevice::Text);
    f.close();
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write(setting_txt.toUtf8());
    f.close();


    cur_style = "ubantu";
    loadstyle(cur_style);
}
void MainWindow::BrightWhite_style_clicked()
{
    //读取编译setting文件,将 curstyle 一栏修改
    QFile f(IDE_Setting_path);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString setting_txt(f.readAll());
    QStringList settinglist = setting_txt.split("\n");
    settinglist.replace(1,"curstyle:BrightWhite");
    setting_txt = settinglist.join("\n");

    f.close();
    f.open(QIODevice::Truncate | QIODevice::Text);
    f.close();
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write(setting_txt.toUtf8());
    f.close();

    cur_style = "BrightWhite";
    loadstyle(cur_style);
}

void MainWindow::ElegantDark_style_clicked()
{
    //读取编译setting文件,将 curstyle 一栏修改
    QFile f(IDE_Setting_path);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString setting_txt(f.readAll());
    QStringList settinglist = setting_txt.split("\n");
    settinglist.replace(1,"curstyle:ElegantDark");
    setting_txt = settinglist.join("\n");

    f.close();
    f.open(QIODevice::Truncate | QIODevice::Text);
    f.close();
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write(setting_txt.toUtf8());
    f.close();

    cur_style = "ElegantDark";
    loadstyle(cur_style);
}


void MainWindow::loadstyle(const QString &style)
{
    if(style == "ubantu")
    {
        QFile file(":/window_styleQss/Ubuntu.qss");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        // newproject_dialog->setStyleSheet(stylesheet);
        debug_dialog->setStyleSheet(stylesheet);
        file_tree_menu->setStyleSheet(stylesheet);
        textbottom->setStyleSheet(stylesheet);
        file.close();
        textEdit->setPaper(QColor(0,0,0));
        textEdit->setStyleSheet(stylesheet);
        Lexer->setPaper(QColor(130,0,130),-1);
        Lexer->setColor(QColor(Qt::white));
        Lexer->setColor(QColor(Qt:: yellow),QsciLexerCPP::DoubleQuotedString);
        Lexer->setColor(QColor(250,235,215),QsciLexerCPP::Number);
        Lexer->setColor(QColor(255,128,10),QsciLexerCPP::Keyword);
        Lexer->setColor(QColor(61,145,64),QsciLexerCPP::Comment);
        Lexer->setColor(QColor(240,255,255),QsciLexerCPP::Operator);

        Lexer->setColor(QColor(Qt:: yellow),QsciLexerPython::DoubleQuotedString);
        Lexer->setColor(QColor(250,235,215),QsciLexerPython::Number);
        Lexer->setColor(QColor(255,128,10),QsciLexerPython::Keyword);
        Lexer->setColor(QColor(61,145,64),QsciLexerPython::Comment);
        Lexer->setColor(QColor(240,255,255),QsciLexerPython::Operator);
        Lexer->setColor(QColor(Qt:: yellow),QsciLexerPython::SingleQuotedString);
        Lexer->setColor(QColor(Qt::gray),QsciLexerCPP::UnclosedString);
        textEdit->setMarginsBackgroundColor(QColor(128,138,135));
        textEdit->setPaper(QColor(246,134,86));
        textEdit->update();
        textEdit->setSelectionBackgroundColor(QColor(246,134,86));
        textEdit->setCallTipsBackgroundColor(QColor(176,48,96));
        qDebug()<<"change style";
    }
    else if (style == "BrightWhite") {
        QFile file(":/window_styleQss/Aqua.qss");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        // newproject_dialog->setStyleSheet(stylesheet);
        debug_dialog->setStyleSheet(stylesheet);
        file_tree_menu->setStyleSheet(stylesheet);
        textbottom->setStyleSheet(stylesheet);
        file.close();

        Lexer->setPaper(QColor(250,250,250),-1);//背景颜色
        Lexer->setColor(QColor(41,36,33));//默认字体颜色
        Lexer->setColor(QColor(138,43,226),QsciLexerCPP::DoubleQuotedString);//双引号内颜色
        Lexer->setColor(QColor(0,225,225),QsciLexerCPP::Number);//数字颜色
        Lexer->setColor(QColor(128,138,135),QsciLexerCPP::Keyword);//关键字颜色
        Lexer->setColor(QColor(106,90,205),QsciLexerCPP::Comment);//注释颜色
        Lexer->setColor(QColor(0,0,0),QsciLexerCPP::Operator);//运算符颜色

        Lexer->setColor(QColor(138,43,226),QsciLexerPython::DoubleQuotedString);
        Lexer->setColor(QColor(0,225,225),QsciLexerPython::Number);
        Lexer->setColor(QColor(128,138,135),QsciLexerPython::Keyword);
        Lexer->setColor(QColor(106,90,205),QsciLexerPython::Comment);
        Lexer->setColor(QColor(0,0,0),QsciLexerPython::Operator);
        Lexer->setColor(QColor(138,43,226),QsciLexerPython::SingleQuotedString);
        Lexer->setColor(QColor(Qt::gray),QsciLexerCPP::UnclosedString);

        textEdit->setMarginsBackgroundColor(QColor(240,240,240));
        textEdit->update();
        textEdit->setSelectionBackgroundColor(QColor(246,134,86));
        textEdit->setCallTipsBackgroundColor(QColor(176,48,96));
        qDebug()<<"change style";

    }
    else if (style == "ElegantDark") {

        QFile file(":/window_styleQss/ElegantDark.qss");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        // newproject_dialog->setStyleSheet(stylesheet);
        debug_dialog->setStyleSheet(stylesheet);
        file_tree_menu->setStyleSheet(stylesheet);
        textbottom->setStyleSheet(stylesheet);
        file.close();
        textEdit->setPaper(QColor(82,82,82));
        textEdit->setStyleSheet(stylesheet);
        Lexer->setPaper(QColor(82,82,82),-1);
        Lexer->setColor(QColor(Qt::white));
        Lexer->setColor(QColor(Qt:: yellow),QsciLexerCPP::DoubleQuotedString);
        Lexer->setColor(QColor(250,235,215),QsciLexerCPP::Number);
        Lexer->setColor(QColor(255,128,10),QsciLexerCPP::Keyword);
        Lexer->setColor(QColor(61,145,64),QsciLexerCPP::Comment);
        Lexer->setColor(QColor(240,255,255),QsciLexerCPP::Operator);

        Lexer->setColor(QColor(Qt:: yellow),QsciLexerPython::DoubleQuotedString);
        Lexer->setColor(QColor(250,235,215),QsciLexerPython::Number);
        Lexer->setColor(QColor(255,128,10),QsciLexerPython::Keyword);
        Lexer->setColor(QColor(61,145,64),QsciLexerPython::Comment);
        Lexer->setColor(QColor(240,255,255),QsciLexerPython::Operator);
        Lexer->setColor(QColor(Qt:: yellow),QsciLexerPython::SingleQuotedString);
        Lexer->setColor(QColor(Qt::gray),QsciLexerCPP::UnclosedString);
        textEdit->setMarginsBackgroundColor(QColor(128,138,135));
        textEdit->update();
        textEdit->setSelectionBackgroundColor(QColor(246,134,86));
        textEdit->setCallTipsBackgroundColor(QColor(176,48,96));
        qDebug()<<"change style";

    }

    return;
}
//=======================================中文路径支持===================================
/**
 * @brief convert char * to wstring
 * @param charp
 * @param codePage
 * @param existUnknownCharacters if there are unknown character in charp, set to 1.
 * @return
 */
std::wstring MainWindow::charpToWString(const char *charp, const int codePage,
        int *existUnknownCharacters) {
    int num = MultiByteToWideChar(codePage, 0, charp, -1,
    NULL, 0);
    wchar_t *wide = new wchar_t[num + 1];
    int writeInNum = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, charp,
            -1, wide, num);
    std::wstring w_str(wide);
    delete[] wide;
    if (writeInNum == 0 && num != 0) {
        *existUnknownCharacters = true;
    } else {
        *existUnknownCharacters = false;
    }
    return w_str;
}

std::wstring MainWindow::StringToWString(const string &str) {
    auto codepagev = { CP_UTF8, CP_ACP, CP_MACCP, CP_OEMCP, CP_SYMBOL,
    CP_THREAD_ACP, CP_UTF7 };

    int existUnknownCharacters1 = false;
    wstring ws;
    for (auto pg : codepagev) {

        ws = charpToWString(str.c_str(), pg, &existUnknownCharacters1);

        if (existUnknownCharacters1 == 0) {
            break;
        }
    }
    return ws;
}

std::string MainWindow::WStringToString(const wstring &wstr, const int codePage,
        int *existUnknownCharacters) {

    int iSize;

    char *strBytes;
    char rep = '?';

    iSize = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS, wstr.c_str(),
            -1, NULL, 0, NULL,
            NULL);

    strBytes = new char[(iSize + 1)]/**sizeof(char)*/;

    int writeInNum = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS,
            wstr.c_str(), -1, strBytes, iSize, &rep, existUnknownCharacters);

    std::string str(strBytes);
    delete[] strBytes;
    if (writeInNum == 0 && iSize != 0) {
        *existUnknownCharacters = true;
    } else {
        *existUnknownCharacters = false;
    }
    return str;
}

/**
 * @brief
 * @param wstr
 * @return
 */
std::string MainWindow::WStringToString(const wstring &wstr) {

    string s;

    int existUnknownCharacters1 = false;
    s = WStringToString(wstr.c_str(), CP_ACP, &existUnknownCharacters1);

    return s;
}

void terminateProcessByName(const QString& processName) {
    // Take a snapshot of all processes
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        qWarning("Failed to create process snapshot.");
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Iterate through the process list
    if (Process32First(hProcessSnap, &pe32)) {
        do {
            // Compare process name
            if (QString::fromWCharArray(pe32.szExeFile) == processName) {
                // Open the process with terminate rights
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0); // Terminate the process
                    CloseHandle(hProcess);
                    qDebug("Process terminated: %s", processName.toUtf8().constData());
                } else {
                    qWarning("Failed to open process: %s", processName.toUtf8().constData());
                }
            }
        } while (Process32Next(hProcessSnap, &pe32));
    } else {
        qWarning("Failed to retrieve process information.");
    }

    // Clean up
    CloseHandle(hProcessSnap);
}
