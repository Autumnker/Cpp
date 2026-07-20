#include "utils.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "csv.h"

namespace Utils {

std::string getCurrentTimeStr() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    return buf;
}

bool saveGameRecord(const GameRecord& record, const std::string& filepath) {
    std::ifstream check(filepath);
    bool fileExists = check.good();
    check.close();

    std::ofstream file;
    if (fileExists) {
        file.open(filepath, std::ios::app);  // 追加模式
    } else {
        file.open(filepath);
        // 写入 CSV 表头
        file << "Date,Player1,Player2,Moves,Winner\n";
    }

    if (!file.is_open()) {
        log("[ERROR] Cannot open file: " + filepath);
        return false;
    }

    // 写入 CSV 行
    file << record.date << ","
         << record.player1 << ","
         << record.player2 << ","
         << record.moveCount << ","
         << record.winner << "\n";

    file.close();
    return true;
}

std::vector<GameRecord> loadGameRecords(const std::string& filepath) {
    std::vector<GameRecord> records;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        return records;  // 文件不存在，返回空
    }
    file.close();

    try {
        // 使用 fast-cpp-csv-parser 读取
        io::CSVReader<5> reader(filepath);
        reader.read_header(io::ignore_extra_column, "Date", "Player1", "Player2",
                           "Moves", "Winner");

        std::string date, p1, p2, winner;
        int moves = 0;

        while (reader.read_row(date, p1, p2, moves, winner)) {
            GameRecord rec;
            rec.date = date;
            rec.player1 = p1;
            rec.player2 = p2;
            rec.moveCount = moves;
            rec.winner = winner;
            records.push_back(std::move(rec));
        }
    } catch (const std::exception& e) {
        log(std::string("[WARN] Failed to read CSV: ") + e.what());
    }

    return records;
}

bool saveConfig(const std::string& key, const std::string& value,
                const std::string& filepath) {
    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    file << key << "=" << value << "\n";
    return true;
}

std::string loadConfig(const std::string& key, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "";
    }
    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos && line.substr(0, pos) == key) {
            return line.substr(pos + 1);
        }
    }
    return "";
}

void log(const std::string& message) {
    std::cout << "[Gobang] " << message << std::endl;
}

}  // namespace Utils
