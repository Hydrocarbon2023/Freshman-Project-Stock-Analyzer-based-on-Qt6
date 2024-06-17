#include "indexthread.h"
#include <iostream>
#include <fstream>

void IndexThread::generateIndex(const std::string &inputPath, const std::string &indexPath) {
    emit updateCurrentState("开始创建索引！");
    std::ifstream inputFile(inputPath, std::ios::binary);
    std::ofstream indexFile(indexPath);
    if (!inputFile.is_open() || !indexFile.is_open())
        throw std::runtime_error("打开文件失败TT");
    std::string line;
    std::streampos pos = inputFile.tellg();
    std::getline(inputFile, line);
    StockData formerData = lineToData(line);
    StockData data;
    indexFile << formerData.symbol_code << "," << formerData.trade_date << "," << pos << std::endl;
    pos = inputFile.tellg();
    while (std::getline(inputFile, line)) {
        data = lineToData(line);
        if (data.symbol_code != formerData.symbol_code || data.trade_date != formerData.trade_date) {
            indexFile << data.symbol_code << "," << data.trade_date << "," << inputFile.tellg() << std::endl;
            formerData = data;
            pos = inputFile.tellg();
        }
    }
}
