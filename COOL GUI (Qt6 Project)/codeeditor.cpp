#include "codeeditor.h"
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
#include <qsciapis.h>
#include <QDebug>

CodeEditor::CodeEditor()
{
    //文本高亮设置====================================================================================
    textEdit = new QsciScintilla;
    QsciLexerCPP *lexercpp = new QsciLexerCPP;
//    lexercpp->setColor(QColor(Qt:: yellow),QsciLexerCPP::CommentLine);//c++的注释设置为黄色
    textEdit->setLexer(lexercpp);

    //行号显示区域设置==================================================================================
    textEdit->setMarginType(0, QsciScintilla::NumberMargin);
    textEdit->setMarginLineNumbers(0,true);
    textEdit->setMarginWidth(0,50);
    //断点设置区域
//    textEdit->setMarginType(1, QsciScintilla::SymbolMargin);
//    textEdit->setMarginLineNumbers(1, false);
//    textEdit->setMarginWidth(1,20);
//    textEdit->setMarginSensitivity(1, true);    //设置是否可以显示断点
//    textEdit->setMarginMarkerMask(1, 0x02);
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
    QsciAPIs*apis = new QsciAPIs(lexercpp); //实例化一个api，将其插入到lexer父对象
    if(!apis->load(QString(":/autocpp.txt")))//给api添加词源
        QMessageBox::warning(NULL,QString("Warning"),QString("Failed to read file"));
    else
        qDebug()<<QString(":/autocpp.txt");
        apis->prepare();
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

}


void CodeEditor::MarginMarker(int m, int n, Qt::KeyboardModifiers)
{
    //m为被点击的页边的编号（列），n为行
       int maskn=textEdit->SendScintilla(QsciScintilla::SCI_MARKERGET,n);
       //qDebug()<<n<<endl;
       if(maskn == 0)
       {
           textEdit->SendScintilla(QsciScintilla::SCI_MARKERADD,n,m);
           QString str = "break "+QString::number(n+1)+"\r\n"+QString::number(m+1);
           qDebug()<<str;
       }
       else
       {
            textEdit->SendScintilla(QsciScintilla::SCI_MARKERDELETE,n,m);
       }

}
