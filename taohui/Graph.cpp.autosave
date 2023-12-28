#include <Graph.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <stack>
#include <QMessageBox>
#include <set>
#include <string>
#include <algorithm>

void Graph::addRate(std::string from, std::string to, double rate)
{
    // 确保两个货币都已经添加到图中
    if (currencyIndex.find(from) == currencyIndex.end())
    {
        currencyIndex[from] = currencyCount;
        indexToCurrency.push_back(from);
        currencyCount++;
    }
    if (currencyIndex.find(to) == currencyIndex.end())
    {
        currencyIndex[to] = currencyCount;
        indexToCurrency.push_back(to);
        currencyCount++;
    }

    // 扩展邻接矩阵以适应新的货币数量
    int newSize = currencyCount;
    for (auto& row : adjMatrix)
    {
        row.resize(newSize, std::numeric_limits<double>::max());
    }
    adjMatrix.resize(newSize, std::vector<double>(newSize, std::numeric_limits<double>::max()));

    // 对自身的汇率进行更新
    for (int i = 0; i < newSize; ++i)
    {
        adjMatrix[i][i] = 1.0;
    }

    // 更新或添加汇率
    int fromIndex = currencyIndex[from];
    int toIndex = currencyIndex[to];
    adjMatrix[fromIndex][toIndex] = rate;
}



void Graph::ReadRatesFromFile(std::string filename, Graph &graph, QWidget* parent)
{
    std::ifstream file(filename);
    std::string line, from, to;
    double rate;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::istringstream iss(line);
            if (iss >> from >> to >> rate)
            {
                graph.addRate(from, to, rate);
            }
        }
        file.close();
    }
    else
    {
        QMessageBox::critical(parent, "错误", "无法打开文件，请重新输入");
        emit fileOpenError();
    }
}


bool Graph::CanArbitrage(const std::string &startCurrency)
{
    if (currencyIndex.find(startCurrency) == currencyIndex.end())
    {
        return false;
    }

    std::vector<double> maxProduct(currencyCount, 0.0);
    int startIdx = currencyIndex[startCurrency];
    maxProduct[startIdx] = 1.0;

    // 贝尔曼-福特算法
    for (int i = 0; i < currencyCount - 1; i++)
    {
        bool updated = false;
        for (int from = 0; from < currencyCount; from++)
        {
            for (int to = 0; to < currencyCount; to++)
            {
                if (adjMatrix[from][to] != std::numeric_limits<double>::max() && maxProduct[from] * adjMatrix[from][to] > maxProduct[to])
                {
                    maxProduct[to] = maxProduct[from] * adjMatrix[from][to];
                    updated = true;
                }
            }
        }
        if (!updated)
        {
            break; // 没有更新，提前退出
        }
    }

    // 检测是否存在套汇机会
    for (int from = 0; from < currencyCount; from++)
    {
        for (int to = 0; to < currencyCount; to++)
        {
            if (adjMatrix[from][to] != std::numeric_limits<double>::max() && maxProduct[from] * adjMatrix[from][to] > maxProduct[to])
            {
                return true; // 发现套汇机会
            }
        }
    }

    return false; // 未发现套汇机会
}


std::vector<std::pair<std::vector<std::string>, double>> Graph::FindArbitrageLoops(const std::string &startCurrency)
{
    std::vector<std::pair<std::vector<std::string>, double>> arbitrageLoops;

    if (currencyIndex.find(startCurrency) == currencyIndex.end())
    {
        return arbitrageLoops; // 如果起始货币不存在于图中，则直接返回空列表
    }

    std::vector<bool> visited(currencyCount, false);
    std::vector<int> path;
    DFSFindLoops(currencyIndex[startCurrency], currencyIndex[startCurrency], 1.0, path, visited, arbitrageLoops, 1);

    return arbitrageLoops;
}

void Graph::DFSFindLoops(int start, int current, double product, std::vector<int>& path, std::vector<bool>& visited, std::vector<std::pair<std::vector<std::string>, double>>& arbitrageLoops, int depth) {
    const double epsilon = 1e-6; // 用于处理浮点数精度问题
    if (depth > 100) {
        return;
    }
    visited[current] = true;
    path.push_back(current);

    for (int next = 0; next < currencyCount; next++) {
        if (adjMatrix[current][next] != std::numeric_limits<double>::max()) {
            if (next == start && path.size() > 1) {
                double loopProduct = product * adjMatrix[current][next];
                if (loopProduct > 1.0 + epsilon) { // 精确判断套汇环路
                    std::vector<std::string> currencyPath;
                    for (int node : path) {
                        currencyPath.push_back(indexToCurrency[node]);
                    }
                    currencyPath.push_back(indexToCurrency[start]); // 完成环路
                    arbitrageLoops.push_back({currencyPath, loopProduct});
                }
            } else if (!visited[next]) {
                DFSFindLoops(start, next, product * adjMatrix[current][next], path, visited, arbitrageLoops, depth + 1);
            }
        }
    }

    path.pop_back();
    visited[current] = false;
}


std::vector<std::string> Graph::FindArbitrageCurrencies()
{
    std::vector<std::string> arbitrageCurrencies;

    for (const auto& pair : currencyIndex)
    {
        const std::string& currency = pair.first;
        auto loops = FindArbitrageLoops(currency);
        if (!loops.empty())
        {
            arbitrageCurrencies.push_back(currency);
        }
    }

    return arbitrageCurrencies;
}


std::vector<std::string> Graph::GetAllCurrencies() const
{
    std::vector<std::string> currencies;
    for (const auto& pair : currencyIndex)
    {
        currencies.push_back(pair.first);
    }
    return currencies;
}


std::pair<std::vector<std::string>, double> Graph::FindBestExchangePath(const std::string &fromCurrency, const std::string &toCurrency)
{
    if (currencyIndex.find(fromCurrency) == currencyIndex.end() || currencyIndex.find(toCurrency) == currencyIndex.end())
    {
        return {{"NULL"}, 0.0}; // 如果货币不存在于图中，返回"NULL"
    }

    int fromIndex = currencyIndex[fromCurrency];
    int toIndex = currencyIndex[toCurrency];

    std::vector<double> maxExchangeRates(currencyCount, 0.0);
    std::vector<int> predecessors(currencyCount, -1);
    maxExchangeRates[fromIndex] = 1.0;

    // 贝尔曼-福特算法更新最大兑换率
    for (int i = 0; i < currencyCount - 1; i++)
    {
        bool updated = false;
        for (int j = 0; j < currencyCount; j++)
        {
            for (int k = 0; k < currencyCount; k++)
            {
                if (adjMatrix[j][k] != std::numeric_limits<double>::max())
                {
                    double newRate = maxExchangeRates[j] * adjMatrix[j][k];
                    if (newRate > maxExchangeRates[k])
                    {
                        maxExchangeRates[k] = newRate;
                        predecessors[k] = j;
                        updated = true;
                    }
                }
            }
        }
        if (!updated) break;
    }

    // 回溯最优路径
    if (maxExchangeRates[toIndex] == 0.0)
    {
        return {{"NULL"}, 0.0}; // 如果无法到达目标货币
    }

    std::vector<std::string> path;
    int at = toIndex;
    while (at != -1 && at != fromIndex)
    {
        path.insert(path.begin(), indexToCurrency[at]);
        at = predecessors[at];
    }
    path.insert(path.begin(), fromCurrency);

    return {path, maxExchangeRates[toIndex]};
}




std::pair<std::vector<std::string>, int> Graph::FindArbitragePath(const std::string &fromCurrency, const std::string &toCurrency, double startAmount, double targetAmount)
{
    std::vector<std::string> fullPath;
    int totalTransactions = 0;

    if (fromCurrency == toCurrency)
    {
        auto loops = FindArbitrageLoops(fromCurrency);
        double bestRate = 1.0;
        std::vector<std::string> bestLoop;
        for (const auto& loop : loops)
        {
            if (loop.second > bestRate)
            {
                bestRate = loop.second;
                bestLoop = loop.first;
            }
        }
        double currentAmount = startAmount;
        while (currentAmount < targetAmount)
        {
            fullPath.insert(fullPath.end(), bestLoop.begin(), bestLoop.end());
            currentAmount *= bestRate;
            totalTransactions += bestLoop.size() - 1; // 每个循环中的交易次数
        }
    }
    else
    {
        auto bestPathResult = FindBestExchangePath(fromCurrency, toCurrency);
        if (!bestPathResult.first.empty())
        {
            double rate = bestPathResult.second;
            double currentAmount = startAmount;
            while (currentAmount < targetAmount)
            {
                fullPath.insert(fullPath.end(), bestPathResult.first.begin(), bestPathResult.first.end());
                currentAmount *= rate;
                totalTransactions += bestPathResult.first.size() - 1; // 每次路径中的交易次数
            }
        }
    }

    return {fullPath, totalTransactions};
}



Graph* Graph::ExtractSubgraph(const std::set<std::string>& includedNodes) {
    Graph* subgraph = new Graph();

    // 遍历每个节点，如果它在 includedNodes 中，则检查与其他节点的边
    for (const auto& node : includedNodes)
    {
        if (currencyIndex.find(node) != currencyIndex.end())
        {
            int nodeIndex = currencyIndex[node];

            // 添加与这个节点相关的边
            for (const auto& target : includedNodes)
            {
                if (currencyIndex.find(target) != currencyIndex.end())
                {
                    int targetIndex = currencyIndex[target];

                    if (adjMatrix[nodeIndex][targetIndex] != std::numeric_limits<double>::max())
                    {
                        subgraph->addRate(node, target, adjMatrix[nodeIndex][targetIndex]);
                    }
                }
            }
        }
    }

    return subgraph;
}



Graph* Graph::GenerateRandomSubgraph(std::size_t nodeCount)
{
    if (nodeCount > currencyIndex.size())
    {
        nodeCount = currencyIndex.size(); // 确保节点数量不超过图中的货币总数
    }

    std::vector<std::string> allCurrencies;
    for (const auto& pair : currencyIndex)
    {
        allCurrencies.push_back(pair.first);
    }

    // 随机打乱并选取前nodeCount个货币
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allCurrencies.begin(), allCurrencies.end(), g);

    std::set<std::string> selectedNodes(allCurrencies.begin(), allCurrencies.begin() + nodeCount);

    return ExtractSubgraph(selectedNodes);
}


bool Graph::IsValidSequence(const std::vector<std::string> &currencySequence)
{
    return currencySequence.size() >= 2 && currencySequence.front() == currencySequence.back();
}


float Graph::CanArbitrageOnSequence(const std::vector<std::string> &currencySequence)
{
    if (!IsValidSequence(currencySequence))
    {
        return 0; // 如果序列无效，则直接返回false
    }

    double rateProduct = 1.0;
    for (size_t i = 0; i < currencySequence.size() - 1; ++i)
    {
        std::string from = currencySequence[i];
        std::string to = currencySequence[i + 1];

        if (currencyIndex.find(from) == currencyIndex.end() || currencyIndex.find(to) == currencyIndex.end() || adjMatrix[currencyIndex[from]][currencyIndex[to]] == std::numeric_limits<double>::max())
        {
            return 0; // 如果序列中的货币不存在或者没有从from到to的汇率，则返回false
        }

        rateProduct *= adjMatrix[currencyIndex[from]][currencyIndex[to]];
    }

    return rateProduct;
}

void Graph::displayRatesInTextBrowser(QTextBrowser *textBrowser)
{
    std::string output;
    for (int i = 0; i < currencyCount; ++i)
    {
        for (int j = 0; j < currencyCount; ++j)
        {
            if (i != j && adjMatrix[i][j] != std::numeric_limits<double>::max())
            { // assuming 0 means no direct rate
                output += indexToCurrency[i] + " 到 " + indexToCurrency[j] +
                          "的汇率是" + std::to_string(adjMatrix[i][j]) + "\n";
            }
        }
    }
    textBrowser->setText(QString::fromStdString(output));
}

void Graph::resetGraph()
{
    // 清空货币索引映射
    currencyIndex.clear();
    // 清空索引到货币的向量
    indexToCurrency.clear();
    // 重置货币计数
    currencyCount = 0;
    // 清空邻接矩阵
    adjMatrix.clear();
}
