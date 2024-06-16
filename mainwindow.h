#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sortthread.h"
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
private slots:
    void on_sortButtonClicked();
    void on_sortingComplete();
    void on_browseButtonClicked();
    void on_sortingStateUpdated(const QString &message);
private:
    Ui::MainWindow *ui;
    SortThread *sortThread;
    std::string inputPath;
    std::string outputPath;
};

#endif // MAINWINDOW_H
