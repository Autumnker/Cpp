#pragma once

#include <filesystem>
#include <string>
#include <functional>

namespace fs = std::filesystem;

class DirSize {
public:
	// ����Ŀ¼��ͳ���ļ���С�ĺ�����ʹ�ûص���������ÿ���ļ�
	static void traverseDirectory(const std::string& directoryPath, uintmax_t& fileSize, int& fileCount);
};
