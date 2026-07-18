#pragma once

#include <filesystem>
#include <string>
#include <functional>

namespace fs = std::filesystem;

extern std::vector<std::string> path_visited;
extern std::vector<std::string> path_denied;
class DirSize {
public:
	// 遍历目录并统计文件大小的函数，使用回调函数处理每个文件
	static void traverseDirectory_incomplete(const std::string& directoryPath, uintmax_t& fileSize, size_t& fileCount);
	static void traverseDirectory(const std::string& directoryPath, uintmax_t& fileSize, size_t& fileCount);
};
