#include "ThreadPool.h"
#include "DirSize.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	// 设置要遍历的目录路径
	string directoryPath{};
	if (argc == 2) {
		std::filesystem::path absPath = std::filesystem::absolute(argv[1]);
		directoryPath.assign(absPath.string());
	}
	else {
		directoryPath = R"(C:\Windows\WinSxS)";
	}

	// 遍历文件
	uintmax_t totalSize(0);
	size_t fileCount(0);
	std::chrono::time_point time_start = std::chrono::system_clock::now();
	DirSize::traverseDirectory(directoryPath, totalSize, fileCount);
	std::chrono::time_point time_finish = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = time_finish - time_start;

	// 输出日志
	std::ofstream ofs("./log.md", std::ios::trunc);
	ofs << "[visited]" << '\n';
	for (const auto& path : path_visited) { ofs << path << '\n'; }
	ofs << "[denied]" << '\n';
	for (const auto& path : path_denied) { ofs << path << '\n'; }
	ofs.close();

	// 输出最终结果
	cout << "\n统计完成!" << endl;
	cout << "路径为： " << directoryPath << endl;
	cout << "总文件数： " << fileCount << endl;
	cout << "总大小： " << totalSize << " 字节 (" << fixed << setprecision(4) << totalSize / (1024.0 * 1024.0) << " MB)" << endl;
	std::cout << "共耗时：" << elapsed.count() << " 秒" << std::endl;

	return 0;
}
