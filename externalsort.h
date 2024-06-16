#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include "stockdata.h"
#include <QObject>

class ExternalSort : public QObject {
Q_OBJECT
public:
    ExternalSort(QObject *parent=nullptr);
public slots:
    void doSort(const std::string &inputPath, const std::string &outputPath);
signals:
    void updateSortingState(const QString &message);
private:
    std::vector<std::string> tmpFiles;
    StockData lineToData(const std::string &line);
    void splitCSV(const std::string &inputPath);
    void sortingRewrite(const std::string &chunkPath);
};

#endif // EXTERNALSORT_H
