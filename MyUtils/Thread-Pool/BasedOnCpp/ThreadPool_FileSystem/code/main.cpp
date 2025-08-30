#include "ThreadPool.h"
#include "DirSize.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main() {
	// 设置要遍历的目录路径
	string directoryPath = R"(C:\Users\Public\AppData)";

	// 初始化原子变量用于统计总大小和文件计数
	uintmax_t totalSize(0);
	int fileCount(0);

	DirSize::traverseDirectory(directoryPath, totalSize, fileCount);

	// 输出最终结果
	cout << "\n统计完成!" << endl;
	cout << "路径为: " << directoryPath << endl;
	cout << "总文件数: " << fileCount << endl;
	cout << "总大小: " << totalSize << " 字节 (" << fixed << setprecision(4) << totalSize / (1024.0 * 1024.0) << " MB)" << endl;

	return 0;
}