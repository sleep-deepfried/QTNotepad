#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    // Set the window title
    this->setWindowTitle("Notepad");

    // Load the icon file
    QPixmap logo(":/assets/icons/notepad.png");
    QIcon icon(logo);

    // Set the window icon
    this->setWindowIcon(icon);


    // Create status bar labels for line and column
    QLabel *lineLabel = new QLabel("Ln: 1");
    QLabel *colLabel = new QLabel("Col: 1");
    statusBar()->addWidget(lineLabel);
    statusBar()->addWidget(colLabel);

    // Connect cursor position changed signal to updateStatus slot
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateStatus);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    file_path = "";
    ui->textEdit->setText("");
}


void MainWindow::on_actionOpen_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(file_name);
    file_path = file_name;

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "file not open");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(file_path);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "file not open");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Choose save location");
    QFile file(file_name);
    file_path = file_name;

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionAbout_Notepad_triggered()
{
    QString about_text;
    about_text = "Author: Earl John Pulido\n";
    about_text += "Date: April 14, 2023\n";
    about_text += "(C) Notepad (R)";

    QMessageBox::about(this, "About Notepad", about_text);

}

void MainWindow::updateStatus()
{
    int line = ui->textEdit->textCursor().blockNumber() + 1;
    int column = ui->textEdit->textCursor().columnNumber() + 1;
    ui->statusbar->showMessage("Ln: " + QString::number(line) + "  Col: " + QString::number(column));
}


