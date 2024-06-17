#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sortthread.h"
#include "indexthread.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    sortThread = new SortThread(this);
    indexThread = new IndexThread(this);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::on_sortButtonClicked);
    connect(sortThread, &SortThread::complete, this, &MainWindow::on_sortingComplete);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButtonClicked);
    connect(sortThread->exSort, &ExternalSort::updateSortingState, this, &MainWindow::on_sortingStateUpdated);
    connect(indexThread, &IndexThread::complete, this, &MainWindow::on_indexComplete);
    connect(indexThread, &IndexThread::updateCurrentState, this, &MainWindow::on_sortingStateUpdated);
    isGoingToSort = true;
}

MainWindow::~MainWindow() {
    delete ui;
    if (sortThread->isRunning()) {
        sortThread->terminate();
    }
    delete sortThread;
}

void MainWindow::on_sortButtonClicked() {
    if (isGoingToSort) {
        QString defaultLine = "请选择文件(*.csv):";
        QString lineText = ui->inputPathLine->text();
        if (lineText != defaultLine) {
            inputPath = lineText.toStdString();
            std::filesystem::path iPath(inputPath);
            outputPath = iPath.parent_path().string() + "/output.txt";
        }
        if (inputPath.empty() || outputPath.empty()) {
            QMessageBox::warning(this, "注意", "请先选择路径");
            return;
        }
        sortThread->setInputPath(inputPath);
        sortThread->setOutputPath(outputPath);
        sortThread->start();
    }
    else {
        std::filesystem::path iPath(outputPath);
        indexPath = iPath.parent_path().string() + "/index.txt";
        indexThread->setInputPath(outputPath);
        indexThread->setIndexPath(indexPath);
        indexThread->start();
    }
}

void MainWindow::on_sortingComplete() {
    QMessageBox::information(this, "报告", "排序已完成。");
    ui->sortButton->setText("创建索引");
    isGoingToSort = false;
}

void MainWindow::on_browseButtonClicked() {
    QString inputPath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("CSV files (*.csv)"));
    if (!inputPath.isEmpty())
        ui->inputPathLine->setText(inputPath);
}

void MainWindow::on_sortingStateUpdated(const QString &message) {
    ui->inputPathLine->setText(message);
}

void MainWindow::on_indexComplete() {
    QMessageBox::information(this, "报告", "索引已生成。");
    ui->sortButton->setText("一键排序");
    isGoingToSort = true;
}
