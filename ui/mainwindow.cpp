#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QRunnable>
#include <QThreadPool>
#include <QToolBox>

#include "compilationtable.h"
#include "codeeditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasChanges(false)
{
    ui->setupUi(this);
    ui->errorTextEdit->setStyleSheet(
        "QTextEdit"
        "{"
        "color: red;"
        "}"
    );
    workingDirectory = QDir::currentPath();
    ui->workingDirectoryLineEdit->setText(workingDirectory);
    ui->workingDirectoryLineEdit->setCursorPosition(0);
    setupTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

class DOSRunner : public QRunnable {
public:
    explicit DOSRunner(QFileInfo const& info) : exePath(info)
    {
    }

    void run() override
    {
        QProcess process;
        QStringList list;
        list << "-c" << "mount C \""+exePath.absolutePath()+"\"" << "-c" << "C:"+exePath.fileName();
        process.start("DOS/DOSBox.exe", list);
        process.waitForFinished(-1);
    }

private:
    QFileInfo exePath;
};

bool MainWindow::tryToOpen()
{
    QString newFilename = QFileDialog::getOpenFileName(
                this, "Select file", ".", "Asm source files(*.asm)");

    if (!newFilename.isEmpty()){
        fileName = newFilename;
    }

    return !newFilename.isEmpty();
}

bool MainWindow::isNeedToSave()
{
    if (hasChanges){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "AsmForDos",
                                      "File contains changes. Do you want to save file?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            return true;
    }

    return false;
}

void MainWindow::saveCurrentFile()
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        return;

    file.write(ui->codeTextEdit->toPlainText().toLatin1());
    printToOutput(QString("File saved to %1").arg(file.fileName()));

    file.close();
}

QByteArray MainWindow::getFileData()
{
    QByteArray byteArr;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
       return byteArr;

    byteArr = file.readAll();
    file.close();

    return byteArr;
}

void MainWindow::setupTableView()
{
    compTablePtr.reset(new CompilationTable());

    ui->tableView->setModel(compTablePtr.data());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void MainWindow::updateTableView()
{
    compTablePtr.reset(new CompilationTable(codeData, binaryData));
    ui->tableView->setModel(compTablePtr.data());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void MainWindow::printToOutput(const QString &text)
{
    if (text.size() != 0){
        ui->outputTextEdit->append(
            QString("<h6>%1</h6>\n").arg(text)
        );
    }else{
        ui->outputTextEdit->append("\n");
    }
}

void MainWindow::clearOutput()
{
    ui->outputTextEdit->clear();
}

void MainWindow::printErrors()
{
    QString errorText;
    for (auto &error : errors){
        errorText.append(
            QString("<h3>Error: %1 at line %2</h3>\n").arg(
                QString::fromStdString(error.second),
                QString::number(error.first + 1)
            )
        );
    }
    ui->errorTextEdit->setText(errorText);
}

void MainWindow::loadData()
{
    codeData = ui->codeTextEdit->toPlainText().split("\n");
}

void MainWindow::compile()
{
    std::vector<std::string> data;
    for (const QString& row : codeData) {
        data.push_back(row.toStdString());
    }

    Parser parser(data);
    parser.Parse();
    parser.Compile();

    binaryData = parser.GetBinaryData();
    errors = parser.GetErrors();
}

void MainWindow::write()
{
    QString name;
    if (fileName.size() == 0){

    }else{
        name = QFileInfo(fileName).baseName();
    }

    comFilename = workingDirectory + QDir::separator() + name + ".com";
    QByteArray bytes;
    for (auto &data : binaryData){
        for (u8 byte : data.data){
            bytes.append(byte);
        }
    }

    printToOutput(QString("Writing to file %1").arg(comFilename));
    QFile file(comFilename);

    if (!file.open(QIODevice::WriteOnly))
        return;

    file.write(bytes);

    file.close();
}

void MainWindow::on_openButton_clicked()
{
    if (isNeedToSave())
        saveCurrentFile();

    if (tryToOpen()){
        ui->codeTextEdit->clear();
        QByteArray data = getFileData();
        ui->codeTextEdit->setPlainText(data);
        hasChanges = false;

        printToOutput(QString("Opened a file: %1").arg(fileName));
    }
}

void MainWindow::on_saveButton_clicked()
{
    if (!fileName.isEmpty()){
        saveCurrentFile();
        hasChanges = false;
    } else {
        QString newFilename = QFileDialog::getSaveFileName(
                    this, "Set filename", ".", "Asm source files(*.asm)");

        if (!newFilename.isEmpty()){
            fileName = newFilename;
            saveCurrentFile();
            hasChanges = false;
        }
    }
}

void MainWindow::on_saveAsButton_clicked()
{
    if (!fileName.isEmpty()){
        saveCurrentFile();
        hasChanges = false;
    } else {
        QString newFilename = QFileDialog::getSaveFileName(this, "Set filename", ".");

        if (!newFilename.isEmpty()){
            fileName = newFilename;
            saveCurrentFile();
            hasChanges = false;
        }
    }
}

void MainWindow::on_codeTextEdit_textChanged()
{
    if (!fileName.isEmpty())
        hasChanges = true;
}

void MainWindow::on_closeButton_clicked()
{
    if (isNeedToSave())
        saveCurrentFile();

    printToOutput(QString("Cosed a file: %1").arg(fileName));

    fileName = "";
    ui->codeTextEdit->clear();
}

void MainWindow::on_compileButton_clicked()
{
    ui->errorTextEdit->clear();

    printToOutput(" ");
    printToOutput("Compilation started");

    loadData();
    compile();

    if (errors.empty()){
        updateTableView();
        write();
        printToOutput("Compilation successful");
        QMessageBox::information(this, "AsmForDos", "Compilation successful");
    }else{
        printToOutput("Compilation failed. See Errors tab for more details");
        QMessageBox::critical(this, "AsmForDos", "Compilation failed");
        setupTableView();
        printErrors();
    }
}

void MainWindow::on_runButton_clicked()
{
    if (!comFilename.isEmpty()){
        DOSRunner *runner = new DOSRunner(QFileInfo(comFilename));
        QThreadPool::globalInstance()->start(runner);
    }
}

void MainWindow::on_compileAndRunButton_clicked()
{
    on_compileButton_clicked();
    if (errors.empty())
        on_runButton_clicked();
}

void MainWindow::on_clearButton_clicked()
{
    clearOutput();
}

void MainWindow::on_workingDirectoryPushButton_clicked()
{
    QString newFolder = QFileDialog::getExistingDirectory(
                this, "Select folder", workingDirectory);

    if (!newFolder.isEmpty()){
        workingDirectory = newFolder;
    }
    ui->workingDirectoryLineEdit->setText(workingDirectory);
    ui->workingDirectoryLineEdit->setCursorPosition(0);
}
