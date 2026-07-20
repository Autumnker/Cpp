#pragma once

#include <string>
#include <vector>

/// 用于 CSV 存储的单条游戏记录
struct GameRecord {
    std::string date;       // 日期 (YYYY-MM-DD HH:MM:SS)
    std::string player1;    // 黑方玩家名称
    std::string player2;    // 白方玩家名称
    int moveCount;          // 总步数
    std::string winner;     // 胜者 "Black" / "White" / "Draw"
};

/// 工具函数
namespace Utils {

    /// 获取当前时间字符串
    std::string getCurrentTimeStr();

    /// 保存游戏记录到 CSV 文件（追加模式）
    bool saveGameRecord(const GameRecord& record, const std::string& filepath);

    /// 从 CSV 文件加载所有游戏记录
    std::vector<GameRecord> loadGameRecords(const std::string& filepath);

    /// 保存配置键值对（保留备用）
    bool saveConfig(const std::string& key, const std::string& value,
        const std::string& filepath);

    /// 根据键加载配置值（保留备用）
    std::string loadConfig(const std::string& key, const std::string& filepath);

    /// 简单的控制台日志器
    void log(const std::string& message);

}  // namespace Utils
