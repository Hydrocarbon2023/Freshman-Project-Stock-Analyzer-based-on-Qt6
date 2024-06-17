#include <sstream>
#include "indexthread.h"

IndexThread::IndexThread(QObject *parent) :
    QThread(parent) {
}

void IndexThread::setInputPath(const std::string &path) {
    inputPath = path;
}

void IndexThread::setIndexPath(const std::string &path) {
    indexPath = path;
}

StockData IndexThread::lineToData(const std::string &line) {
    StockData data;
    std::istringstream in(line);
    std::string field;
    std::vector<std::string> fields;
    while (std::getline(in, field, ','))
        fields.push_back(field);
    int s = fields.size();
    if (s != 11) {
        emit updateCurrentState("不好，数据格式有误");
        return data;
    }
    data.symbol_code = fields[0];
    data.trade_date = fields[1];
    data.open = std::stod(fields[2]);
    data.high = std::stod(fields[3]);
    data.low = std::stod(fields[4]);
    data.close = std::stod(fields[5]);
    data.pre_close = std::stod(fields[6]);
    data.change = std::stod(fields[7]);
    data.percent_change = std::stod(fields[8]);
    data.volume = std::stod(fields[9]);
    data.turnover = std::stod(fields[10]);
    return data;
}

void IndexThread::run() {
    generateIndex(inputPath, indexPath);
    emit complete();
}
