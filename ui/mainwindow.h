#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QDir>

#include "compilationtable.h"

#include "../core/parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool tryToOpen();
    bool isNeedToSave();

    void saveCurrentFile();
    QByteArray getFileData();
    void setupTableView();
    void updateTableView();

    void printToOutput(const QString& text);
    void clearOutput();
    void printErrors();

    void loadData();
    void compile();
    void write();
    void run();

private slots:
    void on_openButton_clicked();

    void on_saveButton_clicked();

    void on_saveAsButton_clicked();

    void on_compileButton_clicked();

    void on_runButton_clicked();

    void on_compileAndRunButton_clicked();

    void on_closeButton_clicked();

    void on_codeTextEdit_textChanged();

    void on_clearButton_clicked();

    void on_workingDirectoryPushButton_clicked();

private:
    Ui::MainWindow *ui;

    QSharedPointer<CompilationTable> compTablePtr;
    QStringList codeData;
    QString fileName, workingDirectory, comFilename;
    bool hasChanges;

    std::vector<Parser::BinaryData> binaryData;
    std::vector<std::pair<u32, std::string>> errors;

    const QString DOS = QDir::currentPath() + "/" +
            "DOS" + "/" + "DOSBox.exe";
};

#endif // MAINWINDOW_H
