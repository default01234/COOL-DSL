#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QMessageBox>
#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include <qscilexer.h>

class CodeEditor:public QsciScintilla
{
    Q_OBJECT
public:
    CodeEditor();

private:
    QsciScintilla *textEdit;
    QsciLexer *Lexer;

private slots:
    void MarginMarker(int m, int n, Qt::KeyboardModifiers);
};

#endif // CODEEDITOR_H
