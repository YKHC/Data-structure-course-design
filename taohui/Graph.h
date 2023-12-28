#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <ctime>
#include <limits>
#include <QTextBrowser>

class Graph : public QObject {
    Q_OBJECT

private:
    std::unordered_map<std::string, int> currencyIndex; // 用于存储货币到索引的映射
    std::vector<std::vector<double>> adjMatrix; // 邻接矩阵
    std::vector<std::string> indexToCurrency; // 索引到货币的映射
    int currencyCount; // 货币的数量
    std::string StartCurrency;
    double StartAmount;
    std::string targetCurrency;
    double targetAmount;

    void DFSFindLoops(int start, int current, double product, std::vector<int>& path, std::vector<bool>& visited, std::vector<std::pair<std::vector<std::string>, double>>& arbitrageLoops, int depth);

public:
    Graph(QObject *parent = nullptr) : QObject(parent), currencyCount(0) {}

    void addRate(std::string from, std::string to, double rate);
    void ReadRatesFromFile(std::string filename, Graph &graph, QWidget* parent = nullptr);
    bool CanArbitrage(const std::string &startCurrency);
    std::vector<std::pair<std::vector<std::string>, double>> FindArbitrageLoops(const std::string &startCurrency);
    std::vector<std::string> FindArbitrageCurrencies();
    std::vector<std::string> GetAllCurrencies() const;
    std::pair<std::vector<std::string>, double> FindBestExchangePath(const std::string &fromCurrency, const std::string &toCurrency);
    std::pair<std::vector<std::string>, int> FindArbitragePath(const std::string &fromCurrency, const std::string &toCurrency, double startAmount, double targetAmount);
    Graph* ExtractSubgraph(const std::set<std::string>& includedNodes);
    Graph* GenerateRandomSubgraph(std::size_t nodeCount);
    bool IsValidSequence(const std::vector<std::string>& currencySequence);
    float CanArbitrageOnSequence(const std::vector<std::string>& currencySequence);
    void resetGraph();

    int Action;

signals:
    void arbitrageFound(const std::string& path, double profit);
    void fileOpenError();

public slots:
    void displayRatesInTextBrowser(QTextBrowser *textBrowser);

};

#endif // GRAPH_H
