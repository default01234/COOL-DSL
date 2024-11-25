//#include "treemenu.h"
//#include<QPushButton>
//#include <QLabel>
//#include <QLayout>
//#include <QApplication>
//#include <QMenu>
//#include <QDebug>
//#include <QProcess>
//#include <QMessageBox>
//#include <QScrollBar>
//#include <QHeaderView>
//#include <QTreeWidget>
//#include <QTreeWidgetItem>
//#include <QKeyEvent>

//treemenu::treemenu(QWidget *parent):QTreeWidget(parent)
//{
//        this->setObjectName("TreeMenu");
//        this->verticalScrollBar()->setObjectName("TreeMenuVerticl");
//        this->horizontalScrollBar()->setObjectName("TreeMenuHorizontal");



//        this->setColumnCount(1);
//        this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
//        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//        this->setAutoScroll(true);
//        this->header()->setVisible(false);   //隐藏表头

//        this->setFrameStyle(QFrame::Sunken);
//        this->setAnimated(true); //设置折叠动画


//        this->setAnimated(true);
//        this->setIndentation(20);
//        this->setSortingEnabled(true);
//        this->setColumnWidth(0,this->width()/3);
//        this->setHeaderHidden(true);
//        this->setColumnHidden(1,true);
//        this->setColumnHidden(2,true);
//        this->setColumnHidden(3,true);
//        QFont file_tree_font("ZYSong18030",10);
//        this->setFont(file_tree_font);

//        newFileWidget = new NewBulidFile();
//        connect(newFileWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(bulidNewFileSlot(bool)));
//        connect(newFileWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(closeBuildFileSlot(bool)));

//        newDirWidget = new NewBuildfolder();
//        connect(newDirWidget->okButton,SIGNAL(clicked(bool)),this,SLOT(buildNewDirSlot(bool)));
//        connect(newDirWidget->cancalButton,SIGNAL(clicked(bool)),this,SLOT(cancelDirSlot(bool)));

//        // 创建右击文件夹的菜单栏
//        dirMenu = new QMenu();
//        dirMenu->addAction("新建文件");
//        dirMenu->addAction("重命名");
//        dirMenu->addSeparator();
//        dirMenu->addAction("新建文件夹");
//        dirMenu->addAction("删除文件夹");
//        dirMenu->addAction("在文件夹中查找             ");
//        dirMenu->addAction("关闭项目");
//        // 创建右击文件的菜单栏
//        fileMenu = new QMenu();
//        fileMenu->addAction("重命名");
//        fileMenu->addAction("删除文件");
//        fileMenu->addAction("打开所在文件夹");
//        connect(dirMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));
//        connect(fileMenu,SIGNAL(triggered(QAction*)),this,SLOT(tempActionInformation(QAction*)));

//        connect(this,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(itemPressedSlot(QTreeWidgetItem*,int)));
//        connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(DoubleClickpath(QTreeWidgetItem*,int)));
//        connect(this,SIGNAL(collapseItemSignal(const QTreeWidgetItem*)),this,SLOT(collapseItem(const QTreeWidgetItem*)));



//}

//NewBulidFile::NewBulidFile(QWidget *parent):QDialog(parent){
//    QLabel *fileNameLabel = new QLabel("文件名");
//    QLabel *fileNameTypeLabel = new QLabel("文件类型");
//    fileNameEdit = new QLineEdit();
//    fileNameTypeBox = new QComboBox();
//    fileNameTypeBox->addItem(".txt"); //这里可以多加几个Item,方法类似，用于新建文件时指定文件的类型
//    fileNameTypeBox->addItem(".c");
//    fileNameTypeBox->addItem(".cpp");
//    fileNameTypeBox->addItem(".py");
//    okButton = new QPushButton("确定");
//    cancalButton = new QPushButton("取消");

//    QHBoxLayout *Hlayout1 = new QHBoxLayout();
//    Hlayout1->addWidget(fileNameLabel);
//    Hlayout1->addWidget(fileNameEdit);
//    QHBoxLayout *Hlayout2 = new QHBoxLayout();
//    Hlayout2->addWidget(fileNameTypeLabel);
//    Hlayout2->addWidget(fileNameTypeBox);
//    QHBoxLayout *Hlayout3 = new QHBoxLayout();
//    Hlayout3->addWidget(okButton);
//    Hlayout3->addWidget(cancalButton);
//    QVBoxLayout *MainVlayout = new QVBoxLayout(this); //布局使用嵌套布局
//    MainVlayout->addLayout(Hlayout1);
//    MainVlayout->addLayout(Hlayout2);
//    MainVlayout->addLayout(Hlayout3);
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
//}


//NewBuildfolder::NewBuildfolder(QWidget *parent):QDialog(parent){
//    QLabel *fileNameLabel = new QLabel("新建文件夹");
//    fileNameEdit = new QLineEdit();
//    okButton = new QPushButton("确定");
//    cancalButton = new QPushButton("取消");
//    QHBoxLayout *Hlayout1 = new QHBoxLayout();
//    Hlayout1->addWidget(fileNameLabel);
//    Hlayout1->addWidget(fileNameEdit);
//    QHBoxLayout *Hlayout2 = new QHBoxLayout();
//    Hlayout2->addWidget(okButton);
//    Hlayout2->addWidget(cancalButton);
//    QVBoxLayout *MainVlayout = new QVBoxLayout(this);
//    MainVlayout->addLayout(Hlayout1);
//    MainVlayout->addLayout(Hlayout2);
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

//}


//// 创建文件树的根
//void treemenu::CreateTopItem(QString path){
//    QDir *rootdir = new QDir(path);
//    root = new QTreeWidgetItem();
//    this->addTopLevelItem(root);
//    root->setText(0,rootdir->dirName());
//    FindFile(rootdir,root);
//    QApplication::processEvents(); //防止界面假死
//    root->setExpanded(true);
//}

//// 创建文件树
//void treemenu::FindFile(QDir *path,QTreeWidgetItem *parent){
//    QApplication::processEvents(); //防止界面假死
//    QStringList AllDirName = path->entryList();  //返回所有文件名
//    QString PathContent = path->absolutePath();
//    for(int i=2;i<AllDirName.size();i++) //从该目录开始获取文件和文件夹，i如果等于1则会直接把上一层目录遍历一遍，会卡死
//    {
//        QFileInfo *FileInfo = new QFileInfo(PathContent+"/"+AllDirName[i]);
//        if(FileInfo->isFile())
//        {
//            QTreeWidgetItem *child = new QTreeWidgetItem(parent); //创建对象并添加所属关系
//            QString File_tree_file = AllDirName[i];
////            qDebug()<<File_tree_file;
//            if(File_tree_file.indexOf(".cpp")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//            }
//            else if(File_tree_file.indexOf(".c")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//            }
//            else if(File_tree_file.indexOf(".h")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//            }
//            else if(File_tree_file.indexOf(".py")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/py.png")); //设置Item的图标，python
//            }
//            else if(File_tree_file.indexOf(".java")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/JAVA.png")); //设置Item的图标，java
//            }
//            else if(File_tree_file.indexOf(".txt")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/txt.png")); //设置Item的图标，txt
//            }
//            else if(File_tree_file.indexOf(".exe")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/exe.png")); //设置Item的图标，txt
//            }
//            else if(File_tree_file.indexOf(".规")!=-1)
//            {
//                child->setIcon(0,QIcon(":/images/icon/gui.png")); //设置Item的图标，规语言
//            }
//            else
//            {
//                child->setIcon(0,QIcon(":/images/icon/filetree_file.png")); //设置Item的图标，其他文件
//            }
//            child->setText(0,AllDirName[i]); //设置Item的名字及所扫描到的文件名。
//            child->setToolTip(0,PathContent+"/"+AllDirName[i]);
//            fileNameToPath.insert(AllDirName[i],PathContent+"/"+AllDirName[i]);
//        }
//        else if(FileInfo->isDir())
//        {
//            QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]); //返回包含文件名的绝对路径。
//            QTreeWidgetItem *child = new QTreeWidgetItem(parent);  //创建对象并添加所属关系。
//            qDebug()<<child;
//            child->setIcon(0,QIcon(":/images/icon/filetree_dir.png")); //设置Item的图标
//            child->setText(0,AllDirName[i]);  //设置Item的名字及所扫描到的文件名。
//            child->setToolTip(0,PathContent+"/"+AllDirName[i]); //鼠标移动到Item上所显示的内容，这里为绝对路径。
//            FindFile(NextDir,child);
//        }
//    }
//}

//// 双击槽函数检测是文件还是目录
//void treemenu::DoubleClickpath(QTreeWidgetItem *item, int /*column*/){
//    QString absolute_Path = item->toolTip(0);
//    QFileInfo *FileInfo = new QFileInfo(absolute_Path);
//    this->resizeColumnToContents(0);

//    if(FileInfo->isFile())
//    {
//        // 发送信号量打开文件
//        //emit showTextSignal(absolute_Path,FileInfo->baseName(),item);
//        emit showTextSignal(absolute_Path);
//        qDebug()<<absolute_Path<<" "<<FileInfo->baseName();
//    }
//    else{
//        // 由于不是文件所以除了执行展开或折叠文件外不再需要执行其他操作，因此此处不需要执行别的操作。
//    }
//}


//// 鼠标右键单击文件或文件夹时所执行的操作
//void treemenu::itemPressedSlot(QTreeWidgetItem * pressedItem, int column){
//    this->resizeColumnToContents(0);
//    nowItem = pressedItem;
//    nowCol = column;
//    if(qApp->mouseButtons() == Qt::RightButton)   //判断是否为右键
//    {
//        QString absolute_Path = pressedItem->toolTip(0);
//        QFileInfo *FileInfo = new QFileInfo(absolute_Path);
//        if(FileInfo->isFile()){
//            fileMenu->exec(QCursor::pos()); //显示右键菜单栏
//        }
//        else{
//            dirMenu->exec(QCursor::pos());  //显示右键菜单栏
//        }
//    }
//}

//void treemenu::tempActionInformation(QAction *act){
//    qDebug() << "Item " <<act->text();
//    if(act->text()=="关闭项目"){
//        QTreeWidgetItem *item = this->currentItem();
//        QTreeWidgetItem* parItem=item->parent();
//        if(parItem==0)
//        {
//            delete item;
//        }
//        else {
//            parItem->removeChild(item) ;
//            delete item;
//        }
//    }
//    if(act->text() == "删除文件"){
//        QTreeWidgetItem *item = this->currentItem();

//        //获取当前所选文件路径
//        QString clicked_file_path = nowItem->toolTip(0);
//        qDebug()<<nowItem->toolTip(0);
//        QFile::remove(clicked_file_path);


//        delete item;
//    }
//    if(act->text() == "删除文件夹"){
//        QTreeWidgetItem *item = this->currentItem();
//        QString clicked_file_path = nowItem->toolTip(0);
//        QDir clicked_dir(clicked_file_path);
//        clicked_dir.removeRecursively();
//        qDebug()<<clicked_file_path;
//        delete item;
//    }
//    if(act->text() == "重命名"){
//        QString file_old_name = nowItem->toolTip(0);
//        QString file_new_name;
//        QTreeWidgetItem *t = this->currentItem();

//        nowItem->setFlags(Qt::ItemIsEditable |Qt::ItemIsEnabled);
//        QTreeWidget::editItem(nowItem,0);
//        file_new_name = t->text(0);
//        nowItem->setFlags(Qt::ItemIsEnabled);
//        QString clicked_file_path = nowItem->toolTip(0);
//        emit treemenu::renameSignal(t);
//    }
//    if(act->text() == "打开所在文件夹"){
//        QString path = nowItem->toolTip(0);
//        int index=0;
//        for(int i=0;i<path.length();i++){
//            if(path[i]=="/")
//                index=i;
//        }
//        QString filePath = path.mid(0,index).replace("/","\\");
//        QProcess::startDetached("explorer "+filePath);//打开上面获取的目录

//    }
//    if(act->text() == "新建文件"){
//        newFileWidget->setModal(true);
//        newFileWidget->show();
//    }
//    if(act->text()=="新建文件夹"){
//        newDirWidget->setModal(true);
//        newDirWidget->exec();
//    }
//}

//// 文件夹折叠的实现主要为顶部按钮提供方法
//void treemenu::buttonCollapseItem(){
//    QTreeWidgetItem *t = this->currentItem();
//    if(t==0)
//        return;
//    if(t->childCount()==0){
//        QTreeWidgetItem *p = t->parent();
//        if(p==0){
//            return;
//        }
//        else{
//            emit collapseItemSignal(p); //折叠文件夹
//        }
//    }
//    else{
//        emit collapseItemSignal(t);
//    }
//}

//// 创建文件的具体实现,新建文件文件树下新建文件
//void treemenu::bulidNewFileSlot(bool /*flag*/){
//    QString fileName = newFileWidget->fileNameEdit->text();
//    QString filetype = newFileWidget->fileNameTypeBox->currentText();
//    if(fileName==""){
//        QMessageBox::warning(this, tr("警告"),
//                             tr("文件名不能为空"));
//        return;
//    }
//    QString tempName = fileName+filetype;
//    QString path = nowItem->toolTip(0)+"/"+tempName;
//    QFile file(path);
//    if(file.exists()){
//        QMessageBox::warning(this, tr("警告"),
//                             tr("文件已存在"));
//        return;
//    }
//    file.open(QIODevice::ReadWrite | QIODevice::Text);
//    file.close();
//    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
//    child->setText(0,tempName);
//    if(filetype.indexOf(".cpp")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//    }
//    else if(filetype.indexOf(".c")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//    }
//    else if(filetype.indexOf(".h")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/c++.png")); //设置Item的图标，c语言
//    }
//    else if(filetype.indexOf(".py")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/py.png")); //设置Item的图标，python
//    }
//    else if(filetype.indexOf(".java")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/JAVA.png")); //设置Item的图标，java
//    }
//    else if(filetype.indexOf(".txt")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/txt.png")); //设置Item的图标，txt
//    }
//    else if(filetype.indexOf(".exe")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/exe.png")); //设置Item的图标，txt
//    }
//    else if(filetype.indexOf(".规")!=-1)
//    {
//        child->setIcon(0,QIcon(":/images/icon/gui.png")); //设置Item的图标，规语言
//    }
//    else
//    {
//        child->setIcon(0,QIcon(":/images/icon/filetree_file.png")); //设置Item的图标，其他文件
//    }
//    //这里根据新建文件类型设置图标
//    child->setToolTip(0,path);
//    newFileWidget->close();
//}

////取消按钮的具体实现
//void treemenu::closeBuildFileSlot(bool /*flag*/){
//    newFileWidget->close();
//}


//// 创建文件夹的具体实现
//void treemenu::buildNewDirSlot(bool /*flag*/){
//    QString dirName = newDirWidget->fileNameEdit->text();
//    if(dirName==""){
//        QMessageBox::warning(this, tr("Application"),
//                             tr("文件名不能为空"));
//        return;
//    }
//    QString path = nowItem->toolTip(0)+"/"+dirName;
//    QDir *temp = new QDir;
//    bool exist = temp->exists(path);
//    if(exist){
//        QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
//        return;
//    }
//    temp->mkdir(path);
//    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
//    child->setText(0,dirName);
//    child->setIcon(0,QIcon(":/images/icon/filetree_dir.png"));
//    child->setToolTip(0,path);
//    newDirWidget->close();
//}

//// 取消按钮的具体实现
//void treemenu::cancelDirSlot(bool /*flag*/){
//    newDirWidget->close();
//}

